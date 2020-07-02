using System;
using System.ComponentModel;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("UIRadioButton", true)]
    public class UIRadioButton : UIButtonBase
    {
        /// <summary>
        ///		Event triggered internally when state of check-box changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> CheckStateChanged;

        /// <summary>
        ///		true if radio button is selected (has check mark).
        /// </summary>
        [XmlFieldSerializable("isChecked")]
        protected bool isChecked;
        /// <summary>
        /// Radio button group ID.
        /// </summary>
        [XmlFieldSerializable("groupID")]
        protected int groupID;

        /// <summary>
        ///		Get/Set the checked state of the check box.
        /// </summary>
        /// <value>
        ///		true to select the widget and give it the check mark.  
        ///		false to deselect the widget and remove the check mark.
        /// </value>
        [Browsable(false)]
        public bool Checked
        {
            get { return isChecked; }
            set 
            {
                if( value != isChecked)
                {
                    isChecked = value;

                    // if new state is 'checked', de-select any other buttons in our group
                    if (isChecked)
                    {
                        DeselectOtherButtonsInGroup();
                    }

                    // event notification
                    OnCheckStateChanged(new WindowEventArgs(this));
                }
            }
        }

        /// <summary>
        ///		Get/Set the group ID for this radio button.
        /// </summary>
        /// <value>Integer value that holds the group ID which this radio button belongs to.</value>
        public int GroupID
        {
            get
            {
                return groupID;
            }
            set
            {
                groupID = value;

                // if new state is 'checked', de-select any other buttons in our group
                if (isChecked)
                {
                    DeselectOtherButtonsInGroup();
                }
            }
        }

        public UIRadioButton() { } //default ctor not valid - we want to enforce initializing our data

        public UIRadioButton(string name)
            : base(name)
        {
            normalBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            normalBackground.AtlasItemName = "N_sys-button-b";

            hoveringBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            hoveringBackground.AtlasItemName = "N_sys-button-b";

            pushedBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            pushedBackground.AtlasItemName = "N_sys-button-b-l";

            disableBackground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            disableBackground.AtlasItemName = "N_sys-button-b";
        }

        /// <summary>
        ///		Deselect any selected radio buttons attached to the same parent within the same group
        ///		(but not do not deselect 'this').
        /// </summary>
        protected void DeselectOtherButtonsInGroup()
        {
            // nothing to do unless we are attached to another window
            if (parent != null)
            {
                // scan all children
                foreach (Window child in parent.Children)
                {
                    if (child is UIRadioButton)
                    {
                        UIRadioButton button = (UIRadioButton)child;

                        // if it is enabled, is not this, and belongs to our group..
                        if (button.Checked && button != this && button.GroupID == groupID)
                        {
                            // .. disable it
                            button.Checked = false;
                        }
                    }
                }
            }
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
                        // toggle selected state, which will deselect all other radio buttons in our group
                        if (!isChecked)
                        {
                            Checked = true;
                        }
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
