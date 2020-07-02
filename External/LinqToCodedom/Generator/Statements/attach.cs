using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Expressions;
using System.CodeDom;
using LinqToCodedom.Visitors;

namespace LinqToCodedom.Generator
{
    public static partial class Emit
    {
        public static CodeAttachEventStatement attachDelegate(Base owner, string eventName, 
            Base target, string methodName)
        {
            return new CodeAttachEventStatement(
                CodeDom.GetTargetObject(owner), eventName, 
                new CodeMethodReferenceExpression(CodeDom.GetTargetObject(target), methodName)
            );
        }

        public static CodeRemoveEventStatement detachDelegate(Base owner, string eventName,
            Base target, string methodName)
        {
            return new CodeRemoveEventStatement(
                CodeDom.GetTargetObject(owner), eventName,
                new CodeMethodReferenceExpression(CodeDom.GetTargetObject(target), methodName)
            );
        }

        public static CodeAttachEventStatement attachDelegate(Base owner, string eventName,
            string staticMethodName)
        {
            return new CodeAttachEventStatement(
                CodeDom.GetTargetObject(owner), eventName,
                new CodeMethodReferenceExpression(null, staticMethodName)
            );
        }

        public static CodeRemoveEventStatement detachDelegate(Base owner, string eventName,
            string staticMethodName)
        {
            return new CodeRemoveEventStatement(
                CodeDom.GetTargetObject(owner), eventName,
                new CodeMethodReferenceExpression(null, staticMethodName)
            );
        }
    }
}
