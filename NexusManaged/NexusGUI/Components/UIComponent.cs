using System;
using System.ComponentModel;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    [TypeConverter(typeof(UICompTypeConverter))]
    [XmlClassSerializable("UIComp_DrawBase", false)]
    public abstract class UIComponent
    {
        public EventHandler<GUIEventArgs> OwnerChanged;

        [NonSerialized]
        protected Window    owner;

        public Window Owner
        {
            get { return owner; }
            set 
            { 
                if(owner != value)
                {
                    owner = value;
                    OnOwnerChanged(new GUIEventArgs());
                }
            }
        }

        protected UIComponent() { } //default ctor not valid - we want to enforce initializing our data

        protected UIComponent(Window owner)
        {
            this.Owner = owner;
        }

        protected virtual void OnOwnerChanged(GUIEventArgs e)
        {
            if(OwnerChanged!=null)
            {
                OwnerChanged(owner,e);
            }
        }

#region CustomSerialization
        /// <summary>
        /// 序列化完成之后调用，用于完成构建自身的Component关系
        /// </summary>
        internal virtual void PostCustomSerial(Window _owner)
        {
            this.Owner = _owner;
        }
#endregion
    }
}
