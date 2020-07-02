using System;
using System.ComponentModel;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;

using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    [Serializable]
    [XmlClassSerializable("UIComp_DrawTextureAtlas", true)]
    public class UIComp_DrawTexture : UIComp_DrawBase
    {
        [NonSerialized]
        private NResourceTexture2D texture;

        [XmlFieldSerializable("textureColor")]
        protected ColorRect textureColor;
        [XmlFieldSerializable("textureUVRect")]
        protected Rect textureUVRect;


        #region Properties
        [Browsable(false)]
        public override float Alpha
        {
            get
            {
                return textureColor.left_bottom.A;
            }
            set
            {
                float realValue = Owner != null ? Math.Min(value, Owner.EffectiveAlpha) : 1.0f;

                textureColor.left_top.A = realValue;
                textureColor.left_bottom.A = realValue;
                textureColor.right_top.A = realValue;
                textureColor.right_bottom.A = realValue;
            }
        }

        public override System.Drawing.Color Color
        {
            get
            {
                return textureColor.left_top.ColorRGB;
            }
            set
            {
                ColorRect color = new ColorRect(new Color4f(value));
                TextureColor = color;
                Alpha = Color.A;
            }
        }

        [Browsable(false)]
        public ColorRect TextureColor
        {
            get { return textureColor; }
            set { textureColor = value; }
        }

        /// <summary>
        /// 指定贴图的UV坐标，坐标范围[0,1]
        /// </summary>
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

        [EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
        public NResourceTexture2D Texture
        {
            get { return texture; }
            set { texture = value; }
        }
        #endregion

        public UIComp_DrawTexture() { } //default ctor not valid - we want to enforce initializing our data

        public UIComp_DrawTexture(Window owner)
            : base(owner)
        {
            textureColor = new ColorRect(-1);
        }

        public override void Draw()
        {
            if (texture != null)
            {
                GUISystem.Instance.Renderer.DrawTexture(texture, textureUVRect, targetRect, textureColor);
            }
        }

    }
}
