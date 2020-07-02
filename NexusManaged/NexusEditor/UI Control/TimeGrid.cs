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
    public partial class TimeGrid : Control
    {
        //主绘布
        private Bitmap m_Canvas;
        //单元格的边长，单位像素
        private int m_GridSize;
        //每个像素间隔对应世界空间中的长度间隔
        private float m_GridScale;
        //上一次鼠标点击的数据
        private int m_X, m_Y;
        //平移的数据
        private int m_OffsetX, m_OffsetY;
        //当前窗体能显示的范围
        private WorldSpaceRect m_View;
        //缩放级别
        private float[] m_ScaleStep;
		private int m_ScaleStepIndex;

        //当前被选中的TimelineItem
		private TimelineStreams m_Stream;
        //当前被选中的KeyPoint
		private KeyPoint m_CurrentKeyPoint;
		private int m_CurrentStreamIndex;
		private int m_CurrentKeyPointIndex;

		//鼠标悬浮提示信息
		public delegate void TimeGridTooltip(string text);
		public TimeGridTooltip TooltipDelegate
		{
			get
			{
				return m_TooltipGate;
			}
			set
			{
				m_TooltipGate=value;
			}
		}
		private TimeGridTooltip m_TooltipGate;
		private float m_TooltipX, m_TooltipY;
		private string m_Tooltip;

        public TimeGrid()
        {
            InitializeComponent();
            m_ScaleStep = new float[20];
            for (int i = 0; i < 20; i += 2)
            {
                m_ScaleStep[i] = 0.00002f * (float)Math.Pow(10, i / 2) / 2;
                m_ScaleStep[i + 1] = 0.00002f * (float)Math.Pow(10, i / 2);
            }
            m_ScaleStepIndex = 10;
            m_GridScale = m_ScaleStep[m_ScaleStepIndex];
            m_GridSize = 50;
            m_Canvas = new Bitmap(1, 1, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            m_X = m_Y = 0;
            m_OffsetX = m_OffsetY = 0;
            PointF lt = ScreenToWorld(0, 0);
            PointF rb = ScreenToWorld(Width, Height);
            m_View = new WorldSpaceRect(lt.X, lt.Y, rb.X, rb.Y);
            //----------------------------------------------------------------------------------------
            m_CurrentKeyPointIndex = -1;
            m_CurrentStreamIndex = -1;
			m_Tooltip = "";
        }

        //添加时间轴对象，添加时会自动按照时间先后进行一次排序
        public void ShowStream(TimelineStreams item)
        {
            //需要Item按照时间排序
			m_Stream = item;
			if (item.GetStreamCount() == 4)
			{
				m_ScaleStepIndex = 6;
				m_GridScale = m_ScaleStep[m_ScaleStepIndex];
			}
			else
			{
				m_ScaleStepIndex = 10;
				m_GridScale = m_ScaleStep[m_ScaleStepIndex];
			}
			PointF lt = ScreenToWorld(0, 0);
			PointF rb = ScreenToWorld(Width, Height);
			m_View = new WorldSpaceRect(lt.X, lt.Y, rb.X, rb.Y);
            Invalidate();
        }

        //清空所有数据
        public void Clear()
        {
			m_Stream = null;
            Invalidate();
        }

        //检测点pt是否在线段AB上
        public static bool IsPointOnLine(PointF pt, PointF start, PointF end)
        {
            WorldSpaceRect rect = new WorldSpaceRect(start, end);
            rect.Regular();
            if (!rect.Contains(pt.X, pt.Y)) return false;
            PointF pa = new PointF(pt.X - start.X, pt.Y - start.Y);
            PointF ab = new PointF(end.X - start.X, end.Y - start.Y);
            float ma = (float)Math.Sqrt(pa.X * pa.X + pa.Y * pa.Y);
            pa.X /= ma; pa.Y /= ma;

            float mb = (float)Math.Sqrt(ab.X * ab.X + ab.Y * ab.Y);
            ab.X /= mb; ab.Y /= mb;

            float dot = pa.X * ab.X + pa.Y * ab.Y;
            //0.9962f表示允许5°的误差
            if (Math.Abs(dot)<0.9962f)
            {
                return false;
            }
            return true;
        }

        //将屏幕坐标系中的坐标映射到世界坐标系
        public PointF ScreenToWorld(float x, float y)
        {
            float nx = ((x - m_OffsetX - Width * 0.5f) * m_GridScale);
            float ny = ((y - m_OffsetY - Height * 0.5f) * -m_GridScale);
            PointF pt = new PointF(nx, ny);
            return pt;
        }

        //控件大小发生改变时的处理
        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
            if (Width == 0 || Height == 0) return;
            m_Canvas = new Bitmap(Width, Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            PointF lt = ScreenToWorld(0, 0);
            PointF rb = ScreenToWorld(Width, Height);
            m_View = new WorldSpaceRect(lt.X, lt.Y, rb.X, rb.Y);

            Invalidate();
        }

        //在指定Graphics对象上绘制直线，直线的坐标使用正常笛卡尔坐标系，函数内部会自动转换为屏幕坐标系
        protected void DrawLine(Graphics canvas, Pen pen, float x1, float y1, float x2, float y2)
        {
            canvas.DrawLine(pen, x1 / m_GridScale + Width * 0.5f + m_OffsetX, -y1 / m_GridScale + Height * 0.5f + m_OffsetY,
                x2 / m_GridScale + Width * 0.5f + m_OffsetX, -y2 / m_GridScale + Height * 0.5f + m_OffsetY);
        }

		protected void DrawLine(Graphics canvas, Pen pen, KeyPoint st,KeyPoint ed,uint index)
		{
			{
				DrawLine(canvas, pen, st.Time, st.GetValue(index), ed.Time, ed.GetValue(index));
			}
		}

        //在指定Graphics对象上绘制直线，坐标使用正常笛卡尔坐标系，函数内部会自动转换为屏幕坐标系
        protected void DrawString(Graphics canvas, string text, Brush brush, float x, float y)
        {
            canvas.DrawString(text, Font, brush, x / m_GridScale + Width * 0.5f + m_OffsetX, -y / m_GridScale + Height * 0.5f + m_OffsetY);
        }

        //在指定Graphics对象上绘制网格
        protected void DrawGrid(Graphics canvas)
        {
            Pen pen = new Pen(ForeColor);
            Pen ThickPen = new Pen(Color.White, 2.0f);
            SolidBrush brush = new SolidBrush(ForeColor);
            float step = m_GridScale * m_GridSize;
            float sx = m_View.Left - m_View.Left % step;
            float sy = m_View.Bottom - m_View.Bottom % step;
            for (float i = sx; i <= m_View.Right; i += step)
            {
                if (Math.Abs(i) <= 0.00001f)
                {
                    DrawLine(canvas, ThickPen, i, m_View.Top, i, m_View.Bottom);
                }
                else
                {
                    DrawLine(canvas, pen, i, m_View.Top, i, m_View.Bottom);
                }
                DrawString(canvas, i.ToString("F04"), brush, i, m_View.Bottom + m_GridScale * (Font.Size + 6));
            }

            for (float i = sy; i <= m_View.Top; i += step)
            {
                if (Math.Abs(i)<=0.00001f)
                {
                    DrawLine(canvas, ThickPen, m_View.Left, i, m_View.Right, i);
                }
                else
                {
                    DrawLine(canvas, pen, m_View.Left, i, m_View.Right, i);
                }
                DrawString(canvas, i.ToString("F04"), brush, m_View.Left, i);
            }
        }

        //绘制操作手柄
        protected void DrawHandle(Graphics canvas, float x1, float y1, float size, Color color, Color bordercolor)
        {
            float x = x1 / m_GridScale + Width * 0.5f + m_OffsetX, y = -y1 / m_GridScale + Height * 0.5f + m_OffsetY;
            RectangleF rect = new RectangleF(x - size * 0.5f, y - size * 0.5f, size, size);
            SolidBrush brush = new SolidBrush(color);
            canvas.FillRectangle(brush, rect);
            Pen pen = new Pen(bordercolor);
            canvas.DrawRectangle(pen, rect.Left, rect.Top, size, size);
        }

		protected void DrawHandle(Graphics canvas, KeyPoint kp,uint index, float size, Color color, Color bordercolor)
		{
			DrawHandle(canvas, kp.Time, kp.GetValue(index), size,color,bordercolor);
		}

        //绘制折线段，也可以改成绘制样条曲线
        protected void DrawLines(Graphics layer)
        {
            if (m_Stream == null) return;
			for (int j = m_Stream.GetStreamCount() - 1; j >= 0; --j)
            {
				if (!m_Stream.GetStreamEnable(j)) continue;
				Pen pen = new Pen(m_Stream.GetStreamColor(j));
				List<KeyPoint> list = m_Stream.GetStream();
                for (int k = 0; k < list.Count - 1; ++k)
                {
                    DrawLine(layer, pen, list[k], list[k + 1],(uint)j);
                }
                for (int k = 0; k < list.Count; ++k)
                {
                    if (list[k] == m_CurrentKeyPoint)
                    {
                        DrawHandle(layer, list[k], (uint)j, 12.0f, Color.Black, Color.White);
                    }
                    else
                    {
						DrawHandle(layer, list[k], (uint)j, 8.0f, m_Stream.GetStreamColor(j), Color.White);
                    }
                }
            }

			if (m_Tooltip.Length > 0)
			{
				SolidBrush brush = new SolidBrush(Color.White);
				layer.DrawString(m_Tooltip,Font,brush, m_TooltipX, m_TooltipY);
			}
        }

        //重载的绘图函数，使用双缓冲，减轻闪烁感
        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics g = Graphics.FromImage(m_Canvas);
            //清空背景
            g.Clear(BackColor);
            //绘制背景网格
            DrawGrid(g);
            //绘制折线
            DrawLines(g);
            e.Graphics.DrawImage(m_Canvas, 0, 0);
        }

        //禁用此处的背景擦除可以有效的避免闪烁
        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
            //base.OnPaintBackground(pevent);
        }

        // ctrl+左键为在指定折线上添加一个细分，alt+左键为删除当前操作手柄对应的时间点，左键点击操作手柄为选择操作手柄，可以上下移动被选中的手柄
        protected override void OnMouseDown(MouseEventArgs e)
        {
            Focus();
            m_X = e.X;
            m_Y = e.Y;
			if (e.Button == MouseButtons.Left)
			{
				float size = 6.0f * m_GridScale;
				//如果按下ctrl键则视为在所选位置增加节点
				if ((Control.ModifierKeys & Keys.Control) == Keys.Control)
				{
					if (m_Stream == null) return;
					PointF pt = ScreenToWorld(e.X, e.Y);
					m_Stream.AddKeyPoint(pt.X);
					Invalidate();
				}
				if ((Control.ModifierKeys & Keys.Alt) == Keys.Alt)
				{
					if (m_Stream == null) return;
					//否则检测是否选中了操作器
					for (int j = 0; j < m_Stream.GetStreamCount(); ++j)
					{
						if (!m_Stream.GetStreamEnable(j)) continue;
						List<KeyPoint> list = m_Stream.GetStream();
						for (int k = list.Count - 1; k >= 0; --k)
						{
							RectangleF rect = new RectangleF(list[k].Time - size, list[k].GetValue((uint)j) - size, size * 2, size * 2);
							PointF pt = ScreenToWorld(e.X, e.Y);
							if (rect.Contains(pt))
							{
								if (list.Count <= 2)
								{
									m_CurrentKeyPoint = list[k];
									m_CurrentStreamIndex = j;
									m_CurrentKeyPointIndex = k;
									Invalidate();
									base.OnMouseDown(e);
									return;
								}
								m_Stream.RemoveKeyPoint(k);
								Invalidate();
								m_CurrentKeyPoint = null;
								base.OnMouseDown(e);
								return;
							}
						}
					}
					m_CurrentKeyPoint = null;
					Invalidate();
				}
				else
				{
					if (m_Stream == null) return;
					//否则检测是否选中了操作器					
					for (int j = 0; j < m_Stream.GetStreamCount(); ++j)
					{
						if (!m_Stream.GetStreamEnable(j)) continue;
						List<KeyPoint> list = m_Stream.GetStream();
						for (int k = list.Count - 1; k >= 0; --k)
						{
							RectangleF rect = new RectangleF(list[k].Time - size, list[k].GetValue((uint)j) - size, size * 2, size * 2);
							PointF pt = ScreenToWorld(e.X, e.Y);
							if (rect.Contains(pt))
							{
								m_CurrentKeyPoint = list[k];
								m_CurrentStreamIndex = j;
								m_CurrentKeyPointIndex = k;
								Invalidate();
								base.OnMouseDown(e);
								return;
							}
						}
					}
					m_CurrentKeyPoint = null;
					Invalidate();
				}
			}

            base.OnMouseDown(e);
        }

        //平移图像，或者平移选中的操作手柄
        protected override void OnMouseMove(MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                //控制Graphics矩阵的平移
                m_OffsetX += e.X - m_X;
                m_OffsetY += e.Y - m_Y;
                m_X = e.X;
                m_Y = e.Y;

                PointF lt = ScreenToWorld(0, 0);
                PointF rb = ScreenToWorld(Width, Height);
                m_View = new WorldSpaceRect(lt.X, lt.Y, rb.X, rb.Y);

                Invalidate();
            }
			else if (e.Button == MouseButtons.Left)
			{
				//如果当前有选中的操作器，则将上下移动映射到改变操作器的值
				if (m_CurrentKeyPoint != null)
				{
					PointF pt = ScreenToWorld(e.X, e.Y);
					m_CurrentKeyPoint.SetValue((uint)m_CurrentStreamIndex,pt.Y);
					{
						//如果允许修改时间,但是不能超过左右两侧
						m_CurrentKeyPoint.Time = pt.X;
						List<KeyPoint> list = m_Stream.GetStream();
						//不能小于左侧
						if (m_CurrentKeyPointIndex > 0)
						{
							KeyPoint kp = list[m_CurrentKeyPointIndex - 1];
							if (kp.Time > pt.X)
							{
								m_CurrentKeyPoint.Time = kp.Time;
							}
						}
						if (m_CurrentKeyPointIndex < list.Count - 1)
						{
							KeyPoint kp = list[m_CurrentKeyPointIndex + 1];
							if (kp.Time < pt.X)
							{
								m_CurrentKeyPoint.Time = kp.Time;
							}
						}
					}
					m_Stream.Update(m_CurrentKeyPoint);
					m_Tooltip = "(";
					m_Tooltip += pt.X.ToString(); m_Tooltip += ","; m_Tooltip += pt.Y.ToString(); m_Tooltip += ")";
					m_TooltipX = e.X;
					m_TooltipY = e.Y;
					Invalidate();
				}
				else
				{
					//控制Graphics矩阵的平移
					m_OffsetX += e.X - m_X;
					m_OffsetY += e.Y - m_Y;
					m_X = e.X;
					m_Y = e.Y;

					PointF lt = ScreenToWorld(0, 0);
					PointF rb = ScreenToWorld(Width, Height);
					m_View = new WorldSpaceRect(lt.X, lt.Y, rb.X, rb.Y);
					m_Tooltip = "";
					Invalidate();
				}
			}
			else
			{
				if(m_Tooltip.Length>0)
				{
					m_Tooltip = "";
					Invalidate();
				}
			}

			if (TooltipDelegate != null)
			{
				PointF pt = ScreenToWorld(e.X, e.Y);
				string text;
				text = "(";
				text += pt.X.ToString(); text += ","; text += pt.Y.ToString(); text += ")";
				TooltipDelegate(text);
			}
            base.OnMouseMove(e);
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
        }

        //缩放绘图范围，以当前鼠标所在的坐标为缩放的中心
        public void OnCanvasMouseWheel(MouseEventArgs e)
        {
            int ex = e.X - Left, ey = e.Y - Top;
            PointF wpos = ScreenToWorld(ex, ey);
            //控制Graphics矩阵的缩放
            if (e.Delta > 0)
            {
                --m_ScaleStepIndex;
                if (m_ScaleStepIndex < 0) m_ScaleStepIndex = 0;
                m_GridScale = m_ScaleStep[m_ScaleStepIndex];
            }
            else
            {
                ++m_ScaleStepIndex;
                if (m_ScaleStepIndex > 18) m_ScaleStepIndex = 18;
                m_GridScale = m_ScaleStep[m_ScaleStepIndex];
            }

            m_OffsetX = -(int)(wpos.X / m_GridScale + Width * 0.5f - ex);
            m_OffsetY = -(int)(wpos.Y / -m_GridScale + Height * 0.5f - ey);

            PointF lt = ScreenToWorld(0, 0);
            PointF rb = ScreenToWorld(Width, Height);
            m_View = new WorldSpaceRect(lt.X, lt.Y, rb.X, rb.Y);
            Invalidate();
        }
    }

    // 正常笛卡尔坐标系下面的矩形
    class WorldSpaceRect
    {
        public float Left;
        public float Top;
        public float Right;
        public float Bottom;
        public float Width
        {
            get
            {
                return Right - Left;
            }
        }
        public float Height
        {
            get
            {
                return Bottom - Top;
            }
        }
        public WorldSpaceRect()
        {
        }
        public WorldSpaceRect(float left, float top, float right, float bottom)
        {
            Left = left; Top = top; Right = right; Bottom = bottom;
        }
        public WorldSpaceRect(PointF a, PointF b)
        {
            Left = a.X; Top = a.Y; Right = b.X; Bottom = b.Y;
        }
        public void Regular()
        {
            if (Left > Right)
            {
                float t = Left; Left = Right; Right = t;
            }
            if (Bottom > Top)
            {
                float t = Top; Top = Bottom; Bottom = t;
            }
        }
        public bool Contains(float x, float y)
        {
            if (x < Left || x > Right || y < Bottom || y > Top)
            {
                return false;
            }
            return true;
        }
    }
}
