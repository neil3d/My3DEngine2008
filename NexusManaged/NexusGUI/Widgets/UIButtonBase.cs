using System;

using System.ComponentModel;
using NexusEngine;

namespace Nexus.GUI.Widgets
{
    using Nexus.GUI.Components;
    using System.Runtime.Serialization;
    using System.Security.Permissions;
    using NexusEngineExtension;

    [XmlClassSerializable("UIButtonBase", true)]
    public abstract class UIButtonBase : Nexus.GUI.Window
    {
        /// <summary>
        /// The button was clicked.
        /// </summary>
        public event EventHandler<GUIEventArgs> Clicked;

        #region Fields
        /// <summary>
        /// Normal state
        /// </summary>
        [XmlFieldSerializable("normalText")]
        protected UIComp_DrawText normalText;
        [XmlFieldSerializable("normalBackground")]
        protected UIComp_DrawTextureAtlas normalBackground;

        /// <summary>
        /// Hovering State
        /// </summary>
        [XmlFieldSerializable("hoveringText")]
        protected UIComp_DrawText hoveringText;
        [XmlFieldSerializable("hoveringBackground")]
        protected UIComp_DrawTextureAtlas hoveringBackground;

        /// <summary>
        /// pushed state
        /// </summary>
        [XmlFieldSerializable("pushedText")]
        protected UIComp_DrawText pushedText;
        [XmlFieldSerializable("pushedBackground")]
        protected UIComp_DrawTextureAtlas pushedBackground;
        /// <summary>
        /// Disable state
        /// </summary>
        [XmlFieldSerializable("disableText")]
        protected UIComp_DrawText disableText;
        [XmlFieldSerializable("disableBackground")]
        protected UIComp_DrawTextureAtlas disableBackground;
        #endregion


        [Category("Normal Component")]
        public UIComp_DrawText NormalText
        {
            get { return normalText; }
        }

        [Category("Normal Component")]
        public UIComp_DrawTextureAtlas NormalBackground
        {
            get { return normalBackground; }
        }

        [Category("Hovering Component")]
        public UIComp_DrawText HoveringText
        {
            get { return hoveringText; }
        }

        [Category("Hovering Component")]
        public UIComp_DrawTextureAtlas HoveringBackground
        {
            get { return hoveringBackground; }
        }
        [Category("Pushed Component")]
        public UIComp_DrawText PushedText
        {
            get { return pushedText; }
        }

        [Category("Pushed Component")]
        public UIComp_DrawTextureAtlas PushedBackground
        {
            get { return pushedBackground; }
        }
        [Category("Disable Component")]
        public UIComp_DrawText DisableText
        {
            get { return disableText; }
        }

        [Category("Disable Component")]
        public UIComp_DrawTextureAtlas DisableBackground
        {
            get { return disableBackground; }
        }

        [Category("Data")]
        virtual public string TextKey
        {
            get
            {
                return normalText.TextKey;
            }
            set
            {
                normalText.TextKey = value;
                hoveringText.TextKey = value;
                pushedText.TextKey = value;
                disableText.TextKey = value;
            }
        }

        protected UIButtonBase() { } //default ctor not valid - we want to enforce initializing our data

        public UIButtonBase(string name)
            : base(name)
        {
            normalText = new UIComp_DrawText(this);
            normalBackground = new UIComp_DrawTextureAtlas(this);

            hoveringText = new UIComp_DrawText(this);
            hoveringBackground = new UIComp_DrawTextureAtlas(this);

            pushedText = new UIComp_DrawText(this);
            pushedBackground = new UIComp_DrawTextureAtlas(this);

            disableText = new UIComp_DrawText(this);
            disableBackground = new UIComp_DrawTextureAtlas(this);
        }


        /// <summary>
        /// Handler invoked internally when the button is clicked.
        /// </summary>
        protected internal void OnCliked(GUIEventArgs e)
        {
            if (Clicked != null)
            {
                Clicked(this, e);
            }
        }


        #region Serialization

        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            this.normalText.PostCustomSerial(this);
            this.normalBackground.PostCustomSerial(this);
            this.hoveringText.PostCustomSerial(this);
            this.hoveringBackground.PostCustomSerial(this);
            this.pushedText.PostCustomSerial(this);
            this.pushedBackground.PostCustomSerial(this);
            this.disableText.PostCustomSerial(this);
            this.disableBackground.PostCustomSerial(this); 
        }

        #endregion

        protected override void OnStringDictionaryChanged()
        {
            this.normalText.OnStringDictionaryChanged();
            this.hoveringText.OnStringDictionaryChanged();
            this.pushedText.OnStringDictionaryChanged();
            this.disableText.OnStringDictionaryChanged();
        }

    }
}
