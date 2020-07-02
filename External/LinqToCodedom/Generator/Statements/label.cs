using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;

namespace LinqToCodedom.Generator
{
    public static partial class Emit
    {
        public static CodeStatement label(string label)
        {
            return new CodeLabeledStatement(label);
        }

        public static CodeStatement @goto(string label)
        {
            return new CodeGotoStatement(label);
        }
    }
}
