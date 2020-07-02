using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using LinqToCodedom.Visitors;
using System.Linq.Expressions;
using LinqToCodedom.CodeDomPatterns;

namespace LinqToCodedom.Generator
{
    public static partial class Emit
    {
        public static CodeDoStatement @do(Expression<Func<bool>> testExp,
            params CodeStatement[] statements)
        {
            return new CodeDoStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(testExp),
                statements
            );
        }

        public static CodeDoStatement @do<T>(Expression<Func<bool, T>> testExp,
            params CodeStatement[] statements)
        {
            return new CodeDoStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(testExp),
                statements
            );
        }

        public static CodeDoStatement @do<T, T2>(Expression<Func<bool, T, T2>> testExp,
            params CodeStatement[] statements)
        {
            return new CodeDoStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(testExp),
                statements
            );
        }

        public static CodeDoStatement @do<T, T2, T3>(Expression<Func<bool, T, T2, T3>> testExp,
           params CodeStatement[] statements)
        {
            return new CodeDoStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(testExp),
                statements
            );
        }
    }
}
