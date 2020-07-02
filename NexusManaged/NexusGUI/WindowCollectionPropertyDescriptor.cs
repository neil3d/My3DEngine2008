using System;
using System.Text;
using System.ComponentModel;

namespace Nexus.GUI
{
    public class WindowCollectionPropertyDescriptor : PropertyDescriptor
    {
        private WindowCollection wndList;
        private int index;

        public WindowCollectionPropertyDescriptor(WindowCollection wnds, int idx)
            : base("#" + idx.ToString(System.Globalization.CultureInfo.CurrentCulture),null)
        {
            this.wndList = wnds;
            this.index = idx;
        }

        public override AttributeCollection Attributes
        {
            get
            {
                return new AttributeCollection(null);
            }
        }

        public override bool CanResetValue(object component)
        {
            return true;
        }

        public override Type ComponentType
        {
            get
            {
                return this.wndList.GetType();
            }
        }

        public override string DisplayName
        {
            get
            {
                Window wnd = wndList[index];
                return wnd.Name + " " + wnd.GetType();
            }
        }

        public override string Description
        {
            get
            {
                Window wnd = wndList[index];
                return wnd.Name + " " + wnd.GetType();
            }
        }

        public override object GetValue(object component)
        {
            return this.wndList[this.index];
        }

        public override bool IsReadOnly
        {
            get 
            {
                return false;
            }
        }

        public override string Name
        {
            get
            {
                return "#" + index.ToString(System.Globalization.CultureInfo.CurrentCulture);
            }
        }

        public override Type PropertyType
        {
            get { return this.wndList[index].GetType(); }
        }

        public override void ResetValue(object component)
        {
        }

        public override bool ShouldSerializeValue(object component)
        {
            return true;
        }

        public override void SetValue(object component, object value)
        {
            // this.wndList[index] = value;
        }
    }
}
