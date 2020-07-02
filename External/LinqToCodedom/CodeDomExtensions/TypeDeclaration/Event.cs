using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using LinqToCodedom.Generator;

namespace LinqToCodedom.Extensions
{
    public static partial class CodeTypeDeclarationExtensions
    {
        #region Event

        public static CodeTypeDeclaration AddEvents(this CodeTypeDeclaration classCode, params CodeMemberEvent[] @event)
        {
            classCode.Members_AddRange(@event);

            return classCode;
        }

        public static CodeMemberEvent AddEvent(this CodeTypeDeclaration classCode,
            string delegateType, MemberAttributes ma, string name)
        {
            var ev = Define.Event(delegateType, CorrectAttributes(classCode, ma), name);

            classCode.Members_Add(ev);

            return ev;
        }

        public static CodeMemberEvent AddEvent(this CodeTypeDeclaration classCode,
            Type delegateType, MemberAttributes ma, string name)
        {
            var ev = Define.Event(delegateType, CorrectAttributes(classCode, ma), name);

            classCode.Members_Add(ev);

            return ev;
        }

        #endregion

    }
}
