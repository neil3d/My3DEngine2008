using System;

using System.ComponentModel;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("UIButton", true)]
    public class UIButton : UIButtonBase
    {
        /// <summary>
        /// true when widget is pushed
        /// </summary>
        protected bool pushed;
        /// <summary>
        /// true when the button is in 'hover' state and requires the hover rendering.
        /// </summary>
        protected bool hovering;

        [Browsable(false)]
        public bool Pushed
        {
            get { return pushed; }
        }

        [Browsable(false)]
        public bool Hovering
        {
            get { return hovering; }
        }

        public override string TextKey
        {
            get
            {
                if (hovering)
                {
                    return hoveringText.TextKey;
                }
                else if (pushed)
                {
                    return pushedText.TextKey;
                }
                else if (!IsEnabled)
                {
                    return disableText.TextKey;
                }
                else
                {
                    return normalText.TextKey;
                }
            }
            set
            {
                base.TextKey = value;
            }
        }

        public UIButton() { } //default ctor not valid - we want to enforce initializing our data

        public UIButton(string name)
            : base(name)
        {
            normalBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            normalBackground.AtlasItemName = "0";

            hoveringBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            hoveringBackground.AtlasItemName = "1";

            pushedBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            pushedBackground.AtlasItemName = "2";

            //disableBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            //disableBackground.AtlasItemName = "0";
        }


        /// <summary>
        /// Perform the rendering for this widget.
        /// </summary>
        protected override void DrawSelf()
        {
            if (hovering)
            {
                hoveringBackground.Draw();
                hoveringText.Draw();
            }
            else if (pushed)
            {
                pushedBackground.Draw();
                pushedText.Draw();
            }
            else if (!IsEnabled)
            {
                disableBackground.Draw();
                disableText.Draw();
            }
            else
            {
                normalBackground.Draw();
                normalText.Draw();
            }
        }

        /// <summary>
        /// 更新Button状态
        /// </summary>
        /// <param name="mousePos"></param>
        protected internal virtual void UpdateInternalState(Vector2 mousePos)
        {
            bool oldState = hovering;

            hovering = false;

            Window captureWnd = Window.CaptureWindow;
            if (captureWnd == null || captureWnd == this)
            {
                Window sheet = GUISystem.Instance.RootWindow;
                if (sheet != null
                    && (this == sheet.GetChildAtPosition(mousePos)) != pushed)
                {
                    hovering = true;
                }
            }

            if (oldState != hovering)
            {
                RequestRedraw();
            }
        }

        protected internal override bool OnMouseMove(GUIMouseEventArgs e)
        {
            if (!pushed)
            {
                UpdateInternalState(e.Position);
            }
            return base.OnMouseMove(e);
        }

        protected internal override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseButtonDown(e);

            if (e.Button == GUISystem.ClickButton)
            {
                CaptureInput();
                pushed = true;
                UpdateInternalState(e.Position);
                RequestRedraw();

                return true;
            }
            return ret;
        }

        protected internal override bool OnMouseButtonUp(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseButtonUp(e);

            if (e.Button == GUISystem.ClickButton)
            {
                ReleaseInput();

                Window sheet = GUISystem.Instance.RootWindow;
                if (sheet != null)
                {
                    if (this == sheet.GetChildAtPosition(e.Position))
                    {
                        OnCliked(new GUIEventArgs());
                        return true;
                    }
                }
            }
            return ret;
        }

        protected internal override void OnCaptureLost(GUIEventArgs e)
        {
            base.OnCaptureLost(e);

            pushed = false;
            UpdateInternalState(MouseCursor.Instance.Position);
            RequestRedraw();
        }

        protected internal override void OnMouseLeaves(GUIMouseEventArgs e)
        {
            base.OnMouseLeaves(e);

            hovering = false;
            RequestRedraw();
        }
    }
}
