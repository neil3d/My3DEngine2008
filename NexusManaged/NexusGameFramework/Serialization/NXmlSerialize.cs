using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;
using NexusEngineExtension;
using System.Diagnostics;

namespace Nexus.GameFramework
{
    internal class TypeInfo : IEquatable<TypeInfo>
    {
        public string TypeName { get; private set; }

        public string AssemblyQualifiedName { get; private set; }

        internal TypeInfo(string typeName, string assemblyName)
        {
            TypeName = typeName;
            AssemblyQualifiedName = assemblyName;
        }

        public override bool Equals(object obj)
        {
            TypeInfo other = obj as TypeInfo;
            if (other == null)
                return false;

            return this.GetHashCode() == other.GetHashCode();
        }

        public override string ToString()
        {
            return this.TypeName + " " + this.AssemblyQualifiedName;
        }

        public override int GetHashCode()
        {
            return this.TypeName.GetHashCode() + this.AssemblyQualifiedName.GetHashCode();
        }

        #region IEquatable<TypeInfo> 成员

        bool IEquatable<TypeInfo>.Equals(TypeInfo other)
        {
            return TypeName.Equals(other.TypeName) && AssemblyQualifiedName.Equals(other.AssemblyQualifiedName);
        }

        #endregion
    }

    internal class TypeDictionary
    {
        Dictionary<string, TypeInfo> dictKeyToType = new Dictionary<string,TypeInfo>();
        Dictionary<TypeInfo, string> dictTypeToKey = new Dictionary<TypeInfo,string>();


        public Dictionary<string, TypeInfo> KeyToType 
        {
            get { return dictKeyToType; }
        }

        public Dictionary<TypeInfo, string> TypeToKey
        {
            get { return dictTypeToKey; }
        }


        internal void Add(string key, TypeInfo typeInfo)
        {
            dictKeyToType[key] = typeInfo;
            dictTypeToKey[typeInfo] = key;
        }

        internal bool ContainsKey(string key)
        {
            return dictKeyToType.ContainsKey(key);
        }

        internal bool ContainsType(TypeInfo info)
        {
            return dictTypeToKey.ContainsKey(info);
        }

        internal string GetTypeKey(TypeInfo info)
        {
            if (info == null)
                return null;
            if(!ContainsType(info))
            {
                return "TK" + dictTypeToKey.Count;
            }

            return dictTypeToKey[info];
        }
    }

    public class NXmlSerializeDataReader : NXmlSerializeReader, ISerializeReaderController
    {
        private readonly string ArrayItemKey = "array_item";
        private readonly string TypeDictionaryKey = "TK";
        private readonly string TypeDictionaryCollectionKey = "TKC";
        private readonly string Key = "KEY";
        private readonly string TypeKey = "TY";
        private readonly string ValueKey = "VL";
        private readonly string NameKey = "NAME";
        private readonly string AssemblyKey = "ASM";
        private readonly string SerializableDataKey = "SD";
        private readonly string SerializableDataCollectionKey = "SDC";

        private TypeDictionary typeDict = new TypeDictionary();

        public NXmlSerializeDataReader(string pkgName, string fileName)
            : base(pkgName, fileName)
        {

        }

        public void Reset() { }

        public void Read(ref SerializableData data)
        {
            // 先序列化类型字典
            int count = 0;
            this.ArrayBegin(TypeDictionaryCollectionKey, out  count);
            for (int index = 0; index < count; ++index)
            {
                this.ObjectBegin(ArrayItemKey);
                string key = this.ObjectDescription(Key);
                string Type = this.ObjectDescription(TypeKey);
                string Assembly = this.ObjectDescription(AssemblyKey);

                TypeInfo info = new TypeInfo(Type, Assembly);
                typeDict.Add(key, info);

                this.ObjectEnd();

                this.ArrayNext();
            }
            this.ArrayEnd();

            ReadXML(ref data, SerializableDataKey);
        }

