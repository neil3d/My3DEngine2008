using System;
using System.ComponentModel;
using Nexus.GUI.Components;
using System.Runtime.Serialization;
using System.Security.Permissions;

using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    [Serializable]
    [XmlClassSerializable("UILabel",true)]
    public class UILabel : Window
    {
		[XmlFieldSerializable("textRenderComp")]
		protected UIComp_DrawText textRenderComp;
		[XmlFieldSerializable("backgroundRenderComp")]
		protected UIComp_DrawTextureAtlas backgroundRenderComp;

		[Category("Component")]
		[Description("Display some text.")]
		public UIComp_DrawText TextRenderComp
		{
			get { return textRenderComp; }
			set { textRenderComp = value; }
		}

		[Category("Component")]
		[Description("Display some image.")]
		public UIComp_DrawTextureAtlas BackgroundRenderComp
		{
			get { return backgroundRenderComp; }
			set { backgroundRenderComp = value; }
		}

        public UILabel() { } //default ctor not valid - we want to enforce initializing our data

        public UILabel(string name):base(name)
        {
            textRenderComp = new UIComp_DrawText(this);
            textRenderComp.Text = "UILabel";

            backgroundRenderComp = new UIComp_DrawTextureAtlas(this);

            backgroundRenderComp.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            backgroundRenderComp.AtlasItemName = "0";
        }

        protected override void DrawSelf()
        {
            backgroundRenderComp.Draw( );
            textRenderComp.Draw();
        }

        protected override void OnStringDictionaryChanged()
        {
            if(textRenderComp != null)
            {
                textRenderComp.OnStringDictionaryChanged();
            }
        }

        #region CustromSeialization
        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            if (textRenderComp != null)
            {
                textRenderComp.PostCustomSerial(this);
            }
            if(backgroundRenderComp != null)
            {
                backgroundRenderComp.PostCustomSerial(this);
            }
        }
        #endregion

    }
}
