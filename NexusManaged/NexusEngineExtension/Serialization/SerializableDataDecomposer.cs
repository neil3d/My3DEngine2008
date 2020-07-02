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
    /// Builds a SerializableData from a SerializableData.
    /// </summary>
    public class SerializableDataDecomposer : SerializableDataController
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        public SerializableDataDecomposer()
        {
        }

        #endregion

        #region Public Functions

        /// <summary>
        /// Checks if the passed data is serializable and if it is decomposing the data.
        /// </summary>
        /// <param name="data">Data to serialize.</param>
        virtual public void Decompose(object data)
        {
            if (data == null)
                throw new XmlSerializationException(data, null);

            FindXmlSerializableClassAttribute(data, SerializableDataInfo);
        }

        #endregion

        #region Protected Functions

        /// <summary>
        /// Finds the XmlClassSerializable attribute of the data and fill the SerializableData.
        /// </summary>
        /// <param name="data">Data to serialize.</param>
        /// <param name="serializableData">SerializableData to fill with found attributes.</param>
        virtual protected bool FindXmlSerializableClassAttribute(object data, SerializableData serializableData)
        {
            Type type = data.GetType();
            XmlClassSerializable attribute = GetXmlClassSerializableAttribute(type);
            if (attribute == null)
                return false;

            CreateSerializableData(data, serializableData);

            Collection<DataMember> dataMembers = new Collection<DataMember>();

            FillDataMembers(type, dataMembers, true, attribute.Deep, GetFlags(type, attribute));
            FindClassFields(data, serializableData, dataMembers);

            return true;
        }

        /// <summary>
        /// If the data derives from System.Collections.ICollection, it fills serializableData.
        /// Can override this function to customize the filling in a user collection.
        /// </summary>
        /// <param name="data">Data to check.</param>
        /// <param name="serializableData">SerializableData to fill.</param>
        virtual protected bool FillCollection(object data, SerializableData serializableData)
        {
            if (!IsCollection(data))
                return false;

            System.Collections.ICollection collection = data as System.Collections.ICollection;

            foreach (object internalData in collection)
            {
                SerializableData newSerializableData = new SerializableData();

                XmlClassSerializable attribute = GetXmlClassSerializableAttribute(internalData.GetType());

                if (!FindXmlSerializableClassAttribute(internalData, newSerializableData))
                {
                    Type type = internalData.GetType();

                    Collection<DataMember> dataMembers = new Collection<DataMember>();

                    FillDataMembers(type, dataMembers, false, IsDeepSerializable(type), GetFlags(type, attribute));
                    if (dataMembers.Count == 0)
                        continue;

                    CreateSerializableData(internalData, newSerializableData);
                    FindClassFields(internalData, newSerializableData, dataMembers);
                }

                serializableData.SerializableDataCollection.Add(newSerializableData);
            }

            return true;
        }

        /// <summary>
        /// Recursive filling of dataMembers.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <param name="dataMembers">DataMember collection to fill.</param>
        /// <param name="custom">Filtering with custom serialize attributes.</param>
        /// <param name="deep">Deep filling in the hierarchy.</param>
        /// <param name="flags">Flags to specified the filling members type.</param>
        virtual protected void FillDataMembers(Type type, Collection<DataMember> dataMembers, bool custom, bool deep, BindingFlags flags)
        {
            MemberInfo[] members = type.FindMembers(MemberTypes.Field | MemberTypes.Property, flags, custom ? new MemberFilter(CustomSearching) : new MemberFilter(Searching), dataMembers);

            if (!type.BaseType.Equals(typeof(object)) && !type.BaseType.Equals(typeof(ValueType)) && type.BaseType != null && deep)
                FillDataMembers(type.BaseType, dataMembers, custom, deep, flags);
        }

        /// <summary>
        /// Adds a DataMember to dataMembers.
        /// </summary>
        /// <param name="memberInfo">MemberInfo used to create a DataMember.</param>
        /// <param name="dataMembers">DataMember collection to fill.</param>
        /// <returns>True if the MemberInfo is inserted.</returns>
        virtual protected bool FillDataMembers(MemberInfo memberInfo, Collection<DataMember> dataMembers)
        {
            if (memberInfo.MemberType == MemberTypes.Field)
            {
                FieldInfo fieldInfo = (FieldInfo)memberInfo;

                DataMember dataMember = new DataMember(memberInfo, fieldInfo.FieldType);
                dataMembers.Add(dataMember);

                return true;
            }
            else if (memberInfo.MemberType == MemberTypes.Property)
            {
                PropertyInfo propertyInfo = (PropertyInfo)memberInfo;

                DataMember dataMember = new DataMember(memberInfo, propertyInfo.PropertyType);
                dataMembers.Add(dataMember);
    
                return true;
            }

            return false;
        }

        /// <summary>
        /// Function called by FillDataMembers for custom serializable attribute.
        /// </summary>
        /// <param name="memberInfo">Reference MemberInfo.</param>
        /// <param name="dataMembersFinder">DataMember collection to fill.</param>
        /// <returns>True if the field if found.</returns>
        virtual protected bool CustomSearching(MemberInfo memberInfo, object dataMembersFinder)
        {
            Collection<DataMember> dataMembers = dataMembersFinder as Collection<DataMember>;
            if (dataMembers == null)
                return false;

            if (memberInfo.GetCustomAttributes(typeof(XmlFieldSerializable), true).GetLength(0) != 1)
                return false;

            // Checks if a DataMember in the hierarhy of classes is just inserted in the collection
            foreach (DataMember dataMember in dataMembers)
                if (dataMember.DataInfo.ToString() == memberInfo.ToString())
                    return false;

            return FillDataMembers(memberInfo, dataMembers);
        }

        /// <summary>
        /// Fills DataMember collection.
        /// </summary>
        /// <param name="memberInfo">Reference DataMember.</param>
        /// <param name="dataMembersFinder">DataMember collection to fill.</param>
        /// <returns>True if the field if found.</returns>
        virtual protected bool Searching(MemberInfo memberInfo, object dataMembersFinder)
        {
            Collection<DataMember> dataMembers = dataMembersFinder as Collection<DataMember>;
            if (dataMembers == null)
                return false;

            return FillDataMembers(memberInfo, dataMembers);
        }

        /// <summary>
        /// Finds the XmlClassSerializable attribute of the data.
        /// </summary>
        /// <param name="data">Data in which searching.</param>
        /// <param name="serializableData">Reference SerializableData.</param>
        /// <param name="dataMembers">Data mebers</param>
        virtual protected void FindClassFields(object data, SerializableData serializableData, Collection<DataMember> dataMembers)
        {
            Type type = data.GetType();
            
            for (int i = 0; i < dataMembers.Count; i++)
            {
                SerializableData newSerializableData = new SerializableData();

                if (GetXmlClassSerializableAttribute(dataMembers[i].TypeInfo) != null)
                {
                    object field = type.InvokeMember(dataMembers[i].DataInfo.Name, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance | BindingFlags.GetField | BindingFlags.GetProperty, null, data, null);
                    newSerializableData.FieldName = dataMembers[i].DataInfo.Name;

                    if (!FindXmlSerializableClassAttribute(field, newSerializableData))
                        continue;
                }
                else
                {
                    newSerializableData = CreateSerializableData(data, dataMembers[i]);
                    if (newSerializableData == null)
                        continue;
                }

                serializableData.SerializableDataCollection.Add(newSerializableData);
            }

            FillCollection(data, serializableData);
        }

        /// <summary>
        /// Create a SerializableData from an object and a DataMember.
        /// </summary>
        /// <param name="data">Reference data.</param>
        /// <param name="dataMember">Reference DataMember.</param>
        /// <returns>New SerializableData.</returns>
        virtual protected SerializableData CreateSerializableData(object data, DataMember dataMember)
        {
            bool foundAttribute = true;

            XmlFieldSerializable attribute = GetXmlFieldSerializableAttribute(dataMember.DataInfo);
            if (attribute == null || attribute.TagName == string.Empty)
                foundAttribute = false;

            object value = null;
            SerializableData newSerializableData = new SerializableData();
            BindingFlags flags = GetFlags(data.GetType());

            PropertyInfo property = GetProperty(dataMember.DataInfo.ReflectedType, dataMember.DataInfo.Name, flags);
            if (property != null && property.CanRead)
                value = property.GetGetMethod(true).Invoke(data, null);
            else
            {
                FieldInfo field = GetField(dataMember.DataInfo.ReflectedType, dataMember.DataInfo.Name, flags);
                if (field != null)
                    value = field.GetValue(data);
                    //value = dataMember.DataInfo.ReflectedType.InvokeMember(dataMember.DataInfo.Name, flags, null, data, null);
                else
                    return null;
            }

            newSerializableData.Type = dataMember.TypeInfo.FullName;
            newSerializableData.AssemblyQualifiedName = dataMember.TypeInfo.AssemblyQualifiedName;
            newSerializableData.Value = value != null ? value.ToString() : string.Empty;
            newSerializableData.TagName = foundAttribute ? attribute.TagName : dataMember.TypeInfo.Name;
            newSerializableData.FieldName = dataMember.DataInfo.Name;

            FillCollection(value, newSerializableData);

            return newSerializableData;
        }

        /// <summary>
        /// Create a SerializableData from an object. FieldName will be empty.
        /// </summary>
        /// <param name="data">Reference Data.</param>
        /// <param name="serializableData">new SerializableData.</param>
        virtual protected void CreateSerializableData(object data, SerializableData serializableData)
        {
            Type type = data.GetType();

            bool foundAttribute = true;
            XmlClassSerializable attribute = GetXmlClassSerializableAttribute(type);
            if (attribute == null || attribute.TagName == string.Empty)
                foundAttribute = false;

            serializableData.Type = type.FullName;
            serializableData.AssemblyQualifiedName = type.AssemblyQualifiedName;
            serializableData.Value = foundAttribute ? string.Empty : data.ToString();
            serializableData.TagName = foundAttribute ? attribute.TagName : type.Name;
        }

        /// <summary>
        /// Returns binding flags.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <param name="attribute">Type to check.</param>
        /// <returns>Binding flags.</returns>
        protected override BindingFlags GetFlags(Type type, XmlClassSerializable attribute)
        {
            BindingFlags flags = base.GetFlags(type, attribute);
            flags |= BindingFlags.GetField | BindingFlags.GetProperty;

            return flags;
        }

        #endregion
    }
}
