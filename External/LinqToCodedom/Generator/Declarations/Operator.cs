using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CodeDomPatterns;
using System.Linq.Expressions;

namespace LinqToCodedom.Generator
{
    public static partial class Define
    {
        public static CodeMemberOperatorOverride Operator(
            CodeTypeReference returnType,
            Expression<Func<OperatorType>> paramsAndType,
            params CodeStatement[] statements)
        {
            CodeParameterDeclarationExpressionCollection pars = new CodeParameterDeclarationExpressionCollection();
            
            var c = new CodeMemberOperatorOverride(
                CodeDom.GetMethodName<OperatorType>(paramsAndType, pars),
                pars.ToArray(), returnType, statements);

            return c;
        }

        public static CodeMemberOperatorOverride Operator<T>(
            CodeTypeReference returnType,
            Expression<Func<T, OperatorType>> paramsAndType,
            params CodeStatement[] statements)
        {
            CodeParameterDeclarationExpressionCollection pars = new CodeParameterDeclarationExpressionCollection();

            var c = new CodeMemberOperatorOverride(
                CodeDom.GetMethodName<OperatorType>(paramsAndType, pars),
                pars.ToArray(), returnType, statements);

            return c;
        }
    }
}
