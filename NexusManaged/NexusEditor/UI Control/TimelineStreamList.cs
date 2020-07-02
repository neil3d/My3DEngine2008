using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace NexusEditor
{
    public partial class TimelineStreamList : Control
    {
        private List<TimelineListItem> m_Items;
        private List<TimelineListItem> m_VisibleItems;
        private TimelineListItem m_SelectedItem;
        private int m_ItemHeight;
        private Bitmap m_Canvas;
        private StringFormat m_FontFormat;
        private Color m_ShadowColor;

        public TimelineStreamList()
        {
            InitializeComponent();
            m_ShadowColor = Color.Black;
            m_Canvas = new Bitmap(1, 1, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            m_Items = new List<TimelineListItem>();
            m_VisibleItems = new List<TimelineListItem>();
            m_Scrollbar.Left = Left;
            m_Scrollbar.Top = Top;
            m_Scrollbar.Width = 20;
            m_Scrollbar.Height = Height;
            m_Scrollbar.Visible = false; ;
            m_ItemHeight = 40;
            m_FontFormat = new StringFormat();
            m_FontFormat.Alignment = StringAlignment.Near;
            m_FontFormat.LineAlignment = StringAlignment.Center;
        }

        //每个列表项目的高度
        [Category("Appearance")]
        public int ItemHeight
        {
            get
            {
                return m_ItemHeight;
            }
            set
            {
                m_ItemHeight = Math.Abs(value);
            }
        }

        //文字阴影的色彩
        [Category("Appearance")]
        public Color ShadowColor
        {
            get
            {
                return m_ShadowColor;
            }
            set
            {
                m_ShadowColor = value;
            }
        }

        //绘制项目
        protected void OnDrawItem(DrawItemArgs args)
        {
            if (args.IsSelected)
            {
                LinearGradientBrush lbrush1 = new LinearGradientBrush(args.Bounds, Color.Transparent, Color.FromArgb(255, 100, 100, 255), 120.0f, true);
                LinearGradientBrush lbrush2 = new LinearGradientBrush(args.Bounds, Color.FromArgb(128, 255, 255, 255), Color.FromArgb(0, 100, 100, 255), -30.0f, true);
                args.Canvas.FillRectangle(lbrush1, args.Bounds);
                args.Canvas.FillRectangle(lbrush2, args.Bounds);
            }
            else
            {
                LinearGradientBrush lbrush1 = new LinearGradientBrush(args.Bounds, ForeColor, Color.Transparent, 120.0f, true);
                LinearGradientBrush lbrush2 = new LinearGradientBrush(args.Bounds, Color.FromArgb(128, 255, 255, 255), BackColor, -45.0f, true);
                args.Canvas.FillRectangle(lbrush1, args.Bounds);
                args.Canvas.FillRectangle(lbrush2, args.Bounds);
            }
            //输出文字
            SolidBrush brush = new SolidBrush(args.ForeColor);
            SolidBrush shadowBrush = new SolidBrush(m_ShadowColor);
            Rectangle fontLayout = new Rectangle(args.Bounds.Left + 4, args.Bounds.Top, args.Bounds.Width-4, args.Bounds.Height); ;
            Rectangle shadowRect = new Rectangle(fontLayout.Left + 1, fontLayout.Top + 1, fontLayout.Width, fontLayout.Height);
            string name = args.Item.Streams.GetStreamName(args.Item.StreamIndex);
            args.Canvas.DrawString(name, Font, shadowBrush, shadowRect, m_FontFormat);
            args.Canvas.DrawString(name, Font, brush, fontLayout, m_FontFormat);
            Pen pen = new Pen(args.ForeColor);
            //绘制色彩标记块
            Rectangle rect = new Rectangle(args.Bounds.Right - 20, args.Bounds.Top + (m_ItemHeight - 10) / 2, 10, 10);
            if (args.Item.Streams.GetStreamEnable(args.Item.StreamIndex))
            {
                SolidBrush colorBrush = new SolidBrush(args.Item.Streams.GetStreamColor(args.Item.StreamIndex));
                args.Canvas.FillRectangle(colorBrush, rect);
            }
            else
            {
                SolidBrush colorBrush = new SolidBrush(Color.Transparent);
                args.Canvas.FillRectangle(colorBrush, rect);
            }
            args.Canvas.DrawRectangle(pen, rect);
            args.Canvas.DrawRectangle(pen, args.Bounds);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics g = Graphics.FromImage(m_Canvas);
            g.Clear(BackColor);
            int bound_left = 0;
            int bound_width = Width;
            if (m_Scrollbar.Visible)
            {
                bound_left = m_Scrollbar.Width;
                bound_width = Width - m_Scrollbar.Width;
            }
            for (int i = 0; i < m_VisibleItems.Count; ++i)
            {
                DrawItemArgs args = new DrawItemArgs();
                args.Canvas = g;
                args.Item = m_VisibleItems[i];
				args.Bounds = new Rectangle(bound_left, i * m_ItemHeight, bound_width - 1, m_ItemHeight);
                if (args.Item == m_SelectedItem)
                {
                    args.IsSelected = true;
                    args.BackColor = Color.FromArgb(255, 0, 0, 128);
                    args.ForeColor = Color.FromArgb(255, 255, 255, 255);
                }
                else
                {
                    args.ForeColor = ForeColor;
                    args.BackColor = BackColor;
                }
                OnDrawItem(args);
            }
            e.Graphics.DrawImage(m_Canvas, 0, 0);
        }

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
            //base.OnPaintBackground(pevent);
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);
        }

        private void m_Scrollbar_ValueChanged(object sender, EventArgs e)
        {
            UpdateVisibleList();
        }

        private void UpdateVisibleList()
        {
            m_VisibleItems.Clear();
            int visible_count = Height / m_ItemHeight;
            if (m_Items.Count <= visible_count)
            {
                for (int i = 0; i < m_Items.Count; ++i)
                {
                    m_VisibleItems.Add(m_Items[i]);
                }
            }
            else
            {
                for (int i = m_Scrollbar.Value; i < m_Scrollbar.Value + visible_count; ++i)
                {
                    m_VisibleItems.Add(m_Items[i]);
                }
            }
            Invalidate();
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
            if (Width == 0 || Height == 0) return;
            m_Canvas = new Bitmap(Width, Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            m_Scrollbar.Left = Left;
            m_Scrollbar.Top = Top;
            m_Scrollbar.Width = 18;
            m_Scrollbar.Height = Height;

            //判断是否需要显示滚动条
            int visible_count = Height / m_ItemHeight;
            if (visible_count < m_Items.Count)
            {
                m_Scrollbar.Minimum = 0;
                m_Scrollbar.Maximum = m_Items.Count - visible_count - 1;
                m_Scrollbar.Maximum = m_Scrollbar.Maximum + m_Scrollbar.LargeChange;
                m_Scrollbar.Enabled = true;
                m_Scrollbar.Visible = true;
            }
            else
            {
                m_Scrollbar.Enabled = false;
                m_Scrollbar.Visible = false;
            }
            m_VisibleItems.Clear();
            if (m_Items.Count <= visible_count)
            {
                for (int i = 0; i < m_Items.Count; ++i)
                {
                    m_VisibleItems.Add(m_Items[i]);
                }
            }
            else
            {
                for (int i = m_Scrollbar.Value; i < m_Scrollbar.Value + visible_count; ++i)
                {
                    m_VisibleItems.Add(m_Items[i]);
                }
            }
            Invalidate();
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            TimelineListItem item = GetItemInPosition(e.X, e.Y);
            if (item != null)
            {
                Rectangle itemBound = GetItemBounds(item);
                //如果是按在色彩方块中，则修改相应的色彩方块，否则选中当前项目                
                Rectangle rect = new Rectangle(itemBound.Right - 20, itemBound.Top + (m_ItemHeight - 10) / 2, 10, 10);
                if (rect.Contains(e.X, e.Y))
                {
                    item.Streams.SetStreamEnable(item.StreamIndex, !item.Streams.GetStreamEnable(item.StreamIndex));
                    Invalidate(itemBound);
                    return;
                }
                m_SelectedItem = item;
            }
            else
            {
                m_SelectedItem = null;
            }
            Invalidate();
        }
        //----------------------------------------------------------------------------------------
        public void ShowItem(TimelineStreams item)
        {
            for (int i = 0; i < item.GetStreamCount(); ++i)
            {
                TimelineListItem li=new TimelineListItem();
                li.Streams = item;
                li.StreamIndex = i;
                m_Items.Add(li);
            }
            int visible_count = Height / m_ItemHeight;
            if (visible_count < m_Items.Count)
            {
                m_Scrollbar.Enabled = true;
                m_Scrollbar.Minimum = 0;
                m_Scrollbar.Maximum = m_Items.Count - visible_count - 1;
                m_Scrollbar.Visible = true;
                m_Scrollbar.Maximum = m_Scrollbar.Maximum + m_Scrollbar.LargeChange;
            }
            else
            {
                m_Scrollbar.Enabled = false;
                m_Scrollbar.Visible = false;
            }
            UpdateVisibleList();
        }

        public void Clear()
        {
            m_Items.Clear();
            m_Scrollbar.Enabled = false;
            m_Scrollbar.Visible = false;
            m_VisibleItems.Clear();
            m_SelectedItem = null;
            Invalidate();
        }

        public TimelineListItem SelectedItem
        {
            get
            {
                return m_SelectedItem;
            }
            set
            {
                m_SelectedItem = value;
            }
        }

        //获取鼠标指针所在的item
        public TimelineListItem GetItemInPosition(int x, int y)
        {
            if (!Bounds.Contains(x, y)) return null;
            int bound_left = 0;
            int bound_width = Width;
            if (m_Scrollbar.Visible)
            {
                bound_left = m_Scrollbar.Width;
                bound_width = Width - m_Scrollbar.Width;
            }
            for (int i = 0; i < m_VisibleItems.Count; ++i)
            {
                Rectangle itemBounds = new Rectangle();
                itemBounds.X = bound_left;
                itemBounds.Y = i * m_ItemHeight;
                itemBounds.Width = bound_width - 1;
                itemBounds.Height = m_ItemHeight;
                if (itemBounds.Contains(x, y))
                {
                    return m_VisibleItems[i];
                }
            }
            return null;
        }

        public Rectangle GetItemBounds(TimelineListItem item)
        {
            int bound_left = 0;
            int bound_width = Width;
            if (m_Scrollbar.Visible)
            {
                bound_left = m_Scrollbar.Width;
                bound_width = Width - m_Scrollbar.Width;
            }
            for (int i = 0; i < m_VisibleItems.Count; ++i)
            {
                if (m_VisibleItems[i]!=item) continue;
                Rectangle itemBounds = new Rectangle();
                itemBounds.X = bound_left;
                itemBounds.Y = i * m_ItemHeight;
                itemBounds.Width = bound_width - 1;
                itemBounds.Height = m_ItemHeight;
                return itemBounds;
            }
            return new Rectangle(0, 0, 0, 0);
        }
    }

    //========================================================================================
    public class DrawItemArgs
    {
		private Rectangle m_Bounds;
		public Rectangle Bounds
		{
			get
			{
				return m_Bounds;
			}
			set
			{
				m_Bounds=value;
			}
		}
		private int m_Index;
		public int Index
		{
			get
			{
				return m_Index;
			}
			set
			{
				m_Index = value;
			}
		}
		private Graphics m_Canvas;
		public Graphics Canvas
		{
			get
			{
				return m_Canvas;
			}
			set
			{
				m_Canvas = value;
			}
		}
		private TimelineListItem m_Item;
		public TimelineListItem Item
		{
			get
			{
				return m_Item;
			}
			set
			{
				m_Item = value;
			}
		}
		private bool m_IsSelected;
		public bool IsSelected
		{
			get
			{
				return m_IsSelected;
			}
			set
			{
				m_IsSelected = value;
			}
		}
		private Color m_BackColor;
		public Color BackColor
		{
			get
			{
				return m_BackColor;
			}
			set
			{
				m_BackColor = value;
			}
		}
		private Color m_ForeColor;
		public Color ForeColor
		{
			get
			{
				return m_ForeColor;
			}
			set
			{
				m_ForeColor = value;
			}
		}
    }

    public class TimelineListItem
    {
        private TimelineStreams m_Streams;
        private int m_StreamIndex;

		public TimelineStreams Streams
		{
			get
			{
				return m_Streams;
			}
			set
			{
				m_Streams=value;
			}
		}

		public int StreamIndex
		{
			get
			{
				return m_StreamIndex;
			}
			set
			{
				m_StreamIndex = value;
			}
		}

        public TimelineListItem()
        {
        }
        public TimelineListItem(TimelineStreams streams,int streamIndex)
        {
            StreamIndex = streamIndex;
            Streams = streams;
        }
    }
    //========================================================================================
}
