using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;
using Nexus.GameFramework;

namespace Nexus.GUI.Components
{
    [XmlClassSerializable("UIEditBox", true)]
    public class UIComp_DrawEditBoxText : UIComp_DrawBase
    {
		public EventHandler<FontChangeArgs> FontChanged;

        #region Fields
        [XmlFieldSerializable("fontDesc")]
        protected FontDescription fontDesc;
        [NonSerialized]
        protected NexusEngine.NRenderFont font;
		[NonSerialized]
		private TextBuffer m_TextBuffer;	//字符串解析器
		[NonSerialized]
		int m_StartVisible, m_VisibleCount,m_StartOffset;	//可见字符的范围
		[NonSerialized]
		string m_First, m_Mid, m_Last;		//被分为三段的字符串
		[NonSerialized]
		Rect m_BeforeSelectionRect;			//被选中字符串之前的字符串矩形
		[NonSerialized]
		Rect m_SelectionRect;				//选中字符串的矩形
		[NonSerialized]
		Rect m_AfterSelectionRect;			//选中字符串之后的矩形，如果没有被选中的字符串，则使用该元素绘制所有的字符
        /// <summary>
        /// 文本绘制颜色
        /// </summary>
        [XmlFieldSerializable("normalTextColor")]
        protected Color4f normalTextColor;
        [XmlFieldSerializable("shadowColor")]
        protected Color4f shadowColor;
        [XmlFieldSerializable("selectTextColor")]
        protected Color4f selectTextColor;
        [XmlFieldSerializable("selectBrushColor")]
        protected ColorRect selectBrushColor;
        [XmlFieldSerializable("cursorColor")]
        protected ColorRect cursorColor;
        [XmlFieldSerializable("cursorSize")]
        protected float cursorSize;
        #endregion

        #region Properties
        [Browsable(false)]
        public override float Alpha
        {
            get
            {
                return normalTextColor.A;
            }
            set
            {
                normalTextColor.A = value;
                shadowColor.A = Math.Min(value,shadowColor.A);
                selectTextColor.A = value;
                selectBrushColor.left_top.A = value;
                selectBrushColor.left_bottom.A = value;
                selectBrushColor.right_top.A = value;
                selectBrushColor.right_bottom.A = value;
                cursorColor.left_top.A = value;
                cursorColor.left_bottom.A = value;
                cursorColor.right_top.A = value;
                cursorColor.right_bottom.A = value;
            }
        }

        [Category("Present")]
        public override System.Drawing.Color Color
        {
            get
            {
                return normalTextColor.ColorRGB;
            }
            set
            {
                NormalTextColor = new Color4f(value);
                Alpha = Owner != null ? Owner.EffectiveAlpha : 1.0f;
            }
        }

        /// <summary>
        /// Font
        /// </summary>
        public FontDescription FontDescription
        {
            get { return fontDesc; }
            set
            {
				fontDesc = value;
				font = NFontManager.Instance.ConstructFont(fontDesc);
				m_TextBuffer = new TextBuffer(font);
				this.OnOwnerTextChanged(null, null);
				this.OnOwnerTextSelectionChanged(null, null);

				if (FontChanged != null)
				{
					FontChanged(this, new FontChangeArgs(font, fontDesc));
				}
            }
        }

