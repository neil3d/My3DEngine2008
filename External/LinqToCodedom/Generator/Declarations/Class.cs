using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.CodeDom;

namespace LinqToCodedom.Generator
{
    public static partial class Define
    {
        #region Class declaration

        public static CodeTypeDeclaration Class(string className)
        {
            return new CodeTypeDeclaration(className);
        }

        public static CodeTypeDeclaration Class(string className, TypeAttributes attributes)
        {
            return new CodeTypeDeclaration(className)
            {
                TypeAttributes = attributes
            };
        }

        public static CodeTypeDeclaration Class(string className, TypeAttributes attributes, bool partial)
        {
            var c = new CodeTypeDeclaration(className)
            {
                TypeAttributes = attributes,
                IsPartial = partial
            };

            return c;
        }

        #endregion

        #region Struct

        public static CodeTypeDeclaration Struct(string structName)
        {
            return new CodeTypeDeclaration(structName) { IsStruct = true };
        }

        public static CodeTypeDeclaration Struct(string structName, MemberAttributes attributes)
        {
            return new CodeTypeDeclaration(structName)
            {
                Attributes = attributes,
                IsStruct = true
            };
        }

        public static CodeTypeDeclaration Struct(string structName, MemberAttributes attributes, bool partial)
        {
            var c = new CodeTypeDeclaration(structName)
            {
                Attributes = attributes,
                IsPartial = partial,
                IsStruct = true
            };

            return c;
        }

        #endregion

        #region Interface

        public static CodeTypeDeclaration Interface(string interfaceName)
        {
            return new CodeTypeDeclaration(interfaceName) { IsInterface = true };
        }

        public static CodeTypeDeclaration Interface(string interfaceName, MemberAttributes attributes)
        {
            return new CodeTypeDeclaration(interfaceName)
            {
                Attributes = attributes,
                IsInterface = true
            };
        }

        #endregion

        #region Enum

        public static CodeTypeDeclaration Enum(string enumName)
        {
            return new CodeTypeDeclaration(enumName) { IsEnum = true };
        }

        public static CodeTypeDeclaration Enum(string enumName, MemberAttributes attributes)
        {
            return new CodeTypeDeclaration(enumName)
            {
                Attributes = attributes,
                IsEnum = true
            };
        }

        #endregion

    }
}
