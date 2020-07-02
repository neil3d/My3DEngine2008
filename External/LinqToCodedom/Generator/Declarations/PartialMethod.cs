using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using System.Linq.Expressions;
using LinqToCodedom.CodeDomPatterns;

namespace LinqToCodedom.Generator
{
    public static partial class Define
    {

        public static CodeTypeMember PartialMethod<T>(MemberAttributes ma,
           Expression<Func<T, string>> paramsAndName, params CodeStatement[] statements)
        {
            return new CodePartialMethod(Method(ma, paramsAndName, statements));
        }

        public static CodeTypeMember PartialMethod<T, T2>(MemberAttributes ma,
           Expression<Func<T, T2, string>> paramsAndName, params CodeStatement[] statements)
        {
            return new CodePartialMethod(Method(ma, paramsAndName, statements));
        }

        public static CodeTypeMember PartialMethod<T, T2, T3>(MemberAttributes ma,
           Expression<Func<T, T2, T3, string>> paramsAndName, params CodeStatement[] statements)
        {
            return new CodePartialMethod(Method(ma, paramsAndName, statements));
        }

        public static CodeTypeMember PartialMethod(MemberAttributes ma,
            Expression<Func<string>> paramsAndName, params CodeStatement[] statements)
        {
            return new CodePartialMethod(Method(ma, paramsAndName, statements));
        }

    }
}
