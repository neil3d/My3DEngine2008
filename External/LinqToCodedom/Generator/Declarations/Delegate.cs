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
        public static CodeTypeDelegate Delegate<T>(Type returnType, MemberAttributes ma,
            Expression<Func<T, string>> paramsAndName)
        {
            var c = new CodeTypeDelegate()
            {
                Attributes = ma,
                ReturnType = new CodeTypeReference(returnType),
            };

            c.Name = CodeDom.GetMethodName<string>(paramsAndName, c.Parameters);

            return c;
        }

        public static CodeTypeDelegate Delegate<T>(string returnType, MemberAttributes ma,
            Expression<Func<T, string>> paramsAndName)
        {
            var c = new CodeTypeDelegate()
            {
                Attributes = ma,
                ReturnType = new CodeTypeReference(returnType),
            };

            c.Name = CodeDom.GetMethodName<string>(paramsAndName, c.Parameters);

            return c;
        }

        public static CodeTypeDelegate Delegate(string returnType, MemberAttributes ma,
            Expression<Func<string>> paramsAndName)
        {
            var c = new CodeTypeDelegate()
            {
                Attributes = ma,
                ReturnType = new CodeTypeReference(returnType),
            };

            c.Name = CodeDom.GetMethodName<string>(paramsAndName, c.Parameters);

            return c;
        }

        public static CodeTypeDelegate Delegate(Type returnType, MemberAttributes ma,
            Expression<Func<string>> paramsAndName)
        {
            var c = new CodeTypeDelegate()
            {
                Attributes = ma,
                ReturnType = new CodeTypeReference(returnType),
            };

            c.Name = CodeDom.GetMethodName<string>(paramsAndName, c.Parameters);

            return c;
        }

        public static CodeTypeDelegate Delegate<T>(CodeTypeReference returnType, MemberAttributes ma,
            Expression<Func<T, string>> paramsAndName)
        {
            var c = new CodeTypeDelegate()
            {
                Attributes = ma,
                ReturnType = returnType,
            };

            c.Name = CodeDom.GetMethodName<string>(paramsAndName, c.Parameters);

            return c;
        }

        public static CodeTypeDelegate Delegate<T>(MemberAttributes ma,
            Expression<Func<T, string>> paramsAndName)
        {
            var c = new CodeTypeDelegate()
            {
                Attributes = ma
            };

            c.Name = CodeDom.GetMethodName<string>(paramsAndName, c.Parameters);

            return c;
        }
    }
}
