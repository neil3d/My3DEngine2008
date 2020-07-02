using System;
using System.ComponentModel;
using Nexus.GUI.Components;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    #region GridReference Structure

    /// <summary>
    ///		Simple structure used to hold grid references.
    /// </summary>
    [XmlClassSerializable("GridReference")]
    public struct GridReference
    {

        #region Fields

        /// <summary>
        /// The zero based row index of this grid reference.
        /// </summary>
        [XmlFieldSerializable("Column")]
        public int Row { get; set; }

        /// <summary>
        /// The zero based column index of this grid reference.
        /// </summary>
        [XmlFieldSerializable("Column")]
        public int Column { get; set; }

        #endregion

        #region Constructor

        public GridReference(int row, int col)
            :this()
        {
            Row = row;
            Column = col;
        }

        public GridReference(GridReference obj)
            :this()
        {
            Row = obj.Row;
            Column = obj.Column;
        }

        #endregion

        #region Operators
        public override int GetHashCode()
        {
            return this.Row.GetHashCode() + this.Column.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            GridReference other = (GridReference)obj;
            if (other == null)
            {
                return false;
            }
            return ((this.Row == other.Row) && (this.Column == other.Column));
        }


        public static bool operator ==(GridReference left, GridReference right)
        {
            return ((left.Row == right.Row) && (left.Column == right.Column));
        }

        public static bool operator !=(GridReference left, GridReference right)
        {
            return !(left == right);
        }

        #endregion
    }

    #endregion

    /// <summary>
    /// multi-column list / grid widget.
    /// </summary>
    [XmlClassSerializable("UIListView", true)]
    public class UIListView : Nexus.GUI.Window
    {
        #region Event Declarations

        /// <summary>
        ///		Event fired when the selection mode for the grid changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> SelectionModeChanged;

        /// <summary>
        ///		Event fired when the nominated select column changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> NominatedSelectColumnChanged;

        /// <summary>
        ///		Event fired when the nominated select row changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> NominatedSelectRowChanged;

        /// <summary>
        ///		Event fired when the vertical scroll bar 'force' setting changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> VerticalScrollBarModeChanged;

        /// <summary>
        ///		Event fired when the horizontal scroll bar 'force' setting changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> HorizontalScrollBarModeChanged;

        /// <summary>
        ///		Event fired when the current selection(s) within the grid changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> SelectionChanged;

        /// <summary>
        ///		Event fired when the contents of the grid changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> ContentsChanged;

        /// <summary>
        ///		Event fired when the sort column changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> SortColumnChanged;

        /// <summary>
        ///		Event fired when the sort direction changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> SortDirectionChanged;

        /// <summary>
        ///		Event fired when the width of a column in the grid changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> ColumnSized;

        /// <summary>
        ///		Event fired when the column order changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> ColumnMoved;

        #endregion

        #region Fields
        /// <summary>
        /// 垂直滚动条
        /// </summary>
        private UIVerticalScrollBar verticalScrollBar;
        /// <summary>
        ///		true if vertical scrollbar should always be displayed
        /// </summary>
        [XmlFieldSerializable("forceVerticalScrollbar")]
        private bool forceVerticalScrollbar;
        /// <summary>
        /// 水平滚动条
        /// </summary>
        private UIHorizontalScrollBar horizontalScrollBar;
        /// <summary>
        ///		true if horizontal scrollbar should always be displayed
        /// </summary>
        [XmlFieldSerializable("forceHorizontalScrollbar")]
        private bool forceHorizontalScrollbar;
        /// <summary>
        /// 表头
        /// </summary>
        private UIListHeader listHeader;
        /// <summary>
        /// 最近一次被选中的项
        /// </summary>
        private UIComp_DrawListboxItem lastSelectedItem;
        /// <summary>
        /// grid data
        /// </summary>
        [XmlFieldSerializable("gridData")]
        protected GridRowCollection gridData;

        /// <summary>
        ///		Allow multiple selections.
        /// </summary>
        [XmlFieldSerializable("multiSelect")]
        private bool multiSelect;

        /// <summary>
        /// 背景图渲染
        /// </summary>
        [XmlFieldSerializable("backgroundRenderComp")]
        protected UIComp_DrawTextureAtlas backgroundRenderComp;
        /// <summary>
        /// 渲染选中光标
        /// </summary>
        [XmlFieldSerializable("stickRenderComp")]
        protected UIComp_DrawTextureAtlas stickRenderComp;

        #endregion Fields

        #region Constructor
        public UIListView() { } //default ctor not valid - we want to enforce initializing our data
        public UIListView(string name)
            : base(name)
        {
            this.gridData = new GridRowCollection();

            this.stickRenderComp = new UIComp_DrawTextureAtlas(this);
            this.stickRenderComp.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            stickRenderComp.Width = new UIDim(1.0f, 0);
            this.stickRenderComp.AtlasItemName = @"N_sys-gb-a";

            this.backgroundRenderComp = new UIComp_DrawTextureAtlas(this);

            this.backgroundRenderComp.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            this.backgroundRenderComp.AtlasItemName = "0";

            // 创建子控件
            this.verticalScrollBar = CreateVerticalScrollBar();
            this.horizontalScrollBar = CreateHorizontalScrollBar();
            this.listHeader = CreateListHeader();

            InitEventHander();

        }
        #endregion Constructor

        #region Properties
        [Category("Action")]
        public bool MultiSelect
        {
            get { return multiSelect; }
            set { multiSelect = value; }
        }

        [Category("Component")]
        [Description("Display some image.")]
        public UIComp_DrawTextureAtlas BackgroundRenderComp
        {
            get { return backgroundRenderComp; }
        }

        [Category("Component")]
        [Description("Display stick image.")]
        public UIComp_DrawTextureAtlas StickRenderComp
        {
            get { return stickRenderComp; }
        }

        [TypeConverter(typeof(GridRowCollectionConverter))]
        public GridRowCollection GridData
        {
            get { return gridData; }
        }

        [Category("Action")]
        public bool UserSortControlEnabled
        {
            get
            {
                return listHeader.SortEnabled;
            }

            set
            {
                listHeader.SortEnabled = value;
            }
        }

        /// <summary>
        /// 可缩放列大小
        /// </summary>
        [Category("Action")]
        public bool UserColumnSizingEnabled
        {
            get
            {
                return listHeader.SegmentSizingEnabled;
            }

            set
            {
                listHeader.SegmentSizingEnabled = value;
            }
        }

        /// <summary>
        /// 总共有多少列
        /// </summary>
        [Category("Data")]
        public int ColumnCount
        {
            get
            {
                return listHeader.ColumnCount;
            }
        }

        /// <summary>
        /// 总共有多少行
        /// </summary>
        [Category("Data")]
        public int RowCount
        {
            get { return gridData.Count; }
        }

        [Category("Data")]
        public int SelectedItemCount
        {
            get
            {
                int count = 0;
                foreach (var row in gridData)
                {
                    foreach (UIComp_DrawListboxItem item in row)
                    {
                        if (item != null && item.Selected)
                        {
                            ++count;
                        }
                    }
                }
                return count;
            }
        }

        [Category("Action")]
        public int SortColumnIndex
        {
            get
            {
                return listHeader.SortColumnIndex;
            }

            set
            {
                if (listHeader.SortColumnIndex != value)
                {
                    listHeader.SortColumnIndex = value;
                }
            }
        }

        /// <summary>
        /// 总表头宽度
        /// </summary>
        [Category("Data")]
        public float TotalColumnHeadersWidth
        {
            get
            {
                return listHeader.TotalPixelExtent;
            }
        }

        [Category("Data")]
        public SortDirection SortDirection
        {
            get
            {
                return listHeader.SortDirection;
            }

            set
            {
                if (listHeader.SortDirection != value)
                {
                    listHeader.SortDirection = value;
                }
            }
        }

        /// <summary>
        /// 总行高
        /// </summary>
        protected float TotalRowsHeight
        {
            get
            {
                float height = 0.0f;

                foreach (var row in gridData)
                {
                    if( row != null )
                    {
                        height += row.GetHighestRowItemHeight();
                    }
                }

                return height;
            }
        }

        /// <summary>
        /// 总是显示垂直滚动条
        /// </summary>
        [Category("Data")]
        public bool AlwaysShowVerticalScrollBar
        {
            get
            {
                return forceVerticalScrollbar;
            }

            set
            {
                if (forceVerticalScrollbar != value)
                {
                    forceVerticalScrollbar = value;

                    ConfigureScrollbars();

                    OnVerticalScrollBarModeChanged(new WindowEventArgs(this));
                }
            }
        }

        /// <summary>
        /// 总是显示水平滚动条
        /// </summary>
        [Category("Data")]
        public bool AlwaysShowHorizontalScrollBar
        {
            get
            {
                return forceHorizontalScrollbar;
            }

            set
            {
                if (forceHorizontalScrollbar != value)
                {
                    forceHorizontalScrollbar = value;

                    ConfigureScrollbars();

                    OnHorizontalScrollBarModeChanged(new WindowEventArgs(this));
                }
            }
        }

        #endregion Properties

        #region Methods
        protected virtual UIListHeader CreateListHeader()
        {
            UIListHeader header = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIListHeader", name + "_header", this) as UIListHeader;

            header.Height = new UIDim(0, 28);
            header.Width = new UIDim(1.0f, 0);
            header.Y = new UIDim(0, -header.Height.offest);

            return header;
        }

        protected virtual UIHorizontalScrollBar CreateHorizontalScrollBar()
        {
            UIHorizontalScrollBar hsb = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIHorizontalScrollBar", name + "_HSB", this) as UIHorizontalScrollBar;

            hsb.Height = new UIDim(0, 28);
            hsb.Width = new UIDim(1.0f, 0);
            hsb.Y = new UIDim(1, 0);

            return hsb;
        }

        protected virtual UIVerticalScrollBar CreateVerticalScrollBar()
        {
            UIVerticalScrollBar vsb = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIVerticalScrollBar", name + "_VSB", this) as UIVerticalScrollBar;

            vsb.Width = new UIDim(0, 28);
            vsb.Height = new UIDim(1.0f, 0);
            vsb.X = new UIDim(1, 0);

            return vsb;
        }


        /// <summary>
        ///		Setup size and position for the component widgets.
        /// </summary>
        protected void LayoutComponentWidgets()
        {
            float yOffest = - verticalScrollBar.ScrollPosition;
            for ( int row =0; row <RowCount; ++row)
            {
                float xOffest = - horizontalScrollBar.ScrollPosition;
                float taller = 0.0f;
                for (int col = 0; col<ColumnCount;++col)
                {
                    UIComp_DrawListboxItem item = gridData[row][col];
                    item.X = new UIDim(0, xOffest);
                    item.Width = new UIDim(0, GetColumnPixelWidth(col)); 
                    item.Y = new UIDim(0, yOffest);
                    xOffest += item.Size.x;
                    taller = Math.Max(taller, item.Size.y);
                }
                yOffest += taller;
            }

            listHeader.SegmentOffset = -horizontalScrollBar.ScrollPosition;

            RequestRedraw();
        }

        protected override void PostDrawSelf()
        {
            base.PostDrawSelf();
            GUISystem.Instance.Renderer.PopScissorRect();
        }

        protected override void DrawSelf()
        {
            // 渲染背景图片
            backgroundRenderComp.Draw();

            // 裁剪单元格渲染
            GUISystem.Instance.Renderer.PushScissorRect(this.Rect);
           
            // 渲染单元格
            foreach (var row in gridData)
            {
                float stickY = this.Rect.bottom;
                float stickH = 0.0f;

                foreach (UIComp_DrawListboxItem item in row.RowItems)
                {
                    if (item != null && this.Rect.Intersection(item.TargetRect))
                    {
                        item.Draw();

                        if (item.Selected)
                        {
                            if(stickH < item.Size.y) 
                            {
                                stickH = item.Size.y;
                            }
                            if( stickY > item.TargetRect.top )
                            {
                                stickY = item.TargetRect.top;
                            }
                        }
                    }
                }

                // 渲染选中光标
                if( stickH > 0.0f )
                {
                    GUISystem.Instance.Renderer.DrawTexture(stickRenderComp.Texture,
                        stickRenderComp.TextureUVRect,
                        new Rect(stickRenderComp.TargetRect.left, stickY, stickRenderComp.TargetRect.right, stickY + stickH), 
                        stickRenderComp.TextureColor);
                }
            }
        }

        #endregion Methods

        #region Accessor type methods

        /// <summary>
        ///		Return the width of the column at the specified zero based index.
        /// </summary>
        /// <param name="columnIndex">Zero based index of the column whos width is to be returned.</param>
        /// <returns>Width of the column at the zero based column index specified by 'columnIndex', in absolute pixels.</returns>
        /// <exception cref="InvalidRequestException">thrown if columnIndex is out of range.</exception>
        public float GetColumnPixelWidth(int columnIndex)
        {
            return listHeader.GetPixelWidthOfColumn(columnIndex);
        }

        /// <summary>
        ///		Return the UIListHeaderSegment object for the column at the specified index.
        /// </summary>
        /// <param name="columnIndex">Zero based index of the column whos UIListHeaderSegment is to be returned.</param>
        /// <returns>UIListHeaderSegment object for the column at the requested index.</returns>
        /// <exception cref="InvalidRequestException">thrown if columnIndex is out of range.</exception>
        public UIListHeaderSegment GetColumnHeaderSegment(int columnIndex)
        {
            return listHeader.Segments[columnIndex];
        }

        /// <summary>
        ///		Return the zero based index of the Row that contains the given UIComp_DrawListboxItem.
        /// </summary>
        /// <param name="item">UIComp_DrawListboxItem for which the row index is to be returned.</param>
        /// <returns>Zero based index of the row that contains UIComp_DrawListboxItem 'item'.</returns>
        /// <exception cref="InvalidRequestException">thrown if 'item' is not attached to the grid.</exception>
        public int GetRowIndexOfItem(UIComp_DrawListboxItem item)
        {
            for (int i = 0; i < RowCount; ++i)
            {
                if (IsItemInRow(item, i))
                {
                    return i;
                }
            }

            throw new InvalidRequestException("The specified UIComp_DrawListboxItem is not attached to the grid.");
        }

        /// <summary>
        ///		Return the zero based index of the column that contains 'item'.
        /// </summary>
        /// <param name="item">UIComp_DrawListboxItem for which the column index is to returned.</param>
        /// <returns>Zero based index of the column that contains UIComp_DrawListboxItem 'item'.</returns>
        /// <exception cref="InvalidRequestException">thrown if 'item' is not attached to the grid.</exception>
        public int GetColumnIndexOfItem(UIComp_DrawListboxItem item)
        {
            for (int i = 0; i < ColumnCount; ++i)
            {
                if (IsItemInColumn(item, i))
                {
                    return i;
                }
            }

            throw new InvalidRequestException("The specified UIComp_DrawListboxItem is not attached to the grid.");
        }

        /// <summary>
        ///		Return the grid reference for 'item'.
        /// </summary>
        /// <param name="item">UIComp_DrawListboxItem for which the grid reference is to be returned.</param>
        /// <returns>GridReference describing the current grid reference of UIComp_DrawListboxItem 'item'.</returns>
        /// <exception cref="InvalidRequestException">thrown if 'item' is not attached to the grid.</exception>
        public GridReference GetGridReferenceOfItem(UIComp_DrawListboxItem item)
        {
            return new GridReference(GetRowIndexOfItem(item), GetColumnIndexOfItem(item));
        }

        /// <summary>
        ///		Return the UIComp_DrawListboxItem at the specified grid reference.
        /// </summary>
        /// <param name="gridRef">GridReference that describes the position of the UIComp_DrawListboxItem to be returned.</param>
        /// <returns>UIComp_DrawListboxItem at grid reference 'gridRef'</returns>
        /// <exception cref="InvalidRequestException">thrown if 'gridRef' is invalid.</exception>
        public UIComp_DrawListboxItem GetItemAtGridReference(GridReference gridRef)
        {
            // check for invalid grid ref
            if (gridRef.Column >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The column index {0} is invalid for this grid.", gridRef.Column));
                return null;
            }
            else if (gridRef.Row >= RowCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The row index {0} is invalid for this grid.", gridRef.Row));
                return null;
            }
            else
            {
                return gridData[gridRef.Row][gridRef.Column];
            }
        }

        /// <summary>
        ///		Return whether UIComp_DrawListboxItem 'item' is attached to the column at index 'columnIndex'.
        /// </summary>
        /// <param name="item">UIComp_DrawListboxItem to look for.</param>
        /// <param name="columnIndex">Zero based index of the column that is to be searched.</param>
        /// <returns>
        ///		- true if 'item' is attached to column 'columnIndex'.
        ///		- false if 'item' is not attached to column 'columnIndex'.
        /// </returns>
        /// <exception cref="InvalidRequestException">thrown if 'columnIndex' is out of range.</exception>
        public bool IsItemInColumn(UIComp_DrawListboxItem item, int columnIndex)
        {
            // check for invalid index
            if (columnIndex >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "Column index {0} is invalid for this grid.", columnIndex));
                return false;
            }
            else
            {
                for (int i = 0; i < RowCount; ++i)
                {
                    if (gridData[i][columnIndex] == item)
                    {
                        return true;
                    }
                }

                // item was not in search column
                return false;
            }
        }

        /// <summary>
        ///		Return whether UIComp_DrawListboxItem 'item' is attached to the row at index 'rowIndex'.
        /// </summary>
        /// <param name="item">UIComp_DrawListboxItem to look for.</param>
        /// <param name="rowIndex">Zero based index of the row that is to be searched.</param>
        /// <returns>
        ///		- true if 'item' is attached to row 'rowIndex'.
        ///		- false if 'item' is not attached row 'rowIndex'.
        /// </returns>
        /// <exception cref="InvalidRequestException">thrown if 'rowIndex' is out of range.</exception>
        public bool IsItemInRow(UIComp_DrawListboxItem item, int rowIndex)
        {
            // check for invalid index
            if (rowIndex >= RowCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "Row index {0} is invalid for this grid.", rowIndex));
                return false;
            }
            else
            {
                for (int i = 0; i < ColumnCount; ++i)
                {
                    if (gridData[rowIndex][i] == item)
                    {
                        return true;
                    }
                }

                // item was not in search row
                return false;
            }
        }

        /// <summary>
        ///		Return whether UIComp_DrawListboxItem 'item' is attached to the grid.
        /// </summary>
        /// <param name="item">UIComp_DrawListboxItem to look for.</param>
        /// <returns>
        ///		- true if 'item' is attached to the grid.
        ///		- false if 'item' is not attached to the grid.
        /// </returns>
        public bool IsItemInGrid(UIComp_DrawListboxItem item)
        {
            for (int row = 0; row < RowCount; ++row)
            {
                for (int column = 0; column < ColumnCount; ++column)
                {
                    if (gridData[row][column] == item)
                    {
                        return true;
                    }
                }
            }

            // item was not in grid.
            return false;
        }

        /// <summary>
        ///		Return a the first selected UIComp_DrawListboxItem attached to the grid.
        /// </summary>
        /// <remarks>Searching progresses across the columns in each row.</remarks>
        /// <returns>The first UIComp_DrawListboxItem attached to the grid that is selected, or null if no item is selected.</returns>
        public UIComp_DrawListboxItem GetFirstSelectedItem()
        {
            return GetNextSelectedItem(null);
        }

        /// <summary>
        ///		Return the next selected UIComp_DrawListboxItem after 'startItem'.
        /// </summary>
        /// <remarks>Searching progresses across the columns in each row.</remarks>
        /// <param name="startItem">UIComp_DrawListboxItem where the exclusive search is to start, or null to search the whole grid.</param>
        /// <returns>The first selected UIComp_DrawListboxItem attached to the grid, after 'startItem', or null if no such item is selected.</returns>
        /// <exception cref="InvalidRequestException">thrown if 'startItem' is not attached to the grid.</exception>
        public UIComp_DrawListboxItem GetNextSelectedItem(UIComp_DrawListboxItem startItem)
        {
            GridReference searchRef;

            // obtain starting location
            if (startItem == null)
            {
                searchRef = new GridReference(0, 0);
            }
            else
            {
                searchRef = GetGridReferenceOfItem(startItem);
            }

            // perform search
            for (; searchRef.Row < RowCount; ++searchRef.Row)
            {
                for (; searchRef.Column < ColumnCount; ++searchRef.Column)
                {
                    // is this a match?
                    UIComp_DrawListboxItem item = gridData[searchRef.Row][searchRef.Column];
                    if ((item != null) && item.Selected)
                    {
                        return item;
                    }
                }
            }

            // no match
            return null;
        }

        /// <summary>
        ///		Return whether the UIComp_DrawListboxItem at 'gridRef' is selected.
        /// </summary>
        /// <param name="gridRef">GridReference describing the grid reference that is to be examined.</param>
        /// <returns>
        ///		- true if there is a UIComp_DrawListboxItem at 'gridRef' and it is selected.
        ///		- false if there is no UIComp_DrawListboxItem at 'gridRef', or if the item is not selected.
        /// </returns>
        /// <exception cref="InvalidRequestException">thrown if 'gridRef' contains an invalid grid position.</exception>
        public bool IsItemSelected(GridReference gridRef)
        {
            UIComp_DrawListboxItem item = GetItemAtGridReference(gridRef);

            if (item != null)
            {
                return item.Selected;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        ///		Remove all items from the grid.
        /// </summary>
        public void ResetList()
        {
            if (ResetListImpl())
            {
                ResortGrid();
                ConfigureScrollbars();
                LayoutComponentWidgets();

                OnContentsChanged(new WindowEventArgs(this));
            }
        }

        /// <summary>
        ///		Add a column to the grid.
        /// </summary>
        /// <param name="heading">string containing the text label for the column header.</param>
        /// <param name="width">Initial width to be set for the column using the active metrics mode for this window.</param>
        public void AddColumn(string heading, float width)
        {
            InsertColumn(heading, width, ColumnCount);
        }

        /// <summary>
        /// Insert a new column in the grid.
        /// </summary>
        /// <param name="heading">string containing the text label for the column header.</param>
        /// <param name="width">Initial width to be set for the column using the active metrics mode for this window.</param>
        /// <param name="positionIndex">Zero based index where the column is to be inserted.  If this is greater than the current number of columns, the new column is inserted at the end.</param>
        public void InsertColumn(string heading, float width, int positionIndex)
        {
            // if position is out of range, insert at end
            if (positionIndex > ColumnCount)
            {
                positionIndex = ColumnCount;
            }

            // insert new column into header
            listHeader.InsertColumn(heading, width, positionIndex);

            // insert a blank entry for new column in each row
            for (int i = 0; i < RowCount; ++i)
            {
                gridData[i].RowItems.Insert(positionIndex, new UIComp_DrawListboxItem(this));
            }

            ResortGrid();
            ConfigureScrollbars();
            LayoutComponentWidgets();

            // signal that the grid content has changed
            OnContentsChanged(new WindowEventArgs(this));
        }

        /// <summary>
        ///		Removes a column from the grid.
        /// </summary>
        /// <param name="columnIndex">Zero based index of the column to be removed.</param>
        /// <exception cref="InvalidRequestException">thrown if 'columnIndex' is invalid.</exception>
        public void RemoveColumn(int columnIndex)
        {
            // check for invalid index
            if (columnIndex >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "Column index {0} is invalid for this grid.", columnIndex));
            }
            else
            {
                // remove the column entry in each row
                for (int i = 0; i < RowCount; ++i)
                {
                    gridData[i].RowItems.RemoveAt(columnIndex);
                }

                // remove the header segment
                listHeader.RemoveColumn(columnIndex);

                ResortGrid();
                ConfigureScrollbars();
                LayoutComponentWidgets();

                // signal that the grid content has changed
                OnContentsChanged(new WindowEventArgs(this));
            }
        }

        /// <summary>
        ///		Move the column at index 'columnIndex' so it is at index 'positionIndex'.
        /// </summary>
        /// <param name="columnIndex">Zero based index of the column to be moved.</param>
        /// <param name="positionIndex">Zero based index of the new position for the column.</param>
        public void MoveColumn(int columnIndex, int positionIndex)
        {
            // all we have to do here is move the header segment.  Events will ensure that
            // the data gets moved.
            listHeader.MoveColumn(columnIndex, positionIndex);
        }
        
        /// <summary>
        /// 追加一行
        /// </summary>
        public int AppendRow()
        {
            return InsertRow(RowCount);
        }

        /// <summary>
        /// 插入一行
        /// </summary>
        /// <param name="item"></param>
        /// <param name="rowIndex"></param>
        /// <returns></returns>
        public int InsertRow(int rowIndex)
        {
            // build new row
            GridRow row = new GridRow();
            for (int i = 0; i < ColumnCount; ++i)
            {
                row.RowItems.Add(new UIComp_DrawListboxItem(this));
            }

            // if insert index is too big, insert at end
            if (rowIndex > RowCount)
            {
                rowIndex = RowCount;
            }
            gridData.Insert(rowIndex, row);

            row.SortColumnIndex = SortColumnIndex;

            //ResortGrid();
            ConfigureScrollbars();
            LayoutComponentWidgets();

            // signal that the grid content has changed
            OnContentsChanged(new WindowEventArgs(this));

            return rowIndex;
        }

        /// <summary>
        ///		Remove the grid row at index 'rowIndex'.
        /// </summary>
        /// <param name="rowIndex">Zero based index of the row to be removed.</param>
        /// <exception cref="InvalidRequestException">thrown if 'rowIndex' is invalid.</exception>
        public void RemoveRow(int rowIndex)
        {
            // check index
            if (rowIndex >= RowCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The row index {0} is invalid for this grid.", rowIndex));
            }
            else
            {
                // remove row from the grid
                gridData.RemoveAt(rowIndex);

                ResortGrid();
                ConfigureScrollbars();
                LayoutComponentWidgets();

                // signal that the grid content has changed
                OnContentsChanged(new WindowEventArgs(this));
            }
        }


        public UIComp_DrawListboxItem GetGridItem(GridReference gridRef)
        {
            return GetGridItem(gridRef.Row, gridRef.Column);
        }

        public UIComp_DrawListboxItem GetGridItem(int rowIndex, int colIndex)
        {
            if (rowIndex >= 0 && rowIndex < RowCount && colIndex >= 0 && colIndex < ColumnCount)
            {
                return gridData[rowIndex][colIndex];
            }
            return null;
        }

        /// <summary>
        ///		Set the UIComp_DrawListboxItem for grid reference 'gridRef'.
        /// </summary>
        /// <param name="gridRef">GridReference describing the location of the item to be set.</param>
        /// <param name="item">UIComp_DrawListboxItem to be set at 'gridRef'.</param>
        /// <exception cref="InvalidRequestException">thrown if 'gridRef' contains an invalid grid reference.</exception>
        public void SetGridItem(GridReference gridRef, UIComp_DrawListboxItem item)
        {
            // check for invalid grid ref
            if (gridRef.Column >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The column index {0} is invalid for this grid.", gridRef.Column));
            }
            else if (gridRef.Row >= RowCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The row index {0} is invalid for this grid.", gridRef.Row));
            }
            else
            {
                // set new item into grid position
                gridData[gridRef.Row][gridRef.Column] = item;

                ResortGrid();
                ConfigureScrollbars();
                LayoutComponentWidgets();

                // signal that the grid content has changed
                OnContentsChanged(new WindowEventArgs(this));
            }
        }

        public void SetGridItem(GridReference gridRef,string text)
        {
            SetGridItem(gridRef.Row, gridRef.Column, text);
        }
        public void SetGridItem(int rowIndex, int colIndex, string text)
        {
            UIComp_DrawListboxItem item = GetGridItem(rowIndex, colIndex);
            if(item!=null)
            {
                item.Text = text;
            }
        }

         public void SetGridItem(int rowIndex, int colIndex, UIComp_DrawListboxItem item)
        {
            SetGridItem(new GridReference(rowIndex, colIndex), item);
        }

        /// <summary>
        ///		Remove the selected state from any currently selected UIComp_DrawListboxItem attached to the grid.
        /// </summary>
        public void ClearAllSelections()
        {
            if (ClearAllSelectionsImpl())
            {
                OnSelectionChanged(new WindowEventArgs(this));
            }
        }

        /// <summary>
        ///		Sets or clears the selected state of the given UIComp_DrawListboxItem (which must be already attached to the grid).
        /// </summary>
        /// <remarks>
        ///		Depending upon the current selection mode, this may cause other items to be selected, other
        ///		items to be deselected, or for nothing to actually happen at all.
        /// </remarks>
        /// <param name="item">UIComp_DrawListboxItem to be affected.</param>
        /// <param name="state">
        ///		- true to put the UIComp_DrawListboxItem into the selected state.
        ///		- false to put the UIComp_DrawListboxItem into the de-selected state.
        /// </param>
        /// <exception cref="InvalidRequestException">thrown if 'item' is not attached to the grid.</exception>
        public void SetItemSelectState(UIComp_DrawListboxItem item, bool state)
        {
            SetItemSelectState(GetGridReferenceOfItem(item), state);
        }

        /// <summary>
        ///		Sets or clears the selected state of the UIComp_DrawListboxItem at the given grid reference.
        /// </summary>
        /// <remarks>
        ///		Depending upon the current selection mode, this may cause other items to be selected, other
        ///		items to be deselected, or for nothing to actually happen at all.
        /// </remarks>
        /// <param name="gridRef">GridReference describing the position of the item to be affected.</param>
        /// <param name="state">
        ///		- true to put the UIComp_DrawListboxItem into the selected state.
        ///		- false to put the UIComp_DrawListboxItem into the de-selected state.
        /// </param>
        /// <exception cref="InvalidRequestException">thrown if 'gridRef' is invalid.</exception>
        public void SetItemSelectState(GridReference gridRef, bool state)
        {
            if (SetItemSelectStateImpl(gridRef, state))
            {
                OnSelectionChanged(new WindowEventArgs(this));
            }
        }

        /// <summary>
        ///		Inform the grid that one or more attached ListboxItems have been externally modified, and
        ///		the list should re-sync its internal state and refresh the display as needed.
        /// </summary>
        public void HandleUpdatedItemData()
        {
            ConfigureScrollbars();
            RequestRedraw();
        }

        /// <summary>
        ///		Set the width of the specified column.
        /// </summary>
        /// <param name="columnIndex">Zero based column index of the column whos width is to be set.</param>
        /// <param name="width">float value specifying the new width for the column using the active metrics system.</param>
        /// <exception cref="InvalidRequestException">thrown if 'columnIndex' is out of range.</exception>
        public void SetColumnWidth(int columnIndex, float width)
        {
            listHeader.SetColumnPixelWidth(columnIndex, width);
        }
        #endregion Accessor type methods

        #region Methods
        /// <summary>
        ///		display required integrated scroll bars according to current state of the list box and update their values.
        /// </summary>
        protected void ConfigureScrollbars()
        {
            float totalHeight = TotalRowsHeight;
            float totalWidth = listHeader.TotalPixelExtent;

            //
            // First show or hide the scroll bars as needed (or requested)
            //
            // show or hide vertical scroll bar as required (or as specified by option)
            Rect ListRenderArea = this.Rect;
            if ((totalHeight > ListRenderArea.Height) || forceVerticalScrollbar)
            {
                verticalScrollBar.Show();

                // show or hide horizontal scroll bar as required (or as specified by option)
                if ((totalWidth > ListRenderArea.Width) || forceHorizontalScrollbar)
                {
                    horizontalScrollBar.Show();
                }
                else
                {
                    horizontalScrollBar.Hide();
                }
            }
            else
            {
                // show or hide horizontal scroll bar as required (or as specified by option)
                if ((totalWidth > ListRenderArea.Width) || forceHorizontalScrollbar)
                {
                    horizontalScrollBar.Show();

                    // show or hide vertical scroll bar as required (or as specified by option)
                    if ((totalHeight > ListRenderArea.Height) || forceVerticalScrollbar)
                    {
                        verticalScrollBar.Show();
                    }
                    else
                    {
                        verticalScrollBar.Hide();
                    }
                }
                else
                {
                    verticalScrollBar.Hide();
                    horizontalScrollBar.Hide();
                }
            }

            //
            // Set up scroll bar values
            //
            Rect renderArea = ListRenderArea;

            verticalScrollBar.DocumentSize = totalHeight;
            verticalScrollBar.PageSize = renderArea.Height;
            verticalScrollBar.StepSize = Math.Max(1.0f, verticalScrollBar.PageSize * 0.1f);
            //verticalScrollBar.ScrollPosition = verticalScrollBar.ScrollPosition;

            horizontalScrollBar.DocumentSize = totalWidth;
            horizontalScrollBar.PageSize = renderArea.Width;
            horizontalScrollBar.StepSize = Math.Max(1.0f, horizontalScrollBar.PageSize * 0.1f);
            //horizontalScrollBar.ScrollPosition = horizontalScrollBar.ScrollPosition;
        }

        /// <summary>
        ///		select all items between positions 'start' and 'end' (inclusive).
        /// </summary>
        /// <param name="start"></param>
        /// <param name="end"></param>
        /// <returns>true if something was modified.</returns>
        protected bool SelectRange(GridReference start, GridReference end)
        {
            GridReference tmpStart = new GridReference(start);
            GridReference tmpEnd = new GridReference(end);

            // ensure start is before end
            if (tmpStart.Row > tmpEnd.Row)
            {
                tmpStart.Row = tmpEnd.Row;
                tmpEnd.Row = start.Row;
            }

            if (tmpStart.Column > tmpEnd.Column)
            {
                tmpStart.Column = tmpEnd.Column;
                tmpEnd.Column = start.Column;
            }

            bool modified = false;

            // loop through range, selecting items
            for (int row = tmpStart.Row; row <= tmpEnd.Row; ++row)
            {
                for (int column = tmpStart.Column; column <= tmpEnd.Column; ++column)
                {
                    UIComp_DrawListboxItem item = gridData[row][column];

                    if (item != null)
                    {
                        modified |= SetItemSelectStateImpl(GetGridReferenceOfItem(item), true);
                    }
                }
            }

            return modified;
        }

        /// <summary>
        ///		Return the width of the widest item in the given column
        /// </summary>
        /// <param name="columnIndex"></param>
        /// <returns>width of the widest item in the given column</returns>
        protected float GetWidestColumnItemWidth(int columnIndex)
        {
            if (columnIndex >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "Column index {0} is invalid for the grid.", columnIndex));
                return 0;
            }
            else
            {
                float widest = 0.0f;

                for (int i = 0; i < RowCount; ++i)
                {
                    UIComp_DrawListboxItem item = gridData[i][columnIndex];

                    // if slot has an item
                    if (item != null)
                    {
                        Vector2 size = item.Size;

                        // is this item widest so far?
                        if (size.x > widest)
                        {
                            widest = size.x;
                        }
                    }
                }

                return widest;
            }
        }

        /// <summary>
        ///		Return the height of the highest item in the given row.
        /// </summary>
        /// <param name="rowIndex"></param>
        /// <returns>height of the highest item in the given row.</returns>
        protected float GetHighestRowItemHeight(int rowIndex)
        {
            if (rowIndex >= RowCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "Row index {0} is invalid for the grid.", rowIndex));
                return 0;
            }
            else
            {
                float tallest = 0.0f;

                for (int i = 0; i < ColumnCount; ++i)
                {
                    UIComp_DrawListboxItem item = gridData[rowIndex][i];

                    // if slot has an item
                    if (item != null)
                    {
                        Vector2 size = item.Size;

                        // is this item tallest so far?
                        if (size.y > tallest)
                        {
                            tallest = size.y;
                        }
                    }
                }

                return tallest;
            }
        }

        /// <summary>
        ///		Clear the selected state for all items (implementation)
        /// </summary>
        /// <returns>true if some selections were cleared, false if nothing was changed.</returns>
        protected bool ClearAllSelectionsImpl()
        {
            bool modified = false;

            for (int row = 0; row < RowCount; ++row)
            {
                for (int column = 0; column < ColumnCount; ++column)
                {
                    UIComp_DrawListboxItem item = gridData[row][column];

                    if ((item != null) && item.Selected)
                    {
                        item.Selected = false;
                        modified = true;
                    }
                }
            }

            return modified;
        }

        /// <summary>
        ///		Return the UIComp_DrawListboxItem under the given window local pixel co-ordinate.
        /// </summary>
        /// <param name="point"></param>
        /// <returns>
        ///		UIComp_DrawListboxItem that is under window pixel co-ordinate 'point', or null if no
        ///		item is under that position.
        /// </returns>
        protected UIComp_DrawListboxItem GetItemAtPoint(Vector2 point)
        {
            Rect listArea = this.Rect;

            float y = listArea.top - verticalScrollBar.ScrollPosition;
            float x = listArea.left - horizontalScrollBar.ScrollPosition;

            for (int row = 0; row < RowCount; ++row)
            {
                y += GetHighestRowItemHeight(row);

                // is this the row?
                if (point.y < y)
                {
                    // scan across to find the column
                    for (int column = 0; column < ColumnCount; ++column)
                    {
                        x += GetColumnPixelWidth(column);

                        // is this the column?
                        if (point.x < x)
                        {
                            // return item at this grid cell
                            return gridData[row][column];
                        }
                    }
                }
            }

            // no item was clicked
            return null;
        }

        /// <summary>
        ///		Set select state for the given item.
        ///		This appropriately selects other  items depending upon the select mode.
        /// </summary>
        /// <param name="gridRef"></param>
        /// <param name="state"></param>
        /// <returns>true if something is changed, else false.</returns>
        protected bool SetItemSelectStateImpl(GridReference gridRef, bool state)
        {
            // check for invalid grid ref
            if (gridRef.Column >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The column index {0} is invalid for this grid.", gridRef.Column));
            }
            else if (gridRef.Row >= RowCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "The row index {0} is invalid for this grid.", gridRef.Row));
            }

            // only continue if state is changing
            if (gridData[gridRef.Row][gridRef.Column].Selected != state)
            {
                // clear current selection if not multi-select box
                if (state && !multiSelect)
                {
                    ClearAllSelectionsImpl();
                }

                SetSelectForItemsInRow(gridRef.Row, state);
     
                return true;
            }

            return false;
        }

        /// <summary>
        ///		Set select state for all items in the given row
        /// </summary>
        /// <param name="rowIndex"></param>
        /// <param name="state"></param>
        protected void SetSelectForItemsInRow(int rowIndex, bool state)
        {
            for (int i = 0; i < ColumnCount; ++i)
            {
                UIComp_DrawListboxItem item = gridData[rowIndex][i];

                if (item != null)
                {
                    item.Selected = state;
                }
            }
        }

        /// <summary>
        ///		Set select state for all items in the given column
        /// </summary>
        /// <param name="columnIndex"></param>
        /// <param name="state"></param>
        protected void SetSelectForItemsInColumn(int columnIndex, bool state)
        {
            for (int i = 0; i < RowCount; ++i)
            {
                UIComp_DrawListboxItem item = gridData[i][columnIndex];

                if (item != null)
                {
                    item.Selected = state;
                }
            }
        }

        /// <summary>
        ///		Move the column at index 'columnIndex' so it is at index 'positionIndex'.
        ///		Implementation version which does not move the header segment (since that may have already happned).
        /// </summary>
        /// <param name="columnIndex"></param>
        /// <param name="positionIndex"></param>
        /// <exception cref="InvalidRequestException">thrown if 'columnIndex' is invalid.</exception>
        protected void MoveColumnImpl(int columnIndex, int positionIndex)
        {
            // ensure column is valid
            if (columnIndex >= ColumnCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "Column index {0} is out of range for this grid.", columnIndex));
            }

            // if new position is too big, move to the end
            if (positionIndex > ColumnCount)
            {
                positionIndex = ColumnCount;
            }

            // move entry for this column in each row.
            for (int i = 0; i < RowCount; ++i)
            {
                UIComp_DrawListboxItem item = gridData[i][columnIndex];

                // remove old entry
                gridData[i].RowItems.RemoveAt(columnIndex);

                // insert item at new position
                gridData[i].RowItems.Insert(positionIndex, item);
            }
        }

        /// <summary>
        ///		Remove all items from the grid.
        /// </summary>
        /// <returns>
        ///		- true if the grid contents were changed.
        ///		- false if the grid contents were not changed (already empty?).
        /// </returns>
        protected bool ResetListImpl()
        {
            // return false if the list is empty
            if (RowCount == 0)
            {
                return false;
            }
            else
            {
                gridData.Clear();

                lastSelectedItem = null;

                return true;
            }
        }

        /// <summary>
        ///		Causes a re-sort of the grid data in the appropriate direction
        /// </summary>
        protected void ResortGrid()
        {
            if (SortColumnIndex == -1)
                return;

            if (SortDirection == SortDirection.Descending)
            {
                gridData.SortDescending();
            }
            else if (SortDirection == SortDirection.Ascending)
            {
                gridData.SortAscending();
            }

            // else no sorting, so do nothing
        }
        #endregion

        #region Trigger Methods

        /// <summary>
        /// Internal handler triggered when the selection mode changes
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnSelectionModeChanged(WindowEventArgs e)
        {
            if (SelectionModeChanged != null)
            {
                SelectionModeChanged(this, e);
            }
        }

        /// <summary>
        /// Internal handler triggered when the nominated selection column is changed.
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnNominatedSelectColumnChanged(WindowEventArgs e)
        {
            if (NominatedSelectColumnChanged != null)
            {
                NominatedSelectColumnChanged(this, e);
            }
        }

        /// <summary>
        /// Internal handler triggered when the nominated selection row is changed.
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnNominatedSelectRowChanged(WindowEventArgs e)
        {
            if (NominatedSelectRowChanged != null)
            {
                NominatedSelectRowChanged(this, e);
            }
        }

        /// <summary>
        /// Internal handler triggered when the vertical scroll bar 'force' setting is changed.
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnVerticalScrollBarModeChanged(WindowEventArgs e)
        {
            if (VerticalScrollBarModeChanged != null)
            {
                VerticalScrollBarModeChanged(this, e);
            }
        }

        /// <summary>
        /// Internal handler triggered when the horizontal scroll bar 'force' setting is changed.
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnHorizontalScrollBarModeChanged(WindowEventArgs e)
        {
            if (HorizontalScrollBarModeChanged != null)
            {
                HorizontalScrollBarModeChanged(this, e);
            }
        }

        /// <summary>
        /// Internal handler triggered when the grid selection changes.
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnSelectionChanged(WindowEventArgs e)
        {
            RequestRedraw();

            if (SelectionChanged != null)
            {
                SelectionChanged(this, e);
            }
        }

        /// <summary>
        /// Internal handler triggered when the grid content changes.
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnContentsChanged(WindowEventArgs e)
        {
            if (ContentsChanged != null)
            {
                ContentsChanged(this, e);
            }
        }

        /// <summary>
        /// Internal handler triggered when the sort column changes.
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnSortColumnChanged(WindowEventArgs e)
        {
            RequestRedraw();

            if (SortColumnChanged != null)
            {
                SortColumnChanged(this, e);
            }
        }

        /// <summary>
        /// Internal handler triggered when the sort direction changes.
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnSortDirectionChanged(WindowEventArgs e)
        {
            RequestRedraw();

            if (SortDirectionChanged != null)
            {
                SortDirectionChanged(this, e);
            }
        }

        /// <summary>
        /// Internal handler triggered when a column size changes.
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnColumnSized(WindowEventArgs e)
        {
            ConfigureScrollbars();
            RequestRedraw();

            if (ColumnSized != null)
            {
                ColumnSized(this, e);
            }
        }

        /// <summary>
        /// Internal handler triggered when the column sequence changes.
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnColumnMoved(WindowEventArgs e)
        {
            RequestRedraw();

            if (ColumnMoved != null)
            {
                ColumnMoved(this, e);
            }
        }
        #endregion

        #region Overridden Event Triggers

        protected internal override void OnSized(GUIEventArgs e)
        {
            // base class processing
            base.OnSized(e);

            ConfigureScrollbars();
        }

        protected internal override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            // base class processing
            base.OnMouseButtonDown(e);

            if (e.Button == GUISystem.ClickButton)
            {
                bool modified = false;

                // clear old selections if no control key is pressed or if not multi-select
                if (!(GUISystem.Instance.Modifiers == System.Windows.Forms.Keys.Control) || !multiSelect)
                {
                    modified = ClearAllSelectionsImpl();
                }

                // get mouse position as a local pixel value
                UIComp_DrawListboxItem item = GetItemAtPoint(e.Position);
                if (item != null)
                {
                    modified = true;

                    // select range or item depending upon system keys and last selected item
                    if ((GUISystem.Instance.Modifiers==System.Windows.Forms.Keys.Shift) && (lastSelectedItem != null) && multiSelect)
                    {
                        modified |= SelectRange(GetGridReferenceOfItem(item), GetGridReferenceOfItem(lastSelectedItem));
                    }
                    else
                    {
                        modified |= SetItemSelectStateImpl(GetGridReferenceOfItem(item), item.Selected ^ true);
                    }

                    // update last selected item
                    lastSelectedItem = item.Selected ? item : null;
                }

                // fire event if something was changed.
                if (modified)
                {
                    OnSelectionChanged(new WindowEventArgs(this));
                }
            }
            return true;
        }

        protected internal override void OnMouseWheel(GUIMouseEventArgs e)
        {
            // base class processing
            base.OnMouseWheel(e);

            if (verticalScrollBar.Visible && (verticalScrollBar.DocumentSize > verticalScrollBar.PageSize))
            {
                verticalScrollBar.ScrollPosition = verticalScrollBar.ScrollPosition + verticalScrollBar.StepSize * (e.Delta > 0 ? -1 : 1);
            }
            else if (horizontalScrollBar.Visible && (horizontalScrollBar.DocumentSize > horizontalScrollBar.PageSize))
            {
                horizontalScrollBar.ScrollPosition = horizontalScrollBar.ScrollPosition + horizontalScrollBar.StepSize * (e.Delta > 0 ? -1 : 1);
            }
        }

        #endregion

        #region Event Handling methods
        private void InitEventHander()
        {
            // subscribe to event notifications
            listHeader.SegmentSequenceChanged += new EventHandler<HeaderSequenceEventArgs>(listHeader_SegmentSequenceChanged);
            listHeader.SegmentSized += new EventHandler<WindowEventArgs>(listHeader_SegmentSized);
            listHeader.SortColumnChanged += new EventHandler<WindowEventArgs>(listHeader_SortColumnChanged);
            listHeader.SortDirectionChanged += new EventHandler<WindowEventArgs>(listHeader_SortDirectionChanged);
            verticalScrollBar.ScrollPositionChanged += new EventHandler<WindowEventArgs>(verticalScrollBar_PositionChanged);
            horizontalScrollBar.ScrollPositionChanged += new EventHandler<WindowEventArgs>(horizontalScrollBar_PositionChanged);
        }

        private void listHeader_SegmentSequenceChanged(object sender, HeaderSequenceEventArgs e)
        {
            MoveColumnImpl(e.OldIndex, e.NewIndex);

            // signal change
            OnColumnMoved(new WindowEventArgs(this));
        }

        private void listHeader_SegmentSized(object sender, WindowEventArgs e)
        {
            ConfigureScrollbars();
            LayoutComponentWidgets();

            // signal change
            OnColumnSized(new WindowEventArgs(this));
        }

        private void listHeader_SortColumnChanged(object sender, WindowEventArgs e)
        {
            int sortColumn = SortColumnIndex;

            // set new sort column on all rows
            for (int i = 0; i < RowCount; ++i)
            {
                gridData[i].SortColumnIndex = sortColumn;
            }

            ResortGrid();
            ConfigureScrollbars();
            LayoutComponentWidgets();

            // signal change
            OnSortColumnChanged(new WindowEventArgs(this));
        }

        private void listHeader_SortDirectionChanged(object sender, WindowEventArgs e)
        {
            ResortGrid();
            ConfigureScrollbars();
            LayoutComponentWidgets();

            // signal change
            OnSortDirectionChanged(new WindowEventArgs(this));
        }


        private void verticalScrollBar_PositionChanged(object sender, WindowEventArgs e)
        {
            LayoutComponentWidgets();
        }
        private void horizontalScrollBar_PositionChanged(object sender, WindowEventArgs e)
        {
            LayoutComponentWidgets();
        }
        #endregion

        #region Serialization
        protected override void OnStringDictionaryChanged()
        {
            foreach (GridRow row in gridData)
            {
                foreach (UIComp_DrawListboxItem item in row)
                {
                    item.OnStringDictionaryChanged();
                }
            }
        }

        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            this.backgroundRenderComp.PostCustomSerial(this);
            this.stickRenderComp.PostCustomSerial(this);

            foreach (GridRow row in gridData)
            {
                foreach (UIComp_DrawListboxItem item in row)
                {
                    item.PostCustomSerial(this);
                }
            }

            InitEventHander();
        }

        [Browsable(false)]
        [XmlFieldSerializable("listHeaderHelper")]
        private string listHeaderHelper
        {
            get
            {
                return this.listHeader.Name;
            }
            set
            {
                this.listHeader = Children[value] as UIListHeader;
            }
        }
       
        [Browsable(false)]
        [XmlFieldSerializable("VerticalScrollBarHelper")]
        private string VerticalScrollBarHelper
        {
            get
            {
                return this.verticalScrollBar.Name;
            }
            set
            {
                this.verticalScrollBar = Children[value] as UIVerticalScrollBar;
            }
        }

        [Browsable(false)]
        [XmlFieldSerializable("HorizontalScrollBarHelper")]
        private string HorizontalScrollBarHelper
        {
            get
            {
                return this.horizontalScrollBar.Name;
            }
            set
            {
                this.horizontalScrollBar = Children[value] as UIHorizontalScrollBar;
            }
        }
        #endregion

    }
}
