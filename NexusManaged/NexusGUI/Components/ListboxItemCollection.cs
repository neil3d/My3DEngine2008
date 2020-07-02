using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Collections;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    #region ListboxItemCollection
    [XmlClassSerializable("ListboxItemCollection", true)]
    public class ListboxItemCollection : List<UIComp_DrawListboxItem>, ICustomTypeDescriptor
    {
        /// <summary>
        /// default ctor not valid - we want to enforce initializing our data
        /// </summary>
        public ListboxItemCollection() 
            :base()
        {
        }

        //#region Serialization
        //private ListboxItemCollection(SerializationInfo info, StreamingContext context)
        //{
        //    int count = info.GetInt32("Count");
        //    for (int ix = 0; ix < count; ix++)
        //    {
        //        string key = "item_" + ix.ToString();
        //        UIComp_DrawListboxItem item = (UIComp_DrawListboxItem)info.GetValue(key, typeof(UIComp_DrawListboxItem));
        //        this.Add(item);
        //    }
        //}
        //[SecurityPermission(SecurityAction.LinkDemand, Flags = SecurityPermissionFlag.SerializationFormatter)]
        //public void GetObjectData(SerializationInfo info, StreamingContext context)
        //{
        //    info.AddValue("Count", this.Count);
        //    int ix = 0;
        //    foreach (UIComp_DrawListboxItem item in this)
        //    {
        //        string key = "item_" + ix.ToString();
        //        info.AddValue(key, item, typeof(UIComp_DrawListboxItem));
        //        ix++;
        //    }
        //}
        //#endregion
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
                ListboxItemCollectionPropertyDescriptor pd = new ListboxItemCollectionPropertyDescriptor(this, i);
                pds.Add(pd);
            }
            // return the property descriptor collection
            return pds;
        }

        #endregion
    }
    #endregion ListboxItemCollection

    #region ListboxItemCollectionConverter
    internal class ListboxItemCollectionConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is ListboxItemCollection)
            {
                return "ListboxItem Collection data";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }
    #endregion ListboxItemCollectionConverter
}
