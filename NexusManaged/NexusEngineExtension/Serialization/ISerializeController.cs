using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NexusEngineExtension
{
    /// <summary>
    /// 序列化控制器，可以派生出不同的序列化控制器，如XML、SOAP、Bin等
    /// </summary>
    public interface ISerializeReaderController
    {
        /// <summary>
        /// 对序列化控制器进行重置
        /// </summary>
        void Reset();
        /// <summary>
        /// 实现对SerializableData序列化
        /// </summary>
        /// <param name="serializableData">需要序列化的内容</param>
        void Read(ref SerializableData serializableData);
    }

    /// <summary>
    /// 序列化控制器，可以派生出不同的序列化控制器，如XML、SOAP、Bin等
    /// </summary>
    public interface ISerializeWriterController
    {
        /// <summary>
        /// 对序列化控制器进行重置
        /// </summary>
        void Reset();
        /// <summary>
        /// 实现对SerializableData序列化
        /// </summary>
        /// <param name="serializableData">需要序列化的内容</param>
        void Write(SerializableData serializableData);
    }
}
