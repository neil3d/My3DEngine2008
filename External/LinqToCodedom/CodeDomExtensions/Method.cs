using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;

namespace LinqToCodedom.Extensions
{
    public static class CodeMemberMethodExtensions
    {
        
        public static CodeMemberMethod Implements(this CodeMemberMethod method, string t)
        {
            if ((method.Attributes & MemberAttributes.Private) == MemberAttributes.Private)
                method.PrivateImplementationType = new CodeTypeReference(t);
            else
                method.ImplementationTypes.Add(t);

            return method;
        }

        public static CodeMemberMethod Implements(this CodeMemberMethod method, Type t)
        {
            if ((method.Attributes & MemberAttributes.Private) == MemberAttributes.Private)
                method.PrivateImplementationType = new CodeTypeReference(t);
            else
                method.ImplementationTypes.Add(t);

            return method;
        }

        public static CodeMemberMethod Implements(this CodeMemberMethod method, CodeTypeReference t)
        {
            if ((method.Attributes & MemberAttributes.Private) == MemberAttributes.Private)
                method.PrivateImplementationType = t;
            else
                method.ImplementationTypes.Add(t);

            return method;
        }

        public static CodeMemberMethod Generic(this CodeMemberMethod method, string paramName)
        {
            method.TypeParameters.Add(paramName);
            return method;
        }

        public static CodeMemberMethod Generic(this CodeMemberMethod method, string paramName, 
            params string[] constraints)
        {
            var p = new CodeTypeParameter(paramName);
            p.Constraints.AddRange(constraints.Select((t) => new CodeTypeReference(t)).ToArray());
            method.TypeParameters.Add(p);
            return method;
        }

        public static CodeMemberMethod Generic(this CodeMemberMethod method, string paramName,
            params Type[] constraints)
        {
            var p = new CodeTypeParameter(paramName);
            p.Constraints.AddRange(constraints.Select((t) => new CodeTypeReference(t)).ToArray());
            method.TypeParameters.Add(p);
            return method;
        }

        public static CodeMemberMethod Generic(this CodeMemberMethod method, string paramName,
            params CodeTypeReference[] constraints)
        {
            var p = new CodeTypeParameter(paramName);
            p.Constraints.AddRange(constraints);
            method.TypeParameters.Add(p);
            return method;
        }

        public static CodeMemberMethod Generic(this CodeMemberMethod method, string paramName,
            bool hasConstructor, params Type[] constraints)
        {
            var p = new CodeTypeParameter(paramName) {HasConstructorConstraint = hasConstructor};
            p.Constraints.AddRange(constraints.Select((t) => new CodeTypeReference(t)).ToArray());
            method.TypeParameters.Add(p);
            return method;
        }

        public static CodeMemberMethod Generic(this CodeMemberMethod method, string paramName,
            bool hasConstructor, params string[] constraints)
        {
            var p = new CodeTypeParameter(paramName) { HasConstructorConstraint = hasConstructor };
            p.Constraints.AddRange(constraints.Select((t) => new CodeTypeReference(t)).ToArray());
            method.TypeParameters.Add(p);
            return method;
        }

        public static CodeMemberMethod Generic(this CodeMemberMethod method, string paramName,
            bool hasConstructor, params CodeTypeReference[] constraints)
        {
            var p = new CodeTypeParameter(paramName) {HasConstructorConstraint = hasConstructor};
            p.Constraints.AddRange(constraints);
            method.TypeParameters.Add(p);
            return method;
        }

        //public static CodeMemberMethod Partial(this CodeMemberMethod method, bool partial)
        //{
        //    method.UserData["linq2codedom:partial"] = partial;
        //    return method;
        //}
    }
}
