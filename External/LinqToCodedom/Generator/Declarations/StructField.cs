using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using LinqToCodedom.Visitors;
using System.Linq.Expressions;

namespace LinqToCodedom.Generator
{
    public static partial class Define
    {
        public static CodeMemberField StructField(string name)
        {
            return new CodeMemberField()
            {
                Name = name,
            };
        }

        public static CodeMemberField StructField<T, TResult>(string name,
            Expression<Func<T, TResult>> exp)
        {
            var c = new CodeMemberField()
            {
                Name = name,
                InitExpression = new CodeExpressionVisitor(new VisitorContext()).Visit(exp),
            };

            return c;
        }

        public static CodeMemberField StructField<TResult>(string name,
            Expression<Func<TResult>> exp)
        {
            var c = new CodeMemberField()
            {
                Name = name,
                InitExpression = new CodeExpressionVisitor(new VisitorContext()).Visit(exp),
            };

            return c;
        }
    }
}
