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
    /// Specified that a field (member or property with get and set methods) are serializable.
    /// </summary>
    [AttributeUsage(AttributeTargets.Property | AttributeTargets.Field, AllowMultiple = false, Inherited = false)]
    public class XmlFieldSerializable : XmlSerializable
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        public XmlFieldSerializable()
        {
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="tagName">Tag name in the xml.</param>
        public XmlFieldSerializable(string tagName) : base(tagName)
        {
        }

        #endregion
    }
}
