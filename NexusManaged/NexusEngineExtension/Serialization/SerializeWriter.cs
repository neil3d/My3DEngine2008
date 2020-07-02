using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NexusEngineExtension
{
    /// <summary>
    /// Serialize
    /// </summary>
    public class SerializeWriter
    {
        #region Constructors

        /// <summary>
        /// Default constructor.
        /// </summary>
        private SerializeWriter()
        {
        }

        public SerializeWriter(ISerializeWriterController controller)
        {
            SerializeController = controller;
        }

        #endregion

        #region Properties

        /// <summary>
        /// 序列化控制器
        /// </summary>
        ISerializeWriterController SerializeController { get; set; }

        SerializableDataDecomposer _decomposer = new SerializableDataDecomposer();
        /// <summary>
        /// Gets or sets the component used to decompose a serialzable object from a SerializableData.
        /// </summary>
        public SerializableDataDecomposer Decomposer
        {
            get { return _decomposer; }
            set { _decomposer = value; }
        }

        #endregion

        #region Public Functions

        /// <summary>
        /// Reset serializer state.
        /// </summary>
        virtual public void Reset()
        {
            _decomposer.SerializableDataInfo.Reset();
            
            SerializeController.Reset();
        }

        /// <summary>
        /// Serialize an object.
        /// </summary>
        /// <param name="data">Data to serialize.</param>
        virtual public void Serialize(object data)
        {
            _decomposer.Decompose(data);
            SerializeController.Write(_decomposer.SerializableDataInfo);
        }

        #endregion
    }
}
