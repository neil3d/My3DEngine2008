using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using System.Linq.Expressions;

namespace LinqToCodedom.Generator
{
    public static partial class Define
    {
        #region Method
        public static CodeMemberMethod Method<T>(MemberAttributes ma, Type returnType, 
            Expression<Func<T, string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                ReturnType = returnType == null ? null : new CodeTypeReference(returnType),
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        public static CodeMemberMethod Method<T, T2>(MemberAttributes ma, Type returnType, 
            Expression<Func<T, T2, string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                ReturnType = returnType == null ? null : new CodeTypeReference(returnType),
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        public static CodeMemberMethod Method<T>(MemberAttributes ma, string returnType, 
            Expression<Func<T, string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                ReturnType = returnType == null ? null : new CodeTypeReference(returnType),
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        public static CodeMemberMethod Method<T>(MemberAttributes ma, CodeTypeReference returnType, 
            Expression<Func<T, string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                ReturnType = returnType,
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        public static CodeMemberMethod Method<T>(MemberAttributes ma,
           Expression<Func<T, string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        public static CodeMemberMethod Method<T, T2>(MemberAttributes ma,
           Expression<Func<T, T2, string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        public static CodeMemberMethod Method<T, T2, T3>(MemberAttributes ma,
           Expression<Func<T, T2, T3, string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        public static CodeMemberMethod Method(MemberAttributes ma, Type returnType, 
            Expression<Func<string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                ReturnType = returnType == null ? null : new CodeTypeReference(returnType),
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        public static CodeMemberMethod Method(MemberAttributes ma, string returnType, 
            Expression<Func<string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                ReturnType = returnType == null ? null : new CodeTypeReference(returnType),
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        public static CodeMemberMethod Method(MemberAttributes ma, CodeTypeReference returnType, 
            Expression<Func<string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                ReturnType = returnType,
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        public static CodeMemberMethod Method(MemberAttributes ma,
            Expression<Func<string>> paramsAndName, params CodeStatement[] statements)
        {
            CodeMemberMethod method = new CodeMemberMethod()
            {
                Attributes = ma
            };

            method.Name = CodeDom.GetMethodName<string>(paramsAndName, method.Parameters);
            method.Statements.AddRange(statements);

            return method;
        }

        #endregion

    }
}
