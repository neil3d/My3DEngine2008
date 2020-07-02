using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using System.Reflection;
using System.Collections.ObjectModel;

namespace NexusEngineExtension
{
    /// <summary>
    /// Contains common functionalities used by SerializableDataComposer and SerializableDataDecomposer components.
    /// </summary>
    public abstract class SerializableDataController
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        public SerializableDataController()
        {
        }

        #endregion

        #region Properties

        public SerializableData _serializableDataInfo = new SerializableData();
        /// <summary>
        /// Gets the composed SerializableData
        /// </summary>
        public SerializableData SerializableDataInfo
        {
            get { return _serializableDataInfo; }
            set { _serializableDataInfo = value; }
        }

        #endregion

        #region Protected Functions

        /// <summary>
        /// Returns the XmlClassSerializable attribute of the class type if the class type is serializable.
        /// A class type is serializable if it has a XmlClassSerializable attribute.
        /// </summary>
        /// <param name="type">Class type to check.</param>
        /// <returns>XmlClassSerializable attribute.</returns>
        protected XmlClassSerializable GetXmlClassSerializableAttribute(Type type)
        {
            object[] attributes = type.GetCustomAttributes(typeof(XmlClassSerializable), true);
            if (attributes.GetLength(0) != 1)
                return null;

            ConstructorInfo[] constructors = type.GetConstructors();
            if (constructors.Length == 0 && !type.IsValueType)
                return null;

            foreach (ConstructorInfo constructor in constructors)
                if (constructor.GetParameters().Length == 0)
                    return attributes[0] as XmlClassSerializable;

            return attributes[0] as XmlClassSerializable;
        }

        /// <summary>
        /// Returns the XmlFieldSerializable attribute of the field type if the field type is serializable.
        /// A field type is serializable if it his a XmlFieldSerializable attribute.
        /// </summary>
        /// <param name="type">Field type to check.</param>
        /// <returns>XmlFieldSerializable attribute.</returns>
        protected XmlFieldSerializable GetXmlFieldSerializableAttribute(Type type)
        {
            object[] attributes = type.GetCustomAttributes(typeof(XmlFieldSerializable), true);
            if (attributes.GetLength(0) != 1)
                return null;

            return attributes[0] as XmlFieldSerializable;
        }

        /// <summary>
        /// Returns the XmlFieldSerializable attribute of the field type if the field type is serializable.
        /// A field type is serializable if it has a XmlFieldSerializable.
        /// </summary>
        /// <param name="memberInfo">MemberInfo type to check</param>
        /// <returns>XmlFieldSerializable attribute.</returns>
        protected XmlFieldSerializable GetXmlFieldSerializableAttribute(MemberInfo memberInfo)
        {
            object[] attributes = memberInfo.GetCustomAttributes(typeof(XmlFieldSerializable), true);
            if (attributes.GetLength(0) != 1)
                return null;

            return attributes[0] as XmlFieldSerializable;
        }

        /// <summary>
        /// Checks if the object inherits from System.Collection.ICollection.
        /// </summary>
        /// <param name="data">Data to check.</param>
        /// <returns>True if the data derives from System.Collection.ICollection.</returns>
        protected bool IsCollection(object data)
        {
            System.Collections.ICollection collection = data as System.Collections.ICollection;
            if (collection == null)
                return false;

            return true;
        }

        /// <summary>
        /// Checks if the type his System.Collection.ICollection interface.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <returns>True if the type has System.Collection.ICollection interface.</returns>
        protected bool IsCollection(Type type)
        {
            if (type == null)
                return false;

            Type InterfaceType = type.GetInterface("System.Collections.ICollection");
            if (InterfaceType == null)
                return false;

            return true;
        }

        /// <summary>
        /// Checks if the type or any type in the hierarchy is equal to System.Array.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <returns>True if the type or a type in the hierarchy has System.Collection.ICollection interface.</returns>
        protected bool IsArray(Type type)
        {
            if (type == null)
                return false;

            if (type.Equals(typeof(System.Array)))
                return true;

            return IsArray(type.BaseType);
        }

