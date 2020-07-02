using System;
using System.ComponentModel;
using Nexus.GUI.Components;
using System.Runtime.Serialization;
using System.Security.Permissions;

using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    /// <summary>
    /// 用于显示单一图片
    /// </summary>
    [Serializable]
    [XmlClassSerializable("UIStaticImage",true)]
    public class UIStaticImage : Window
    {
        [XmlFieldSerializable("backgroundRenderComp")]
        protected UIComp_DrawTextureAtlas backgroundRenderComp;

        [Category("Component")]
        [Description("Display some image.")]
        public UIComp_DrawTextureAtlas BackgroundRenderComp
        {
            get { return backgroundRenderComp; }
            set { backgroundRenderComp = value; }
        }

        public UIStaticImage() { } //default ctor not valid - we want to enforce initializing our data

        public UIStaticImage(string name):base(name)
        {
            backgroundRenderComp = new UIComp_DrawTextureAtlas(this);

            backgroundRenderComp.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            backgroundRenderComp.AtlasItemName = "0";
        }

        protected override void OnStringDictionaryChanged()
        {
        }

        protected override void DrawSelf()
        {
            backgroundRenderComp.Draw( );
        }

        #region CustromSeialization
        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            if(backgroundRenderComp != null)
            {
                backgroundRenderComp.PostCustomSerial(this);
            }
        }
        #endregion
    }
}
