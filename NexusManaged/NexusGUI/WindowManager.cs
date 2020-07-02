using System;
using System.Text;
using System.Collections.Generic;
using System.IO;

using System.Reflection;
using System.CodeDom;
using System.CodeDom.Compiler;
using Microsoft.CSharp;
using System.Security.Permissions;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Soap;
using NexusEngine;
using NexusEngineExtension;
using Nexus.GameFramework;

namespace Nexus.GUI
{
    /// <summary>
    /// 管理Window对象的创建和生命周期
    /// </summary>
    public sealed class WindowManager : IDisposable
    {
        #region  Fields
        /// <summary>
        /// 注册<窗口类,程序集>map
        /// 提供一个可扩展的类工厂
        /// </summary>
        private Dictionary<string, Assembly> registedWindowTypes = new Dictionary<string, Assembly>();

        #endregion

        #region Properties
        public Dictionary<string, Assembly> AllWindowTypes
        {
            get { return registedWindowTypes; }
        }
        #endregion

        #region Singleton Implemention
        private static WindowManager instance;

        public WindowManager()
        {
            if (instance == null)
            {
                instance = this;

                NLogger.Instance.WriteString(LogType.Info, "WindowManager Singleton Created.");
            }
        }

        /// <summary>
        /// 单件访问
        /// </summary>
        public static WindowManager Instance
        {
            get { return instance; }
        }

        #endregion Singleton Implemention

        #region Methods

        /// <summary>
        /// 动态编译C#文件，允许将C#作为脚本定义自定义的窗口类
        /// </summary>
        /// <param name="path">加载脚本的目录</param>
        /// <param name="debug"></param>
        /// <returns></returns>
        [EnvironmentPermissionAttribute(SecurityAction.LinkDemand, Unrestricted = true)]
        public bool CompileScripts(string path, bool debug, params string[] dynamicAssemblyRefs)
        {
            // Get all C# files
            string[] scriptFiles = Directory.GetFiles(path, "*.cs");

            // Create a code compiler
            CSharpCodeProvider provider = new CSharpCodeProvider();

            // configure params for the script compilation
            CompilerParameters parms = new CompilerParameters(dynamicAssemblyRefs);

            // this is an in memory assembly
            // TODO: Consider compiling to file and loading if no changes have occurred on the next load
            parms.GenerateInMemory = true;
            parms.GenerateExecutable = false;
            parms.IncludeDebugInformation = debug;

            // compile the list of files found in the script directory
            CompilerResults results = provider.CompileAssemblyFromFile(parms, scriptFiles);

            // check for errors and log them
            if (results.Errors.Count != 0)
            {
                StringBuilder errorBuilder = new StringBuilder(results.Errors.Count);

                string errorMsg = "Script Error [File: {0}, Line: {1}: {2}]" + Environment.NewLine;

                foreach (CompilerError error in results.Errors)
                {
                    errorBuilder.AppendFormat(errorMsg, error.FileName, error.Line, error.ErrorText);
                }

                NLogger.Instance.WriteString(LogType.Error, errorBuilder.ToString());

                return false;
            }

            AttachAssembly(results.CompiledAssembly);

            // success
            return true;
        }

        /// <summary>
        /// 加载制定文件名称的程序集
        /// 异常:
        ///   System.ArgumentNullException:
        ///     assemblyFile 为 null。
        ///
        ///   System.IO.FileNotFoundException:
        ///     未找到 assemblyFile，或者试图加载的模块没有指定文件扩展名。
        ///
        ///   System.IO.FileLoadException:
        ///     发现一个未能加载的文件。
        ///
        ///   System.BadImageFormatException:
        ///     assemblyFile 不是有效程序集。- 或 -当前加载的是 2.0 或更高版本的公共语言运行库，assemblyFile 是用更高版本的公共语言运行库编译的。
        ///
        ///   System.Security.SecurityException:
        ///     在没有所需 System.Net.WebPermission 的情况下，指定了不以“file://”开始的基本代码。
        ///
        ///   System.ArgumentException:
        ///     assemblyFile 参数是空字符串 ("")。
        ///
        ///   System.IO.PathTooLongException:
        ///     程序集名称的长度大于 MAX_PATH 个字符。
        /// </summary>
        /// <param name="assemblyName">程序集文件名称</param>
        public void LoadAssembly(string assemblyName)
        {
            AttachAssembly(Assembly.LoadFrom(assemblyName));
        }

        /// <summary>
        /// 注册程序集中的Window类型
        /// </summary>
        /// <param name="assembly"></param>
        public void AttachAssembly(Assembly assembly)
        {
            // 遍历程序集中的所有类型，将Window的子类注册
            Type BaseType = typeof(Nexus.GUI.Window);
			try
			{
				foreach (Type type in assembly.GetTypes())
				{
					if (type.IsSubclassOf(BaseType) && !type.IsAbstract)
					{
						RegisterType(type, assembly);
					}
				}
			}
			catch (Exception e)
			{
				//加载NexusGameClient时发生异常："无法加载一个或多个请求的类型。有关更多信息，请检索 LoaderExceptions 属性。"
				//原因待查。
                NLogger.Instance.WriteString(LogType.Exception, e.Message);
			}
        }

