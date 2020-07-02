using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LinqToCodedom.Visitors;
using System.CodeDom;
using System.Linq.Expressions;
using LinqToCodedom.CodeDomPatterns;

namespace LinqToCodedom.Generator
{
    public static partial class Emit
    {
        public static CodeSwitchStatement @switch<TResult>(Expression<Func<TResult>> switchExp)
        {
            return new CodeSwitchStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(switchExp)
            );
        }

        public static CodeSwitchStatement @switch<TResult, T>(Expression<Func<TResult, T>> switchExp)
        {
            return new CodeSwitchStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(switchExp)
            );
        }
    }
}
