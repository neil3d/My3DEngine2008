using System;
using System.ComponentModel;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;

using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    /// <summary>
    /// 用于组合滚动条，或者滑动条等控件
    /// </summary>
    [XmlClassSerializable("UIScrollThumb", true)]
    public class UIScrollThumb : Nexus.GUI.Widgets.UIButton
    {
        #region Event Declarations
        /// <summary>
        /// 位置发生变化
        /// </summary>
        public event EventHandler<WindowEventArgs> PositionChanged;
        #endregion Event Declarations

        #region Fields

        /// <summary>
        ///	true:鼠标移动时实时触发位置变化消息，false:当鼠标释放时才触发位置变化消息
        /// </summary>
        [XmlFieldSerializable("hotTrack")]
        protected bool hotTrack;
        /// <summary>
        ///	可否垂直移动
        /// </summary>
        [XmlFieldSerializable("vertFree")]
        protected bool vertFree;
        /// <summary>
        ///	可否水平移动
        /// </summary>
        [XmlFieldSerializable("horzFree")]
        protected bool horzFree;

        /// <summary>
        ///	垂直移动最小范围
        /// </summary>
        [XmlFieldSerializable("vertMin")]
        protected float vertMin;
        /// <summary>
        ///	垂直移动最大范围.
        /// </summary>
        [XmlFieldSerializable("vertMax")]
        protected float vertMax;
        /// <summary>
        ///	水平移动最小范围.
        /// </summary>
        [XmlFieldSerializable("horzMin")]
        protected float horzMin;
        /// <summary>
        ///	水平移动最大范围.
        /// </summary>
        [XmlFieldSerializable("horzMax")]
        protected float horzMax;

        /// <summary>
        ///	开始拖动
        /// </summary>
        protected bool isBeingDragged;
        /// <summary>
        ///	拖动起始位置.
        /// </summary>
        protected Vector2 dragPosition;

        #endregion Fields

        #region Properties

        [Category("Data")]
        public bool HotTrack
        {
            get
            {
                return hotTrack;
            }
            set
            {
                hotTrack = value;
            }
        }

        [Category("Data")]
        public float VerticalMin
        {
            get
            {
                return vertMin;
            }
            set
            {
                SetVerticalRange(value, vertMax);
            }
        }

        [Category("Data")]
        public float VerticalMax
        {
            get
            {
                return vertMax;
            }
            set
            {
                SetVerticalRange(vertMin, value);
            }
        }

        [Category("Data")]
        public float HorizontalMin
        {
            get
            {
                return horzMin;
            }
            set
            {
                SetHorizontalRange(value,horzMax);
            }
        }

        [Category("Data")]
        public float HorizontalMax
        {
            get
            {
                return horzMax;
            }
            set
            {
                SetHorizontalRange(horzMin, value);
            }
        }

        [Category("Data")]
        public bool Horizontal
        {
            get
            {
                return horzFree;
            }
            set
            {
                horzFree = value;
            }
        }

        [Category("Data")]
        public bool Vertical
        {
            get
            {
                return vertFree;
            }
            set
            {
                vertFree = value;
            }
        }

        #endregion Properties

        #region Constructor
        public UIScrollThumb() { } //default ctor not valid - we want to enforce initializing our data
        public UIScrollThumb(string name)
            : base(name)
        {
            hotTrack = true;
            horzMax = 1.0f;
            vertMax = 1.0f;
        }
        #endregion Constructor

        #region Methods

        public void SetHorizontalRange(float min, float max)
        {
            // ensure min <= max, swap if not.
            if (min > max)
            {
                float tmp = min;
                max = min;
                min = tmp;
            }

            horzMax = max;
            horzMin = min;
        }

        public void SetVerticalRange(float min, float max)
        {
            // ensure min <= max, swap if not.
            if (min > max)
            {
                float tmp = min;
                max = min;
                min = tmp;
            }

            vertMax = max;
            vertMin = min;
        }

        #endregion Methods

        #region Trigger Methods

        /// <summary>
        ///		Triggers an event when the position of the thumb widget has changed.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected internal void OnPositionChanged(WindowEventArgs e)
        {
            if (PositionChanged != null)
            {
                PositionChanged(this, e);
            }
        }

        #endregion Trigger Methods

        #region Overridden Trigger Methods
        protected internal override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseButtonDown(e);

            if (e.Button == GUISystem.ClickButton)
            {
                isBeingDragged = true;
                dragPosition = e.Position;
                return true;
            }

            return ret;
        }

        protected internal override void OnCaptureLost(GUIEventArgs e)
        {
            base.OnCaptureLost(e);

            // we are no longer dragging no that the thumb is released
            isBeingDragged = false;

            // send notification whenever thumb is released
            OnPositionChanged(new WindowEventArgs(this));
        }

        protected internal override bool OnMouseMove(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseMove(e);
            if (isBeingDragged)
            {
                Vector2 delta = e.Position - dragPosition;
                dragPosition = e.Position;

                if (horzFree)
                {
                    UIDim newX = this.X;
                    newX.offest += delta.x;

                    // limit value to within currently set range
                    newX.offest = (newX.offest < horzMin) ? horzMin : (newX.offest > horzMax) ? horzMax : newX.offest;

                    if (newX != this.X)
                    {
                        this.X = newX;
                        // send notification as required
                        if (hotTrack)
                        {
                            OnPositionChanged(new WindowEventArgs(this));
                        }
                    }
                }

                if (vertFree)
                {
                    UIDim newY = this.Y;
                    newY.offest += delta.y;

                    // limit new position to within currently set range
                    newY.offest = (newY.offest < vertMin) ? vertMin : (newY.offest > vertMax) ? vertMax : newY.offest;

                    if (newY != this.Y)
                    {
                        this.Y = newY;
                        // send notification as required
                        if (hotTrack)
                        {
                            OnPositionChanged(new WindowEventArgs(this));
                        }
                    }
                }
                return true;
            }
            return ret;
        }

        #endregion Overridden Trigger Methods
 
    }
}
