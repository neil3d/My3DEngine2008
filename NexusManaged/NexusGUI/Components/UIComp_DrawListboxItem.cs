using System;
using System.ComponentModel;
using Nexus.GUI;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    [XmlClassSerializable("UIComp_DrawListboxItem", true)]
    public class UIComp_DrawListboxItem : UIComp_DrawText
    {
        #region Fields
        /// <summary>
        /// 应用程序标记
        /// </summary>
        protected object tagObj;
        /// <summary>
        /// 是否被选中
        /// </summary>
        protected bool selected;

        #endregion Fields

        #region Properties
        /// <summary>
        /// 是否被选中
        /// </summary>
        public bool Selected
        {
            get { return selected; }
            set { selected = value; }
        }

        /// <summary>
        /// 应用程序标记数据
        /// </summary>
        [Browsable(false)]
        public object UserData
        {
            get { return tagObj; }
            set { tagObj = value; }
        }

        /// <summary>
        /// 大小
        /// </summary>
        [Browsable(false)]
        public Vector2 Size
        {
            get { return targetRect.Size; }
        }
        #endregion

        #region Constructors
        public UIComp_DrawListboxItem() { } //default ctor not valid - we want to enforce initializing our data
        public UIComp_DrawListboxItem(Window owner)
            : base(owner)
        {
            this.textFormat = DrawTextFormat.Left | DrawTextFormat.VerticalCenter;
            this.Height = new UIDim(0.0f, 24.0f);
        }

        public UIComp_DrawListboxItem(Window owner,string text)
            : base(owner)
        {
            this.text = text;
            this.textFormat = DrawTextFormat.Left | DrawTextFormat.VerticalCenter;
            this.tagObj = null;
            this.Height = new UIDim(0.0f, 24.0f);
        }

        public UIComp_DrawListboxItem(Window owner,string text,object userData)
            : base(owner)
        {
            this.text = text;
            this.textFormat = DrawTextFormat.Left | DrawTextFormat.VerticalCenter;
            this.tagObj = userData;
            this.Height = new UIDim(0.0f, 24.0f);
        }
        #endregion

        #region Implementation Methods
        /// <summary>
		///		Method called from static operator overload to virtualise the compare operation
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		protected virtual bool lessthan_operator(UIComp_DrawListboxItem other)
		{
            return string.CompareOrdinal(this.Text, other.Text) != 0;
            //return (this.Text.CompareTo(other.Text) < 0);
		}

		/// <summary>
		///		Method called from static operator overload to virtualise the compare operation
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		protected virtual bool greaterthan_operator(UIComp_DrawListboxItem other)
		{
            return string.CompareOrdinal(this.Text, other.Text)!=0;
            //return (this.Text.CompareTo(other.Text) > 0);
		}

		#endregion

		#region Operator Overloads

		/// <summary>
		///		Less-than operator for UIComp_DrawListboxItem objects.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		public static bool operator<(UIComp_DrawListboxItem left, UIComp_DrawListboxItem right)
		{
			// This is done via a virtual method, which allows a derived class to provide
			// custom sorting.
			return left.lessthan_operator(right);
		}

		/// <summary>
		///		Greater-than operator for UIComp_DrawListboxItem objects.
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		public static bool operator>(UIComp_DrawListboxItem left, UIComp_DrawListboxItem right)
		{
			// This is done via a virtual method, which allows a derived class to provide
			// custom sorting.
			return left.greaterthan_operator(right);
		}

		#endregion

    }
}
