using System;
using System.ComponentModel;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    [Serializable]
    [XmlClassSerializable("UIComp_DrawBase", true)]
    public abstract class UIComp_DrawBase : UIComponent
    {
        // 矩形区域发生变化
        public EventHandler<GUIEventArgs> RectChanged;

        #region Fields
        /// <summary>
        /// 组件的位置，相对宿主窗口坐标
        /// </summary>
        [XmlFieldSerializable("x")]
        protected UIDim x;
        [XmlFieldSerializable("y")]
        protected UIDim y;

        /// <summary>
        /// 组建的大小，相对于宿主窗口坐标
        /// </summary>
        [XmlFieldSerializable("width")]
        protected UIDim width;
        [XmlFieldSerializable("height")]
        protected UIDim height;

        /// <summary>
        /// 绝对像素坐标
        /// </summary>
        [NonSerialized]
        protected Rect targetRect;
        #endregion

        #region Properties

        /// <summary>
        /// 改变组件的Alpha值
        /// </summary>
        public abstract float Alpha { get; set; }

        /// <summary>
        /// 改变组件的颜色
        /// </summary>
        public abstract System.Drawing.Color Color { get; set; }

        [Category("Data")]
        [Description("the x position of the component.")]
        public UIDim X
        {
            get
            {
                return x;
            }
            set
            {
                if (x != value)
                {
                    x = value;
                    InvalidateComponentRect();
                }
            }
        }

        [Category("Data")]
        [Description("the y position of the component.")]
        public UIDim Y
        {
            get
            {
                return y;
            }
            set
            {
                if (y != value)
                {
                    y = value;
                    InvalidateComponentRect();
                }
            }
        }

        [Category("Data")]
        [Description("the width of the component.")]
        public UIDim Width
        {
            get
            {
                return width;
            }
            set
            {
                if (width != value)
                {
                    width = value;
                    InvalidateComponentRect();
                }
            }
        }

        [Category("Data"), DefaultValue(50)]
        [Description("the height of the component.")]
        public UIDim Height
        {
            get
            {
                return height;
            }
            set
            {
                if (height != value)
                {
                    height = value;
                    InvalidateComponentRect();
                }
            }
        }

        [Browsable(false)]
        public Rect TargetRect
        {
            get { return targetRect; }
            set { targetRect = value; }
        }
        #endregion

        /// <summary>
        /// 更新组件矩形区域大小
        /// </summary>
        protected virtual void InvalidateComponentRect()
        {
            Rect oldRect = targetRect;
            // 重新计算窗口区域
            if (owner != null)
            {
                targetRect.left = owner.Rect.left + owner.Rect.Width * x.scale + x.offest;
                targetRect.top = owner.Rect.top + owner.Rect.Height * y.scale + y.offest;
                targetRect.right = targetRect.left + owner.Rect.Width * width.scale + width.offest;
                targetRect.bottom = targetRect.top + owner.Rect.Height * height.scale + height.offest;
            }
            else
            {
                targetRect.left = x.offest;
                targetRect.top = y.offest;
                targetRect.right = targetRect.left + width.offest;
                targetRect.bottom = targetRect.top + height.offest;
            }

            if (oldRect != targetRect)
            {
                OnRectChanged(new GUIEventArgs());
            }
        }

        /// <summary>
        /// 矩形区域发生变化
        /// </summary>
        protected virtual void OnRectChanged(GUIEventArgs e)
        {
            if (RectChanged != null)
            {
                RectChanged(owner, e);
            }
        }

        /// <summary>
        /// 宿主窗口的区域发生改变
        /// </summary>
        private void owner_RectChanged(object sender, GUIEventArgs e)
        {
            InvalidateComponentRect();
        }

        /// <summary>
        /// 构造
        /// </summary>
        protected UIComp_DrawBase() { } //default ctor not valid - we want to enforce initializing our data

        protected UIComp_DrawBase(Window owner)
            : base(owner)
        {
            this.Width = new UIDim(1.0f, 0);
            this.Height = new UIDim(1.0f, 0);
        }

        protected override void OnOwnerChanged(GUIEventArgs e)
        {
            base.OnOwnerChanged(e);
            InvalidateComponentRect();
            owner.RectChanged += new EventHandler<GUIEventArgs>(owner_RectChanged);
            owner.AlphaChanged += new EventHandler<GUIEventArgs>(owner_AlphaChanged);
            owner.InheritsAlphaChanged += new EventHandler<GUIEventArgs>(owner_InheritsAlphaChanged);

            Alpha = Owner != null ? Owner.EffectiveAlpha : 1.0f;
        }

        void owner_InheritsAlphaChanged(object sender, GUIEventArgs e)
        {
            Alpha = Owner != null ? Owner.EffectiveAlpha : 1.0f;
        }

        void owner_AlphaChanged(object sender, GUIEventArgs e)
        {
            Alpha = Owner != null ? Owner.EffectiveAlpha : 1.0f;
        }


        /// <summary>
        /// 组件渲染抽象接口
        /// </summary>
        abstract public void Draw();

    }
}
