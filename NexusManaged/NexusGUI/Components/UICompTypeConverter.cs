using System;
using System.ComponentModel; 

namespace Nexus.GUI.Components
{
    public class UICompTypeConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
        {
            if( destinationType == typeof(string)
                && value is UIComponent )
            {
                return value.ToString();
            }

            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override bool GetCreateInstanceSupported(ITypeDescriptorContext context)
        {
            return false;
        }
    }
}
