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
        #region Fields

        public static CodeTypeDeclaration AddFields(this CodeTypeMember member, params CodeMemberField[] field)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddFields(field);
        }

        public static CodeMemberField AddField(this CodeTypeMember member,
            Type fieldType, MemberAttributes ma, string name)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddField(fieldType, ma, name);
        }

        public static CodeMemberField AddField(this CodeTypeMember member,
            Type fieldType, string name)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddField(fieldType, MemberAttributes.Private, name);
        }

        public static CodeMemberField AddField<T>(this CodeTypeMember member,
            Type fieldType, string name, Expression<Func<T>> exp)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddField(fieldType, MemberAttributes.Private, name, exp);
        }

        #endregion

    }
}
