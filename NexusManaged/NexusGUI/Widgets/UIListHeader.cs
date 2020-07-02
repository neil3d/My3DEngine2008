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
    [XmlClassSerializable("UIListHeader", true)]
    public class UIListHeader : Nexus.GUI.Window
    {
        #region Event Declarations
        /// <summary>
        /// The current sort column changed.
        /// </summary>
        public event EventHandler<WindowEventArgs> SortColumnChanged;

        /// <summary>
        /// The sort direction changed.
        /// </summary>
        public event EventHandler<WindowEventArgs> SortDirectionChanged;

        /// <summary>
        /// A segment has been sized by the user (e.Window is the segment).
        /// </summary>
        public event EventHandler<WindowEventArgs> SegmentSized;

        /// <summary>
        /// A segment has been clicked by the user (e.Window is the segment).
        /// </summary>
        public event EventHandler<WindowEventArgs> SegmentClicked;

        /// <summary>
        /// The order of the segments has changed.  ('e' is a HeaderSequenceEventArgs)
        /// </summary>
        public event EventHandler<HeaderSequenceEventArgs> SegmentSequenceChanged;

        /// <summary>
        /// A segment has been added to the header.
        /// </summary>
        public event EventHandler<WindowEventArgs> SegmentAdded;

        /// <summary>
        /// A segment has been removed from the header.
        /// </summary>
        public event EventHandler<WindowEventArgs> SegmentRemoved;

        /// <summary>
        /// The setting that controls user modification to sort configuration changed.
        /// </summary>
        public event EventHandler<WindowEventArgs> SortSettingChanged;

        /// <summary>
        /// The setting that controls user sizing of segments changed.
        /// </summary>
        public event EventHandler<WindowEventArgs> DragSizeSettingChanged;
        #endregion

        #region Fields
        [XmlFieldSerializable("backgroundRenderComp")]
        protected UIComp_DrawTextureAtlas backgroundRenderComp;

        HeaderSegmentCollection segments = new HeaderSegmentCollection();
        UIListHeaderSegment sortSegment;
        /// <summary>
        /// true if segments can be sized by the user.
        /// </summary>
        [XmlFieldSerializable("sizingEnabled")]
        bool sizingEnabled;

        /// <summary>
        /// true if the sort criteria modifications by user are enabled (no sorting is actuall done)
        /// </summary>
        [XmlFieldSerializable("sortingEnabled")]
        bool sortingEnabled;
        /// <summary>
        /// Brief copy of the current sort direction.
        /// </summary>
        [XmlFieldSerializable("sortingDirection")]
        SortDirection sortingDirection;

        /// <summary>
        /// Base offset used to layout the segments (allows scrolling within the window area)
        /// </summary>
        [XmlFieldSerializable("segmentOffset")]
        float segmentOffset;
        #endregion Fields

        #region Constructor
        public UIListHeader() { } //default ctor not valid - we want to enforce initializing our data

        public UIListHeader(string name)
            : base(name)
        {
            sortSegment = null;
            sizingEnabled = true;
            sortingEnabled = true;
            sortingDirection = SortDirection.None;
            //this.segmentOffset = 0.0f;

            backgroundRenderComp = new UIComp_DrawTextureAtlas(this);

            backgroundRenderComp.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            backgroundRenderComp.AtlasItemName = "0";
        }
        #endregion

        #region Properties

        [Category("Component")]
        [Description("Display some image.")]
        public UIComp_DrawTextureAtlas BackgroundRenderComp
        {
            get { return backgroundRenderComp; }
        }


        [Category("Data")]
        public int ColumnCount
        {
            get { return segments.Count; }
        }

        [TypeConverter(typeof(HeaderSegmentCollectionConverter))]
        public HeaderSegmentCollection Segments
        {
            get { return segments; }
        }

        /// <summary>
        /// Gets/Sets the current sort segment
        /// </summary>
        [Browsable(false)]
        public UIListHeaderSegment SortSegment
        {
            get
            {
                return sortSegment;
            }

            set
            {
                SortColumnIndex = GetColumnIndexFromSegment(value);
            }
        }

        /// <summary>
        /// Gets/Sets the current sort column index
        /// </summary>
        [Category("Data")]
        public int SortColumnIndex
        {
            get
            {
                if (SortSegment != null)
                {
                    return GetColumnIndexFromSegment(SortSegment);
                }
                else
                {
                    return -1;
                }
            }

            set
            {
                if (value < 0 || value >= ColumnCount)
                {
                    NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The specified index ({0}) is out of range for this ListHeader.", value));
                }
                else
                {
                    // if column is different to current segment
                    if (sortSegment != segments[value])
                    {
                        // set sort direction on current segment to none
                        if (sortSegment != null)
                        {
                            sortSegment.SortDirection = SortDirection.None;
                        }

                        // set up new sort segment
                        sortSegment = segments[value];
                        sortSegment.SortDirection = sortingDirection;

                        // fire event
                        OnSortColumnChanged(new WindowEventArgs(this));
                    }
                }
            }
        }

        /// <summary>
        /// Get the pixel extent of all segments
        /// </summary>
        [Category("Data")]
        public float TotalPixelExtent
        {
            get
            {
                float extent = 0.0f;

                for (int i = 0; i < ColumnCount; ++i)
                {
                    extent += segments[i].Rect.Width;
                }

                return extent;
            }
        }

        /// <summary>
        /// Gets/Sets the current sorting direction.
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

                    // set direction of current sort segment
                    if (sortSegment != null)
                    {
                        sortSegment.SortDirection = sortingDirection;
                    }

                    // fire event
                    OnSortDirectionChanged(new WindowEventArgs(this));
                }
            }
        }

        /// <summary>
        /// Gets/Sets whether use control of the sort column and direction is enabled.
        /// </summary>
        [Category("Data")]
        public bool SortEnabled
        {
            get
            {
                return sortingEnabled;
            }

            set
            {
                if (sortingEnabled != value)
                {
                    sortingEnabled = value;

                    // fire event
                    OnSortSettingChanged(new WindowEventArgs(this));
                }
            }
        }

        /// <summary>
        /// Gets/Sets whether the user may re-size the header segments.
        /// </summary>
        [Category("Data")]
        public bool SegmentSizingEnabled
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

                    // modify setting on all segments
                    for (int i = 0; i < ColumnCount; ++i)
                    {
                        segments[i].Sizable = sizingEnabled;
                    }

                    // fire event
                    OnDragSizeSettingChanged(new WindowEventArgs(this));
                }
            }
        }

        /// <summary>
        /// Gets/Sets the current segment scroll offset value.
        /// </summary>
        public float SegmentOffset
        {
            get
            {
                return segmentOffset;
            }

            set
            {
                if (segmentOffset != value)
                {
                    segmentOffset = value;
                    LayoutSegments();
                    RequestRedraw();
                }
            }
        }

        #endregion

        #region Methods

        protected override void DrawSelf()
        {
            backgroundRenderComp.Draw();

            // 裁剪子控件渲染
            GUISystem.Instance.Renderer.PushScissorRect(this.Rect);
        }

        protected override void PostRender()
        {
            base.PostRender();
            GUISystem.Instance.Renderer.PopScissorRect();
        }

        public int GetColumnIndexFromSegment(UIListHeaderSegment segment)
        {
            for (int i = 0; i < ColumnCount; ++i)
            {
                if (segments[i] == segment)
                {
                    return i;
                }
            }
            return -1;
        }

        public float GetPixelOffsetToSegment(UIListHeaderSegment segment)
        {
            float offset = 0.0f;

            for (int i = 0; i < ColumnCount; ++i)
            {
                if (segments[i] == segment)
                {
                    return offset;
                }

                offset += segments[i].Rect.Width;
            }

            // no such segment...
            throw new InvalidRequestException("The specified UIListHeaderSegment is not attached to this ListHeader.");
        }

        public float GetPixelOffsetToColumn(int column)
        {
            if (column >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The specified index ({0}) is out of range for this ListHeader.", column));
                return 0;
            }
            else
            {
                float offset = 0.0f;

                for (int i = 0; i < column; ++i)
                {
                    offset += segments[i].Rect.Width;
                }

                return offset;
            }
        }

        public float GetPixelWidthOfColumn(int column)
        {
            if (column >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The specified index ({0}) is out of range for this ListHeader.", column));
                return 0;
            }
            else
            {
                return segments[column].Rect.Width;
            }
        }

        public void AddColumn(string text, float width)
        {
            // add just inserts at the end
            InsertColumn(text, width, ColumnCount);
        }

        public void InsertColumn(string text, float width, int position)
        {
            // if position is too big, insert at end
            if (position > ColumnCount)
            {
                position = ColumnCount;
            }

            UIListHeaderSegment newSegment = CreateInitialisedSegment(text, width);
            segments.Insert(position, newSegment);

            LayoutSegments();

            // fire event
            OnSegmentAdded(new WindowEventArgs(this));

            // if sort segment is invalid, set-up sorting now we have a segment
            if (sortSegment != null)
            {
                SortColumnIndex = position;
            }
        }

        public void MoveColumn(int columnIndex, int positionIndex)
        {
            if (columnIndex < 0 || columnIndex >= ColumnCount
                || positionIndex < 0 || positionIndex >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The specified index ({0}) is out of range for this ListHeader.", columnIndex));
            }
            else
            {
                // if the target position is out of range, move to the end
                if (positionIndex > ColumnCount)
                {
                    positionIndex = ColumnCount - 1;
                }

                UIListHeaderSegment seg = segments[columnIndex];

                // remove original copy of segment
                segments.Remove(seg);

                // re-insert segment at its new position
                segments.Insert(positionIndex, seg);

                // fire event
                OnSegmentSequenceChanged(new HeaderSequenceEventArgs(columnIndex, positionIndex));

                LayoutSegments();
            }
        }

        public void RemoveColumn(int columnIndex)
        {
            if (columnIndex < 0 || columnIndex >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The specified index ({0}) is out of range for this ListHeader.", columnIndex));
            }
            else
            {
                UIListHeaderSegment seg = segments[columnIndex];

                // remove the segment from the list
                segments.Remove(seg);

                // patch up sort segment if we have removed it
                if (sortSegment == seg)
                {
                    // more columns?
                    if (ColumnCount > 0)
                    {
                        // make first column the new sort column
                        sortingDirection = SortDirection.None;
                        SortColumnIndex = 0;
                    }
                    // no columns
                    else
                    {
                        sortSegment = null;
                    }
                }

                // remove the segment from our child list
                RemoveChild(seg);

                DestroyListSegment(seg);
                LayoutSegments();

                // fire event
                OnSegmentRemoved(new WindowEventArgs(this));
            }
        }

        public void RemoveSegment(UIListHeaderSegment segment)
        {
            RemoveColumn(GetColumnIndexFromSegment(segment));
        }

        public void SetColumnPixelWidth(int columnIndex, float width)
        {
            if (columnIndex >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The specified index ({0}) is out of range for this ListHeader.", columnIndex));
            }
            else
            {
                UIListHeaderSegment seg = segments[columnIndex];

                seg.Width = new UIDim(seg.Width.scale, width);

                LayoutSegments();

                OnSegmentSized(new WindowEventArgs(segments[columnIndex]));
            }
        }

        protected UIListHeaderSegment CreateInitialisedSegment(string text, float width)
        {
            UIListHeaderSegment seg = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIListHeaderSegment", this) as UIListHeaderSegment;

            seg.Width = new UIDim(0, width);

            seg.NormalText.Text = text;
            seg.HoveringText.Text = text;
            seg.PushedText.Text = text;
            seg.DisableText.Text = text;

            seg.SegmentSized += new EventHandler<WindowEventArgs>(SegmentSized_handler);
            return seg;
        }

        protected void DestroyListSegment(UIListHeaderSegment segment)
        {
            if (sortSegment == segment)
            {
                SortSegment = null;
            }

            segments.Remove(segment);

            LayoutSegments();
        }

        protected void LayoutSegments()
        {
            float posOffest = segmentOffset;
            for (int i = 0; i < ColumnCount; ++i)
            {
                segments[i].X = new UIDim(0, posOffest);
                posOffest += segments[i].Rect.Width;
            }
            RequestRedraw();
        }
        #endregion

        #region Trigger Methods
        /// <summary>
        ///		Handler invoked internally when the sort column is changed.
        /// </summary>
        /// <param name="e">Events args.</param>
        protected internal void OnSortColumnChanged(WindowEventArgs e)
        {
            if (SortColumnChanged != null)
            {
                SortColumnChanged(this, e);
            }
        }

        /// <summary>
        ///		Handler invoked internally when the dort direction is changed.
        /// </summary>
        /// <param name="e">Events args.</param>
        protected internal void OnSortDirectionChanged(WindowEventArgs e)
        {
            if (SortDirectionChanged != null)
            {
                SortDirectionChanged(this, e);
            }
        }
        /// <summary>
        ///		Handler invoked internally when a segment size is changed.
        /// </summary>
        /// <param name="e">Events args.</param>
        protected internal void OnSegmentSized(WindowEventArgs e)
        {
            if (SegmentSized != null)
            {
                SegmentSized(this, e);
            }
        }
        /// <summary>
        ///		Handler invoked internally when a segment is clicked.
        /// </summary>
        /// <param name="e">Events args.</param>
        protected internal void OnSegmentClicked(WindowEventArgs e)
        {
            if (SegmentClicked != null)
            {
                SegmentClicked(this, e);
            }
        }
        /// <summary>
        ///		Handler invoked internally when the segment order is changed.
        /// </summary>
        /// <param name="e">Events args.</param>
        protected internal void OnSegmentSequenceChanged(HeaderSequenceEventArgs e)
        {
            if (SegmentSequenceChanged != null)
            {
                SegmentSequenceChanged(this, e);
            }
        }
        /// <summary>
        ///		Handler invoked internally when a segment is added.
        /// </summary>
        /// <param name="e">Events args.</param>
        protected internal void OnSegmentAdded(WindowEventArgs e)
        {
            if (SegmentAdded != null)
            {
                SegmentAdded(this, e);
            }
        }
        /// <summary>
        ///		Handler invoked internally when a segment is removed.
        /// </summary>
        /// <param name="e">Events args.</param>
        protected internal void OnSegmentRemoved(WindowEventArgs e)
        {
            if (SegmentRemoved != null)
            {
                SegmentRemoved(this, e);
            }
        }
        /// <summary>
        ///		Handler invoked internally when user control of sorting is enabled or disabled.
        /// </summary>
        /// <param name="e">Events args.</param>
        protected internal void OnSortSettingChanged(WindowEventArgs e)
        {
            if (SortSettingChanged != null)
            {
                SortSettingChanged(this, e);
            }
        }

        /// <summary>
        ///		Handler invoked internally when user sizing of segments is enabled or disabled.
        /// </summary>
        /// <param name="e">Events args.</param>
        protected internal void OnDragSizeSettingChanged(WindowEventArgs e)
        {
            if (DragSizeSettingChanged != null)
            {
                DragSizeSettingChanged(this, e);
            }
        }

        private void SegmentSized_handler(object sender, WindowEventArgs e)
        {
            LayoutSegments();

            // fire event
            OnSegmentSized(e);
        }
        #endregion

        #region Serialization
        protected override void OnStringDictionaryChanged()
        {
        }

        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);
        
            foreach (Window window in children)
            {
                UIListHeaderSegment segment = window as UIListHeaderSegment;
                if (segment != null)
                {
                    this.segments.Add(segment);
                    segment.SegmentSized += new EventHandler<WindowEventArgs>(SegmentSized_handler);
                }
            }

            if (this.backgroundRenderComp != null)
            {
                this.backgroundRenderComp.PostCustomSerial(this);
            }

        }
        #endregion Serialization
    }
}
