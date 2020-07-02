using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;

using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    [XmlClassSerializable("UIComp_DrawTextureAlphaMap", true)]
    public class UIComp_DrawTextureAlphaMap : UIComp_DrawBase
	{
		[NonSerialized]
		private NResourceTexture2D texture;

		[NonSerialized]
		private float rotation;

		[XmlFieldSerializable("textureLoc")]
		protected NResourceLoc textureLoc;

		[NonSerialized]
		private NResourceTextureAtlas alphaMap;
		[XmlFieldSerializable("alphaMapLoc")]
		protected NResourceLoc alphaMapLoc;

		[XmlFieldSerializable("alphaMaskAltasName")]
		protected string alphaMapAtlasName;

		[XmlFieldSerializable("textureColor")]
		protected Color4f textureColor;

		[XmlFieldSerializable("textureUVRect")]
		protected Rect textureUVRect;

		[XmlFieldSerializable("alphaMapUVRect")]
		protected Rect alphaMapUVRect;

		public event EventHandler<GUIEventArgs> OnRendering;

		#region Properties
		public NResourceLoc ImageLocation
		{
			get { return textureLoc; }
			set
			{
				if (textureLoc != value)
				{
					textureLoc = value;

					ReLoadTextures();
				}
			}
		}

		public NResourceLoc AlphaMapLocation
		{
			get { return alphaMapLoc; }
			set
			{
				if (alphaMapLoc != value)
				{
					alphaMapLoc = value;

					ReLoadTextures();
				}
			}
		}

		/// <summary>
		/// 纹理在Altas中的名称标识
		/// </summary>
		[Description("The name of alphamap in altas.")]
		public string AlphaAtlasItemName
		{
			get { return alphaMapAtlasName; }
			set
			{
				if (alphaMapAtlasName != value)
				{
					alphaMapAtlasName = value;
					if (alphaMap != null)
					{
						NTexAtlasItem item = alphaMap.GetItem(alphaMapAtlasName);
						alphaMapUVRect = new Rect(item.UVStart.x, item.UVStart.y, item.UVEnd.x, item.UVEnd.y);
					}
				}

			}
		}

		public override float Alpha
		{
			get
			{
				return textureColor.A;
			}
			set
			{
				textureColor.A = Owner != null ? Math.Min(value, Owner.EffectiveAlpha) : 1.0f;
			}
		}

		public override System.Drawing.Color Color
		{
			get
			{
				return textureColor.ColorRGB;
			}
			set
			{
				BackColor = new Color4f(value);
				Alpha = Color.A;
			}
		}

		[Browsable(false)]
		public Color4f BackColor
		{
			get { return textureColor; }
			set { textureColor = value; }
		}

		/// <summary>
		/// 指定贴图的UV坐标，坐标范围[0,1]
		/// </summary>
		public Rect ImageUVRect
		{
			get { return textureUVRect; }
			set { textureUVRect = value; }
		}

		/// <summary>
		/// 指定AlphaMap的UV坐标，坐标范围[0,1]
		/// </summary>
		[Browsable(false)]
		public Rect AlphaMapUVRect
		{
			get { return alphaMapUVRect; }
			set { alphaMapUVRect = value; }
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

		[EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
		public NResourceTexture AlphaMap
		{
			get { return alphaMap as NResourceTexture; }
		}

		public float Rotation
		{
			get
			{
				return rotation;
			}
			set
			{
				rotation=value;
			}
		}
		#endregion

		public UIComp_DrawTextureAlphaMap() { } //default ctor not valid - we want to enforce initializing our data

		public UIComp_DrawTextureAlphaMap(Window owner)
            : base(owner)
        {
			textureColor = new Color4f(1, 1, 1, 1);
			rotation = 0;
        }

		private void ReLoadTextures()
		{
			if (textureLoc != null
				&& textureLoc.IsValid()
				&& !string.IsNullOrEmpty(textureLoc.FileName)
				&& !string.IsNullOrEmpty(textureLoc.PackageName))
			{
				texture = NResourceManager.Instance.LoadResourceTexture2D(
				textureLoc, EResourceIOMode.Auto, EResourceIOPriority.Normal);
			}
			else
			{
				Texture = null;
			}

			if (alphaMapLoc != null
				&& alphaMapLoc.IsValid()
				&& !string.IsNullOrEmpty(alphaMapLoc.FileName)
				&& !string.IsNullOrEmpty(alphaMapLoc.PackageName))
			{
				alphaMap = NResourceManager.Instance.LoadTextureAtlas(
				alphaMapLoc, EResourceIOMode.Auto, EResourceIOPriority.Normal);

				if (alphaMap != null)
				{
					NTexAtlasItem item = alphaMap.GetItem(alphaMapAtlasName);
					alphaMapUVRect = new Rect(item.UVStart.x, item.UVStart.y, item.UVEnd.x, item.UVEnd.y);
				}
			}
			else
			{
				alphaMap = null;
			}
		}

        public override void Draw()
        {
            if (texture != null && alphaMap!=null)
            {
				GUISystem.Instance.Renderer.DrawTexture(texture, textureUVRect, alphaMap, alphaMapUVRect,targetRect,rotation,textureColor.ToArgb());
				if (OnRendering!=null)
				{
					OnRendering(this, new GUIEventArgs());
				}
            }
        }
	}
}
