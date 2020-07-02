using System;
using System.Text;
using System.ComponentModel;
using NexusEngine;
using NexusEditor.Design;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 材质参数基类
    /// </summary>
    [TypeConverter(typeof(MaterialParamConverter))]
    public class MaterialParamEdit
    {
        protected NMaterialParam m_data;
        public MaterialParamEdit(NMaterialParam data)
        {
            m_data = data;
        }

        public string Name
        {
            get { return m_data.name; }
        }

        public NMaterialParam GetData()
        {
            return m_data;
        }

        public virtual bool IsValid()
        {
            return true;
        }
    }// end of classMaterialParamEdit

    /// <summary>
    /// int型材质参数
    /// </summary>
    public class MaterialParamInt : MaterialParamEdit
    {
        public MaterialParamInt(NMaterialParam data)
            : base(data)
        { }

        public int Value
        {
            get { return m_data.intVal; }
            set { m_data.intVal = value; }
        }
    }// end of class MaterialParamInt

    /// <summary>
    /// float型材质参数
    /// </summary>
    public class MaterialParamFloat : MaterialParamEdit
    {
        public MaterialParamFloat(NMaterialParam data)
            : base(data)
        { }

        public float Value
        {
            get { return m_data.floatVal.x; }
            set { m_data.floatVal.x = value; }
        }
    }// end of MaterialParamFloat

    /// <summary>
    /// float2型材质参数
    /// </summary>
    public class MaterialParamVector2 : MaterialParamEdit
    {
        public MaterialParamVector2(NMaterialParam data)
            : base(data)
        { }

        public Vector2 Value
        {
            get { return new Vector2(m_data.floatVal.x,m_data.floatVal.y); }
            set 
            {

                m_data.floatVal.x = value.x;
                m_data.floatVal.y = value.y; 
            }
        }
    }// end of class MaterialParamVector2

    /// <summary>
    /// float3型材质参数
    /// </summary>
    public class MaterialParamVector3 : MaterialParamEdit
    {
        public MaterialParamVector3(NMaterialParam data)
            : base(data)
        { }

        public Vector3 Value
        {
            get { return new Vector3(m_data.floatVal.x, m_data.floatVal.y, m_data.floatVal.z); }
            set
            {

                m_data.floatVal.x = value.x;
                m_data.floatVal.y = value.y;
                m_data.floatVal.z = value.z;
            }
        }
    }// end of class MaterialParamVector3

    /// <summary>
    /// float4型材质参数
    /// </summary>
    public class MaterialParamVector4 : MaterialParamEdit
    {
        public MaterialParamVector4(NMaterialParam data)
            : base(data)
        { }

        public Vector4 Value
        {
            get { return m_data.floatVal; }
            set { m_data.floatVal = value; }
        }
    }// end of class MaterialParamVector4

    /// <summary>
    /// 贴图类型材质参数
    /// </summary>
    public class MaterialParamTexture : MaterialParamEdit
    {        
        public MaterialParamTexture(NMaterialParam data)
            : base(data)
        {            
        }

        public NResourceLoc TextureLoc
        {
            get
            {
                return m_data.textureLoc;
                
            }
            set 
            {
                m_data.textureLoc = value;
            }
        }

        public override bool IsValid()
        {
            return this.TextureLoc.IsValid();
        }
    }// end of MaterialParamTexture

    /// <summary>
    /// TypeConverter，指定MaterialParamEdit及其派生类可以在PropertyGrid中展开
    /// </summary>
    internal class MaterialParamConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is MaterialParamEdit)
            {
                MaterialParamEdit mp = (MaterialParamEdit)value;
                return mp.Name;
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }// end of class MaterialParamConverter

    /// <summary>
    /// MaterialParamCollection中一个元素的描述
    /// </summary>    
    public class MaterialParamCollectionDescriptor : NEPropertyCollectionItemDescriptor<MaterialParamCollection, MaterialParamEdit>
    {
        public MaterialParamCollectionDescriptor(MaterialParamCollection collection, int i)
            : base(collection, i)
        {
        }

        public override string Description
        {
            get
            {
                MaterialParamEdit mp = m_collection[m_index];
                return "hello";
            }
        }
        public override object GetValue(object component)
        {
            return this.m_collection[m_index];
        }
    }

    /// <summary>
    /// 允许PropertyGrid中动态显示MaterialParamEdit及其派生类对象的集合
    /// </summary>
    [TypeConverter(typeof(MaterialParamCollectionConverter))]
    public class MaterialParamCollection : NEPropertyCollectionBase<MaterialParamEdit>, ICustomTypeDescriptor
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
                MaterialParamCollectionDescriptor pd = new MaterialParamCollectionDescriptor(this, i);
                pds.Add(pd);
            }
            return pds;
        }
    }

    /// <summary>
    /// TypeConverter,允许MaterialParamCollection在PropertyGrid中展开显示
    /// </summary>
    internal class MaterialParamCollectionConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is MaterialParamCollection)
            {
                return "MaterialParameters";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }
}
