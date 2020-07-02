using System;
using System.CodeDom;
using LinqToCodedom.CodeDomPatterns;

namespace LinqToCodedom.Generator
{
    public static partial class Define
    {
        #region Event

        public static CodeMemberEvent Event(Type delegateType, MemberAttributes ma, string name)
        {
            return Event(new CodeTypeReference(delegateType), ma, name);
        }

        public static CodeMemberEvent Event(string delegateType, MemberAttributes ma, string name)
        {
            return Event(new CodeTypeReference(delegateType), ma, name);
        }

        public static CodeMemberEvent Event(CodeTypeReference delegateType, MemberAttributes ma, string name)
        {
            var c = new CodeMemberEvent
            {
                Name = name,
                Attributes = ma,
                Type = delegateType,
            };

            return c;
        }

        public static CodeCustomEvent Event(CodeTypeReference delegateType, MemberAttributes ma, string name,
            CodeMemberProperty add, CodeMemberProperty remove)
        {
            var c = new CodeMemberEvent
            {
                Name = name,
                Attributes = ma,
                Type = delegateType,
            };

            return new CodeCustomEvent(c, add, remove) ;
        }

        public static CodeCustomEvent Event(CodeTypeReference delegateType, MemberAttributes ma, string name,
            CodeMemberProperty add, CodeMemberProperty remove, CodeMemberMethod raise)
        {
            var c = new CodeMemberEvent
            {
                Name = name,
                Attributes = ma,
                Type = delegateType,
            };

            return new CodeCustomEvent(c, add, remove, raise);
        }

        #endregion
    }
}
