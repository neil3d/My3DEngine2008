using System;
using System.Text;
using System.Text.RegularExpressions;
using System.ComponentModel;

using NexusEngine;
using Nexus.GUI.Components;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("UIEditBox", true)]
    public class UIEditBox : Nexus.GUI.Window
    {
        #region Event
        /// <summary>
        /// 只读属性发生变化
        /// </summary>
        public event EventHandler<WindowEventArgs> ReadOnlyChanged;
        public event EventHandler<WindowEventArgs> MaskedRenderingModeChanged;
        public event EventHandler<WindowEventArgs> MaskCodePointChanged;
        public event EventHandler<WindowEventArgs> ValidationPatternChanged;
        public event EventHandler<WindowEventArgs> MaxTextLengthChanged;
        /// <summary>
        /// 输入字符发生变化
        /// </summary>
        public event EventHandler<WindowEventArgs> TextInvalidated;
        public event EventHandler<WindowEventArgs> InvalidEntryAttempted;
        public event EventHandler<WindowEventArgs> CaratMoved;
        public event EventHandler<WindowEventArgs> TextSelectionChanged;
        /// <summary>
        /// 输入的字符长度达到最大值
        /// </summary>
        public event EventHandler<WindowEventArgs> EditboxFull;
        /// <summary>
        /// 用户按下回车键接受输入
        /// </summary>
        public event EventHandler<WindowEventArgs> TextAccepted;
        #endregion

        #region Fields
        [XmlFieldSerializable("readOnly")]
        protected bool readOnly;
        [XmlFieldSerializable("maskText")]
        protected bool maskText;
        [XmlFieldSerializable("maskCodePoint")]
        protected char maskCodePoint;
        [XmlFieldSerializable("maxTextLength")]
        protected int maxTextLength;
        /// <summary>
        /// 输入位置
        /// </summary>
        protected int caratPosition;
        /// <summary>
        /// 选中位置
        /// </summary>
        protected int selectionStart;
        protected int selectionEnd;
        protected bool dragging;
        protected int dragAnchorIndex;
        /// <summary>
        /// 执行合法性检测的规则表达式
        /// </summary>
        protected Regex validator;
        [XmlFieldSerializable("validationPattern")]
        protected string validationPattern;
        /// <summary>
        /// 文本
        /// </summary>
        //[XmlFieldSerializable("text")]
        protected StringBuilder text = new StringBuilder();
        [XmlFieldSerializable("textComp")]
        protected UIComp_DrawEditBoxText textComp;
        [XmlFieldSerializable("backgroundComp")]
        protected UIComp_DrawTextureAtlas backgroundComp;

        [Category("Data")]
        [XmlFieldSerializable("CursorTimer")]
        public float CursorTimer { get; set; }
        #endregion

        #region Properties
        [Category("Component")]
        public UIComp_DrawEditBoxText TextComponent
        {
            get { return textComp; }
        }

        [Category("Component")]
        public UIComp_DrawTextureAtlas BackgroundComp
        {
            get { return backgroundComp; }
        }

        [Category("Data")]
        public int CaratPosition
        {
            get { return caratPosition; }
            set
            {
                if (value > text.Length)
                {
                    value = text.Length;
                }
                if (caratPosition != value)
                {
                    caratPosition = value;

                    OnCaratMoved(new WindowEventArgs(this));
                }
            }
        }

        /// <summary>
        /// 输入框是否获得焦点
        /// </summary>
        [Browsable(false)]
        public bool HasInputFocus
        {
            get { return IsActive; }
        }

        /// <summary>
        /// 检验文本是否有效
        /// </summary>
        [Browsable(false)]
        public bool IsTextValid
        {
            get { return IsStringValid(text.ToString()); }
        }

        [Category("Data")]
        public char MaskCodePoint
        {
            get { return maskCodePoint; }
            set
            {
                if (value != maskCodePoint)
                {
                    maskCodePoint = value;

                    OnMaskCodePointChanged(new WindowEventArgs(this));
                }
            }
        }

        [Category("Data")]
        public int MaxTextLength
        {
            get { return maxTextLength; }
            set
            {
                if (maxTextLength != value)
                {
                    maxTextLength = value;

                    WindowEventArgs e = new WindowEventArgs(this);
                    OnMaxTextLengthChanged(e);

                    if (text.Length > maxTextLength)
                    {
                        text = text.Remove(maxTextLength, text.Length - maxTextLength);
                        OnTextChanged(e);

                        if (!IsTextValid)
                        {
                            OnTextInvalidated(e);
                        }
                    }
                }
            }
        }

        [Category("Data")]
        public bool TextMasked
        {
            get { return maskText; }
            set
            {
                if (maskText != value)
                {
                    maskText = value;
                    OnMaskedRenderingModeChanged(new WindowEventArgs(this));
                }
            }
        }

        [Category("Data")]
        public bool ReadOnly
        {
            get { return readOnly; }
            set
            {
                if (readOnly != value)
                {
                    readOnly = value;
                    OnReadOnlyChanged(new WindowEventArgs(this));
                }
            }
        }

        [Browsable(false)]
        public int SelectionStartIndex
        {
            get { return selectionStart != selectionEnd ? selectionStart : caratPosition; }
        }

        [Browsable(false)]
        public int SelectionEndIndex
        {
            get { return selectionEnd != selectionStart ? selectionEnd : caratPosition; }
        }

        [Browsable(false)]
        public int SelectionLength
        {
            get { return selectionEnd - selectionStart; }
        }

        [Category("Data")]
        public string ValidationPattern
        {
            get { return validationPattern; }
            set
            {
                try
                {
                    validator = new Regex(value);
                    validationPattern = value;
                }
                catch
                {
                    throw new InvalidRequestException("The Edit box named {0} had the following bad validation expression set: {1}", this.Name, value);
                }

                WindowEventArgs e = new WindowEventArgs(this);
                OnValidationStringChanged(e);

                if (!IsTextValid)
                {
                    OnTextInvalidated(e);
                }
            }
        }

        [Category("data")]
        public StringBuilder Text
        {
            get { return text; }
        }
        #endregion

        #region Methods
        /// <summary>
        /// 清除当前选择
        /// </summary>
        protected void ClearSelection()
        {
            if( SelectionLength != 0 )
            {
                SetSelection(0, 0);
            }
        }

        /// <summary>
        /// 清除选择文本
        /// </summary>
        /// <param name="modifyText">
        /// true 清除文本
        /// false 只清除选择，不清楚文本
        /// </param>
        protected void EraseSelectedText(bool modifyText)
        {
            if( SelectionLength != 0)
            {
                CaratPosition = SelectionStartIndex;
                if( modifyText )
                {
                    text.Remove(SelectionStartIndex, SelectionLength);
                    OnTextChanged(new WindowEventArgs(this));
                }
                ClearSelection();
            }
        }

        /// <summary>
        /// 将当前选中文本拷贝到系统剪贴板
        /// </summary>
        protected void CopyToClipboard()
        {
            if( SelectionLength != 0 )
            {
                System.Windows.Forms.Clipboard.SetDataObject(text.ToString().Substring(SelectionStartIndex, SelectionLength));
            }
        }

        protected void CutToClipboard()
        {
            CopyToClipboard();

            if (ReadOnly)
                return;

            if (SelectionLength != 0)
            {
                string temp = text.ToString();
                temp = temp.Remove(SelectionStartIndex, SelectionLength);

                if (IsStringValid(temp))
                {
                    EraseSelectedText(true);
                }
                else
                {
                    OnInvalidEntryAttempted(new WindowEventArgs(this));
                }
            }
        }

        /// <summary>
        /// 从系统剪贴板中copy文本
        /// </summary>
        protected void PasteFromClipboard()
        {
            // Declares an IDataObject to hold the data returned from the clipboard.
            // Retrieves the data from the clipboard.
            System.Windows.Forms.IDataObject iData = System.Windows.Forms.Clipboard.GetDataObject();

            // Determines whether the data is in a format you can use.
            if (iData.GetDataPresent(System.Windows.Forms.DataFormats.Text))
            {
                // Yes it is, so display it in a text box.
                InsertText((String)iData.GetData(System.Windows.Forms.DataFormats.Text));
            }
        }

        public void ClearText()
        {
            text.Remove(0,text.Length);
            OnTextChanged(new WindowEventArgs(this));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_text"></param>
        public void InsertText(string inText)
        {
            InsertText(inText, SelectionStartIndex);
        }

        /// <summary>
        /// 追加文本
        /// </summary>
        /// <param name="text"></param>
        public void InsertText(string inText,int inPostion)
        {
            string temp = text.ToString();
            temp = temp.Remove(SelectionStartIndex, SelectionLength);
            if (temp.Length + inText.Length <= maxTextLength)
            {
                temp = temp.Insert(inPostion, inText);

                if (IsStringValid(temp))
                {
                    EraseSelectedText(false);

                    text.Remove(0, text.Length);
                    text.Append(temp);

                    CaratPosition = inPostion + inText.Length;
                    //caratPosition++;

                    OnTextChanged(new WindowEventArgs(this));
                }
                else
                {
                    OnInvalidEntryAttempted(new WindowEventArgs(this));
                }
            }
            else
            {
                OnEditboxFull(new WindowEventArgs(this));
            }
        }

        /// <summary>
        /// 处理退格键
        /// </summary>
        protected void HandleBackspace()
        {
            if (ReadOnly)
                return;

            string temp = text.ToString();
            if( SelectionLength != 0 )
            {
                temp = temp.Remove(SelectionStartIndex, SelectionLength);
                if(IsStringValid(temp))
                {
                    EraseSelectedText(true);
                }
                else
                {
				    OnInvalidEntryAttempted(new WindowEventArgs(this));
                }
            }
            else if( CaratPosition>0)
            {
                temp = temp.Remove(CaratPosition-1, 1);
                if( IsStringValid(temp) )
                {
                    CaratPosition = CaratPosition - 1;

                    text.Remove(0, text.Length);
                    text.Append(temp);
                    OnTextChanged(new WindowEventArgs(this));
                }
                else
                {
				    OnInvalidEntryAttempted(new WindowEventArgs(this));
                }
            }
        }

        /// <summary>
        /// 处理退格键
        /// </summary>
        protected void HandleDelete()
        {
            if (ReadOnly)
                return;

            string temp = text.ToString();
            if( SelectionLength != 0 )
            {
                temp = temp.Remove(SelectionStartIndex, SelectionLength);
            
                if( IsStringValid(temp))
                {
                    EraseSelectedText(true);
                }
                else
                {
                    OnInvalidEntryAttempted(new WindowEventArgs(this));
                }
            }
            else if( CaratPosition < temp.Length )
            {
                temp = temp.Remove(CaratPosition, 1);

                if( IsStringValid(temp) )
                {
                    text.Remove(0, text.Length);
                    text.Append(temp);
                    OnTextChanged(new WindowEventArgs(this));
                }
                else
                {
                    OnInvalidEntryAttempted(new WindowEventArgs(this));
                }
            }
        }

        protected void HandleLeft(System.Windows.Forms.KeyEventArgs e)
        {
            if( e.Shift )
            {
                if (SelectionLength == 0)
                {
                    dragAnchorIndex = caratPosition;
                }

                if (caratPosition > 0)
                {
                    CaratPosition = caratPosition - 1;
                }
                SetSelection(caratPosition, dragAnchorIndex);
            }
            else
            {
                if (SelectionLength != 0)
                {
                    ClearSelection();
                }
                else if (caratPosition > 0)
                {
                    CaratPosition = caratPosition - 1;
                }
            }
        }

        protected void HandleRight(System.Windows.Forms.KeyEventArgs e)
        {
            if (e.Shift)
            {
                if (SelectionLength == 0)
                {
                    dragAnchorIndex = caratPosition;
                }
                if (caratPosition < text.Length)
                {
                    CaratPosition = caratPosition + 1;
                }
                SetSelection(caratPosition, dragAnchorIndex);
            }
            else
            {
                if( SelectionLength != 0 )
                {
                    ClearSelection();
                }
                else if (caratPosition < text.Length)
                {
                    CaratPosition = caratPosition + 1;
                }
            }
        }
        protected void HandleHome(System.Windows.Forms.KeyEventArgs e)
        {
            if( caratPosition > 0 )
            {
                CaratPosition = 0;
            }

            if( e.Shift )
            {
                SetSelection(caratPosition, dragAnchorIndex);
            }
            else
            {
                ClearSelection();
            }
        }
        protected void HandleEnd(System.Windows.Forms.KeyEventArgs e)
        {
            if( caratPosition < text.Length )
            {
                CaratPosition = text.Length;
            }

            if( e.Shift)
            {
                SetSelection(caratPosition, dragAnchorIndex);
            }
            else
            {
                ClearSelection();
            }
        }

        private bool IsStringValid(string inText)
        {
            Match match = validator.Match(inText);

            //
            return (match.Success
                && match.Index == 0
                && match.Length == inText.Length);
        }

        public void SetSelection(int startPos, int endPos)
        {
            if(startPos > text.Length )
            {
                startPos = text.Length;
            }
            if( endPos > text.Length)
            {
                endPos = text.Length;
            }

            if (startPos > endPos )
            {
                int temp = endPos;
                endPos = startPos;
                startPos = temp;
            }

            if( startPos!= selectionStart || endPos !=selectionEnd)
            {
                selectionStart = startPos;
                selectionEnd = endPos;

                OnTextSelectionChanged(new WindowEventArgs(this));
            }
        }

        protected internal override void OnTextChanged(WindowEventArgs e)
        {
            base.OnTextChanged(e);

            // 取消当前选中
            ClearSelection();

            if( CaratPosition > text.Length)
            {
                CaratPosition = text.Length;
            }
        }

        protected virtual int GetTextIndexFromPosition(Vector2 vec)
        {
            if( TextComponent != null )
            {
                return TextComponent.GetTextIndexFromPosition(vec);
            }
            return 0;
        }
        #endregion

        #region Overridden Event Trigger Methods
        protected internal override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseButtonDown(e);

            if (e.Button == GUISystem.ClickButton)
            {
                // grab input
                CaptureInput();
				
                // 处理鼠标点击响应
                ClearSelection();
                dragging = true;
                dragAnchorIndex = GetTextIndexFromPosition(e.Position);
                CaratPosition = dragAnchorIndex;

                return true;
            }
            return ret;
        }

        protected internal override bool OnMouseButtonUp(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseButtonUp(e);
            if (e.Button == GUISystem.ClickButton)
            {
                ReleaseInput();
                return true;
            }
            return ret;
        }

        protected internal override bool OnMouseDoubleClicked(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseDoubleClicked(e);
            if (e.Button == GUISystem.ClickButton)
            {
                // 选择所有文本
                dragAnchorIndex = 0;
                CaratPosition = text.Length;

                SetSelection(dragAnchorIndex, caratPosition);
                return true;
            }
            return ret;
        }

        protected internal override bool OnMouseMove(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseMove(e);
        
            if( dragging )
            {
                CaratPosition = GetTextIndexFromPosition(e.Position);
                SetSelection(caratPosition, dragAnchorIndex);
                return true;
            }
            return ret;
        }

        protected internal override void OnCaptureLost(GUIEventArgs e)
        {
            dragging = false;
            base.OnCaptureLost(e);
        }

        protected internal override void OnKeyDown(System.Windows.Forms.KeyEventArgs e)
        {
            base.OnKeyDown(e);
            
            if( HasInputFocus && !ReadOnly )
            {
                WindowEventArgs args = new WindowEventArgs(this);

                switch (e.KeyCode)
                {
                    case System.Windows.Forms.Keys.Shift:
                	    if( SelectionLength == 0 )
                        {
                            dragAnchorIndex = CaratPosition;
                        }
                        break;

                    case System.Windows.Forms.Keys.Back:
                        HandleBackspace();
                        break;

                    case System.Windows.Forms.Keys.Delete:
                        HandleDelete();
                        break;

                    case System.Windows.Forms.Keys.Tab:
                        break;

                    case System.Windows.Forms.Keys.Enter:
                        OnTextAccepted(args);
                        break;

                    case System.Windows.Forms.Keys.Left:
                        HandleLeft(e);
                        break;

                    case System.Windows.Forms.Keys.Right:
                        HandleRight(e);
                        break;

                    case System.Windows.Forms.Keys.Home:
                        HandleHome(e);
                        break;

                    case System.Windows.Forms.Keys.End:
                        HandleEnd(e);
                        break;

                    case System.Windows.Forms.Keys.A:
                        if(e.Control==true)
                        {
                            CaratPosition = text.Length;
                            dragAnchorIndex = CaratPosition;
                            SetSelection(0, dragAnchorIndex);
                        }
                        break;

                    case System.Windows.Forms.Keys.C:
                        if (e.Control == true)
                        {
                            CopyToClipboard();
                        }
                        break;

                    case System.Windows.Forms.Keys.V:
                        if (e.Control == true)
                        {
                            PasteFromClipboard();
                        }
                        break;

                    case System.Windows.Forms.Keys.X:
                        if (e.Control == true)
                        {
                            CutToClipboard(); 
                        }
                        break;
                }
                e.Handled = true;
            }
        }

        protected internal override void OnKeyPress(System.Windows.Forms.KeyPressEventArgs e)
        {
            base.OnKeyPress(e);

            if (HasInputFocus
                  && !ReadOnly
                  && textComp != null && textComp.IsCharacterAvailable(e.KeyChar))
            {
                InsertText(e.KeyChar.ToString());
                e.Handled = true;
            }
        }

        #endregion

        #region event notification
        protected internal virtual void OnEditboxFull(WindowEventArgs windowEventArgs)
        {
            			if(EditboxFull != null) {
				EditboxFull(this, windowEventArgs);
			}
        }

        protected internal virtual void OnTextAccepted(WindowEventArgs args)
        {
            if( TextAccepted != null )
            {
                TextAccepted(this, args);
            }
        }

        protected internal virtual void OnTextSelectionChanged(WindowEventArgs windowEventArgs)
        {
            RequestRedraw();
            if( TextSelectionChanged != null )
            {
                TextSelectionChanged(this,windowEventArgs);
            }
        }

        protected internal virtual void OnInvalidEntryAttempted(WindowEventArgs windowEventArgs)
        {
            if( InvalidEntryAttempted!=null)
            {
                InvalidEntryAttempted(this, windowEventArgs);
            }
        }

        protected internal virtual void OnValidationStringChanged(WindowEventArgs e)
        {
            if(ValidationPatternChanged!=null)
            {
                ValidationPatternChanged(this, e);
            }
        }

        protected internal virtual void OnReadOnlyChanged(WindowEventArgs windowEventArgs)
        {
            RequestRedraw();
            if( ReadOnlyChanged != null )
            {
                ReadOnlyChanged(this, windowEventArgs);
            }
        }

        protected internal virtual void OnMaskedRenderingModeChanged(WindowEventArgs windowEventArgs)
        {
            RequestRedraw();
            if(MaskedRenderingModeChanged!=null)
            {
                MaskedRenderingModeChanged(this, windowEventArgs);
            }
        }


        protected internal virtual void OnMaskCodePointChanged(WindowEventArgs windowEventArgs)
        {
            if( TextMasked)
            {
                RequestRedraw();
            }
            if(MaskCodePointChanged!=null)
            {
                MaskCodePointChanged(this, windowEventArgs);
            }
        }

        protected internal virtual void OnCaratMoved(WindowEventArgs windowEventArgs)
        {
            RequestRedraw();
            if(CaratMoved!=null)
            {
                CaratMoved(this, windowEventArgs);
            }
        }

        protected internal virtual void OnMaxTextLengthChanged(WindowEventArgs e)
        {
            if(MaxTextLengthChanged!=null)
            {
                MaxTextLengthChanged(this, e);
            }
        }

        protected internal virtual void OnTextInvalidated(WindowEventArgs e)
        {
            if(TextInvalidated!=null)
            {
                TextInvalidated(this, e);
            }
        }
        #endregion


        public UIEditBox() { } //default ctor not valid - we want to enforce initializing our data
        
        public UIEditBox(string name)
            : base(name)
        {
            CursorTimer = 1.1f;
            this.ValidationPattern = ".*";
            validator = new Regex(validationPattern);
            maskCodePoint = '*';
            MaxTextLength = int.MaxValue;

            textComp = new UIComp_DrawEditBoxText(this);

            backgroundComp = new UIComp_DrawTextureAtlas(this);
            backgroundComp.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            backgroundComp.AtlasItemName = "editbox";
        }

        protected override void DrawSelf()
        {
            if (backgroundComp!=null)
            {
                backgroundComp.Draw();
            }
            if(textComp!= null)
            {
                textComp.Draw();
            }
        }

        protected internal override void OnTimePulse(TimePulseEventArgs e)
        {
            base.OnTimePulse(e);

            if (Visible && !ReadOnly && HasInputFocus && textComp != null)
            {
                // 计算光标闪烁
                textComp.CursorColorAlpha = ((e.elapsedTime % CursorTimer) / CursorTimer * Alpha) > 0.5f ? 1.0f : 0.0f;
            }
        }


        #region Serialization

        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            this.backgroundComp.PostCustomSerial(this);
            this.textComp.PostCustomSerial(this);

            this.ValidationPattern = validationPattern;
        }

        protected override void OnStringDictionaryChanged()
        {
        }

        #endregion
    }
}
