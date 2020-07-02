using System;
using System.CodeDom;
using System.Linq.Expressions;
using LinqToCodedom.Visitors;

namespace LinqToCodedom.Generator
{
    public static partial class Define
    {
        public static CodeMemberField Const<T, TResult>(MemberAttributes ma, string name,
            Expression<Func<T, TResult>> exp)
        {
            var c = new CodeMemberField
            {
                Name = name,
                Attributes = ma | MemberAttributes.Const,
                Type = new CodeTypeReference(typeof(TResult)),
                InitExpression = new CodeExpressionVisitor(new VisitorContext()).Visit(exp),
            };

            return c;
        }

        public static CodeMemberField Const<TResult>(MemberAttributes ma, string name,
            Expression<Func<TResult>> exp)
        {
            var c = new CodeMemberField
            {
                Name = name,
                Attributes = ma | MemberAttributes.Const,
                Type = new CodeTypeReference(typeof(TResult)),
                InitExpression = new CodeExpressionVisitor(new VisitorContext()).Visit(exp),
            };

            return c;
        }
    }
}
