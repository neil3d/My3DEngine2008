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
    /// Deserialize
    /// </summary>
    public class SerializeReader
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        private SerializeReader()
        {
        }

        public SerializeReader(ISerializeReaderController controller)
        {
            SerializeController = controller;
        }

        #endregion

        #region Properties

        /// <summary>
        /// ÐòÁÐ»¯¿ØÖÆÆ÷
        /// </summary>
        ISerializeReaderController SerializeController { get; set; }

        SerializableDataComposer _composer = new SerializableDataComposer();
        /// <summary>
        /// Gets or sets the component used to compose a SerialzableData from a serialzable object.
        /// </summary>
        public SerializableDataComposer Composer
        {
            get { return _composer; }
            set { _composer = value; }
        }

        #endregion

        #region Public Functions

        /// <summary>
        /// Reset serializer state.
        /// </summary>
        virtual public void Reset()
        {
            _composer.SerializableDataInfo.Reset();

            SerializeController.Reset();
        }

        /// <summary>
        /// Deserialize an object.
        /// </summary>
        /// <returns>Deserialized data.</returns>
        virtual public object Deserialize()
        {
            SerializeController.Read(ref _composer._serializableDataInfo);
            return _composer.Compose(_composer.SerializableDataInfo);
        }

        #endregion
    }
}
