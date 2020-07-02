using System;
using System.Collections.Generic;
using System.Text;

namespace NexusEngineExtension
{
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = true)]
    public class TitleMapper : Attribute
    {
        public string TitleName { get; private set; }

        public TitleMapper(string inTitleName)
        {
            this.TitleName = inTitleName;
        }
    }
}
