using System;
using System.IO;
using System.Reflection;

namespace NexusEngineExtension
{
    /// <summary>
    /// Used by configuration classes to store assembly/class names and instantiate
    /// objects from them.
    /// </summary>
    public class ObjectCreator
    {
        private Assembly objectAssembly;
        private Type objectType;

        public Type ObjectType
        {
            get { return objectType; }
        }

        public Assembly ObjectAssembly
        {
            get { return objectAssembly; }
        }

        public ObjectCreator(Type type)
        {
            this.objectType = type;
        }

        /// <summary>
        /// 加载指定名称的程序集
        /// </summary>
        public ObjectCreator(string assemblyName, string className)
        {
            string assemblyFile = Path.Combine(System.IO.Directory.GetCurrentDirectory(), assemblyName);
            try
            {
                objectAssembly = Assembly.LoadFrom(assemblyFile);
            }
            catch
            {
                objectAssembly = Assembly.GetExecutingAssembly();
            }

            objectType = objectAssembly.GetType(className);
        }

        public ObjectCreator(string className)
        {
            objectAssembly = Assembly.GetExecutingAssembly();
            objectType = objectAssembly.GetType(className);
        }

        public ObjectCreator(Assembly assembly, Type type)
        {
            objectAssembly = assembly;
            objectType = type;
        }

        public string GetAssemblyTitle()
        {
            Attribute title = Attribute.GetCustomAttribute(objectAssembly, typeof(AssemblyTitleAttribute));
            if (title == null)
                return objectAssembly.GetName().Name;
            return ((AssemblyTitleAttribute)title).Title;
        }

        public Object CreateInstance()
        {
            try
            {
                return objectAssembly.CreateInstance(this.objectType.FullName, true, 0, null, null, null, null);
            }
            catch (Exception e)
            {
                throw e;
                //LogManager.Instance.Write("Failed to create instance of {0} of type {0} from assembly {1}", typeof(T).Name, type, assembly.FullName);
                //LogManager.Instance.Write(e.Message);
            }
        }
    }
}
