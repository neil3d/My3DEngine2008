using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Nexus.GameFramework
{
    /// <summary>
    /// 标识Property为输入影响的变量
    /// </summary>
    [AttributeUsage(AttributeTargets.Property)]
    public class InputAttribute : System.Attribute 
    {
        public InputAttribute()
        {
        }
    }
}
