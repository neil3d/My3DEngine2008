using System;
using System.ComponentModel;
using System.Text;
using System.Collections;
using NexusEditor.Design;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 在PropertyGrid中显示Mesh LOD的一些信息
    /// </summary>
    [TypeConverter(typeof(MeshLODInfoConverter))]
    public class MeshLODInfo
    {
        private int numVertices;
        private int numSection;
        private int numTriangles;
        private MaterialPropertyCollection secMtl;

        public MeshLODInfo()
        {
            secMtl = new MaterialPropertyCollection();
        }

        [ReadOnly(true)]
        public int NumVertices
        {
            get { return numVertices; }
            set { numVertices = value; }
        }

        [ReadOnly(true)]
        public int NumSection
        {
            get { return numSection; }
            set { numSection = value; }
        }

        [ReadOnly(true)]
        public int NumTriangles
        {
            get { return numTriangles; }
            set { numTriangles = value; }
        }

        [ReadOnly(true)]
        public MaterialPropertyCollection Materials
        {
            get { return secMtl; }
        }
    }// end of class MeshLODInfo

    /// <summary>
    /// 使得MeshLODInfo在PropertyGrid中可以展开显示
    /// </summary>
    internal class MeshLODInfoConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is MeshLODInfo)
            {
                MeshLODInfo lod = (MeshLODInfo)value;
                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("V{0},S{1},T{2}",
                    lod.NumVertices, lod.NumSection, lod.NumTriangles);
                return sb.ToString();                
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }// end of class MeshLODInfoConverter

    /// <summary>
    /// MeshLODInfoCollection中元素的属性描述
    /// </summary>
    public class MeshLODInfoCollectionDescriptor : NEPropertyCollectionItemDescriptor<MeshLODInfoCollection, MeshLODInfo>
    {       
        public MeshLODInfoCollectionDescriptor(MeshLODInfoCollection collection, int i)
            : base(collection, i)
        {            
        }       

        public override string Description
        {
            get
            {
                MeshLODInfo lod = m_collection[m_index];
                return "hello";
            }
        }
        public override object GetValue(object component)
        {
            return this.m_collection[m_index];
        }        
    }

    /// <summary>
    /// 在PropertyGrid中动态显示MeshLODInfo数组
    /// </summary>
    [TypeConverter(typeof(MeshLODInfoCollectionConverter))]
    public class MeshLODInfoCollection : NEPropertyCollectionBase<MeshLODInfo>, ICustomTypeDescriptor
    {
        public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
        {
            return GetProperties();
        }

        public PropertyDescriptorCollection GetProperties()
        {
            PropertyDescriptorCollection pds = new PropertyDescriptorCollection(null);
            for (int i = 0; i < this.List.Count; i++)
            {
                MeshLODInfoCollectionDescriptor pd = new MeshLODInfoCollectionDescriptor(this, i);
                pds.Add(pd);
            }
            return pds;
        }
    }

    /// <summary>
    /// 使得MeshLODInfoCollection在PropertyGrid中可以展开显示
    /// </summary>
    internal class MeshLODInfoCollectionConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is MeshLODInfoCollection)
            {
                return "LODInfos";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }
}
