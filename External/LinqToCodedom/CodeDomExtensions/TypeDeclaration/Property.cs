using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using LinqToCodedom.Generator;

namespace LinqToCodedom.Extensions
{
    public static partial class CodeTypeDeclarationExtensions
    {
        #region Properties

        public static CodeTypeDeclaration AddProperty(this CodeTypeDeclaration classCode, CodeMemberProperty property)
        {
            classCode.Members_Add(property);

            return classCode;
        }

        public static CodeMemberProperty AddProperty(this CodeTypeDeclaration classCode,
            string propertyType, MemberAttributes ma, string name, string fieldName)
        {
            var prop = Define.Property(propertyType, CorrectAttributes(classCode, ma), name, fieldName);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddProperty(this CodeTypeDeclaration classCode,
            Type propertyType, MemberAttributes ma, string name, string fieldName)
        {
            var prop = Define.Property(propertyType, CorrectAttributes(classCode, ma), name, fieldName);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddProperty(this CodeTypeDeclaration classCode,
            string propertyType, MemberAttributes ma, string name)
        {
            var prop = Define.Property(propertyType, CorrectAttributes(classCode, ma), name);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddProperty(this CodeTypeDeclaration classCode,
            CodeTypeReference propertyType, MemberAttributes ma, string name)
        {
            var prop = Define.Property(propertyType, CorrectAttributes(classCode, ma), name);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddProperty(this CodeTypeDeclaration classCode,
            Type propertyType, MemberAttributes ma, string name)
        {
            var prop = Define.Property(propertyType, CorrectAttributes(classCode, ma), name);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddProperty(this CodeTypeDeclaration classCode,
            Type propertyType, MemberAttributes ma, string name,
                CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var prop = Define.Property(propertyType, CorrectAttributes(classCode, ma), name, getStatements, setStatements);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddProperty(this CodeTypeDeclaration classCode,
            CodeTypeReference propertyType, MemberAttributes ma, string name,
                CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var prop = Define.Property(propertyType, CorrectAttributes(classCode, ma), name, getStatements, setStatements);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddProperty(this CodeTypeDeclaration classCode,
            string propertyType, MemberAttributes ma, string name,
                CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var prop = Define.Property(propertyType, CorrectAttributes(classCode, ma), name, getStatements, setStatements);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddGetProperty(this CodeTypeDeclaration classCode,
            Type propertyType, MemberAttributes ma, string name,
            params CodeStatement[] statements)
        {
            var prop = Define.GetProperty(propertyType, CorrectAttributes(classCode, ma), name, statements);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddGetProperty(this CodeTypeDeclaration classCode,
            string propertyType, MemberAttributes ma, string name,
            params CodeStatement[] statements)
        {
            var prop = Define.GetProperty(propertyType, CorrectAttributes(classCode, ma), name, statements);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddGetProperty(this CodeTypeDeclaration classCode,
            CodeTypeReference propertyType, MemberAttributes ma, string name,
            params CodeStatement[] statements)
        {
            var prop = Define.GetProperty(propertyType, CorrectAttributes(classCode, ma), name, statements);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddGetProperty(this CodeTypeDeclaration classCode,
           Type propertyType, MemberAttributes ma, string name, string fieldName)
        {
            var prop = Define.GetProperty(propertyType, CorrectAttributes(classCode, ma), name, fieldName);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddGetProperty(this CodeTypeDeclaration classCode,
           string propertyType, MemberAttributes ma, string name)
        {
            var prop = Define.GetProperty(propertyType, CorrectAttributes(classCode, ma), name);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty AddGetProperty(this CodeTypeDeclaration classCode,
           string propertyType, MemberAttributes ma, string name, string fieldName)
        {
            var prop = Define.GetProperty(propertyType, CorrectAttributes(classCode, ma), name, fieldName);

            classCode.Members_Add(prop);

            return prop;
        }

        public static CodeMemberProperty Property(this CodeTypeDeclaration classCode,
            Type propertyType, MemberAttributes ma, string name,
            CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var prop = Define.Property(propertyType, CorrectAttributes(classCode, ma), name, getStatements, setStatements);

            classCode.Members_Add(prop);

            return prop;
        }

        #endregion

    }
}
