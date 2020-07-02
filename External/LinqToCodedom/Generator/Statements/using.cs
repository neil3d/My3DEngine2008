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
        public static CodeUsingStatement @using<TResult, T>(Expression<Func<TResult, T>> exp,
            params CodeStatement[] stmt)
        {
            return new CodeUsingStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp), stmt
            );
        }

        public static CodeUsingStatement @using<TResult>(Expression<Func<TResult>> exp,
            params CodeStatement[] stmt)
        {
            return new CodeUsingStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp), stmt
            );
        }

        public static CodeUsingStatement @using<TResult, T>(string varName, 
            Expression<Func<TResult, T>> exp, params CodeStatement[] stmt)
        {
            return new CodeUsingStatement(
                new CodeAssignExpression(
                    new CodeParameterDeclarationExpression(typeof(TResult), varName),
                    new CodeExpressionVisitor(new VisitorContext()).Visit(exp)
                ), stmt
            );
        }

        public static CodeUsingStatement @using<TResult>(string varName, 
            Expression<Func<TResult>> exp, params CodeStatement[] stmt)
        {
            return new CodeUsingStatement(
                new CodeAssignExpression(
                    new CodeParameterDeclarationExpression(typeof(TResult), varName),
                    new CodeExpressionVisitor(new VisitorContext()).Visit(exp)
                ), stmt
            );
        }

        public static CodeUsingStatement @using<TResult>(CodeTypeReference varType, string varName,
            Expression<Func<TResult>> exp, params CodeStatement[] stmt)
        {
            return new CodeUsingStatement(
                new CodeAssignExpression(
                    new CodeParameterDeclarationExpression(varType, varName),
                    new CodeExpressionVisitor(new VisitorContext()).Visit(exp)
                ), stmt
            );
        }

    }
}
