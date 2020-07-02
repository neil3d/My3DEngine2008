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
        public static CodeVariableDeclarationStatement declare(CodeTypeReference type, string varName)
        {
            return new CodeVariableDeclarationStatement(type, varName, CodeDom.@default(type));
        }

        public static CodeVariableDeclarationStatement declare(string type, string varName)
        {
            return new CodeVariableDeclarationStatement(type, varName, CodeDom.@default(type));
        }

        public static CodeVariableDeclarationStatement declare(CodeTypeReference type, string varName,
            Expression<Action> exp)
        {
            return new CodeVariableDeclarationStatement(type, varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeVariableDeclarationStatement declare(string type, string varName,
            Expression<Action> exp)
        {
            return new CodeVariableDeclarationStatement(type, varName, 
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeVariableDeclarationStatement declare(Type type, string varName,
            Expression<Action> exp)
        {
            return new CodeVariableDeclarationStatement(type, varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeVariableDeclarationStatement declare(Type type, string varName)
        {
            return new CodeVariableDeclarationStatement(type, varName, CodeDom.@default(type));
        }
        
        public static CodeVariableDeclarationStatement declare<TResult>(
            string varName, Expression<Func<TResult>> initExp)
        {
            return new CodeVariableDeclarationStatement(typeof(TResult), varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(initExp));
        }

        public static CodeVariableDeclarationStatement declare<T, TResult>(
            string varName, Expression<Func<T, TResult>> initExp)
        {
            return new CodeVariableDeclarationStatement(typeof(TResult), varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(initExp));
        }

        public static CodeVarStatement declareVar<TResult>(
            string varName, Expression<Func<TResult>> initExp)
        {
            return new CodeVarStatement(varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(initExp));
        }

        public static CodeVarStatement declareVar<T, TResult>(
            string varName, Expression<Func<T, TResult>> initExp)
        {
            return new CodeVarStatement(varName,
                new CodeExpressionVisitor(new VisitorContext()).Visit(initExp));
        }
    }
}
