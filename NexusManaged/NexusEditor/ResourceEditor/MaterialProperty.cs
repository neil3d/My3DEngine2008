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
        protected NMtlBase m_mtl;

        public MaterialBaseProperty(NMtlBase mtl)
        {
            BindMaterial(mtl);
        }

        public virtual void BindMaterial(NMtlBase mtl)
        {
            if (mtl != null)
            {
                m_createName = mtl.Name;                
            }
            else
            {
                m_createName = "Null Material";                
            }

            m_mtl = mtl;
        }

        [Category("Material")]
        public string Name
        {
            get { return m_createName; }
            //set { m_createName = value; }
        }
    }// end of class MaterialBaseProperty

    /// <summary>
    /// NMaterial的数据显示和编辑
    /// </summary>
    public class MaterialProperty : MaterialBaseProperty
    {
        MaterialParamCollection m_params;
        NResourceLoc m_ShaderLoc;

		public MaterialProperty(NMtlBase mtl)
            : base(mtl)
        {
            BindMaterial(mtl);
        }
        
        public override void  BindMaterial(NMtlBase _mtl)
        {
            base.BindMaterial(_mtl);

            NMtlStatic mtl = _mtl as NMtlStatic;
            m_params = new MaterialParamCollection();
            if (mtl != null)
            {
				m_ShaderLoc = _mtl.ShaderLoc;
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
        public NResourceLoc ShaderLoc
        {
            get { return m_ShaderLoc; }
			set 
			{
				m_ShaderLoc = value;
			}
        }

        [Category("Material")]
        public MaterialParamCollection MaterialParams
        {
            get
            {
                return m_params;
            }
        }

		[Category("Material")]
		public bool TwoSide
		{
			get 
			{
				if (m_mtl == null) return true;
				return m_mtl.TwoSide; 
			}
			set 
			{
				if (m_mtl != null) m_mtl.TwoSide = value; 
			}
		}

		[Category("Material")]
		public ETransparentType TransType
		{
			get
			{
				if (m_mtl == null) return ETransparentType.Opaque;
				return  m_mtl.TransType;
			}
			set
			{
				if (m_mtl != null) m_mtl.TransType = value;
			}
		}

        public void ApplyChange()
        {
            try
            {
                NMtlStatic mtl = m_mtl as NMtlStatic;
				if (m_mtl!=null)
				{
					m_ShaderLoc = m_mtl.ShaderLoc;
				}
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
