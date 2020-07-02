using System;
using System.CodeDom;

namespace LinqToCodedom.Extensions
{
    public static class CodeMemberPropertyExtensions
    {
        #region Property

        public static CodeMemberProperty Implements(this CodeMemberProperty property, CodeTypeReference t)
        {
            if ((property.Attributes & MemberAttributes.Private) == MemberAttributes.Private)
                property.PrivateImplementationType = t;
            else
                property.ImplementationTypes.Add(t);
            return property;
        }

        public static CodeMemberProperty Implements(this CodeMemberProperty property, Type t)
        {
            if ((property.Attributes & MemberAttributes.Private) == MemberAttributes.Private)
                property.PrivateImplementationType = new CodeTypeReference(t);
            else
                property.ImplementationTypes.Add(t);
            return property;
        }

        public static CodeMemberProperty Implements(this CodeMemberProperty property, string t)
        {
            if ((property.Attributes & MemberAttributes.Private) == MemberAttributes.Private)
                property.PrivateImplementationType = new CodeTypeReference(t);
            else
                property.ImplementationTypes.Add(t);
            return property;
        }

        #endregion
    }
}
