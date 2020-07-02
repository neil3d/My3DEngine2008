using System;
using System.ComponentModel;
using Nexus.GUI;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    [Serializable]
    [XmlClassSerializable("UIComp_DrawText", true)]
    public class UIComp_DrawText : UIComp_DrawBase
    {
        /// <summary>
        /// 文本内容发生改变
        /// </summary>
        public EventHandler<GUIEventArgs> TextChanged;

        [XmlFieldSerializable("fontDesc")]
        protected FontDescription fontDesc;
        [NonSerialized]
        protected NexusEngine.NRenderFont font;

        protected string text;
        private string textKey;

        [XmlFieldSerializable("textFormat")]
        protected DrawTextFormat textFormat;

        [XmlFieldSerializable("textColor")]
        protected NexusEngine.Color4f textColor;

        [XmlFieldSerializable("shadowColor")]
        protected Color4f shadowColor;

        /// <summary>
        /// Display text content.
        /// </summary>
        [Description("Display text content.")]
        [EditorBrowsable(EditorBrowsableState.Advanced)]
        public string Text
        {
            get { return text; }
            set
            {
                text = value;
                OnTextChanged(new GUIEventArgs());
            }
        }

        [Description("Display text content.")]
        [XmlFieldSerializable("TextKey")]
        public string TextKey
        {
            get { return textKey; }
            set
            {
                textKey = value;
                Text = GUISystem.Instance.StringDictionary.GetValue(textKey);
            }
        }
        
        /// <summary>
        /// Specifies formatting options for text rendering.
        /// </summary>
        [Description("Specifies formatting options for text rendering.")]
        public DrawTextFormat TextFormat
        {
            get { return textFormat; }
            set { textFormat = value; }
        }

        /// <summary>
        /// display text color 
        /// </summary>
        [Browsable(false)]
        [Description("display text color")]
        public Color4f TextColor
        {
            get { return textColor; }
            set { textColor = value; }
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
        public override float Alpha
        {
            get { return textColor.A; }
            set
            {
                float realValue = Owner != null ? Math.Min(value, Owner.EffectiveAlpha) : 1.0f;
                textColor.A = realValue;
                shadowColor.A = Math.Min(value, shadowColor.A);
            }
        }

        [Description("display text color")]
        public override System.Drawing.Color Color
        {
            get { return textColor.ColorRGB; }
            set
            {
                TextColor = new Color4f(value);
                Alpha = Color.A;
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
            }
        }

        [EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
        public NRenderFont Font
        {
            get { return font; }
        }

        public UIComp_DrawText() { } //default ctor not valid - we want to enforce initializing our data

        public UIComp_DrawText(Window owner)
            : base(owner)
        {
            text = string.Empty;
            textColor = new Color4f(1.0f, 0.0f, 0.0f, 0.0f);

            fontDesc = FontDescription.DefaultFontDesc;
            font = NFontManager.Instance.ConstructFont(fontDesc);
        }

        public override void Draw()
        {
            if (font != null)
            {
                GUISystem.Instance.Renderer.DrawString(font, text, targetRect, textFormat, textColor, shadowColor);
            }
        }

        /// <summary>
        /// 文字内容发生改变
        /// </summary>
        protected virtual void OnTextChanged(GUIEventArgs e)
        {
            if (TextChanged != null)
            {
                TextChanged(this, e);
            }
        }


        #region CustromSerialization
        internal override void PostCustomSerial(Window _owner)
        {
            base.PostCustomSerial(_owner);

            // 创建字体
            font = NFontManager.Instance.ConstructFont(fontDesc);
        }
        #endregion


        internal void OnStringDictionaryChanged()
        {
            this.TextKey = this.textKey;
        }
    }
}
