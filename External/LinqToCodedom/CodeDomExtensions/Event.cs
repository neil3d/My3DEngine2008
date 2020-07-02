using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;

namespace LinqToCodedom.Extensions
{
    public static class CodeMemberEventExtensions
    {
        public static CodeMemberEvent Implements(this CodeMemberEvent @event, Type t)
        {
            if ((@event.Attributes & MemberAttributes.Private) == MemberAttributes.Private)
                @event.PrivateImplementationType = new CodeTypeReference(t);
            else
                @event.ImplementationTypes.Add(t);
            return @event;
        }

        public static CodeMemberEvent Implements(this CodeMemberEvent @event, string t)
        {
            if ((@event.Attributes & MemberAttributes.Private) == MemberAttributes.Private)
                @event.PrivateImplementationType = new CodeTypeReference(t);
            else
                @event.ImplementationTypes.Add(t);
            return @event;
        }

        public static CodeMemberEvent Implements(this CodeMemberEvent @event, CodeTypeReference t)
        {
            if ((@event.Attributes & MemberAttributes.Private) == MemberAttributes.Private)
                @event.PrivateImplementationType = t;
            else
                @event.ImplementationTypes.Add(t);
            return @event;
        }
    }
}