        private void ReadXML(ref SerializableData data, string xmlNodeName)
        {
            this.ObjectBegin(xmlNodeName);

            data.Value = this.ObjectDescription(ValueKey);

            string TK = this.ObjectDescription(TypeDictionaryKey);
            Debug.Assert(typeDict.ContainsKey(TK));
            TypeInfo info = typeDict.KeyToType[TK];
            data.Type = info.TypeName;
            data.AssemblyQualifiedName = info.AssemblyQualifiedName;
            //data.Type = this.ObjectDescription("type");
            //data.AssemblyQualifiedName = this.ObjectDescription("assembly");
            data.FieldName = this.ObjectDescription(NameKey);

            int count = 0;
            this.ArrayBegin(SerializableDataCollectionKey, out count);
            for (int index = 0; index < count; ++index)
            {
                SerializableData new_data = new SerializableData();
                this.ReadXML(ref new_data, ArrayItemKey);
                data.SerializableDataCollection.Add(new_data);

                this.ArrayNext();
            }
            this.ArrayEnd();

            this.ObjectEnd();
        }
        
    };


    public class NXmlSerializeDataWriter : NXmlSerializeWriter, ISerializeWriterController
    {
        private readonly string ArrayItemKey = "array_item";
        private readonly string TypeDictionaryKey = "TK";
        private readonly string TypeDictionaryCollectionKey = "TKC";
        private readonly string Key = "KEY";
        private readonly string TypeKey = "TY";
        private readonly string ValueKey = "VL";
        private readonly string NameKey = "NAME";
        private readonly string AssemblyKey = "ASM";
        private readonly string SerializableDataKey = "SD";
        private readonly string SerializableDataCollectionKey = "SDC";

        private TypeDictionary typeDict = new TypeDictionary();

        public NXmlSerializeDataWriter(string pkgName, string fileName)
            : base(pkgName, fileName)
        {

        }

        public void Reset() { }

        public void Write(SerializableData data)
        {
            // 建立类型字典
            BuildTypeDictionary(data);

            // 先序列化类型字典
            int count = typeDict.KeyToType.Count;
            this.ArrayBegin(TypeDictionaryCollectionKey, out  count);
            foreach (var item in typeDict.KeyToType)
            {
                this.ObjectBegin(ArrayItemKey);
                this.ObjectDescription(Key, item.Key);
                this.ObjectDescription(TypeKey, item.Value.TypeName);
                this.ObjectDescription(AssemblyKey, item.Value.AssemblyQualifiedName);
                this.ObjectEnd();

                this.ArrayNext();
            }
            this.ArrayEnd();

            // 序列化数据
            WriteXML(data, SerializableDataKey);
        }

        private void BuildTypeDictionary(SerializableData data)
        {
            TypeInfo type = new TypeInfo(data.Type, data.AssemblyQualifiedName);
            string key = typeDict.GetTypeKey(type);

            typeDict.Add(key, type);

            foreach (var item in data.SerializableDataCollection)
            {
                BuildTypeDictionary(item);
            }
        }

        public void WriteXML(SerializableData data, string xmlNodeName)
        {
            Debug.Assert(!String.IsNullOrEmpty(data.Type));
            this.ObjectBegin(xmlNodeName);

            this.ObjectDescription(ValueKey, data.Value);
            // 写入TK
            TypeInfo info = new TypeInfo(data.Type, data.AssemblyQualifiedName);
            this.ObjectDescription(TypeDictionaryKey, typeDict.GetTypeKey(info));
            this.ObjectDescription(NameKey, data.FieldName);

            int count = data.SerializableDataCollection.Count;
            this.ArrayBegin(SerializableDataCollectionKey, out  count);
            for (int index = 0; index < count; ++index)
            {
                this.WriteXML(data.SerializableDataCollection[index], ArrayItemKey);

                this.ArrayNext();
            }
            this.ArrayEnd();

            this.ObjectEnd();
        }
    };
}
