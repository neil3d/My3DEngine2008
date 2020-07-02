using System;
using System.ComponentModel;

using NexusEngine;
using Nexus.GUI.Components;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    /// <summary>
    /// 可带动父窗口拖动的标题栏
    /// </summary>
    [XmlClassSerializable("UICaptionBar", true)]
    public class UICaptionBar : UILabel
    {
        #region event-delegate
        public EventHandler<GUIEventArgs> CloseButtonClicked;
        #endregion event-delegate

        #region Fields
        protected Vector2 dragStartPosition;
        protected bool dragging;

        [XmlFieldSerializable("moveable")]
        protected bool moveable;

        protected UIButton closeButton;
        #endregion Fields

        #region Properties
        [Category("Windows")]
        public UIButton CloseButton
        {
            get { return closeButton; }
        }

        [Category("Data")]
        public bool Moveable
        {
            get { return moveable; }
            set { moveable = value; }
        }

        #endregion Properties

        #region public method
        public UICaptionBar() { } //default ctor not valid - we want to enforce initializing our data
       
        public UICaptionBar(string name)
            : base(name)
        {
            this.moveable = true;
            this.Width = new UIDim(1, 0);
            this.Height = new UIDim(0, 28);
            this.Y = new UIDim(0, -this.Height.offest);

            // 创建关闭按钮
            closeButton = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", name + "_close", this) as UIButton;

            closeButton.AbsoluteSize = new Vector2(24, 24);
            closeButton.X = new UIDim(1.0f, -28.0f);
            closeButton.Y = new UIDim(0.0f, 2.0f);

            InitEventHandler();
            

            closeButton.NormalBackground.AtlasItemName = @"N_help-guanbi";
            closeButton.HoveringBackground.AtlasItemName = @"N_help-guanbi-l";
            closeButton.PushedBackground.AtlasItemName = @"N_help-guanbi-anxia";

            this.backgroundRenderComp.AtlasItemName = "0";
        }
        #endregion public method

        #region Overridden Event Trigger Methods
        private void InitEventHandler()
        {
            closeButton.Clicked += new EventHandler<GUIEventArgs>(closeButton_Clicked);
        }

        private void closeButton_Clicked(object sender,GUIEventArgs e)
        {
            if( CloseButtonClicked!= null)
            {
                CloseButtonClicked(this, e);
            }
        }

        protected internal override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseButtonDown(e);

            if( e.Button == GUISystem.ClickButton )
            {
                dragging = true;
                CaptureInput();
                dragStartPosition = e.Position;
                return true;
            }
            return ret;
        }

        protected internal override bool OnMouseButtonUp(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseButtonUp(e);

            if( e.Button == GUISystem.ClickButton )
            {
                ReleaseInput();
                dragging = false;
                return true;
            }
            return ret;
        }

        protected internal override void OnCaptureLost(GUIEventArgs e)
        {
            base.OnCaptureLost(e);

            dragging = false;
            e.Handled = true;
        }

        protected internal override bool OnMouseMove(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseMove(e);

            Window parentWindow = this.Parent;
            if (dragging && moveable && parentWindow != null && parentWindow != GUISystem.Instance.RootWindow)
            {
                parentWindow.AbsolutePosition += e.Position - dragStartPosition;
                dragStartPosition = e.Position;

                return true;
            }
            return ret;
        }
        #endregion Overridden Event Trigger Methods

        #region Serialization

        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            InitEventHandler();
        }

        [Browsable(false)]
        [XmlFieldSerializable("CloseButtonSerialHepler")]
        public string CloseButtonSerialHepler
        {
            get
            {
                return closeButton.Name;
            }
            set
            {
                closeButton = this.Children[value] as UIButton;
            }
        }
        #endregion
    }
}
