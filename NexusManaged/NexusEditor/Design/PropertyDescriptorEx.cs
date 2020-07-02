using System;
using System.ComponentModel;
using System.Collections;
using System.Text;

namespace NexusEditor.Design
{
    /// <summary>
    /// Custom Property Description中的使用的collection中元素的PropertyDescriptor基类
    /// </summary>
    /// <typeparam name="TCollection">集合类型</typeparam>
    /// <typeparam name="TItem">集合中的元素类型</typeparam>
    public abstract class NEPropertyCollectionItemDescriptor<TCollection, TItem> : PropertyDescriptor
        where TCollection : CollectionBase
    {
        protected TCollection m_collection;
        protected int m_index;

        protected NEPropertyCollectionItemDescriptor(TCollection col, int i)
            : base(i.ToString(), null)
        {
            m_collection = col;
            m_index = i;
        }

        public override AttributeCollection Attributes
        {
            get
            {
                return new AttributeCollection(null);
            }
        }

        public override bool CanResetValue(object component)
        {
            return false;
        }

        public override Type ComponentType
        {
            get
            {
                return m_collection.GetType();
            }
        }

        public override string DisplayName
        {
            get
            {
                return "[" + m_index.ToString() + "]";
            }
        }

        public override bool IsReadOnly
        {
            get { return true; }
        }

        public override string Name
        {
            get { return "#" + m_index.ToString(); }
        }

        public override Type PropertyType
        {
            get { return typeof(TItem); }
        }

        public override void ResetValue(object component)
        {
        }

        public override bool ShouldSerializeValue(object component)
        {
            return true;
        }

        public override void SetValue(object component, object value)
        {
        }
    }

    /// <summary>
    /// 动态Property集合类型的基类
    /// </summary>
    /// <typeparam name="T">集合成员类型</typeparam>
    public abstract class NEPropertyCollectionBase<T> : CollectionBase
    {
        #region collection impl
        public void Add(T item)
        {
            this.List.Add(item);
        }

        public void Remove(T item)
        {
            this.List.Remove(item);
        }

        public T this[int i]
        {
            get
            {
                return (T)this.List[i];
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
        #endregion
    }
}
