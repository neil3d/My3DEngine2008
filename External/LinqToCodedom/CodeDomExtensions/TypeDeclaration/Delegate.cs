using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using LinqToCodedom.Generator;
using System.Linq.Expressions;

namespace LinqToCodedom.Extensions
{
    public static partial class CodeTypeDeclarationExtensions
    {
        #region Delegate

        public static CodeTypeDeclaration AddDelegates(this CodeTypeDeclaration classCode, params CodeTypeDelegate[] @delegate)
        {
            classCode.Members_AddRange(@delegate);

            return classCode;
        }

        public static CodeTypeDeclaration AddDelegate<T>(this CodeTypeDeclaration classCode,
            Type returnType, MemberAttributes ma, Expression<Func<T, string>> paramsAndName)
        {
            classCode.Members_Add(Define.Delegate(returnType, CorrectAttributes(classCode, ma), paramsAndName));

            return classCode;
        }

        public static CodeTypeDeclaration AddDelegate<T>(this CodeTypeDeclaration classCode,
            string returnType, MemberAttributes ma, Expression<Func<T, string>> paramsAndName)
        {
            classCode.Members_Add(Define.Delegate(returnType, CorrectAttributes(classCode, ma), paramsAndName));

            return classCode;
        }

        #endregion

    }
}
