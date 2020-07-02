using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using System.Linq.Expressions;

namespace LinqToCodedom.Extensions
{
    public static partial class CodeTypeMemberExtensions
    {
        #region Ctor

        public static CodeTypeMember AddCtor(this CodeTypeMember member, CodeConstructor ctor)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddCtor(ctor);
        }

        public static CodeConstructor AddCtor<T>(this CodeTypeMember member,
            Expression<Func<T, MemberAttributes>> paramsAndAccessLevel,
            params CodeStatement[] statements)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddCtor(paramsAndAccessLevel, statements);
        }

        public static CodeConstructor AddCtor(this CodeTypeMember member,
            Expression<Func<MemberAttributes>> paramsAndAccessLevel,
            params CodeStatement[] statements)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddCtor(paramsAndAccessLevel, statements);
        }

        #endregion

    }
}
