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
    /// Attribute to control the serialization system.
    /// </summary>
    [AttributeUsage(AttributeTargets.All, AllowMultiple = false, Inherited = false)]
    public abstract class XmlSerializable : Attribute
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        public XmlSerializable()
        {
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="tagName">Tag name in the xml.</param>
        public XmlSerializable(string tagName)
        {
            _tagName = tagName;
        }

        #endregion

        #region Properties

        string _tagName = string.Empty;
        /// <summary>
        /// Gets the name of the xml tag relative to the current serializable object.
        /// </summary>
        public string TagName
        {
            get { return _tagName; }
        }

        #endregion
    }
}
