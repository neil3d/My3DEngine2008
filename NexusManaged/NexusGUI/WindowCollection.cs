using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.Serialization;
using System.Security.Permissions;

using NexusEngineExtension;

namespace Nexus.GUI
{
    #region WindowCollection
    [Serializable]
    [XmlClassSerializable("WindowCollection", true)]
    public class WindowCollection : List<Window>, ICustomTypeDescriptor, ISerializable
    {
        public WindowCollection()
            : base()
        { } //default ctor not valid - we want to enforce initializing our data

        /// <summry>
        /// 自定义索引器
        /// </summary>
        public Window this[string name]
        {
            get
            {
                for (int i = 0; i < Count; ++i)
                {
                    if (this[i].Name.Equals(name))
                    {
                        return this[i];
                    }
                }
                return null;
            }
        }

        #region Serialization
        private WindowCollection(SerializationInfo info, StreamingContext context)
        {
            int count = info.GetInt32("Count");
            for (int ix = 0; ix < count; ix++)
            {
                string key = "item_" + ix.ToString();
                Window item = (Window)info.GetValue(key, typeof(Window));
                this.Add(item);
            }
        }
        [SecurityPermission(SecurityAction.LinkDemand, Flags = SecurityPermissionFlag.SerializationFormatter)]
        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("Count", this.Count);
            int ix = 0;
            foreach (Window item in this)
            {
                string key = "item_" + ix.ToString();
                info.AddValue(key, item, typeof(Window));
                ix++;
            }
        }
        #endregion

        #region ICustomTypeDescriptor impl

        public String GetClassName()
        {
            return TypeDescriptor.GetClassName(this, true);
        }

        public AttributeCollection GetAttributes()
        {
            return TypeDescriptor.GetAttributes(this, true);
        }

        public String GetComponentName()
        {
            return TypeDescriptor.GetComponentName(this, true);
        }

        public TypeConverter GetConverter()
        {
            return TypeDescriptor.GetConverter(this, true);
        }

        public EventDescriptor GetDefaultEvent()
        {
            return TypeDescriptor.GetDefaultEvent(this, true);
        }

        public PropertyDescriptor GetDefaultProperty()
        {
            return TypeDescriptor.GetDefaultProperty(this, true);
        }

        public object GetEditor(Type editorBaseType)
        {
            return TypeDescriptor.GetEditor(this, editorBaseType, true);
        }

        public EventDescriptorCollection GetEvents(Attribute[] attributes)
        {
            return TypeDescriptor.GetEvents(this, attributes, true);
        }

        public EventDescriptorCollection GetEvents()
        {
            return TypeDescriptor.GetEvents(this, true);
        }

        public object GetPropertyOwner(PropertyDescriptor pd)
        {
            return this;
        }


        /// <summary>
        /// Called to get the properties of this type. Returns properties with certain
        /// attributes. this restriction is not implemented here.
        /// </summary>
        /// <param name="attributes"></param>
        /// <returns></returns>
        public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
        {
            return GetProperties();
        }

        /// <summary>
        /// Called to get the properties of this type.
        /// </summary>
        /// <returns></returns>
        public PropertyDescriptorCollection GetProperties()
        {
            // Create a collection object to hold property descriptors
            PropertyDescriptorCollection pds = new PropertyDescriptorCollection(null);

            // Iterate the list of employees
            for (int i = 0; i < this.Count; i++)
            {
                // Create a property descriptor for the employee item and add to the property descriptor collection
                WindowCollectionPropertyDescriptor pd = new WindowCollectionPropertyDescriptor(this, i);
                pds.Add(pd);
            }
            // return the property descriptor collection
            return pds;
        }

        #endregion
    }
    #endregion WindowCollection

    #region WindowCollectionConverter
    internal class WindowCollectionConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is WindowCollection)
            {
                return "WindowCollection data";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }
    #endregion WindowCollectionConverter
}