        /// <summary>
        /// 注册窗口类
        /// </summary>
        /// <param name="type"></param>
        /// <param name="assembly"></param>
        private void RegisterType(Type type, Assembly assembly)
        {
            if (!registedWindowTypes.ContainsKey(type.FullName))
            {
                registedWindowTypes.Add(type.FullName, assembly);

                // 记录注册成功事件
                NLogger.Instance.WriteString(LogType.Info, "WindowManager: Window type '" + type.FullName + "' registered.");
            }
        }

        /// <summary>
        /// 窗口类型是否已经注册过
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        public bool IsTypeRegedited(string type)
        {
            return registedWindowTypes.ContainsKey(type);
        }

        public string FindUniqueName(string typeName, Window parent)
        {
            if (!string.IsNullOrEmpty(typeName))
            {
                string baseName = (parent == null) ? string.Empty : parent.Name + "/";
                string reversedName = typeName.Substring(typeName.LastIndexOf('.') + 1);

                StringBuilder reversedString = new StringBuilder();
                int index = 0;
                // 最多允许一万个窗口
                while (index++ <= 10000)
                {
                    reversedString.Remove(0, reversedString.Length);
                    reversedString.Append(baseName);
                    reversedString.Append(reversedName);
                    reversedString.Append(index);
                    if ( parent.FindChild(reversedString.ToString(), true) == null &&
                        GUISystem.Instance.RootWindow.FindChild(reversedString.ToString(), true) == null)
                    {
                        return reversedString.ToString();
                    }
                }
            }
            return System.Guid.NewGuid().ToString();
        }

        public Window CreateWindow(string type, Window parent)
        {
            return CreateWindow(type, FindUniqueName(type, parent), parent);
        }

        public Window CreateWindow(string type, string name, Window parent)
        {
            // 如果窗口类没有注册则抛出异常
            if (!registedWindowTypes.ContainsKey(type))
            {
                throw new UnknownObjectException("Window type '{0}' is not currently loaded.", type);
            }

            // 如果窗口已经存在
            if (IsWindowPresent(name))
            {
                throw new BaseGUIException("A window with the name '{0}' already exists.", name);
            }

            // grab the assembly which hosts this type
            Assembly assembly = registedWindowTypes[type];

            // create an instance of the window and initialize it
            Window window = (Window)assembly.CreateInstance(type, true, 0, null, new object[] { name }, null, null);

            if (parent != null)
            {
                parent.AddChild(window);
            }

            // return the newly created window
            return window;
        }

        private bool IsWindowPresent(string name)
        {
            return (GUISystem.Instance.RootWindow == null) ? false : GUISystem.Instance.RootWindow.FindChild(name, true) != null;
        }

        #endregion


        #region IDisposable
        public void Dispose()
        {
            NLogger.Instance.WriteString(LogType.Info, "WindowManager singleton destroyed.");
        }
        #endregion

        #region Custom Serialization
        /// <summary>
        /// 将UI序列化到指定的文件
        /// 需要外部捕获异常
        /// </summary>
        /// <param name="packageName">资源包名称</param>
        /// <param name="fileName">文件名称</param>
        /// <param name="windowToSerialize">指定序列化那个窗体(包含其所有的子窗体)</param>
        public void Serialize(string packageName, string fileName, Window windowToSerialize)
        {
            //XmlSerializeWriter writer = new XmlSerializeWriter("F:/Test.nui");
            NXmlSerializeDataWriter writer = new NXmlSerializeDataWriter(packageName, fileName);
            SerializeWriter serializer = new SerializeWriter(writer);
            serializer.Serialize(windowToSerialize);
            writer.Close();
        }

        /// <summary>
        /// 从组资源文件中反序列化ＵＩ窗体
        /// 需要外部捕获异常
        /// </summary>
        /// <param name="packageName">资源包名称</param>
        /// <param name="fileName">资源文件名称</param>
        /// <returns>序列化窗口，一般情况下不需要处理</returns>
        public Window Deserialize(string packageName, string fileName)
        {
            //XmlSerializeReader reader = new XmlSerializeReader("F:/Test.nui");
            NXmlSerializeDataReader reader = new NXmlSerializeDataReader(packageName, fileName);
            SerializeReader serializer = new SerializeReader(reader);
            Window window = serializer.Deserialize() as Window;
            reader.Close();
            return window;
        }

        #endregion
    }

    internal sealed class VersionConfigToNamespaceAssemblyObjectBinder : SerializationBinder
    {
        public override Type BindToType(string assemblyName, string typeName)
        {
            Type typeToDeserialize = null;
            try
            {
                string ToAssemblyName = assemblyName.Split(',')[0];
                Assembly[] Assemblies = AppDomain.CurrentDomain.GetAssemblies();
                foreach (Assembly ass in Assemblies)
                {
                    if (ass.FullName.Split(',')[0] == ToAssemblyName)
                    {
                        typeToDeserialize = ass.GetType(typeName);
                        break;
                    }
                }
            }
            catch (System.Exception exception)
            {
                NLogger.Instance.WriteString(LogType.Info, "VersionConfigToNamespaceAssemblyObjectBinder Error!" + exception.Message);
                throw exception;
            }
            return typeToDeserialize;
        }
    }
}
