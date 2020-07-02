using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using System.Linq.Expressions;
using LinqToCodedom.Generator;

namespace LinqToCodedom.Extensions
{
    public static partial class CodeTypeDeclarationExtensions
    {
        #region Methods

        public static CodeTypeDeclaration AddMethod(this CodeTypeDeclaration classCode, CodeMemberMethod methodBody)
        {
            classCode.Members_Add(methodBody);

            return classCode;
        }

        public static CodeMemberMethod AddMethod<T>(this CodeTypeDeclaration classCode, 
            MemberAttributes ma, Type returnType, Expression<Func<T, string>> paramsAndName, params CodeStatement[] statements)
        {
            var meth = Define.Method(CorrectAttributes(classCode, ma), returnType, paramsAndName, statements);

            classCode.Members_Add(meth);

            return meth;
        }

        public static CodeMemberMethod AddMethod<T, T2>(this CodeTypeDeclaration classCode, 
            MemberAttributes ma, Type returnType, Expression<Func<T, T2, string>> paramsAndName, 
            params CodeStatement[] statements)
        {
            var meth = Define.Method(CorrectAttributes(classCode, ma), returnType, paramsAndName, statements);

            classCode.Members_Add(meth);

            return meth;
        }

        public static CodeMemberMethod AddMethod<T>(this CodeTypeDeclaration classCode, 
            MemberAttributes ma, string returnType, Expression<Func<T, string>> paramsAndName, params CodeStatement[] statements)
        {
            var meth = Define.Method(CorrectAttributes(classCode, ma), returnType, paramsAndName, statements);

            classCode.Members_Add(meth);

            return meth;
        }

        public static CodeMemberMethod AddMethod(this CodeTypeDeclaration classCode, 
            MemberAttributes ma, string returnType, Expression<Func<string>> paramsAndName, params CodeStatement[] statements)
        {
            var meth = Define.Method(CorrectAttributes(classCode, ma), returnType, paramsAndName, statements);

            classCode.Members_Add(meth);

            return meth;
        }

        public static CodeMemberMethod AddMethod(this CodeTypeDeclaration classCode, 
            MemberAttributes ma, CodeTypeReference returnType, Expression<Func<string>> paramsAndName, params CodeStatement[] statements)
        {
            var meth = Define.Method(CorrectAttributes(classCode, ma), returnType, paramsAndName, statements);

            classCode.Members_Add(meth);

            return meth;
        }

        public static CodeMemberMethod AddMethod(this CodeTypeDeclaration classCode, 
            MemberAttributes ma, Type returnType, Expression<Func<string>> paramsAndName, params CodeStatement[] statements)
        {
            var meth = Define.Method(CorrectAttributes(classCode, ma), returnType, paramsAndName, statements);

            classCode.Members_Add(meth);

            return meth;
        }

        public static CodeMemberMethod AddMethod<T>(this CodeTypeDeclaration classCode,
            MemberAttributes ma,
            Expression<Func<T, string>> paramsAndName, params CodeStatement[] statements)
        {
            var meth = Define.Method(CorrectAttributes(classCode, ma), paramsAndName, statements);

            classCode.Members_Add(meth);

            return meth;
        }

        public static CodeMemberMethod AddMethod<T, T2>(this CodeTypeDeclaration classCode,
            MemberAttributes ma,
            Expression<Func<T, T2, string>> paramsAndName, params CodeStatement[] statements)
        {
            var meth = Define.Method(CorrectAttributes(classCode, ma), paramsAndName, statements);

            classCode.Members_Add(meth);

            return meth;
        }

        public static CodeMemberMethod AddMethod<T, T2, T3>(this CodeTypeDeclaration classCode,
            MemberAttributes ma,
            Expression<Func<T, T2, T3, string>> paramsAndName, params CodeStatement[] statements)
        {
            var meth = Define.Method(CorrectAttributes(classCode, ma), paramsAndName, statements);

            classCode.Members_Add(meth);

            return meth;
        }

        public static CodeMemberMethod AddMethod(this CodeTypeDeclaration classCode,
            MemberAttributes ma, Expression<Func<string>> paramsAndName,
            params CodeStatement[] statements)
        {
            var meth = Define.Method(CorrectAttributes(classCode, ma), paramsAndName, statements);

            classCode.Members_Add(meth);

            return meth;
        }

        #endregion

    }
}
