using System;
using System.ComponentModel;
using Nexus.GUI.Components;
using System.Runtime.Serialization;
using System.Security.Permissions;

using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    /// <summary>
    /// 用于显示文本
    /// </summary>
    [Serializable]
    [XmlClassSerializable("UIStaticText",true)]
    public class UIStaticText : Window
    {
        [XmlFieldSerializable("textRenderComp")]
        protected UIComp_DrawText textRenderComp;

        [Category("Component")]
        [Description("Display some text.")]
        public UIComp_DrawText TextRenderComp
        {
            get { return textRenderComp; }
            set { textRenderComp = value; }
        }

        public UIStaticText() { } //default ctor not valid - we want to enforce initializing our data

        public UIStaticText(string name):base(name)
        {
            textRenderComp = new UIComp_DrawText(this);
            textRenderComp.Text = "UIStaticText";
        }

        protected override void DrawSelf()
        {
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
        }
        #endregion

    }
}