        [EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
        public NRenderFont Font
        {
            get { return font; }
        }

        [Browsable(false)]
        public Color4f NormalTextColor
        {
            get { return normalTextColor; }
            set
            {
                normalTextColor = value;
                owner.RequestRedraw();
            }
        }

        /// <summary>
        /// display shadow color 
        /// </summary>
        [Browsable(false)]
        public Color4f ShadowColor
        {
            get { return shadowColor; }
            set
            {
                shadowColor = value;
                shadowColor.A = Math.Min(value.A, Alpha);
                owner.RequestRedraw();
            }
        }

        [Category("Present")]
        [Description("display shadow color")]
        public System.Drawing.Color ShadowColorRGB
        {
            get
            {
                return shadowColor.ColorRGB;
            }
            set
            {
                ShadowColor = new Color4f(value);
            }
        }

        [Browsable(false)]
        public ColorRect SelectBrushColor
        {
            get { return selectBrushColor; }
            set
            {
                selectBrushColor = value;
                owner.RequestRedraw();
            }
        }

        [Category("Present")]
        public System.Drawing.Color SelectBrushColorRGB
        {
            get
            {
                return selectBrushColor.left_top.ColorRGB;
            }
            set
            {
                SelectBrushColor = new ColorRect(new Color4f(value));
                Alpha = Owner != null ? Owner.EffectiveAlpha : 1.0f;
            }
        }

        [Browsable(false)]
        public Color4f SelectTextColor
        {
            get { return selectTextColor; }
            set
            {
                selectTextColor = value;
                owner.RequestRedraw();
            }
        }
        [Category("Present")]
        public System.Drawing.Color SelectTextColorRGB
        {
            get
            {
                return selectTextColor.ColorRGB;
            }
            set
            {
                SelectTextColor = new Color4f(value);
                Alpha = Owner != null ? Owner.EffectiveAlpha : 1.0f;
            }
        }

        [Browsable(false)]
        public ColorRect CursorColor
        {
            get { return cursorColor; }
            set
            {
                cursorColor = value;
                owner.RequestRedraw();
            }
        }

        [Browsable(false)]
        public float CursorColorAlpha
        {
            get { return cursorColor.left_bottom.A; }
            set { cursorColor.SetAlpha(value); }
        }

        [Category("Present")]
        public System.Drawing.Color CursorColorRGB
        {
            get
            {
                return CursorColor.left_top.ColorRGB;
            }
            set
            {
                CursorColor = new ColorRect(new Color4f(value));
                Alpha = Owner != null ? Owner.EffectiveAlpha : 1.0f;
            }
        }


        [Category("Present")]
        public float CursorSize
        {
            get { return cursorSize; }
            set
            {
                cursorSize = value;
                owner.RequestRedraw();
            }
        }
        #endregion

		public UIComp_DrawEditBoxText() 
		{
			
		}

        public UIComp_DrawEditBoxText(Window owner) :
            base(owner)
        {
            normalTextColor = new Color4f(System.Drawing.Color.Black);
            selectTextColor = new Color4f(System.Drawing.Color.White);
            selectBrushColor = new ColorRect(new Color4f(0.2f, 0.6f, 1.0f));
            cursorColor = new ColorRect(new Color4f(System.Drawing.Color.Black));
            cursorSize = 1;
			m_StartOffset = 2; 
			m_StartVisible = 0; 
			m_VisibleCount = 0;
			m_First = ""; 
			m_Mid = ""; 
			m_Last = "";

			FontDescription = FontDescription.DefaultFontDesc;
			InitEvent(owner);
        }

		private void InitEvent(Window owner)
		{
			Nexus.GUI.Widgets.UIEditBox editBox = owner as Nexus.GUI.Widgets.UIEditBox;
			editBox.TextChange += this.OnOwnerTextChanged;
			editBox.Sized += this.OnOwnerSizeChanged;
			editBox.Moved += this.OnOwnerSizeChanged;
			editBox.TextSelectionChanged += this.OnOwnerTextSelectionChanged;
			editBox.CaratMoved += this.OnOwnerCaretPositionChanged;
		}

		protected void OnOwnerTextChanged(object sender, WindowEventArgs e)
		{
			Nexus.GUI.Widgets.UIEditBox editBox = owner as Nexus.GUI.Widgets.UIEditBox;
			UpdateVisibleRange();
		}

		protected void OnOwnerSizeChanged(object sender, GUIEventArgs e)
		{
			Nexus.GUI.Widgets.UIEditBox editBox = owner as Nexus.GUI.Widgets.UIEditBox;
			UpdateVisibleRange();
		}

		protected void OnOwnerCaretPositionChanged(object sender, WindowEventArgs e)
		{
			Nexus.GUI.Widgets.UIEditBox editBox = owner as Nexus.GUI.Widgets.UIEditBox;
			//重新计算可见的文本范围，基准是插入提示符必须始终在可见范围内
			UpdateVisibleRange();
		}

		protected void OnOwnerTextSelectionChanged(object sender, WindowEventArgs e)
		{
			Nexus.GUI.Widgets.UIEditBox editBox = owner as Nexus.GUI.Widgets.UIEditBox;
			UpdateVisibleRange();
		}

		protected override void InvalidateComponentRect()
		{
			base.InvalidateComponentRect();
			targetRect.left += m_StartOffset;
			targetRect.right -= m_StartOffset;
		}


		//重新计算可见字符的范围
		private void UpdateVisibleRange()
		{
			Nexus.GUI.Widgets.UIEditBox editBox = owner as Nexus.GUI.Widgets.UIEditBox;
			m_TextBuffer.Text = editBox.Text.ToString();
			if(m_TextBuffer.Text.Length==0)
			{
				m_StartVisible = 0; m_VisibleCount = 0;
				m_Last = "";
				m_AfterSelectionRect.left = m_AfterSelectionRect.right = m_TextBuffer.CPtoX(0,false);
				return;
			}

			//计算新的可见字符范围
			int nx1, nx,nx2;
			int cp = editBox.CaratPosition;
			nx1 = m_TextBuffer.CPtoX(m_StartVisible, false);
			nx = m_TextBuffer.CPtoX(cp, false);
			if(cp==m_TextBuffer.Text.Length)
			{
				nx2 = nx;
			}
			else
			{
				nx2 = m_TextBuffer.CPtoX(cp, true);
			}

			if (nx < nx1)
			{
				m_StartVisible = cp;
			}
			else if (nx2 > nx1 + (int)targetRect.Width)
			{
				int nXNewLeft = nx2 - (int)targetRect.Width;

				int NewStart=m_TextBuffer.XtoCP(nXNewLeft);
				int NewX = m_TextBuffer.CPtoX(NewStart, false);
				if (NewX < nXNewLeft)
				{
					++NewStart;
				}

				m_StartVisible = NewStart;
			}

			int end_x = m_TextBuffer.CPtoX(m_StartVisible, false) + (int)targetRect.Width;
			m_VisibleCount = m_TextBuffer.XtoCP(end_x) - m_StartVisible;

			//计算新的渲染矩形
			if(editBox.SelectionLength>0)
			{
				int offset=m_TextBuffer.CPtoX(m_StartVisible,false);

				Segment sa, sb, sc,sd,se;
				sa = new Segment(0, editBox.SelectionStartIndex);
				sb = new Segment(editBox.SelectionStartIndex, editBox.SelectionLength);
				sc = new Segment(editBox.SelectionEndIndex, m_TextBuffer.Text.Length - editBox.SelectionEndIndex);
				sd = new Segment(m_StartVisible, m_VisibleCount);
				se = sd.Intersection(sa);
				m_First = m_TextBuffer.Text.Substring(se.X, se.Length);
				m_BeforeSelectionRect.left = targetRect.left + m_TextBuffer.CPtoX(se.X, false) - offset;
				m_BeforeSelectionRect.right = targetRect.left + m_TextBuffer.CPtoX(se.Right, false) - offset;
				se = sd.Intersection(sb);
				m_Mid = m_TextBuffer.Text.Substring(se.X, se.Length);
				m_SelectionRect.left = targetRect.left + m_TextBuffer.CPtoX(se.X, false) - offset;
				m_SelectionRect.right = targetRect.left + m_TextBuffer.CPtoX(se.Right, false) - offset;
				se = sd.Intersection(sc);
				m_Last = m_TextBuffer.Text.Substring(se.X, se.Length);
				m_AfterSelectionRect.left = targetRect.left + m_TextBuffer.CPtoX(se.X, false) - offset;
				m_AfterSelectionRect.right = targetRect.left + m_TextBuffer.CPtoX(se.Right, false) - offset;
			}
			else
			{
				m_First = m_TextBuffer.Text.Substring(m_StartVisible, m_VisibleCount);
				m_BeforeSelectionRect.left = this.targetRect.left;
				m_BeforeSelectionRect.right = this.targetRect.right;
			}
			m_BeforeSelectionRect.top = this.targetRect.top; m_BeforeSelectionRect.bottom = this.targetRect.top + fontDesc.Height;
			m_SelectionRect.top = this.targetRect.top; m_SelectionRect.bottom = this.targetRect.top + fontDesc.Height;
			m_AfterSelectionRect.top = this.targetRect.top; m_AfterSelectionRect.bottom = this.targetRect.top + fontDesc.Height;
		}

        /// <summary>
        /// TODO(zewu): 优化
        /// </summary>
        public override void Draw()
        {
            Nexus.GUI.Widgets.UIEditBox editBox = owner as Nexus.GUI.Widgets.UIEditBox;
			if (font == null || editBox == null || m_TextBuffer==null)
                return;

            StringBuilder editText = new StringBuilder();
            if (editBox.TextMasked)
            {
                editText.Append(editBox.MaskCodePoint, editBox.Text.Length);
            }
            else
            {
                editText = editBox.Text;
            }
			if (m_TextBuffer.Text.Length > 0)
			{
				//Rect drawRC = this.targetRect;
				DrawTextFormat textFormt = DrawTextFormat.Left | DrawTextFormat.NoClip | DrawTextFormat.Top | DrawTextFormat.SingleLine;
				// 渲染文本
				if (editBox.SelectionLength > 0)
				{
					if (m_First.Length > 0)
					{
						font.DrawString(m_First, m_BeforeSelectionRect, textFormt, normalTextColor, shadowColor);
					}

					GUISystem.Instance.Renderer.DrawRectangle(m_SelectionRect, selectBrushColor);
					if (m_Mid.Length > 0)
					{
						font.DrawString(m_Mid, m_SelectionRect, textFormt, selectTextColor, shadowColor);
					}

					if (m_Last.Length > 0)
					{
						font.DrawString(m_Last, m_AfterSelectionRect, textFormt, normalTextColor, shadowColor);
					}
				}
				else
				{
					font.DrawString(m_First, m_BeforeSelectionRect, textFormt, normalTextColor, shadowColor);
				}
			}

            // 渲染光标
            if (editBox.HasInputFocus && !editBox.ReadOnly)
            {
				int caret_pos = -1;
                if (editBox.CaratPosition >= 0)
                {
					caret_pos = m_TextBuffer.CPtoX(editBox.CaratPosition, false) - m_TextBuffer.CPtoX(m_StartVisible, false);
                }
				Rect caretRC = new Rect();
				caretRC.left = this.targetRect.left + caret_pos;
				caretRC.right = caretRC.left + cursorSize;
				caretRC.top = targetRect.top;
				caretRC.Height = fontDesc.Height;

				GUISystem.Instance.Renderer.DrawRectangle(caretRC, cursorColor);
            }
        }

        public virtual int GetTextIndexFromPosition(Vector2 vec)
        {
            Nexus.GUI.Widgets.UIEditBox editBox = (Nexus.GUI.Widgets.UIEditBox)owner;
            int postion = 0;
            if (font != null
                && editBox != null)
            {
                //if (this.targetRect.IsPointInRect(vec) && editBox.Text.Length > 0)
				//为了选择时能让超出显示范围的文本滚动，需要解除IsPointInRect检测
				if ( editBox.Text.Length > 0)
                {
					Vector2 v = vec - editBox.AbsolutePosition;
					int x = m_TextBuffer.CPtoX(m_StartVisible, false);
					return m_TextBuffer.XtoCP(x+(int)v.x);
                }
            }
            return postion;
        }

        public virtual bool IsCharacterAvailable(char ch)
        {
            if (char.IsDigit(ch) || char.IsLetter(ch) || char.IsPunctuation(ch) || ch==' ')
                return true;

            return false;
        }

        #region Serialization

        internal override void PostCustomSerial(Window _owner)
        {
			base.PostCustomSerial(_owner);

			FontDescription = fontDesc;
			InitEvent(_owner);
        }

        #endregion Serialization

    }

