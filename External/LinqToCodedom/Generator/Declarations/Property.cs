using System;
using System.CodeDom;
using System.Linq.Expressions;
using LinqToCodedom.CodeDomPatterns;

namespace LinqToCodedom.Generator
{
    public static partial class Define
    {
        public static CodeMemberProperty GetProperty(Type propertyType, MemberAttributes ma, string name,
            params CodeStatement[] statements)
        {
            return GetProperty(new CodeTypeReference(propertyType), ma, name, statements);
        }

        public static CodeMemberProperty GetProperty(CodeTypeReference propertyType, MemberAttributes ma, string name,
            params CodeStatement[] statements)
        {
            var c = new CodeMemberProperty
            {
                Name = name,
                Attributes = ma,
                Type = propertyType,
                HasGet = true
            };

            if (statements != null)
                c.GetStatements.AddRange(statements);

            return c;
        }

        public static CodeDefaultProperty GetProperty<T>(CodeTypeReference propertyType, MemberAttributes ma,
            Expression<Func<T, string>> paramsAndName, bool indexer,
            params CodeStatement[] statements)
        {
            var c = new CodeMemberProperty
            {
                Attributes = ma,
                Type = propertyType,
                HasGet = true
            };
            CodeParameterDeclarationExpressionCollection parameters = new CodeParameterDeclarationExpressionCollection();
            c.Name = CodeDom.GetMethodName<string>(paramsAndName, parameters);

            if (statements != null)
                c.GetStatements.AddRange(statements);

            return new CodeDefaultProperty(c, parameters, indexer);
        }

        public static CodeDefaultProperty GetProperty<T, T2>(CodeTypeReference propertyType, MemberAttributes ma,
            Expression<Func<T, T2, string>> paramsAndName, bool indexer,
            params CodeStatement[] statements)
        {
            var c = new CodeMemberProperty
            {
                Attributes = ma,
                Type = propertyType,
                HasGet = true
            };
            CodeParameterDeclarationExpressionCollection parameters = new CodeParameterDeclarationExpressionCollection();
            c.Name = CodeDom.GetMethodName<string>(paramsAndName, parameters);

            if (statements != null)
                c.GetStatements.AddRange(statements);

            return new CodeDefaultProperty(c, parameters, indexer);
        }

        public static CodeDefaultProperty GetProperty<T, T2, T3>(CodeTypeReference propertyType, MemberAttributes ma,
            Expression<Func<T, T2, T3, string>> paramsAndName, bool indexer,
            params CodeStatement[] statements)
        {
            var c = new CodeMemberProperty
            {
                Attributes = ma,
                Type = propertyType,
                HasGet = true
            };
            CodeParameterDeclarationExpressionCollection parameters = new CodeParameterDeclarationExpressionCollection();
            c.Name = CodeDom.GetMethodName<string>(paramsAndName, parameters);

            if (statements != null)
                c.GetStatements.AddRange(statements);

            return new CodeDefaultProperty(c, parameters, indexer);
        }

        public static CodeMemberProperty GetProperty(string propertyType, MemberAttributes ma, string name,
            params CodeStatement[] statements)
        {
            return GetProperty(new CodeTypeReference(propertyType), ma, name, statements);
        }

        public static CodeMemberProperty GetProperty(Type propertyType, MemberAttributes ma, string name,
            string fieldName)
        {
            return GetProperty(propertyType, ma, name,
                Emit.@return(() => CodeDom.@this.Field<object>(fieldName)));
        }

        public static CodeMemberProperty GetProperty(string propertyType, MemberAttributes ma, string name,
            string fieldName)
        {
            return GetProperty(propertyType, ma, name,
                Emit.@return(() => CodeDom.@this.Field<object>(fieldName)));
        }

        public static CodeMemberProperty Property(Type propertyType, MemberAttributes ma, string name,
            CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            return Property(new CodeTypeReference(propertyType), ma, name, getStatements, setStatements);
        }
        
