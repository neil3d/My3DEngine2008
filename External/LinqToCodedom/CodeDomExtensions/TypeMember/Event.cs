using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;

namespace LinqToCodedom.Extensions
{
    public static partial class CodeTypeMemberExtensions
    {
        #region Event

        public static CodeTypeDeclaration AddEvents(this CodeTypeMember member, params CodeMemberEvent[] @event)
        {
            var classCode = member.GetDeclaration();

            classCode.AddEvents(@event);

            return classCode;
        }

        public static CodeMemberEvent AddEvent(this CodeTypeMember member,
            string delegateType, MemberAttributes ma, string name)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddEvent(delegateType, ma, name);
        }

        public static CodeMemberEvent AddEvent(this CodeTypeMember member,
            Type delegateType, MemberAttributes ma, string name)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddEvent(delegateType, ma, name);
        }

        #endregion

    }
}