	class TextBuffer
	{
		private String m_Text;
		private NRenderFont m_Font;
		private List<int> m_CaretPos;
		private bool m_IsAnalyseRequired;
		private int m_StartOffset;
		private int m_SpaceWidth;	//空格的宽度
		
		public TextBuffer(NRenderFont font)
		{
			m_Text = "";
			m_Font = font;
			if(m_Font==null)
			{
				throw new Exception("Must have a font usable.");
			}
			m_IsAnalyseRequired = true;
			m_StartOffset = 0;
			m_CaretPos = new List<int>();

			DrawTextFormat textFormt = DrawTextFormat.Left | DrawTextFormat.Top | DrawTextFormat.NoClip | DrawTextFormat.SingleLine;
			Rect rc1 = m_Font.MeasureString("w w", textFormt);
			Rect rc2 = m_Font.MeasureString("ww", textFormt);
			m_SpaceWidth = (int)(rc1.Width - rc2.Width);
		}

		public int StartOffset
		{
			get { return m_StartOffset; }
			set	{m_StartOffset=value;}
		}

		public String	Text
		{
			get { return m_Text; }
			set { m_Text = value; m_IsAnalyseRequired = true; }
		}

		public int GetWidthOfAll()
		{
			if(m_IsAnalyseRequired)
			{
				Analyse();
			}
			return m_CaretPos[m_CaretPos.Count - 1];
		}

