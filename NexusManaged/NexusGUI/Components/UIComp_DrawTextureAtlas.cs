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
    public class UIComp_DrawTextureAtlas : UIComp_DrawTexture
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

        /// <summary>
        /// 资源存放的路径
        /// </summary>
        private NResourceLoc resourceAtlasLocation;
        private string atlasItemName;

        [EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
        public NResourceTextureAtlas TextureAtlas
        {
            get { return Texture as NResourceTextureAtlas; }
            set { Texture = value; }
        }

        [XmlFieldSerializable("resourceAtlasLocation")]
        public NResourceLoc ResourceAtlasLocation
        {
            get { return resourceAtlasLocation; }
            set
            {
                if (resourceAtlasLocation != value)
                {
                    resourceAtlasLocation = value;

                    ReLoadTextureAtlas();
                    OnResourceAtlasLocationChanged();
                }
            }
        }

        /// <summary>
        /// 纹理在Altas中的名称标识
        /// </summary>
        [Description("The name of texture in altas.")]
        [XmlFieldSerializable("atlasItemName")]
        public string AtlasItemName
        {
            get { return atlasItemName; }
            set
            {
                if (atlasItemName != value)
                {
                    atlasItemName = value;
                    OnAltasItemChanged();
                }
            }
        }

        public UIComp_DrawTextureAtlas() { } //default ctor not valid - we want to enforce initializing our data

        public UIComp_DrawTextureAtlas(Window owner)
            : base(owner)
        {
        }

        /// <summary>
        /// 资源路径发生变化,需要重新加载资源
        /// </summary>
        private void OnResourceAtlasLocationChanged()
        {
            if (ResourceAtlasLocationChanged != null)
            {
                ResourceAtlasLocationChanged(this, new ResourceLocationArgs(resourceAtlasLocation));
            }
        }

        private void ReLoadTextureAtlas()
        {
            if (resourceAtlasLocation != null
                && resourceAtlasLocation.IsValid()
                && !string.IsNullOrEmpty(resourceAtlasLocation.FileName)
                && !string.IsNullOrEmpty(resourceAtlasLocation.PackageName))
            {
                TextureAtlas = NResourceManager.Instance.LoadTextureAtlas(
                resourceAtlasLocation, EResourceIOMode.Auto, EResourceIOPriority.Normal);
            }
            else
            {
                TextureAtlas = null;
            }
        }

        private void OnAltasItemChanged()
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
                AltasItemChanged(this, new GUIEventArgs());
            }
        }

        #region CustomSerialization

        internal override void PostCustomSerial(Window _owner)
        {
            base.PostCustomSerial(_owner);
        }
        #endregion
    }
}
