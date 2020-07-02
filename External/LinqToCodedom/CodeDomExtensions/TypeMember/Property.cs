using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;

namespace LinqToCodedom.Extensions
{
    public static partial class CodeTypeMemberExtensions
    {
        #region Properties

        public static CodeTypeDeclaration AddProperty(this CodeTypeMember member, CodeMemberProperty property)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddProperty(property);
        }

        public static CodeMemberProperty AddProperty(this CodeTypeMember member,
            string propertyType, MemberAttributes ma, string name, string fieldName)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddProperty(propertyType, ma, name, fieldName);
        }

        public static CodeMemberProperty AddProperty(this CodeTypeMember member,
            Type propertyType, MemberAttributes ma, string name, string fieldName)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddProperty(propertyType, ma, name, fieldName);
        }

        public static CodeMemberProperty AddProperty(this CodeTypeMember member,
            string propertyType, MemberAttributes ma, string name)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddProperty(propertyType, ma, name);
        }

        public static CodeMemberProperty AddProperty(this CodeTypeMember member,
            Type propertyType, MemberAttributes ma, string name)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddProperty(propertyType, ma, name);
        }

        public static CodeMemberProperty AddProperty(this CodeTypeMember member,
            CodeTypeReference propertyType, MemberAttributes ma, string name)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddProperty(propertyType, ma, name);
        }

        public static CodeMemberProperty AddProperty(this CodeTypeMember member,
            Type propertyType, MemberAttributes ma, string name,
                CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddProperty(propertyType, ma, name, getStatements, setStatements);
        }

        public static CodeMemberProperty AddProperty(this CodeTypeMember member,
            string propertyType, MemberAttributes ma, string name,
                CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddProperty(propertyType, ma, name, getStatements, setStatements);
        }

        public static CodeMemberProperty AddGetProperty(this CodeTypeMember member,
            Type propertyType, MemberAttributes ma, string name,
            params CodeStatement[] statements)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddGetProperty(propertyType, ma, name, statements);
        }

        public static CodeMemberProperty AddGetProperty(this CodeTypeMember member,
            string propertyType, MemberAttributes ma, string name,
            params CodeStatement[] statements)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddGetProperty(propertyType, ma, name, statements);
        }

        public static CodeMemberProperty AddGetProperty(this CodeTypeMember member,
           Type propertyType, MemberAttributes ma, string name, string fieldName)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddGetProperty(propertyType, ma, name, fieldName);
        }

        public static CodeMemberProperty AddGetProperty(this CodeTypeMember member,
            string propertyType, MemberAttributes ma, string name)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddGetProperty(propertyType, ma, name);
        }

        public static CodeMemberProperty Property(this CodeTypeMember member,
            Type propertyType, MemberAttributes ma, string name,
            CodeStatement[] getStatements, params CodeStatement[] setStatements)
        {
            var classCode = member.GetDeclaration();

            return classCode.AddProperty(propertyType, ma, name, getStatements, setStatements);
        }

        #endregion

    }
}
