using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using LinqToCodedom.Visitors;
using System.Linq.Expressions;

namespace LinqToCodedom.Generator
{
    public static partial class Emit
    {
        public static CodeStatement @return()
        {
            return new CodeMethodReturnStatement();
        }

        public static CodeStatement @return(CodeExpression exp)
        {
            return new CodeMethodReturnStatement(exp);
        }

        public static CodeStatement @return<TResult>(Expression<Func<TResult>> exp)
        {
            return new CodeMethodReturnStatement(new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeStatement @return<TResult, T>(Expression<Func<TResult, T>> exp)
        {
            return new CodeMethodReturnStatement(new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeStatement @return<TResult>(Expression<Action<TResult>> exp)
        {
            return new CodeMethodReturnStatement(new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeStatement @return<TResult, T, T2>(Expression<Func<TResult, T, T2>> exp)
        {
            return new CodeMethodReturnStatement(new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

    }
}
