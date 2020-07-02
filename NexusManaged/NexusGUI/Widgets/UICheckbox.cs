using System;
using System.ComponentModel;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("UICheckbox", true)]
    public class UICheckbox : UIButtonBase
    {
        /// <summary>
        ///		Event triggered internally when state of check-box changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> CheckStateChanged;

        /// <summary>
        ///		true if checkbox is selected (has checkmark).
        /// </summary>
        [XmlFieldSerializable("isChecked")]
        protected bool isChecked;

        /// <summary>
        ///		Get/Set the checked state of the checkbox.
        /// </summary>
        /// <value>
        ///		true to select the widget and give it the checkmark.  
        ///		false to deselect the widget and remove the checkmark.
        /// </value>
        [Browsable(false)]
        public bool Checked
        {
            get
            {
                return isChecked;
            }
            set
            {
                if (value != isChecked)
                {
                    isChecked = value;

                    // event notification
                    OnCheckStateChanged(new WindowEventArgs(this));
                }
            }
        }

        public UICheckbox() { } //default ctor not valid - we want to enforce initializing our data
        
        public UICheckbox(string name)
            : base(name)
        {
            normalBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            normalBackground.AtlasItemName = @"N_sys-button-b";

            hoveringBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            hoveringBackground.AtlasItemName = @"N_sys-button-b";

            pushedBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            pushedBackground.AtlasItemName = @"N_sys-button-b-l";

            disableBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            disableBackground.AtlasItemName = @"N_sys-button-b";
        }


        /// <summary>
        ///		Event triggered internally when state of checkbox changes.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected internal void OnCheckStateChanged(WindowEventArgs e)
        {
            // redraw the widget
            RequestRedraw();

            if (CheckStateChanged != null)
            {
                CheckStateChanged(this, e);
            }
        }

        /// <summary>
        ///		When the mouse up occurs, we change the selected state.
        /// </summary>
        /// <param name="e"></param>
        protected internal override bool OnMouseButtonUp(GUIMouseEventArgs e)
        {
            if (e.Button == GUISystem.ClickButton)
            {
                Window sheet = GUISystem.Instance.RootWindow;

                if (sheet != null)
                {
                    // if mouse was released over this widget
                    if (this == sheet.GetChildAtPosition(e.Position))
                    {
                        // toggle selected state
                        Checked = !isChecked;
                        return true;
                    }
                }
            }

            // default processing
            return base.OnMouseButtonUp(e);
        }

        /// <summary>
        /// Perform the rendering for this widget.
        /// </summary>
        protected override void DrawSelf()
        {
            if (Checked)
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

    }
}
