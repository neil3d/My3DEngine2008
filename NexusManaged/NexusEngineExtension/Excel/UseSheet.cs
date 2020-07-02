using System;
using System.Collections.Generic;
using System.Text;

namespace NexusEngineExtension
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = false)]
    public class UseSheet : Attribute
    {
        public string SheetName { get; set; }

        public UseSheet(string inSheetName)
        {
            this.SheetName = inSheetName;
        }
    }
}
