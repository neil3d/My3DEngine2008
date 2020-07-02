using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using System.Reflection;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace NexusEngineExtension
{
    /// <summary>
    /// Builds an object from a SerializableData.
    /// </summary>
    public class SerializableDataComposer : SerializableDataController
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        public SerializableDataComposer()
        {
        }

        #endregion

        #region Public Functions

        /// <summary>
        /// Composes an object from a SerializableData.
        /// </summary>
        /// <param name="serializableData">SerializableData from which create an object.</param>
        /// <returns>Build object.</returns>
        virtual public object Compose(SerializableData serializableData)
        {
            object data = CreateObject(serializableData);
            FillObject(ref data, serializableData);

            return data;
        }

        #endregion

        #region Protected Functions

        /// <summary>
        /// Creates an object of specified type defined in SerializableData.
        /// </summary>
        /// <param name="serializableData">Reference SerializableData.</param>
        /// <returns>Build object.</returns>
        virtual protected object CreateObject(SerializableData serializableData)
        {
            object obj = null;

            Type type = GetType(serializableData);
            if (type != null)
                obj = Activator.CreateInstance(type, null);
            else
                throw new XmlSerializationException(null, serializableData);

            System.Runtime.Remoting.ObjectHandle objH = obj as System.Runtime.Remoting.ObjectHandle;
            if (objH != null)
                obj = objH.Unwrap();

            return obj;
        }

        /// <summary>
        /// Fills data with fields values specified in SerializableData.
        /// </summary>
        /// <param name="data">Data to fill.</param>
        /// <param name="serializableData">Fields values used to fill the data.</param>
        virtual protected void FillObject(ref object data, SerializableData serializableData)
        {
            foreach (SerializableData member in serializableData.SerializableDataCollection)
                 FillObjectField(data, data.GetType(), member);
        }

        /// <summary>
        /// Fills a single field of the data.
        /// </summary>
        /// <param name="data">Data to fill.</param>
        /// <param name="type">Type of the data to fill.</param>
        /// <param name="serializableData">Field value.</param>
        virtual protected void FillObjectField(object data, Type type, SerializableData serializableData)
        {
            object rightObject = GetNewObject(serializableData);
            Filler(data, type, new object[] { rightObject }, serializableData);

        }

        /// <summary>
        /// Creates an object, a collection, an array or other.
        /// </summary>
        /// <param name="serializableData">Reference SerializableData.</param>
        /// <returns>New build object.</returns>
        virtual protected object GetNewObject(SerializableData serializableData)
        {
            object rightObject = null;

            Type rightType = GetType(serializableData);
            if (rightType == null)
                return null;

            if (rightType.IsArray)
                rightObject = CreateArray(serializableData);
            else if (IsCollection(rightType))
                rightObject = CreateCollection(serializableData);
            else
                rightObject = GetObject(serializableData);

            return rightObject;
        }

        /// <summary>
        /// Fills a data.
        /// </summary>
        /// <param name="dataToFill">data to fill.</param>
        /// <param name="type">Type of the data.</param>
        /// <param name="parameters">Parameters used to fill.</param>
        /// <param name="serializableData">Reference SerializableData.</param>
        protected virtual void Filler(object dataToFill, Type type, object[] parameters, SerializableData serializableData)
        {
            XmlClassSerializable attribute = GetXmlClassSerializableAttribute(type);

            bool deep = IsDeepSerializable(type);
//            bool deep = attribute != null ? attribute.Deep : true;

//            BindingFlags flags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance | BindingFlags.SetField | BindingFlags.SetProperty;
            BindingFlags flags = GetFlags(type, attribute);

            PropertyInfo property = GetProperty(type, serializableData.FieldName, flags);
            FieldInfo field = GetField(type, serializableData.FieldName, flags);

            try
            {
                if ((property != null && property.CanWrite) || (field != null && !field.IsLiteral))
                {
                    type.InvokeMember(serializableData.FieldName, flags, null, dataToFill, parameters);
                }
                else if (!type.BaseType.Equals(typeof(object)) && type.BaseType != null  && deep)
                {
                    FillObjectField(dataToFill, type.BaseType, serializableData);
                }
                else if (IsCollection(type) && !IsArray(type))
                {
                    InvokeAddingMethod(dataToFill, parameters);
                }
            }
            catch
            {
                throw new XmlSerializationException(dataToFill, serializableData, property, field);
            }
        }

        /// <summary>
        /// Gets the object from SerializableData.
        /// </summary>
        /// <param name="serializableData">SerializableData in which there are object informations.</param>
        /// <returns>Build Object.</returns>
        virtual protected object GetObject(SerializableData serializableData)
        {
            object rightObject = null;
            Type rightType = GetType(serializableData);

            if (IsCreateableSerializableData(serializableData))
                rightObject = CreateObject(serializableData);
            else
            {
                try
                {
                    rightObject = ConvertType(serializableData);
                }
                catch
                {
                    throw new XmlSerializationException(rightObject, serializableData);
                }
            }

            FillObject(ref rightObject, serializableData);

            return rightObject;
        }

        /// <summary>
        /// Creates a System.Array.
        /// </summary>
        /// <param name="serializableData">Reference SerializableData.</param>
        /// <returns>Created array.</returns>
        virtual protected object CreateArray(SerializableData serializableData)
        {
            object data = null;

            try
            {
                Type type = GetType(serializableData.SerializableDataCollection[0]);
                System.Array array = System.Array.CreateInstance(type, serializableData.SerializableDataCollection.Count);

                for (int i = 0; i < serializableData.SerializableDataCollection.Count; i++)
                {
                    object internalData = null;
                    if (!IsBaseType(type))
                        internalData = Compose(serializableData.SerializableDataCollection[i]);
                    else
                        internalData = ConvertType(serializableData.SerializableDataCollection[i]);

                    array.SetValue(internalData, i);
                }

                data = array;

                FillObject(ref data, serializableData);
            }
            catch
            {
                throw new XmlSerializationException(data, serializableData);
            }

            return data;
        }

        /// <summary>
        /// Creates a collection.
        /// </summary>
        /// <param name="serializableData">Reference SerializableData.</param>
        /// <returns>Created collection.</returns>
        virtual protected object CreateCollection(SerializableData serializableData)
        {
            object data = CreateObject(serializableData);
            Type type = data.GetType();

            foreach (SerializableData internalData in serializableData.SerializableDataCollection)
            {
                object parameters = Compose(internalData);
                Filler(data, data.GetType(), new object[] { parameters }, internalData);
            }

            return data;
        }

        /// <summary>
        /// Invokes the adding method to populate a collection. It's supposed that the collection has the "Add" method.
        /// </summary>
        /// <param name="invoker">Collection.</param>
        /// <param name="parameters">Parameters, a single object to insert.</param>
        virtual protected void InvokeAddingMethod(object invoker, object[] parameters)
        {
            MethodInfo method = invoker.GetType().GetMethod("Add");
            if (method == null)
                throw new XmlSerializationException(invoker, null);

            try
            {
                method.Invoke(invoker, parameters);
            }
            catch
            {
                throw new XmlSerializationException(invoker, null);
            }
        }

        /// <summary>
        /// Gets true if a SerializableData is a type that can be constructed.
        /// </summary>
        /// <param name="serializableData">SerializableData to check.</param>
        /// <returns>True if SerializableData can be constructed.</returns>
        virtual protected bool IsCreateableSerializableData(SerializableData serializableData)
        {
            Type type = GetType(serializableData);
            TypeCode typeCode = Type.GetTypeCode(type);
            
            ConstructorInfo[] constructors = type.GetConstructors();
            if (constructors.Length == 0 && !type.IsValueType)
                return false;

            foreach (ConstructorInfo constructor in constructors)
                if (constructor.GetParameters().Length == 0)
                    return true;

            if (typeCode == TypeCode.Object)
                return true;

            return false;
        }

        /// <summary>
        /// Changes the SerializableData object in the correct object type and assigns it the value.
        /// </summary>
        /// <param name="serializableData">SerializableData to convert.</param>
        /// <returns>Right object.</returns>
        virtual protected object ConvertType(SerializableData serializableData)
        {
            object rightObject = null;
            Type type = GetType(serializableData);

            // The TypeConverter provides a way to instantite objects by non-parameterless 
            // constructors if they can be converted fro String
            try
            {
                TypeConverter tc = GetConverter(type);
                if (tc.CanConvertFrom(typeof(string)))
                {
                    rightObject = tc.ConvertFrom(serializableData.Value);
                    return rightObject;
                }
            }
            catch { ; }


            TypeCode typeCode = Type.GetTypeCode(type);

            switch (typeCode)
            {
                case TypeCode.Boolean:
                    if (serializableData.Value == string.Empty)
                        rightObject = false;
                    else
                        rightObject = bool.Parse(serializableData.Value);
                    break;
                case TypeCode.Byte:
                    if (serializableData.Value == string.Empty)
                        rightObject = 0;
                    else
                        rightObject = Byte.Parse(serializableData.Value);
                    break;
                case TypeCode.Char:
                    if (serializableData.Value == string.Empty)
                        rightObject = char.MinValue;
                    else
                        rightObject = Char.Parse(serializableData.Value);
                    break;
                case TypeCode.DateTime:
                    if (serializableData.Value == string.Empty)
                        rightObject = DateTime.Now;
                    else
                        rightObject = DateTime.Parse(serializableData.Value);
                    break;
                case TypeCode.Decimal:
                    if (serializableData.Value == string.Empty)
                        rightObject = decimal.MinValue;
                    else
                        rightObject = Decimal.Parse(serializableData.Value);
                    break;
                case TypeCode.Double:
                    if (serializableData.Value == string.Empty)
                        rightObject = double.MinValue;
                    else
                        rightObject = double.Parse(serializableData.Value);
                    break;
                case TypeCode.Int16:
                    if (serializableData.Value == string.Empty)
                        rightObject = Int16.MinValue;
                    else
                        rightObject = Int16.Parse(serializableData.Value);
                    break;
                case TypeCode.Int32:
                    if (serializableData.Value == string.Empty)
                        rightObject = Int32.MinValue;
                    else
                        rightObject = Int32.Parse(serializableData.Value);
                    break;
                case TypeCode.Int64:
                    if (serializableData.Value == string.Empty)
                        rightObject = Int64.MinValue;
                    else
                        rightObject = Int64.Parse(serializableData.Value);
                    break;
                case TypeCode.SByte:
                    if (serializableData.Value == string.Empty)
                        rightObject = SByte.MinValue;
                    else
                        rightObject = SByte.Parse(serializableData.Value);
                    break;
                case TypeCode.Single:
                    if (serializableData.Value == string.Empty)
                        rightObject = Single.MinValue;
                    else
                        rightObject = Single.Parse(serializableData.Value);
                    break;
                case TypeCode.String:
                    rightObject = serializableData.Value;
                    break;
                case TypeCode.UInt16:
                    if (serializableData.Value == string.Empty)
                        rightObject = UInt16.MinValue;
                    else
                        rightObject = UInt16.Parse(serializableData.Value);
                    break;
                case TypeCode.UInt32:
                    if (serializableData.Value == string.Empty)
                        rightObject = UInt32.MinValue;
                    else
                        rightObject = UInt32.Parse(serializableData.Value);
                    break;
                case TypeCode.UInt64:
                    if (serializableData.Value == string.Empty)
                        rightObject = UInt64.MinValue;
                    else
                        rightObject = UInt64.Parse(serializableData.Value);
                    break;
                case TypeCode.DBNull:
                    if (serializableData.Value == string.Empty)
                        rightObject = null;
                    else
                        rightObject = Convert.ToBoolean(serializableData.Value);
                    break;
                case TypeCode.Empty:
                    if (serializableData.Value == string.Empty)
                        rightObject = null;
                    else
                        rightObject = Convert.ToBoolean(serializableData.Value);
                    break;
            }

            return rightObject;
        }

        /// <summary>
        /// Returns the TypeConverter of a Type.
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        protected TypeConverter GetConverter(Type type)
        {
            return TypeDescriptor.GetConverter(type);
        }

        /// <summary>
        /// Returns calling flags.
        /// </summary>
        /// <param name="type">Type to check.</param>
        /// <param name="attribute">Type to check.</param>
        /// <returns>Binding flags.</returns>
        protected override BindingFlags GetFlags(Type type, XmlClassSerializable attribute)
        {
            BindingFlags flags = base.GetFlags(type, attribute);
            flags |= BindingFlags.SetField | BindingFlags.SetProperty;

            return flags;
        }

        #endregion
    }
}