        /// <summary>
        /// Gets the PropertyInfo if exists.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <param name="name">Property name.</param>
        /// <param name="flags">Binding flags.</param>
        /// <returns>Null if the property doesn't exist.</returns>
        protected PropertyInfo GetProperty(Type type, string name, BindingFlags flags)
        {
            MemberInfo[] members = type.FindMembers(MemberTypes.Property, flags, null, null);

            foreach (MemberInfo member in members)
                if (member.Name == name)
                    return member as PropertyInfo;

            return null;
        }

        /// <summary>
        /// Gets the FieldInfo if exists.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <param name="name">Member name.</param>
        /// <param name="flags">Binding flags.</param>
        /// <returns>Null if the field doesn't exist.</returns>
        protected FieldInfo GetField(Type type, string name, BindingFlags flags)
        {
            MemberInfo[] members = type.FindMembers(MemberTypes.Field, flags, null, null);

            foreach (MemberInfo member in members)
                if (member.Name == name)
                    return member as FieldInfo;

            return null;
        }

        /// <summary>
        /// Gets the right type present in the SerializableData.
        /// </summary>
        /// <param name="serializableData">Reference SerializableData.</param>
        /// <returns>Type.</returns>
        protected Type GetType(SerializableData serializableData)
        {
            return Type.GetType(serializableData.AssemblyQualifiedName);
        }

        /// <summary>
        /// Returns calling flags.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <param name="attribute">Type to check.</param>
        /// <returns>Binding flags.</returns>
        virtual protected BindingFlags GetFlags(Type type, XmlClassSerializable attribute)
        {
            bool deep = IsDeepSerializable(type);

            BindingFlags flags = attribute != null && attribute.Flags != BindingFlags.Default ? attribute.Flags : BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static;
            if (!IsBaseType(type) && (attribute == null || attribute.Flags == BindingFlags.Default))
                flags |= BindingFlags.Instance;
            else if (IsBaseType(type))
                flags = BindingFlags.Public | BindingFlags.Static;

            if (attribute == null || attribute.Flags == BindingFlags.Default)
            {
                if (!deep)
                    flags |= BindingFlags.DeclaredOnly;
                else
                    flags |= BindingFlags.FlattenHierarchy | BindingFlags.NonPublic;
            }

            return flags;
        }

        /// <summary>
        /// Returns Binding flags.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <returns>Binding flags.</returns>
        virtual protected BindingFlags GetFlags(Type type)
        {
            XmlClassSerializable attribute = GetXmlClassSerializableAttribute(type);
            return GetFlags(type, attribute);
        }

        /// <summary>
        /// Checks if a type with XmlClassSerializable attribute must be deep serialize.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <returns>True if it must be deep serialize.</returns>
        virtual protected bool IsDeepSerializable(Type type)
        {
            XmlClassSerializable attribute = GetXmlClassSerializableAttribute(type);
            if (attribute == null)
                return false;

            return attribute.Deep;
        }

        /// <summary>
        /// Checks if the type is a base type.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <returns>True if the type is any TypeCode but not object.</returns>
        virtual protected bool IsBaseType(Type type)
        {
            if (type == null)
                return true;

            TypeCode typeCode = Type.GetTypeCode(type);
            if (typeCode == TypeCode.Boolean ||
                typeCode == TypeCode.Byte ||
                typeCode == TypeCode.Char ||
                typeCode == TypeCode.DateTime ||
                typeCode == TypeCode.DBNull ||
                typeCode == TypeCode.Decimal ||
                typeCode == TypeCode.Double ||
                typeCode == TypeCode.Empty ||
                typeCode == TypeCode.Int16 ||
                typeCode == TypeCode.Int32 ||
                typeCode == TypeCode.Int64 ||
                typeCode == TypeCode.SByte ||
                typeCode == TypeCode.Single ||
                typeCode == TypeCode.String ||
                typeCode == TypeCode.UInt16 ||
                typeCode == TypeCode.UInt32 ||
                typeCode == TypeCode.UInt64)
                return true;

            return false;
        }

        #endregion
    }
}
