using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using LinqToCodedom.Generator;
using System.Linq.Expressions;
using LinqToCodedom.CodeDomPatterns;

namespace LinqToCodedom.Extensions
{
    public static partial class CodeTypeDeclarationExtensions
    {

        public static CodeTypeDeclaration AddOperators(this CodeTypeDeclaration classCode, 
            params CodeMemberOperatorOverride[] operators)
        {
            classCode.Members_AddRange(operators);

            return classCode;
        }
    }
}
