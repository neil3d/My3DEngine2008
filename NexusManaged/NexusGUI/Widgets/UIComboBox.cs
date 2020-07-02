using System;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("UIComboBox", true)]
    public class UIComboBox : Nexus.GUI.Window
    {
        public UIComboBox() { } //default ctor not valid - we want to enforce initializing our data
        
        public UIComboBox(string name)
            : base(name)
        {
        }

        protected override void DrawSelf()
        {
        }

        protected override void OnStringDictionaryChanged()
        {

        }
        #region Serialization
        #endregion Serialization

    }
}