		public void Clear()
		{
			m_Text = "";
			m_CaretPos.Clear();
			m_IsAnalyseRequired = false;
		}

		//根据光标所在的字符位置获得光标所在的像素坐标
		public int CPtoX(int nCP, bool bTrail)
		{
			if (bTrail)
			{
				nCP = this.GetNextItemPos(nCP);
			}

			bool succeed = true;
			if (m_IsAnalyseRequired)
			{
				succeed = this.Analyse();
			}

			if (succeed)
			{
				nCP = Math.Min(nCP, m_CaretPos.Count - 1);
				if (nCP < 0) nCP = 0;
				return m_CaretPos[nCP];
			}
			else
			{
				return 0;
			}
		}

		//根据像素坐标计算光标所在的字符位置
		public int XtoCP(int nX)
		{
			if (nX <= 0) return 0;
			if (m_IsAnalyseRequired)
			{
				this.Analyse();
			}
			int count = m_CaretPos.Count;
			if (nX > m_CaretPos[count - 1]) return count - 1;
			for (int i = 0; i < count - 1; ++i)
			{
				if (nX >= m_CaretPos[i] && nX < m_CaretPos[i + 1])
				{
					return i;
				}
			}
			return count - 1;
		}

		//解析字符
		private bool Analyse()
		{
			m_CaretPos.Clear();
			Rect rc=new Rect();
			m_CaretPos.Add(m_StartOffset);
			DrawTextFormat textFormt = DrawTextFormat.Left | DrawTextFormat.Top | DrawTextFormat.NoClip | DrawTextFormat.SingleLine;
			for (int i = 1; i <= m_Text.Length; ++i)
			{
				String text = "";
				text+=m_Text[i - 1];
				if(text[0]==' ')
				{
					m_CaretPos.Add(m_CaretPos[i-1]+m_SpaceWidth);
					continue;
				}
				m_Font.MeasureString(text, textFormt,ref rc);
				m_CaretPos.Add(m_CaretPos[i - 1] + (int)rc.Width);
			}
			m_IsAnalyseRequired = false;
			return true;
		}

		int GetPriorItemPos( int nCP )
		{
			return Math.Max(nCP - 1, 0);
		}

		int GetNextItemPos( int nCP )
		{
			return Math.Min(nCP + 1, m_Text.Length-1);
		}
	}

	class Segment
	{
		private int m_X, m_Length;

		public int X
		{
			get { return m_X; }
			set { m_X = value; }
		}

		public int Length
		{
			get { return m_Length; }
			set { m_Length = value; }
		}

		public int Right
		{
			get { return m_X + m_Length; }
		}

		public Segment()
		{
			m_X = 0; m_Length = 0;
		}

		public Segment(int x,int len)
		{
			m_X = x; m_Length = len;
		}

		public Segment(Segment seg)
		{
			m_X = seg.X; m_Length = seg.Length;
		}

		public Segment	Intersection(Segment seg)
		{
			if ( (m_X+m_Length > seg.X) && (m_X < seg.X+seg.Length) )
			{
				Segment s=new Segment();

				s.X = (m_X > seg.X) ? m_X : seg.X;
				s.m_Length = (Right < seg.Right) ? Right-s.X : seg.Right-s.X;

				return s;
			}

			return new Segment(0,0);
		}
	}
}
