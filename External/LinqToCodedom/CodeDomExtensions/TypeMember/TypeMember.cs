using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using LinqToCodedom.Generator;
using System.Linq.Expressions;

namespace LinqToCodedom.Extensions
{
    public static partial class CodeTypeMemberExtensions
    {
        #region CodeTypeMember

        internal static CodeTypeDeclaration GetDeclaration(this CodeTypeMember member)
        {
            if (member is CodeTypeDeclaration)
                return member as CodeTypeDeclaration;
            else
                return member.UserData["parent"] as CodeTypeDeclaration;
        }

        public static T Comment<T>(this T member, params CodeCommentStatement[] comments)
            where T : CodeTypeMember
        {
            member.Comments.AddRange(comments);
            return member;
        }

        public static T Comment<T>(this T member, params string[] comments)
            where T : CodeTypeMember
        {
            member.Comments.AddRange(comments.Select((c) => new CodeCommentStatement(c)).ToArray());
            return member;
        }

        public static T Document<T>(this T member, params string[] comments)
            where T : CodeTypeMember
        {
            member.Comments.AddRange(comments.Select((c) => new CodeCommentStatement(c, true)).ToArray());
            return member;
        }

        public static T AddAttribute<T>(this T member, params CodeAttributeDeclaration[] attributes)
            where T : CodeTypeMember
        {
            member.CustomAttributes.AddRange(attributes);
            return member;
        }

        public static T AddAttribute<T>(this T member, params Type[] attributes)
            where T : CodeTypeMember
        {
            member.CustomAttributes.AddRange(attributes.Select((a) => new CodeAttributeDeclaration(new CodeTypeReference(a))).ToArray());
            return member;
        }

        public static T AddAttribute<T>(this T member, params string[] attributes)
            where T : CodeTypeMember
        {
            member.CustomAttributes.AddRange(attributes.Select((a) => new CodeAttributeDeclaration(a)).ToArray());
            return member;
        }

        public static CodeTypeDeclaration AddClass(this CodeTypeMember member, string className)
        {
            var c = Define.Class(className);

            member.GetDeclaration().GetNamespace().Types.Add(c);

            return c;
        }

        public static T StartDirective<T>(this T member, CodeDirective directive)
            where T : CodeTypeMember
        {
            member.StartDirectives.Add(directive);
            return member;
        }

        public static T StartDirective<T>(this T member, string title)
            where T : CodeTypeMember
        {
            member.StartDirectives.Add(new CodeRegionDirective(CodeRegionMode.Start, title));
            return member;
        }

        public static T EndDirective<T>(this T member, CodeDirective directive)
            where T : CodeTypeMember
        {
            member.EndDirectives.Add(directive);
            return member;
        }

        public static T EndDirective<T>(this T member)
            where T : CodeTypeMember
        {
            member.EndDirectives.Add(new CodeRegionDirective(CodeRegionMode.End, string.Empty));
            return member;
        }

        public static T WrapRegion<T>(this T member, string title)
            where T : CodeTypeMember
        {
            member.StartDirectives.Add(new CodeRegionDirective(CodeRegionMode.Start, title));
            member.EndDirectives.Add(new CodeRegionDirective(CodeRegionMode.End, string.Empty));
            return member;
        }

        public static T LineMark<T>(this T member, string file, int line)
            where T : CodeTypeMember
        {
            member.LinePragma = new CodeLinePragma(file, line);

            return member;
        }
        #endregion

    }
}
