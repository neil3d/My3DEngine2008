using System;
using System.Collections.Generic;
using System.Text;

namespace NexusEngineExtension
{
   /// <summary>
   /// 生成唯一名称
   /// </summary>
   /// <typeparam name="T">类</typeparam>
    public class NameGenerator<T>
    {
        private static long nextId;
        private static string baseName;

        public long NextIdentifier
        {
            get
            {
                return nextId;
            }
            set
            {
                nextId = value;
            }
        }

        public NameGenerator()
            : this(typeof(T).Name)
        {
        }

        public NameGenerator(string inBaseName)
        {
            if (inBaseName != null)
            {
                baseName = inBaseName;
            }
        }

        public string GetNextUniqueName()
        {
            return GetNextUniqueName(String.Empty);
        }

        public string GetNextUniqueName(string prefix)
        {
            return String.Format("{0}{1}{2}", prefix, baseName, nextId++);
        }
    }
}
