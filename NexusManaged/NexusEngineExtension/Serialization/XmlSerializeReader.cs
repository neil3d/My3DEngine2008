using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace NexusEngineExtension
{
    /// <summary>
    /// XmlDocument实现XML文件的读取
    /// Reads the xml and fill the SerializableData class.
    /// </summary>
    public class XmlSerializeReader : ISerializeReaderController
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        public XmlSerializeReader(string fileName)
        {
            _xmlDocument.Load(fileName);
        }

        #endregion

        #region Properties

        XmlDocument _xmlDocument = new XmlDocument();
        /// <summary>
        /// XmlDocument that contains the DOM struct.
        /// </summary>
        public XmlDocument XmlDocument
        {
            get { return _xmlDocument; }
            set { _xmlDocument = value; }
        }

        #endregion

        #region Public Functions

        public void Reset()
        {
            this.XmlDocument.RemoveAll();
        }

        /// <summary>
        /// Reads an xml file.
        /// </summary>
        /// <param name="fileName">Xml file to read.</param>
        /// <param name="serializableData">SerializableData to fill.</param>
        /// <returns>Filled class</returns>
        public void Read(ref SerializableData serializableData)
        {
            try
            {
                ReadXml(_xmlDocument.ChildNodes[1], serializableData);
            }
            catch
            {
                throw new XmlSerializationException(_xmlDocument, serializableData);
            }
        }

        #endregion

        #region Protected Functions

        /// <summary>
        /// Reads an XmlNode.
        /// </summary>
        /// <param name="xmlNode">XmlNode to read.</param>
        /// <param name="serializableData">SerializableData to fill.</param>
        virtual protected void ReadXml(XmlNode xmlNode, SerializableData serializableData)
        {
            XmlAttribute xmlAttributeName = xmlNode.Attributes["name"];
            XmlAttribute xmlAttributeType = xmlNode.Attributes["type"];
            XmlAttribute xmlAttributeAssemblyQualifiedName = xmlNode.Attributes["assembly"];
            string alias = xmlNode.Name;
            XmlAttribute xmlAttributeValue = xmlNode.Attributes["value"];

            serializableData.TagName = alias;
            serializableData.Type = xmlAttributeType.Value;
            serializableData.AssemblyQualifiedName = xmlAttributeAssemblyQualifiedName.Value;
            serializableData.FieldName = xmlAttributeName.Value;
            serializableData.Value = xmlAttributeValue.Value;
            
            foreach (XmlNode xmlChildNode in xmlNode.ChildNodes)
            {
                SerializableData newSerializableData = new SerializableData();
                serializableData.SerializableDataCollection.Add(newSerializableData);
                ReadXml(xmlChildNode, newSerializableData);
            }
        }

        #endregion
    }
}
