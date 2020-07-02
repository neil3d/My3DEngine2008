using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace NexusEngineExtension
{
    /// <summary>
    /// 命令选项工厂
    /// </summary>
    public class CommandOptionFactory
    {
        /// <summary>
        /// 动态创建ICommandOption
        /// </summary>
        private List<ObjectCreator> typeCommandOptions = new List<ObjectCreator>();

        public CommandOptionFactory()
        {
        }

        /// <summary>
        /// 加载所有ICommandOption子类
        /// </summary>
        /// <param name="assembly">指定从哪个程序集中加载</param>
        public void LoadCommandOptions(Assembly assembly)
        {
            if (assembly != null)
            {
                typeCommandOptions.AddRange(DynamicLoader.Find(assembly, typeof(ICommandOption)));
            }
        }

        /// <summary>
        /// 创建指定类型的ICommandOption
        /// </summary>
        /// <param name="typeName">完整的类型名称</param>
        public ICommandOption CreateCommandOption(string typeName)
        {
            foreach (ObjectCreator creator in typeCommandOptions)
            {
                if (creator.ObjectType.FullName.Equals(typeName))
                {
                    try
                    {
                        ICommandOption newCommandOption = (ICommandOption)creator.CreateInstance();
                        return newCommandOption;
                    }
                    catch (Exception ex)
                    {
                        throw ex;
                    }
                }
            }
            return null;
        }

        public ICommandOption CreateCommandOption(Type type)
        {
            return CreateCommandOption(type.FullName);
        }
    }
}
