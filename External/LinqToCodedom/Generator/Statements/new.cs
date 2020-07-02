using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using System.Linq.Expressions;
using LinqToCodedom.Visitors;

namespace LinqToCodedom.Generator
{
    public static partial class Builder
    {
        public static CodeExpression @new(string type)
        {
            throw new NotSupportedException();
        }

        public static CodeExpression @new<T>(string type, params object[] param)
        {
            throw new NotSupportedException();
        }
    }
}
