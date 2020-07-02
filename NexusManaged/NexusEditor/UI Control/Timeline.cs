using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace NexusEditor
{
    public partial class Timeline : UserControl
    {
        public Timeline()
        {
            InitializeComponent();
            this.MouseWheel += this.TimelineMouseWheel;
        }

		public TimeGrid.TimeGridTooltip TooltipDelegate
		{
			get
			{
				return m_Canvas.TooltipDelegate;
			}
			set
			{
				m_Canvas.TooltipDelegate = value;
			}
		}

        public void AddItem(TimelineStreams item)
        {
            m_Canvas.Clear();
            m_ItemList.Clear();
            if(item!=null)
            {
                m_ItemList.ShowItem(item);
                m_Canvas.ShowStream(item);
            }
        }

        public void Clear()
        {
            m_ItemList.Clear();
            m_Canvas.Clear();
        }

        public void RemoveItem(TimelineStreams item)
        {
            m_ItemList.Clear();
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
            m_ItemList.Left = 0;
            m_ItemList.Top = 0;
            m_ItemList.Height = Height;
            m_Canvas.Left = m_ItemList.Right;
            m_Canvas.Top = 0;
            m_Canvas.Height = Height;
            m_Canvas.Width = Width - m_ItemList.Width;
        }

		private void TimelineMouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (m_Canvas.Bounds.Contains(e.X, e.Y))
            {
                m_Canvas.OnCanvasMouseWheel(e);
            }
        }

        private void m_ItemListClick(object sender, EventArgs e)
        {
            m_Canvas.Invalidate();
        }
    }
}
