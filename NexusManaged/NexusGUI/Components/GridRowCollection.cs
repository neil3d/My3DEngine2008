using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Collections;
using System.ComponentModel;
using System.Runtime.InteropServices;
using NexusEngine;
using NexusEngineExtension;


namespace Nexus.GUI.Components
{
    #region GridRow helper / container class

    /// <summary>
    ///		Class used to wrap a grid 'row' and also ease sorting
    /// </summary>
    [XmlClassSerializable("GridRow")]
    public class GridRow : IEnumerable
    {
        [XmlFieldSerializable("rowItems")]
        protected ListboxItemCollection rowItems;
        [XmlFieldSerializable("sortColumnIndex")]
        protected int sortColumnIndex;

        //default ctor not valid - we want to enforce initializing our data
        public GridRow() 
        { 
            rowItems = new ListboxItemCollection();
        }

        [TypeConverter(typeof(ListboxItemCollectionConverter))]
        public ListboxItemCollection RowItems
        {
            get { return rowItems; }
            set { rowItems = value; }
        }

        public int SortColumnIndex
        {
            get { return sortColumnIndex; }
            set { sortColumnIndex = value; }
        }

        public UIComp_DrawListboxItem this[int index]
        {
            get
            {
                return rowItems[index];
            }

            set
            {
                rowItems[index] = value;
            }
        }

        public IEnumerator GetEnumerator()
        {
            return rowItems.GetEnumerator();
        }

        public static bool operator <(GridRow left, GridRow right)
        {
            UIComp_DrawListboxItem itemA = left.rowItems[left.sortColumnIndex];
            UIComp_DrawListboxItem itemB = right.rowItems[right.sortColumnIndex];

            // handle same object case
            if (left == right)
            {
                return false;
            }

            // handle empty slot cases
            if (itemB == null)
            {
                return true;
            }
            else if (itemA == null)
            {
                return false;
            }
            // both items valid, do the compare
            else
            {
                return itemA < itemB;
            }
        }

        public static bool operator >(GridRow left, GridRow right)
        {
            return right < left;
        }

        /// <summary>
        /// 得到一行中最高的高度
        /// </summary>
        internal float GetHighestRowItemHeight()
        {
            float tallest = 0;
            foreach (UIComp_DrawListboxItem item in rowItems)
            {
                if (item != null && item.Size.y > tallest)
                {
                    tallest = item.Size.y;
                }
            }
            return tallest;
        }
        /// <summary>
        /// 得到一行中最宽的宽度
        /// </summary>
        internal float GetWidestRowItemWidth()
        {
            float widest = 0;
            foreach (UIComp_DrawListboxItem item in rowItems)
            {
                if(item != null && item.Size.x > widest)
                {
                    widest = item.Size.x;
                }
            }
            return widest;
        }
        /// <summary>
        /// 一行的总宽度
        /// </summary>
        /// <returns></returns>
        internal float GetTotalRowItemWidth()
        {
            float width = 0;
            foreach (UIComp_DrawListboxItem item in rowItems)
            {
                if (item != null)
                {
                    width += item.Size.x;
                }
            }
            return width;
        }
    }

    #endregion

    #region Comparers for GridRow Sorting

    /// <summary>
    ///		Greater-than Comparer used for GridRows
    /// </summary>
    public class GridRowGTComparer : IComparer<GridRow>
    {
        #region IComparer Members

        public int Compare(GridRow itemA, GridRow itemB)
        {
            if (itemA < itemB)
            {
                return -1;
            }
            else if (itemA > itemB)
            {
                return 1;
            }
            // items are equal
            else
            {
                return 0;
            }
        }

        #endregion
    }

    /// <summary>
    ///		Less-than Comparer used for GridRows
    /// </summary>
    public class GridRowLTComparer : IComparer<GridRow>
    {
        #region IComparer Members

        public int Compare(GridRow itemA, GridRow itemB)
        {
            return (new GridRowGTComparer()).Compare(itemB, itemA);
        }

        #endregion
    }

    #endregion

    #region GridRowCollection

    [XmlClassSerializable("GridRowCollection", true)]
    public class GridRowCollection : List<GridRow>, ICustomTypeDescriptor
    {
        public GridRowCollection() : base() { } //default ctor not valid - we want to enforce initializing our data

        public void SortAscending()
        {
            this.Sort(new GridRowLTComparer());
        }

        public void SortDescending()
        {
            this.Sort(new GridRowGTComparer());
        }


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
                GridRowCollectionPropertyDescriptor pd = new GridRowCollectionPropertyDescriptor(this, i);
                pds.Add(pd);
            }
            // return the property descriptor collection
            return pds;
        }

        #endregion
    }
    #endregion

    #region GridRowCollectionConverter
    internal class GridRowCollectionConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is GridRowCollectionConverter)
            {
                return "GridRow Collection data";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }
    #endregion GridRowCollectionConverter
}
