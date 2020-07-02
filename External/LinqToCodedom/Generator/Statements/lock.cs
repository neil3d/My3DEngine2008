using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Expressions;
using System.CodeDom;
using LinqToCodedom.Visitors;
using LinqToCodedom.CodeDomPatterns;

namespace LinqToCodedom.Generator
{
    public static partial class Emit
    {
        public static CodeLockStatement @lock<TResult, T>(Expression<Func<TResult, T>> exp,
            params CodeStatement[] stmt)
        {
            return new CodeLockStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp), stmt
            );
        }

        public static CodeLockStatement @lock<TResult>(Expression<Func<TResult>> exp,
            params CodeStatement[] stmt)
        {
            return new CodeLockStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp), stmt
            );
        }
    }
}