        public static CodeMemberProperty Property(CodeTypeReference propertyType, MemberAttributes ma, string name,
            CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var c = new CodeMemberProperty
            {
                Name = name,
                Attributes = ma,
                Type = propertyType,
                HasGet = true,
                HasSet = true
            };

            if (getStatements != null)
                c.GetStatements.AddRange(getStatements);

            if (setStatements != null)
                c.SetStatements.AddRange(setStatements);

            return c;
        }

        public static CodeMemberProperty Property(Type propertyType, MemberAttributes ma, string name,
            string fieldName)
        {
            return Property(propertyType,ma, name,
                CodeDom.CombineStmts(Emit.@return(() => CodeDom.@this.Field<object>(fieldName))),
                Emit.assignField(fieldName, (SetValueRef<object> value) => value));
        }

        public static CodeMemberProperty Property(Type propertyType, MemberAttributes ma, string name)
        {
            return Property(propertyType, ma, name, null, null);
        }

        public static CodeMemberProperty Property(CodeTypeReference propertyType, MemberAttributes ma, string name)
        {
            return Property(propertyType, ma, name, null, null);
        }

        public static CodeMemberProperty Property(string propertyType, MemberAttributes ma, string name)
        {
            return Property(propertyType, ma, name, null, null);
        }

        public static CodeMemberProperty Property(string propertyType, MemberAttributes ma, string name,
            CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            return Property(new CodeTypeReference(propertyType), ma, name, getStatements, setStatements);
        }

        public static CodeMemberProperty Property(string propertyType, MemberAttributes ma, string name,
            string fieldName)
        {
            return Property(propertyType, ma, name,
                CodeDom.CombineStmts(Emit.@return(() => CodeDom.@this.Field<object>(fieldName))),
                Emit.assignField(fieldName, (SetValueRef<object> value) => value));
        }

        public static CodeDefaultProperty Property<T>(CodeTypeReference propertyType, MemberAttributes ma,
            Expression<Func<T, string>> paramsAndName, bool indexer,
            CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var c = new CodeMemberProperty
            {
                Attributes = ma,
                Type = propertyType,
                HasGet = true,
                HasSet = true
            };

            if (getStatements != null)
                c.GetStatements.AddRange(getStatements);

            if (setStatements != null)
                c.SetStatements.AddRange(setStatements);

            CodeParameterDeclarationExpressionCollection parameters = new CodeParameterDeclarationExpressionCollection();
            c.Name = CodeDom.GetMethodName<string>(paramsAndName, parameters);

            return new CodeDefaultProperty(c, parameters, indexer);
        }

        public static CodeDefaultProperty Property<T, T2>(CodeTypeReference propertyType, MemberAttributes ma,
            Expression<Func<T, T2, string>> paramsAndName, bool indexer,
            CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var c = new CodeMemberProperty
            {
                Attributes = ma,
                Type = propertyType,
                HasGet = true,
                HasSet = true
            };

            if (getStatements != null)
                c.GetStatements.AddRange(getStatements);

            if (setStatements != null)
                c.SetStatements.AddRange(setStatements);

            CodeParameterDeclarationExpressionCollection parameters = new CodeParameterDeclarationExpressionCollection();
            c.Name = CodeDom.GetMethodName<string>(paramsAndName, parameters);

            return new CodeDefaultProperty(c, parameters, indexer);
        }

        public static CodeDefaultProperty Property<T, T2, T3>(CodeTypeReference propertyType, MemberAttributes ma,
            Expression<Func<T, T2, T3, string>> paramsAndName, bool indexer,
            CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var c = new CodeMemberProperty
            {
                Attributes = ma,
                Type = propertyType,
                HasGet = true,
                HasSet = true
            };

            if (getStatements != null)
                c.GetStatements.AddRange(getStatements);

            if (setStatements != null)
                c.SetStatements.AddRange(setStatements);

            CodeParameterDeclarationExpressionCollection parameters = new CodeParameterDeclarationExpressionCollection();
            c.Name = CodeDom.GetMethodName<string>(paramsAndName, parameters);

            return new CodeDefaultProperty(c, parameters, indexer);
        }
    }
}
