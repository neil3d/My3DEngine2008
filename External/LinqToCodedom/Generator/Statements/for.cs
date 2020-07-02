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
        public static CodeIterationStatement @for<TResult, T, T2, T3>(string varName,
            Expression<Func<T, TResult>> initStmt,
            Expression<Func<T2, bool>> testStmt, Expression<Func<T3, TResult>> incStmt,
            params CodeStatement[] statements)
        {
            return new CodeIterationStatement(declare(varName, initStmt),
                new CodeExpressionVisitor(new VisitorContext()).Visit(testStmt),
                Emit.assignVar(varName, incStmt),
                statements);
        }

        public static CodeIterationStatement @for<TResult, T, T2>(string varName,
            Expression<Func<T, TResult>> initStmt,
            Expression<Func<T2, bool>> testStmt, Expression<Func<TResult>> incStmt,
            params CodeStatement[] statements)
        {
            return new CodeIterationStatement(declare(varName, initStmt),
                new CodeExpressionVisitor(new VisitorContext()).Visit(testStmt),
                Emit.assignVar(varName, incStmt),
                statements);
        }

        public static CodeIterationStatement @for<TResult, T, T2, T3>(Expression<Func<string>> varName,
            Expression<Func<T, T3, TResult>> initStmt,
            Expression<Func<T2, bool>> testStmt, Expression<Func<TResult>> incStmt,
            params CodeStatement[] statements)
        {
            return new CodeIterationStatement(
                new CodeStatementVisitor(new VisitorContext()).Visit(initStmt),
                new CodeExpressionVisitor(new VisitorContext()).Visit(testStmt),
                new CodeStatementVisitor(new VisitorContext()).Visit(incStmt),
                statements);
        }
    }
}
