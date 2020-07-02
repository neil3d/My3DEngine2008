using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.Specialized;
using System.Resources;
using System.Collections;

namespace Nexus.GUI
{
    /// <summary>
    /// 将键和值强类型化为字符串而不是对象来实现哈希表
    /// </summary>
    public class UIStringDictionary : StringDictionary
    {
        public void CopyFrom(ResXResourceSet res)
        {
            foreach (DictionaryEntry item in res)
            {
                if(item.Value is string)
                {
                    this[item.Key.ToString()] = item.Value as string;
                }
            }
        }

        internal void CopyFrom(ResXResourceReader rsxr)
        {
            foreach (DictionaryEntry item in rsxr)
            {
                if (item.Value is string)
                {
                    this[item.Key.ToString()] = item.Value as string;
                }
            }
        }

        /// <summary>
        /// 重载该函数，如果key不存在则返回空字符串，避免抛出异常
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public string GetValue(string key)
        {
            if (string.IsNullOrEmpty(key))
                return String.Empty;

            if(this.ContainsKey(key))
            {
                return this[key];
            }
            return String.Empty;
        }


    }
}
