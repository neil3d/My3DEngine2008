using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;
using System.ComponentModel;
using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    #region IDrawElement
    /// <summary>
    /// 基本绘制元素
    /// </summary>
    public interface IDrawElement
    {
        /// <summary>
        /// 绘制的屏幕目标区域
        /// </summary>
        Rect TargetRect { get; set; }

        void DrawElement();

        void PostSerial();
    }
    #endregion

    #region DrawTextureElement

    [XmlClassSerializable("DrawTextureElement")]
    public class DrawTextureElement : IDrawElement
    {
        #region Event
        public EventHandler<GUIEventArgs> ResourceTextureChange;
        #endregion

        #region Fields

        /// <summary>
        /// 需要绘制的纹理
        /// </summary>
        private NResourceTexture2D texture;

        /// <summary>
        /// 需要绘制的纹理区域
        /// </summary>
        private Rect textureUVRect;

        /// <summary>
        /// 颜色
        /// </summary>
        private ColorRect textureColor;

        /// <summary>
        /// 屏幕目标区域
        /// </summary>
        [XmlFieldSerializable("targetRect")]
        private Rect targetRect;

        #endregion

        #region Properties

        [Browsable(false)]
        protected NResourceTexture2D Texture
        {
            get { return texture; }
            set
            {
                if (texture != value)
                {
                    texture = value;

                    OnResourceTextureChange(new GUIEventArgs());
                }
            }
        }

        [XmlFieldSerializable("Color")]
        public ColorRect Color
        {
            get { return textureColor; }
            set { textureColor = value; }
        }

        /// <summary>
        /// 指定贴图的UV坐标，坐标范围[0,1]
        /// </summary>
        [XmlFieldSerializable("TextureUVRect")]
        public Rect TextureUVRect
        {
            get { return textureUVRect; }
            set { textureUVRect = value; }
        }

        /// <summary>
        /// 指定纹理坐标，根据纹理的大小转化为UV [0,1]坐标
        /// </summary>
        [EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
        public Rect TextureRect
        {
            get
            {
                Vector2 texSize = texture.GetOriginalSize();
                return new Rect(textureUVRect.left * texSize.x,
                    textureUVRect.top * texSize.y,
                    textureUVRect.right * texSize.x,
                    textureUVRect.bottom * texSize.y);
            }
            set
            {
                Vector2 texSize = texture.GetOriginalSize();
                textureUVRect.left = value.left / texSize.x;
                textureUVRect.top = value.top / texSize.y;
                textureUVRect.right = value.right / texSize.x;
                textureUVRect.bottom = value.bottom / texSize.y;
            }
        }
        #endregion

        #region Constructor

        public DrawTextureElement()
        { }

        #endregion


        #region Method

        virtual protected void OnResourceTextureChange(GUIEventArgs gUIEventArgs)
        {
            if (ResourceTextureChange != null)
            {
                ResourceTextureChange(this, gUIEventArgs);
            }
        }

        virtual protected void SelfPostSerial()
        {

        }

        #endregion

        #region IDrawElement 成员

        Rect IDrawElement.TargetRect
        {
            get { return targetRect; }
            set { targetRect = value; }
        }

        void IDrawElement.DrawElement()
        {
            if (texture != null)
            {
                GUISystem.Instance.Renderer.DrawTexture(texture, textureUVRect, targetRect, textureColor);
            }
        }

        void IDrawElement.PostSerial()
        {
        }

        #endregion
    }
    #endregion

    #region DrawTextureAtlasElement
    [XmlClassSerializable("DrawTextureAtlasElement", true)]
    public class DrawTextureAtlasElement : DrawTextureElement
    {
        #region Event Handler
        /// <summary>
        /// 资源路径发生改变
        /// </summary>
        public EventHandler<ResourceLocationArgs> ResourceAtlasLocationChanged;
        /// <summary>
        /// 资源名称发生改变
        /// </summary>
        public EventHandler<GUIEventArgs> AltasItemChanged;
        /// <summary>
        /// 资源区域发生改变
        /// </summary>
        public EventHandler<GUIEventArgs> AltasRectChanged;
        #endregion

        #region Fields

        [EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
        public NResourceTextureAtlas TextureAtlas
        {
            get { return Texture as NResourceTextureAtlas; }
            set { Texture = value; }
        }

        /// <summary>
        /// 资源存放的路径
        /// </summary>
        [XmlFieldSerializable("resourceAtlasLocation")]
        private NResourceLoc resourceAtlasLocation;

        [XmlFieldSerializable("atlasItemName")]
        private string atlasItemName;
        #endregion

        public NResourceLoc ResourceAtlasLocation
        {
            get { return resourceAtlasLocation; }
            set
            {
                if (!resourceAtlasLocation.Equals(value))
                {
                    resourceAtlasLocation = value;

                    ReLoadTextureAtlas();
                    OnResourceAtlasLocationChanged(new ResourceLocationArgs(resourceAtlasLocation));
                }
            }
        }

        /// <summary>
        /// 纹理在Altas中的名称标识
        /// </summary>
        [Description("The name of texture in altas.")]
        public string AtlasItemName
        {
            get { return atlasItemName; }
            set
            {
                if (!atlasItemName.Equals(value))
                {
                    atlasItemName = value;
                    OnAltasItemChanged(new GUIEventArgs());
                }
            }
        }


        #region Constructor
        public DrawTextureAtlasElement()
        {
        }
        #endregion

        #region Method

        protected override void SelfPostSerial()
        {
            base.SelfPostSerial();

            ReLoadTextureAtlas();
        }

        private void ReLoadTextureAtlas()
        {
            if (resourceAtlasLocation != null
                && !string.IsNullOrEmpty(resourceAtlasLocation.FileName)
                && !string.IsNullOrEmpty(resourceAtlasLocation.PackageName))
            {
                TextureAtlas = NResourceManager.Instance.LoadTextureAtlas(
                resourceAtlasLocation, EResourceIOMode.Auto, EResourceIOPriority.Normal);
            }
        }

        /// <summary>
        /// 资源路径发生变化,需要重新加载资源
        /// </summary>
        virtual protected void OnResourceAtlasLocationChanged(ResourceLocationArgs e)
        {
            if (ResourceAtlasLocationChanged != null)
            {
                ResourceAtlasLocationChanged(this, e);
            }
        }


        virtual protected void OnAltasItemChanged(GUIEventArgs gUIEventArgs)
        {
            NResourceTextureAtlas texAtlas = TextureAtlas;
            if (texAtlas != null)
            {
                NTexAtlasItem item = texAtlas.GetItem(atlasItemName);
                TextureUVRect = new Rect(item.UVStart.x, item.UVStart.y, item.UVEnd.x, item.UVEnd.y);

                if (AltasRectChanged != null)
                {
                    AltasRectChanged(this, new GUIEventArgs());
                }
            }

            if (AltasItemChanged != null)
            {
                AltasItemChanged(this, gUIEventArgs);
            }
        }
        #endregion
    }
    #endregion

    #region DrawStringElement
    public class DrawStringElement : IDrawElement
    {
        #region Event
        public EventHandler<GUIEventArgs> TextChanged;
        #endregion

        #region Fields

        /// <summary>
        /// 渲染器对象
        /// </summary>
        private NRenderFont fontRender;

        /// <summary>
        /// 屏幕目标区域
        /// </summary>
        [XmlFieldSerializable("targetRect")]
        private Rect targetRect;

        [XmlFieldSerializable("fontDesc")]
        protected FontDescription fontDesc;

        [XmlFieldSerializable("textFormat")]
        protected DrawTextFormat textFormat;

        [XmlFieldSerializable("textColor")]
        protected Color4f textColor;

        [XmlFieldSerializable("shadowColor")]
        protected Color4f shadowColor;

        [XmlFieldSerializable("text")]
        protected string text;

        #endregion

        #region Properties
        /// <summary>
        /// Display text content.
        /// </summary>
        [Description("Display text content.")]
        public string Text
        {
            get { return text; }
            set
            {
                text = value;
                OnTextChanged(new GUIEventArgs());
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
        [Description("display shadow color")]
        public Color4f ShadowColor
        {
            get { return shadowColor; }
            set { shadowColor = value; }
        }

        /// <summary>
        /// display text color 
        /// </summary>
        [Description("display text color")]
        public Color4f TextColor
        {
            get { return textColor; }
            set { textColor = value; }
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
                fontRender = NFontManager.Instance.ConstructFont(fontDesc);
            }
        }
        #endregion

        #region Constructor
        public DrawStringElement()
        {
            RecreateFont();
        }

        private void RecreateFont()
        {
            fontRender = NFontManager.Instance.ConstructFont(fontDesc);
        }

        #endregion

        #region Methods
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
        #endregion

        #region IDrawElement 成员

        Rect IDrawElement.TargetRect
        {
            get
            {
                return targetRect;
            }
            set
            {
                targetRect = value;
            }
        }

        void IDrawElement.DrawElement()
        {
            if (fontRender != null)
            {
                GUISystem.Instance.Renderer.DrawString(fontRender, text, targetRect, textFormat, textColor, shadowColor);
            }
        }

        void IDrawElement.PostSerial()
        {
            // 创建字体
            RecreateFont();
        }

        #endregion
    }
    #endregion

}
