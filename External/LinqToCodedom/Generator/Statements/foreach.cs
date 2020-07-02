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
        public static CodeForeachStatement @foreach<TResult>(string varName,
            Expression<Func<IEnumerable<TResult>>> iterStmt,
            params CodeStatement[] statements)
        {
            return new CodeForeachStatement(new CodeTypeReference(typeof(TResult)),
                varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(iterStmt),
                statements);
        }

        public static CodeForeachStatement @foreach<TResult, T>(string varName,
            Expression<Func<T, IEnumerable<TResult>>> iterStmt,
            params CodeStatement[] statements)
        {
            return new CodeForeachStatement(new CodeTypeReference(typeof(TResult)),
                varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(iterStmt),
                statements);
        }

        public static CodeForeachStatement @foreach<TResult, T, T2>(string varName,
            Expression<Func<T, T2, IEnumerable<TResult>>> iterStmt,
            params CodeStatement[] statements)
        {
            return new CodeForeachStatement(new CodeTypeReference(typeof(TResult)),
                varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(iterStmt),
                statements);
        }

        public static CodeForeachStatement @foreach<TResult, T, T2, T3>(string varName,
            Expression<Func<T, T2, T3, IEnumerable<TResult>>> iterStmt,
            params CodeStatement[] statements)
        {
            return new CodeForeachStatement(new CodeTypeReference(typeof(TResult)),
                varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(iterStmt),
                statements);
        }

        public static CodeForeachStatement @foreach<TResult>(CodeTypeReference type,
           string varName,
           Expression<Func<IEnumerable<TResult>>> iterStmt,
           params CodeStatement[] statements)
        {
            return new CodeForeachStatement(type,
                varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(iterStmt),
                statements);
        }

        public static CodeForeachStatement @foreach<TResult, T>(CodeTypeReference type,
           string varName,
           Expression<Func<T, IEnumerable<TResult>>> iterStmt,
           params CodeStatement[] statements)
        {
            return new CodeForeachStatement(type,
                varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(iterStmt),
                statements);
        }

        public static CodeForeachStatement @foreach<TResult, T, T2>(CodeTypeReference type, 
            string varName,
            Expression<Func<T, T2, IEnumerable<TResult>>> iterStmt,
            params CodeStatement[] statements)
        {
            return new CodeForeachStatement(type,
                varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(iterStmt),
                statements);
        }

        public static CodeForeachStatement @foreach<TResult, T, T2, T3>(CodeTypeReference type, 
            string varName,
            Expression<Func<T, T2, T3, IEnumerable<TResult>>> iterStmt,
            params CodeStatement[] statements)
        {
            return new CodeForeachStatement(type,
                varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(iterStmt),
                statements);
        }
    }
}
