using System;
using System.Collections;
using System.ComponentModel;
using System.Text;
using NexusEditor.Design;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 在PropertyGrid中显示动画序列
    /// </summary>
    [TypeConverter(typeof(AnimSequenceInfoConverter))]
    public class AnimSequenceInfo
    {
        private string m_name;
        private float m_length;

        public AnimSequenceInfo(string name, float len)
        {
            m_name = name;
            m_length = len;
        }

        [Category("AnimSequence")]
        public string Name
        {
            get { return m_name; }
        }

        [Category("AnimSequence")]
        public float Length
        {
            get { return m_length; }
        }
    }// end of class AnimSequenceInfo

    internal class AnimSequenceInfoConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is AnimSequenceInfo)
            {
                AnimSequenceInfo lod = (AnimSequenceInfo)value;                
                return string.Format("{0}({1})",
                    lod.Name, lod.Length);
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }// end of class MeshLODInfoConverter

    public class AnimSequenceInfoCollectionDescriptor
        : NEPropertyCollectionItemDescriptor<AnimSequenceInfoCollection, AnimSequenceInfo>
    {
        public AnimSequenceInfoCollectionDescriptor(AnimSequenceInfoCollection collection, int i)
            : base(collection, i)
        {
        }

        public override string Description
        {
            get
            {
                AnimSequenceInfo seq = m_collection[m_index];
                return seq.Name;
            }
        }
        public override object GetValue(object component)
        {
            return this.m_collection[m_index];
        }
    }

    [TypeConverter(typeof(AnimSequenceInfoCollectionConverter))]
    public class AnimSequenceInfoCollection
        : NEPropertyCollectionBase<AnimSequenceInfo>, ICustomTypeDescriptor
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
                AnimSequenceInfoCollectionDescriptor pd = new AnimSequenceInfoCollectionDescriptor(this, i);
                pds.Add(pd);
            }
            return pds;
        }
    }

    internal class AnimSequenceInfoCollectionConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is AnimSequenceInfoCollection)
            {
                return "AnimSequences";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }
}
