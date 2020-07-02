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
        public static CodeConditionStatement @if<T>(Expression<Func<T, bool>> condition,
            params CodeStatement[] trueStatements)
        {
            return ifelse(condition, trueStatements, null);
        }

        public static CodeConditionStatement @if(Expression<Func<bool>> condition,
            params CodeStatement[] trueStatements)
        {
            return ifelse(condition, trueStatements, null);
        }

        public static CodeConditionStatement @if<T, T2>(Expression<Func<T, T2, bool>> condition,
           params CodeStatement[] trueStatements)
        {
            return ifelse(condition, trueStatements, null);
        }

        public static CodeConditionStatement @if<T, T2, T3>(Expression<Func<T, T2, T3, bool>> condition,
           params CodeStatement[] trueStatements)
        {
            return ifelse(condition, trueStatements, null);
        }

        public static CodeConditionStatement ifelse(Expression condition,
            CodeStatement[] trueStatements, params CodeStatement[] falseStatements)
        {
            var condStatement = new CodeConditionStatement();
            condStatement.Condition = new CodeExpressionVisitor(new VisitorContext()).Visit(condition);
            condStatement.TrueStatements.AddRange(trueStatements);
            if (falseStatements != null)
                condStatement.FalseStatements.AddRange(falseStatements);
            return condStatement;
        }

        public static CodeConditionStatement ifelse(Expression<Func<bool>> condition,
            CodeStatement[] trueStatements, params CodeStatement[] falseStatements)
        {
            return ifelse(condition as Expression, trueStatements, falseStatements);
        }

        public static CodeConditionStatement ifelse<T>(Expression<Func<T, bool>> condition,
            CodeStatement[] trueStatements, params CodeStatement[] falseStatements)
        {
            return ifelse(condition as Expression, trueStatements, falseStatements);
        }

        public static CodeConditionStatement ifelse<T, T1>(Expression<Func<T, T1, bool>> condition,
            CodeStatement[] trueStatements, params CodeStatement[] falseStatements)
        {
            return ifelse(condition as Expression, trueStatements, falseStatements);
        }
    }
}
