using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using NexusEngineExtension;

namespace Nexus.GUI
{
    namespace Design
    {

        public class UIDimConverter : ExpandableObjectConverter
        {
            private PropertyDescriptorCollection properties; 

            public UIDimConverter()
            {
                Type type = typeof(UIDim);
                PropertyDescriptor[] propArray = 
                {
                    new FieldPropertyDescriptor(type.GetField("scale")),
                    new FieldPropertyDescriptor(type.GetField("offest"))
                };
			    properties = new PropertyDescriptorCollection(propArray);
            }

            public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
            {
                if (destinationType == typeof(string))
                {
                    return true;
                }

                return base.CanConvertTo(context, destinationType);
            }

            public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
            {
                if (sourceType == typeof(string))
                    return true;
                else
                    return base.CanConvertFrom(context, sourceType);
            }

            public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
            {
                if( destinationType == null )
				    throw new ArgumentNullException( "destinationType" );

			    if( culture == null )
				    culture = System.Globalization.CultureInfo.CurrentCulture;

                UIDim dim = (UIDim)value;
                if (destinationType == typeof(string) && dim!=null)
                {
				    string separator = culture.TextInfo.ListSeparator + " ";
				    TypeConverter converter = TypeDescriptor.GetConverter(typeof(float));
				    string[] stringArray = new string[2];

                    stringArray[0] = converter.ConvertToString(context, culture, dim.scale);
                    stringArray[1] = converter.ConvertToString(context, culture, dim.offest);

				    return String.Join( separator, stringArray );
                }

                return base.ConvertTo(context, culture, value, destinationType);
            }

            public override object ConvertFrom(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value)
            {
                string str = value as string;
                if (!string.IsNullOrEmpty(str))
                {
                    if (culture == null)
                        culture = System.Globalization.CultureInfo.CurrentCulture;

				    TypeConverter converter = TypeDescriptor.GetConverter(typeof(float));
                    string[] stringArray = str.Split(culture.TextInfo.ListSeparator[0]);

				    if( stringArray.Length != 2 )
					    throw new ArgumentException("Invalid vector format.");

				    float scale = (float)converter.ConvertFromString( context, culture, stringArray[0]);
                    float offest = (float)converter.ConvertFromString(context, culture, stringArray[1]);

                    return new UIDim(scale, offest);
                }

                return base.ConvertFrom(context, culture, value);
            }

            public override bool GetCreateInstanceSupported(ITypeDescriptorContext context)
            {
                return true;
            }

            public override object CreateInstance(ITypeDescriptorContext context, System.Collections.IDictionary propertyValues)
            {
                return new UIDim((float)propertyValues["scale"], (float)propertyValues["offest"]);
            }
            public override bool GetPropertiesSupported(ITypeDescriptorContext context)
            {
                return true;
            }

            public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes)
            {
                return properties;
            }
        }

    }


    /// <summary>
    /// Class representing a unified dimension; that is a dimension that has both
    /// a relative 'scale' portion and and absolute 'offset' portion.
    /// 参考 CEGUI.UDim
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    [TypeConverter(typeof(Nexus.GUI.Design.UIDimConverter))]
    [Serializable]
    [XmlClassSerializable("UIDim")]
    public struct UIDim
    {
        #region Fields
        
        /// <summary>
        /// 相对量
        /// </summary>
        [XmlFieldSerializable("scale")]
        public float scale;
    
        /// <summary>
        /// 绝对量
        /// </summary>
        [XmlFieldSerializable("offest")]
        public float offest;
        
        #endregion Fields
        public UIDim(float inScale, float inOffest) 
        {
            scale = inScale;
            offest = inOffest;
        }

        public float AsAbsolute(float inBase)
        {
            return inBase*this.scale + offest;
        }

        public float AsRelative(float inBase)
        {
            return (inBase != 0.0f ? offest / inBase + scale : 0.0f);
        }

        #region Overloaded operators + CLS compliant method equivalents
        public static bool operator == (UIDim left, UIDim right)
        {
            return (left.scale == right.scale && left.offest == right.offest);
        }

        public static bool operator != (UIDim left, UIDim right)
        {
            return (left.scale != right.scale || left.offest != right.offest);
        }
        
        public static UIDim Add(UIDim left, UIDim right)

        {
            return left + right;
        }

        public static UIDim operator + ( UIDim left, UIDim right )
        {
            return new UIDim(left.scale+right.scale, left.offest+right.offest);
        }

        public static UIDim Subtract(UIDim left, UIDim right)
        {
            return left - right;
        }

        public static UIDim operator - (UIDim left, UIDim right)
        {
            return new UIDim(left.scale - right.scale, left.offest - right.offest);
        }

        public static UIDim Multiply(UIDim left, UIDim right)
        {
            return left * right;
        }

        public static UIDim operator*(UIDim left, UIDim right)
        {
            return new UIDim(left.scale * right.scale, left.offest * right.offest);
        }

        public static UIDim Divide(UIDim left, UIDim right)
        {
            return left / right;
        }

        public static UIDim operator / (UIDim left, UIDim right)
        {
            return new UIDim((right.scale == 0.0f ? 0.0f : left.scale / right.scale),
               (right.offest == 0.0f ? 0.0f : left.offest / right.offest));
        }
        #endregion

        #region object overloads
        public override string ToString()
        {
            return string.Format(System.Globalization.CultureInfo.CurrentCulture,"{0},{1}",this.scale, this.offest);
        }

        public override int GetHashCode()
        {
            return scale.GetHashCode() + offest.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            return (obj is UIDim) && (this == (UIDim)obj);
        }

        #endregion
    }
}
