using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ComponentModel;
using Nexus.GUI.Components;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
	[Serializable]
	[XmlClassSerializable("UIShapedImageBox", true)]
	public class UIShapedImageBox : Window
	{
		[XmlFieldSerializable("backgroundRenderComp")]
		protected UIComp_DrawTextureAlphaMap backgroundRenderComp;

		[Category("Component")]
		[Description("Display some image.")]
		public UIComp_DrawTextureAlphaMap BackgroundRenderComp
		{
			get { return backgroundRenderComp; }
			set { backgroundRenderComp = value; }
		}

        public UIShapedImageBox() { } //default ctor not valid - we want to enforce initializing our data

		public UIShapedImageBox(string name)
			: base(name)
        {
			backgroundRenderComp = new UIComp_DrawTextureAlphaMap(this);
        }

        protected override void DrawSelf()
        {
            backgroundRenderComp.Draw( );
        }

        protected override void OnStringDictionaryChanged()
        {
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
