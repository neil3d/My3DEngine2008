using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

namespace NexusEngineExtension
{
    /// <summary>
    /// Class used by Serializer to store informations about serializable fields.
    /// </summary>
    public class DataMember
    {
        #region Constructors

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="dataInfo">MemberInfo of the serializable field.</param>
        /// <param name="typeInfo">Type of the serializable field.</param>
        public DataMember(MemberInfo dataInfo, Type typeInfo)
        {
            _dataInfo = dataInfo;
            _typeInfo = typeInfo;
        }

        #endregion

        #region Properties

        MemberInfo _dataInfo = null;
        /// <summary>
        /// Gets the MemberInfo of the serializable field.
        /// </summary>
        public MemberInfo DataInfo
        {
            get { return _dataInfo; }
        }

        Type _typeInfo = null;
        /// <summary>
        /// Gets the Type of the serializable field.
        /// </summary>
        public Type TypeInfo
        {
            get { return _typeInfo; }
        }

        #endregion
    }
}
