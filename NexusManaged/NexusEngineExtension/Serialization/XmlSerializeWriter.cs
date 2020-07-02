using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Text.RegularExpressions;

namespace NexusEngineExtension
{
    /// <summary>
    /// XmlDocument实现XML文件的写入
    /// Reads an SerializableData class and creates a xml file.
    /// </summary>
    public class XmlSerializeWriter : ISerializeWriterController
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        public XmlSerializeWriter(string fileName)
        {
            this._fileName = fileName;
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

        private string _fileName;

        public void Reset()
        {
            this.XmlDocument.RemoveAll();
        }

        /// <summary>
        /// Writes an xml file.
        /// </summary>
        /// <param name="fileName">Xml file to write.</param>
        /// <param name="serializableData">SerializableData to read.</param>
        public void Write(SerializableData serializableData)
        {
            CreateXmlDeclaration();

            WriteXml(_xmlDocument, serializableData);
            
            _xmlDocument.Save(_fileName);
        }

        #endregion

        #region Protected Functions

        /// <summary>
        /// Writes an xml node.
        /// </summary>
        /// <param name="xmlNode">Xml node.</param>
        /// <param name="serializableData">SerializableData to write in the XmlNode.</param>
        virtual protected void WriteXml(XmlNode xmlNode, SerializableData serializableData)
        {
            XmlElement xmlElementClass = null;

            try
            {
                xmlElementClass = _xmlDocument.CreateElement(GetFormattedText(serializableData.TagName));
//                xmlElementClass = _xmlDocument.CreateCDataSection(serializableData.TagName);
            }
            catch
            {
                throw new XmlSerializationException(_xmlDocument, serializableData);
            }
            
            xmlElementClass.Attributes.Append(CreateXmlAttribute("value", serializableData.Value));
            xmlElementClass.Attributes.Append(CreateXmlAttribute("type", serializableData.Type));
            xmlElementClass.Attributes.Append(CreateXmlAttribute("assembly", serializableData.AssemblyQualifiedName));
            xmlElementClass.Attributes.Append(CreateXmlAttribute("name", serializableData.FieldName));
            
            xmlNode.AppendChild(xmlElementClass);

            foreach (SerializableData data in serializableData.SerializableDataCollection)
                WriteXml(xmlElementClass, data);
        }

        /// <summary>
        /// Create the XmlDeclaration on top of the xml file.
        /// </summary>
        virtual protected void CreateXmlDeclaration()
        {
            XmlDeclaration xmlDeclaration = _xmlDocument.CreateXmlDeclaration("1.0", "UTF-8", "no");
            _xmlDocument.AppendChild(xmlDeclaration);
        }

        /// <summary>
        /// Create an XmlAttribute for an XmlNode.
        /// </summary>
        /// <param name="name">Attribute name.</param>
        /// <param name="value">Attribute value.</param>
        /// <returns>XmlAttribute.</returns>
        protected XmlAttribute CreateXmlAttribute(string name, string value)
        {
            XmlAttribute xmlAttribute = _xmlDocument.CreateAttribute(name);
            xmlAttribute.Value = value;

            return xmlAttribute;
        }

        /// <summary>
        /// Returns a text without special characters.
        /// </summary>
        /// <param name="text">Text to format.</param>
        /// <returns>Formatted text.</returns>
        virtual protected string GetFormattedText(string text)
        {
            Regex regex = new Regex("\\W");
            MatchCollection matches = regex.Matches(text);

            foreach (Match match in matches)
                text = text.Replace(match.Value, string.Empty);

            return text;
        }

        #endregion
    }
}
