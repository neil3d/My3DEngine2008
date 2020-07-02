using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;

namespace LinqToCodedom.Generator
{
    public static partial class Emit
    {
        public static CodeTryCatchFinallyStatement trycatch(params CodeStatement[] statements)
        {
            return new CodeTryCatchFinallyStatement(statements, new CodeCatchClause[]{});
        }
    }
}
