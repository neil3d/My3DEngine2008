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
    /// Specified that a class is serializable.
    /// </summary>
    [AttributeUsage(AttributeTargets.Struct | AttributeTargets.Class, AllowMultiple = false, Inherited = false)]
    public class XmlClassSerializable : XmlSerializable
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        public XmlClassSerializable()
        {
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="tagName">Tag name in the xml.</param>
        public XmlClassSerializable(string tagName) : base(tagName)
        {
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="tagName">Tag name in the xml.</param>
        /// <param name="deep">Serialization of all fields hierarchy.</param>
        public XmlClassSerializable(string tagName, bool deep) : base(tagName)
        {
            _deep = deep;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="tagName">Tag name in the xml.</param>
        /// <param name="deep">Serialization of all fields hierarchy.</param>
        /// <param name="flags">Binding flags.</param>
        public XmlClassSerializable(string tagName, bool deep, BindingFlags flags) : base(tagName)
        {
            _deep = deep;
            _flags = flags;
        }

        #endregion

        #region Properties

        bool _deep = true;
        /// <summary>
        /// Gets if the class must be serialized in the all hierarchy.
        /// </summary>
        public bool Deep
        {
            get { return _deep; }
        }

        BindingFlags _flags = BindingFlags.Default;
        /// <summary>
        /// Gets the binding flags. Some flags will be added by the framework.
        /// </summary>
        public BindingFlags Flags
        {
            get { return _flags; }
        }

        #endregion
    }
}
