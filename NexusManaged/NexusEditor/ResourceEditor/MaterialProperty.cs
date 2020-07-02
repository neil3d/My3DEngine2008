using System;
using System.ComponentModel;
using System.Collections;
using NexusEngine;
using NexusEditor.Design;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// NMaterialBase类的数据显示和创建参数编辑
    /// </summary>
    [TypeConverter(typeof(MaterialBasePropertyConverter))]
    public class MaterialBaseProperty
    {
        protected string m_createName;
        protected NResourceLoc m_createTemplateLoc; 
        protected NMaterialBase m_mtl;

        public MaterialBaseProperty(NMaterialBase mtl)
        {
            BindMaterial(mtl);
        }

        public virtual void BindMaterial(NMaterialBase mtl)
        {
            if (mtl != null)
            {
                m_createName = mtl.GetName();
                m_createTemplateLoc = mtl.GetTemplateLoc();
            }
            else
            {
                m_createName = "InputCreateName";
                m_createTemplateLoc = new NResourceLoc("Input:TemplateLoc");
            }

            m_mtl = mtl;
        }

        [Category("Material")]
        public string Name
        {
            get { return m_createName; }
            set { m_createName = value; }
        }

        [Category("Material")]
        public NResourceLoc TemplateLoc
        {
            get { return m_createTemplateLoc; }
            set { m_createTemplateLoc = value; }
        }
    }// end of class MaterialBaseProperty

    /// <summary>
    /// NMaterial的数据显示和编辑
    /// </summary>
    public class MaterialProperty : MaterialBaseProperty
    {
        MaterialParamCollection m_params;

        public MaterialProperty(NMaterial mtl)
            : base(mtl)
        {
            BindMaterial(mtl);
        }
        
        public override void  BindMaterial(NMaterialBase _mtl)
        {
            base.BindMaterial(_mtl);

            NMaterial mtl = _mtl as NMaterial;

            m_params = new MaterialParamCollection();
            if (mtl != null)
            {
                int num = mtl.GetNumParameter();
                for (int i = 0; i < num; i++)
                {
                    NMaterialParam mp = mtl.GetParameter(i);
                    switch (mp.type)
                    { 
                        case EMaterialParameter.Int:
                            m_params.Add(new MaterialParamInt(mp));
                            break;
                        case EMaterialParameter.Float:
                            m_params.Add(new MaterialParamFloat(mp));
                            break;
                        case EMaterialParameter.Float2:
                            m_params.Add(new MaterialParamVector2(mp));
                            break;
                        case EMaterialParameter.Float3:
                            m_params.Add(new MaterialParamVector3(mp));
                            break;
                        case EMaterialParameter.Float4:
                            m_params.Add(new MaterialParamVector4(mp));
                            break;
                        case EMaterialParameter.Texture:                            
                            m_params.Add(new MaterialParamTexture(mp));
                            break;
                        default:
                            break;
                    }
                }
            }// end of if
        }

        [Category("Material")]
        public MaterialParamCollection MaterialParams
        {
            get
            {
                return m_params;
            }
        }

        public void ApplyChange()
        {
            try
            {
                NMaterial mtl = m_mtl as NMaterial;

                foreach (MaterialParamEdit param in m_params)
                {
                    if( param.IsValid() )
                        mtl.SetParameter(param.GetData());
                }            
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "Material Param Set FAILED!");
            }            
        }
    }// end of class MaterialProperty
    
    /// <summary>
    /// 材质属性数组中的元素描述
    /// </summary>
    public class MaterialPropertyCollectionDescriptor : NEPropertyCollectionItemDescriptor<MaterialPropertyCollection,MaterialBaseProperty>
    {    
        public MaterialPropertyCollectionDescriptor(MaterialPropertyCollection collection, int i)
            : base(collection, i)
        {            
        }        

        public override string Description
        {
            get
            {
                MaterialBaseProperty mtl = m_collection[m_index];
                return mtl.Name;
            }
        }
        public override object GetValue(object component)
        {
            return this.m_collection[m_index];
        }     
    }

    /// <summary>
    /// 在PropertyGrid中动态显示Material数组
    /// </summary>
    [TypeConverter(typeof(MaterialPropertyCollectionConverter))]
    public class MaterialPropertyCollection : NEPropertyCollectionBase<MaterialBaseProperty>, ICustomTypeDescriptor
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
                MaterialPropertyCollectionDescriptor pd = new MaterialPropertyCollectionDescriptor(this, i);
                pds.Add(pd);
            }

            return pds;
        }        
    }

    /// <summary>
    /// 在PropertyGrid中显示MaterialPropertyCollection
    /// </summary>
    internal class MaterialPropertyCollectionConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is MaterialPropertyCollection)
            {                
                return "Materials";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }

    /// <summary>
    /// 在PropertyGrid中使得材质属性可以展开显示
    /// </summary>
    internal class MaterialBasePropertyConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is MaterialBaseProperty)
            {                
                MaterialBaseProperty mtl = (MaterialBaseProperty)value;
                return mtl.Name;
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }
}
