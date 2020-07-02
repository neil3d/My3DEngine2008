using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace NexusEngineExtension
{
    /// <summary>
    /// 动态加载辅助类
    /// </summary>
    public static class DynamicLoader
    {
        #region Methods

        public static Assembly GetAssembly(string assemblyFilename)
        {
            if (String.IsNullOrEmpty(assemblyFilename))
            {
                return Assembly.GetExecutingAssembly();
            }
            else
            {
                return Assembly.LoadFrom(assemblyFilename);
            }
        }


        public static List<ObjectCreator> Find(Assembly assembly, Type baseType)
        {
            List<ObjectCreator> types = new List<ObjectCreator>();
            Type[] assemblyTypes = null;

            try
            {
                assemblyTypes = assembly.GetTypes();

                foreach (Type type in assemblyTypes)
                {
                    if ((type.Equals(baseType) || type.IsSubclassOf(baseType))
                        && !type.IsAbstract)
                    {
                        types.Add(new ObjectCreator(assembly, type));
                    }
                }
            }

            catch (ReflectionTypeLoadException ex)
            {
                throw ex;
                //LogManager.Instance.Write(LogManager.BuildExceptionString(ex));
                //LogManager.Instance.Write("Loader Exceptions:");
                //foreach (Exception lex in ex.LoaderExceptions)
                //{
                //    LogManager.Instance.Write(LogManager.BuildExceptionString(lex));
                //}
            }
            catch (BadImageFormatException ex)
            {
                throw ex;
                //LogManager.Instance.Write(LogMessageLevel.Trivial, true, ex.Message);
            }

            return types;
        }

        #endregion Methods
    }
}
