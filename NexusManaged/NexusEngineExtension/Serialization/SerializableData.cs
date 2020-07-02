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
    /// Rapresents a serializable class to serialize with its fields and properties.
    /// </summary>
    public class SerializableData
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        public SerializableData()
        {
        }

        #endregion

        #region Properties

        string _fieldName = string.Empty;
        /// <summary>
        /// Gets or sets the name of the serializable field.
        /// </summary>
        public string FieldName
        {
            get { return _fieldName; }
            set { _fieldName = value; }
        }

        string _type = string.Empty;
        /// <summary>
        /// Gets or sets the type of the serializable field.
        /// </summary>
        public string Type
        {
            get { return _type; }
            set { _type = value; }
        }

        string _assemblyQualifiedName = string.Empty;
        /// <summary>
        /// Gets or sets the assembly qualified name in which the serializable field is build.
        /// </summary>
        public string AssemblyQualifiedName
        {
            get { return _assemblyQualifiedName; }
            set { _assemblyQualifiedName = value; }
        }

        string _value = string.Empty;
        /// <summary>
        /// Gets or sets the value of the serializable field.
        /// </summary>
        public string Value
        {
            get { return _value; }
            set { _value = value; }
        }

        string _tagName = string.Empty;
        /// <summary>
        /// Tag name (alias, for example in the xml it can be used like the xml node name) of the serializable field.
        /// </summary>
        public string TagName
        {
            get { return _tagName; }
            set { _tagName = value; }
        }

        Collection<SerializableData> _serializableDataCollection = new Collection<SerializableData>();
        /// <summary>
        /// Collection serializable fields if the current object is serializable itself.
        /// </summary>
        public Collection<SerializableData> SerializableDataCollection
        {
            get { return _serializableDataCollection; }
        }

        #endregion

        #region Public Functions

        /// <summary>
        /// Clear all
        /// </summary>
        virtual public void Reset()
        {
            FieldName = string.Empty;
            TagName = string.Empty;
            //Assembly = string.Empty;
            AssemblyQualifiedName = string.Empty;
            Type = string.Empty;
            Value = string.Empty;
            SerializableDataCollection.Clear();
        }

        #endregion
    }
}
