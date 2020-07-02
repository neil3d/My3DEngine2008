using System;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("GUISheet", true)]
    internal sealed class GUISheet : Window
    {
        public GUISheet() { } //default ctor not valid - we want to enforce initializing our data

        public GUISheet(string name)
            :base(name)
        {
        }

        protected override void DrawSelf() { }
        protected override void OnStringDictionaryChanged()
        {

        }
        #region Serialization

        #endregion Serialization
    }
}
