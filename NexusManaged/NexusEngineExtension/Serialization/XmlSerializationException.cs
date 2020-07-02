using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

namespace NexusEngineExtension
{
    /// <summary>
    /// Manages all exceptions thrown by serialization.
    /// </summary>
    public class XmlSerializationException : ApplicationException
    {
        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="data">data that causes the error.</param>
        /// <param name="serializableData">SerializableData associated to error.</param>
        public XmlSerializationException(object data, SerializableData serializableData)
        {
            _data = data;
            _serializableData = serializableData;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="data">data that causes the error.</param>
        /// <param name="serializableData">SerializableData associated to error.</param>
        /// <param name="propertyInfo">PropertyInfo associated to error.</param>
        /// <param name="fieldInfo">FieldInfo associated to error.</param>
        public XmlSerializationException(object data, SerializableData serializableData, PropertyInfo propertyInfo, FieldInfo fieldInfo)
        {
            _data = data;
            _serializableData = serializableData;
            _propertyInfo = propertyInfo;
            _fieldInfo = fieldInfo;
        }

        #endregion

        #region Properties

        object _data = null;
        /// <summary>
        /// Gets the object that thrown the exception.
        /// </summary>
        public object DataInfo
        {
            get { return _data; }
        }

        SerializableData _serializableData = null;
        /// <summary>
        /// Gets the SerializableData that thrown the exception.
        /// </summary>
        public SerializableData SerializableDataInfo
        {
            get { return _serializableData; }
        }

        PropertyInfo _propertyInfo = null;
        /// <summary>
        /// Gets the PropertyInfo that can have generated error.
        /// </summary>
        public PropertyInfo Property
        {
            get { return _propertyInfo; }
        }

        FieldInfo _fieldInfo = null;
        /// <summary>
        /// Gets the FieldInfo that can have generated error.
        /// </summary>
        public FieldInfo Field
        {
            get { return _fieldInfo; }
        }

        /// <summary>
        /// Gets the message for the error.
        /// </summary>
        public override string Message
        {
            get
            {
                string message = base.Message;
                message += _data != null ? "Data:" + _data.ToString() : "NoData:";
                message += Environment.NewLine;
                message += _serializableData != null ? "SerializableData:" +
                    Environment.NewLine +
                    "AssemblyQualifiedName:" + _serializableData.AssemblyQualifiedName +
                    Environment.NewLine +
                    "FieldName:" + _serializableData.FieldName +
                    Environment.NewLine +
                    "TagName:" + _serializableData.TagName +
                    Environment.NewLine +
                    "Type:" + _serializableData.Type +
                    Environment.NewLine +
                    "Value:" + _serializableData.Value +
                    Environment.NewLine +
                    "SerializableDataCollectionCount:" + _serializableData.SerializableDataCollection.Count.ToString() : "NoSerializableData";

                string propertyMessage = string.Empty;

                if (_propertyInfo != null)
                {
                    propertyMessage = "PropertyInfo " + _propertyInfo.Name + ": " ;
                    propertyMessage += _propertyInfo.CanRead ? "Readeable" : "NoReadeable" + "\n";
                    propertyMessage += "PropertyInfo " + _propertyInfo.Name + ": " ;
                    propertyMessage += _propertyInfo.CanWrite ? "Writeable" : "NoWriteable" + "\n";
                }

                string fieldMessage = string.Empty;

                if (_fieldInfo != null)
                {
                    fieldMessage = "FieldInfo " + _fieldInfo.Name + ": " ;
                    fieldMessage += _fieldInfo.IsLiteral ? "Constant" : "NoConstant" + "\n";
                }

                message += propertyMessage + fieldMessage;

                return message;
            }
        }

        #endregion

        #region Protected Functions

        /// <summary>
        /// Gets formatted text.
        /// </summary>
        /// <param name="text">Text to fromat.</param>
        /// <returns>Formatted text.</returns>
        protected string GetFormattedText(string text)
        {
            return "\n" + text + ": ";
        }

        #endregion
    }
}
