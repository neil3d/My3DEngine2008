using System;
using System.CodeDom;
using System.Linq.Expressions;
using LinqToCodedom.Visitors;
using LinqToCodedom.CodeDomPatterns;

namespace LinqToCodedom.Generator
{
    public static partial class Define
    {
        public static CodeMemberField Field(MemberAttributes ma, Type fieldType, string name)
        {
            return Field(ma, new CodeTypeReference(fieldType), name);
        }

        public static CodeMemberField Field(MemberAttributes ma, string fieldType, string name)
        {
            return Field(ma, new CodeTypeReference(fieldType), name);
        }

        public static CodeMemberField Field(MemberAttributes ma, CodeTypeReference fieldType, string name)
        {
            return new CodeMemberField
            {
                Name = name,
                Attributes = ma,
                Type = fieldType,
            };
        }

        public static CodeMemberField Field<T, TResult>(MemberAttributes ma, CodeTypeReference fieldType, string name, Expression<Func<T, TResult>> exp)
        {
            var c = new CodeMemberField
            {
                Name = name,
                Attributes = ma,
                Type = fieldType,
                InitExpression = new CodeExpressionVisitor(new VisitorContext()).Visit(exp),
            };

            return c;
        }

        public static CodeMemberField Field<TResult>(MemberAttributes ma, CodeTypeReference fieldType, string name, Expression<Func<TResult>> exp)
        {
            var c = new CodeMemberField
            {
                Name = name,
                Attributes = ma,
                Type = fieldType,
                InitExpression = new CodeExpressionVisitor(new VisitorContext()).Visit(exp),
            };

            return c;
        }

        public static CodeMemberField Field<T, TResult>(MemberAttributes ma, string fieldType, string name, Expression<Func<T, TResult>> exp)
        {
            return Field(ma, new CodeTypeReference(fieldType), name, exp);
        }

        public static CodeMemberField Field<TResult>(MemberAttributes ma, string fieldType, string name, Expression<Func<TResult>> exp)
        {
            return Field(ma, new CodeTypeReference(fieldType), name, exp);
        }

        public static CodeMemberField Field<T, TResult>(MemberAttributes ma, Type fieldType, string name, Expression<Func<T, TResult>> exp)
        {
            return Field(ma, new CodeTypeReference(fieldType), name, exp);
        }

        public static CodeMemberField Field<TResult>(MemberAttributes ma, Type fieldType, string name, Expression<Func<TResult>> exp)
        {
            return Field(ma, new CodeTypeReference(fieldType), name, exp);
        }

        public static CodeMemberField Field<TResult>(string name,
            Expression<Func<TResult>> exp)
        {
            return Field(MemberAttributes.Private, new CodeTypeReference(typeof(TResult)), name, exp);
        }

        public static CodeReadOnlyField ReadOnlyField(MemberAttributes ma, CodeTypeReference fieldType, string name)
        {
            var field = new CodeMemberField
            {
                Name = name,
                Attributes = ma,
                Type = fieldType,
            };

            return new CodeReadOnlyField(field);
        }

        public static CodeReadOnlyField ReadOnlyField<T, TResult>(MemberAttributes ma, CodeTypeReference fieldType, string name, Expression<Func<T, TResult>> exp)
        {
            var c = new CodeMemberField
            {
                Name = name,
                Attributes = ma,
                Type = fieldType,
                InitExpression = new CodeExpressionVisitor(new VisitorContext()).Visit(exp),
            };

            return new CodeReadOnlyField(c);
        }

        public static CodeReadOnlyField ReadOnlyField<TResult>(MemberAttributes ma, CodeTypeReference fieldType, string name, Expression<Func<TResult>> exp)
        {
            var c = new CodeMemberField
            {
                Name = name,
                Attributes = ma,
                Type = fieldType,
                InitExpression = new CodeExpressionVisitor(new VisitorContext()).Visit(exp),
            };

            return new CodeReadOnlyField(c);
        }
    }
}
