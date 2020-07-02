using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ComponentModel;
using NexusEngine;
using Nexus.GUI.Components;
using System.Runtime.Serialization;
using System.Security.Permissions;

using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("UIScrollThumb", true)]
    public class UIListHeaderSegment : Nexus.GUI.Widgets.UIButton
    {
        #region event delegate
        /// <summary>
        ///		The segment has been re-sized.
        /// </summary>
        public event EventHandler<WindowEventArgs> SegmentSized;
        /// <summary>
        ///		The sort direction of the segment has changed.
        /// </summary>
        public event EventHandler<WindowEventArgs> SortDirectionChanged;
        /// <summary>
        ///		The 'sizable' setting for the segment has changed.
        /// </summary>
        public event EventHandler<WindowEventArgs> SizingSettingChanged;
        #endregion

        #region Constants
        /// <summary>
        ///		Default size of the segment sizing area.
        /// </summary>
        const float DefaultSizingAreaSize = 4.0f;

        #endregion

        #region Fields
        /// <summary>
        ///		pixel width of the sizing area.
        /// </summary>
        [XmlFieldSerializable("splitterSize")]
        protected float splitterSize;

        /// <summary>
        ///		true if the mouse is over the splitter
        /// </summary>
        protected bool splitterHover;

        /// <summary>
        ///		true when sizing is enabled for this segment.
        /// </summary>
        [XmlFieldSerializable("sizingEnabled")]
        protected bool sizingEnabled;

        /// <summary>
        ///		true when we are being sized.
        /// </summary>
        protected bool dragSizing;

        /// <summary>
        ///		point we are being dragged at when sizing or moving.
        /// </summary>
        protected Vector2 dragPoint;

        /// <summary>
        ///		Direction for sorting (used for deciding what icon to display).
        /// </summary>
        protected SortDirection sortingDirection;

        #endregion

        #region Constructor
        public UIListHeaderSegment() { } //default ctor not valid - we want to enforce initializing our data
        /// <summary>
        ///		Constructor.
        /// </summary>
        public UIListHeaderSegment(string name)
            : base(name)
        {
            this.splitterSize = DefaultSizingAreaSize;
            this.splitterHover = false;
            this.dragSizing = false;
            this.sizingEnabled = true;

            this.sortingDirection = SortDirection.None;

            this.Height = new UIDim(1.0f, 0);
        }

        #endregion Constructor

        #region Properties

        /// <summary>
        ///		Get/Set whether the segment can be sized by the user.
        /// </summary>
        [Category("Action")]
        public bool Sizable
        {
            get
            {
                return sizingEnabled;
            }

            set
            {
                if (sizingEnabled != value)
                {
                    sizingEnabled = value;

                    // if sizing is now disabled, ensure current sizing operation is cancelled
                    if (!sizingEnabled && dragSizing)
                    {
                        ReleaseInput();
                    }

                    OnSizingSettingChanged(new WindowEventArgs(this));
                }

            }
        }

        [Category("Data")]
        public float SplitterSize
        {
            get { return splitterSize; }
            set { splitterSize = value; }
        }

        /// <summary>
        ///		Get/Set the currently set sort direction for the segment.
        /// </summary>
        [Category("Data")]
        public SortDirection SortDirection
        {
            get
            {
                return sortingDirection;
            }

            set
            {
                if (sortingDirection != value)
                {
                    sortingDirection = value;
                    OnSortDirectionChanged(new WindowEventArgs(this));
                }
            }
        }
        #endregion

        #region Trigger event
        protected internal void OnSegmentSized(WindowEventArgs e)
        {
            if (SegmentSized != null)
            {
                SegmentSized(this, e);
            }
        }

        protected internal void OnSortDirectionChanged(WindowEventArgs e)
        {
            if (SortDirectionChanged != null)
            {
                SortDirectionChanged(this, e);
            }
        }

        protected internal void OnSizingSettingChanged(WindowEventArgs e)
        {
            if (SizingSettingChanged != null)
            {
                SizingSettingChanged(this, e);
            }
        }
        #endregion

        #region Overridden Event Trigger Methods
        protected internal override bool OnMouseMove(GUIMouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (dragSizing)
            {
                DoDragSizing(e.Position);
            }
            else if (IsHit(e.Position))
            {
                if (Sizable && (e.Position.x > (this.Rect.right - splitterSize)))
                {
                    InitialiseSizingHoverState();
                }
                else
                {
                    splitterHover = false;
                }
            }
            else
            {
                splitterHover = false;
            }
            return true;
        }

        protected internal override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            base.OnMouseButtonDown(e);
            if (e.Button == GUISystem.ClickButton)
            {
                dragPoint = e.Position;
                if (splitterHover)
                {
                    if (Sizable)
                    {
                        dragSizing = true;
                    }
                }
            }
            return true;
        }

        protected internal override void OnCaptureLost(GUIEventArgs e)
        {
            base.OnCaptureLost(e);

            dragSizing = false;
        }

        protected virtual void InitialiseSizingHoverState()
        {
            // only react if settings are changing.
            if (!splitterHover)
            {
                splitterHover = true;

                // TODO: change the mouse cursor.
            }
        }

        protected internal void DoDragSizing(Vector2 mousePos)
        {
            Vector2 offest = mousePos - dragPoint;

            this.Width = new UIDim(this.Width.scale, Math.Max((this.Width.offest + offest.x), DefaultSizingAreaSize*2));

            dragPoint = mousePos;

            OnSegmentSized(new WindowEventArgs(this));
        }
        #endregion

    }

    [Serializable]
    public class HeaderSegmentCollection : List<UIListHeaderSegment>,ICustomTypeDescriptor,ISerializable
    {
        /// <summary>
        /// default ctor not valid - we want to enforce initializing our data
        /// </summary>
        public HeaderSegmentCollection()
            :base()
        {
        }

        #region Serialization
        private HeaderSegmentCollection(SerializationInfo info, StreamingContext context)
        {
            int count = info.GetInt32("Count");
            for (int ix = 0; ix < count; ix++)
            {
                string key = "item_" + ix.ToString();
                UIListHeaderSegment item = (UIListHeaderSegment)info.GetValue(key, typeof(UIListHeaderSegment));
                this.Add(item);
            }
        }
        [SecurityPermission(SecurityAction.LinkDemand, Flags = SecurityPermissionFlag.SerializationFormatter)]
        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("Count", this.Count);
            int ix = 0;
            foreach (UIListHeaderSegment item in this)
            {
                string key = "item_" + ix.ToString();
                info.AddValue(key, item, typeof(UIListHeaderSegment));
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
                HeaderSegmentCollectionPropertyDescriptor pd = new HeaderSegmentCollectionPropertyDescriptor(this, i);
                pds.Add(pd);
            }
            // return the property descriptor collection
            return pds;
        }

        #endregion
    }

    public class HeaderSegmentCollectionPropertyDescriptor : PropertyDescriptor
    {
        private HeaderSegmentCollection collection = null;
        private int index = -1;

        public HeaderSegmentCollectionPropertyDescriptor(HeaderSegmentCollection coll, int idx) :
            base("#" + idx.ToString(System.Globalization.CultureInfo.CurrentCulture), null)
        {
            this.collection = coll;
            this.index = idx;
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
            return true;
        }

        public override Type ComponentType
        {
            get
            {
                return this.collection.GetType();
            }
        }

        public override string DisplayName
        {
            get
            {
                UIListHeaderSegment item = this.collection[index];
                return item.Name;
            }
        }

        public override string Description
        {
            get
            {
                UIListHeaderSegment item = this.collection[index];
                return item.Name;
            }
        }

        public override object GetValue(object component)
        {
            return this.collection[index];
        }

        public override bool IsReadOnly
        {
            get { return false; }
        }

        public override string Name
        {
            get { return "#" + index.ToString(System.Globalization.CultureInfo.CurrentCulture); }
        }

        public override Type PropertyType
        {
            get { return this.collection[index].GetType(); }
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
            // this.collection[index] = value;
        }
    }

    #region HeaderSegmentCollectionConverter
    internal class HeaderSegmentCollectionConverter : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is HeaderSegmentCollectionConverter)
            {
                return "Header Segment Collection data";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }
    #endregion HeaderSegmentCollectionConverter
}
