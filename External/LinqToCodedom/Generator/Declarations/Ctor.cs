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

        public static CodeConstructor Ctor<T>(Expression<Func<T, MemberAttributes>> paramsAndAccessLevel,
            params CodeStatement[] statements)
        {
            var c = new CodeConstructor();

            c.Attributes = CodeDom.GetMethodName<MemberAttributes>(paramsAndAccessLevel, c.Parameters);
            c.Statements.AddRange(statements);

            return c;
        }

        public static CodeConstructor Ctor(Expression<Func<MemberAttributes>> paramsAndAccessLevel,
            params CodeStatement[] statements)
        {
            var c = new CodeConstructor();

            c.Attributes = CodeDom.GetMethodName<MemberAttributes>(paramsAndAccessLevel, c.Parameters);
            c.Statements.AddRange(statements);

            return c;
        }

        public static CodeConstructor Ctor<T, T2>(Expression<Func<T, T2, MemberAttributes>> paramsAndAccessLevel,
            params CodeStatement[] statements)
        {
            var c = new CodeConstructor();

            c.Attributes = CodeDom.GetMethodName<MemberAttributes>(paramsAndAccessLevel, c.Parameters);
            c.Statements.AddRange(statements);

            return c;
        }

    }
}
