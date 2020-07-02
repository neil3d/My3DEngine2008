using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using System.Linq.Expressions;

namespace LinqToCodedom.Generator
{
    public static partial class Define
    {
        public static CodeAttributeDeclaration Attribute(CodeTypeReference type)
        {
            return new CodeAttributeDeclaration(type);
        }

        public static CodeAttributeDeclaration Attribute(Type type)
        {
            return new CodeAttributeDeclaration(new CodeTypeReference(type));
        }

        public static CodeAttributeDeclaration Attribute(string type)
        {
            return new CodeAttributeDeclaration(type);
        }

        public static CodeAttributeDeclaration Attribute<T>(CodeTypeReference type,
            Expression<Func<T>> anonymType)
        {
            var c = new CodeAttributeDeclaration(type);
            InitAttributeArgs(anonymType, c);
            return c;
        }

        public static CodeAttributeDeclaration Attribute(CodeTypeReference type,
            params object[] args)
        {
            var c = new CodeAttributeDeclaration(type);
            c.Arguments.AddRange(args.Select((a) => new CodeAttributeArgument(new CodePrimitiveExpression(a))).ToArray());
            return c;
        }

        public static CodeAttributeDeclaration Attribute<T>(string type,
            Expression<Func<T>> anonymType)
        {
            var c = new CodeAttributeDeclaration(type);

            InitAttributeArgs(anonymType, c);

            return c;
        }

        public static CodeAttributeDeclaration Attribute(string type,
            params object[] args)
        {
            var c = new CodeAttributeDeclaration(type);
            c.Arguments.AddRange(args.Select((a) => new CodeAttributeArgument(new CodePrimitiveExpression(a))).ToArray());
            return c;
        }

        public static CodeAttributeDeclaration Attribute(Type type,
            params object[] args)
        {
            var c = new CodeAttributeDeclaration(new CodeTypeReference(type));
            c.Arguments.AddRange(args.Select((a) => new CodeAttributeArgument(new CodePrimitiveExpression(a))).ToArray());
            return c;
        }

        public static void InitAttributeArgs<T>(Expression<Func<T>> anonymType, CodeAttributeDeclaration c)
        {
            InitAttributeArgs((Expression)anonymType, c);
        }

        private static void InitAttributeArgs(Expression anonymType, CodeAttributeDeclaration c)
        {
            object o = CodeDom.Eval<object>(anonymType);

            foreach (System.Reflection.PropertyInfo pi in
                o.GetType().GetProperties(System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.Instance))
            {
                c.Arguments.Add(new CodeAttributeArgument(pi.Name,
                    LinqToCodedom.Visitors.CodeExpressionVisitor.GetFromPrimitive(pi.GetValue(o, null))
                ));
            }
        }
    }
}
