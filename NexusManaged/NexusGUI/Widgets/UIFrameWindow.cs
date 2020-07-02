using System;
using System.ComponentModel;

using NexusEngine;
using Nexus.GUI.Components;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;


namespace Nexus.GUI.Widgets
{
    /// <summary>
    /// 可缩放、可移动的Form窗口
    /// </summary>
    [XmlClassSerializable("UIFrameWindow", true)]
    public class UIFrameWindow : Nexus.GUI.Widgets.UILabel
    {
        #region Fields
        const float DefaultSizingBorderSize = 4.0f;
        /// <summary>
        /// 标题栏
        /// </summary>
        protected UICaptionBar captionBar;
        /// <summary>
        /// 是否可缩放大小
        /// </summary>
        [XmlFieldSerializable("isSizingEnabled")]
        protected bool isSizingEnabled;
        /// <summary>
        ///	是否处于缩放状态
        /// </summary>
        protected bool isBeingSized;
        /// <summary>
        /// 拖放起始位置
        /// </summary>
        protected Vector2 dragStartPosition;
        /// <summary>
        /// 改变大小的方向
        /// </summary>
        protected SizingLocation sizingLocation;
        /// <summary>
        ///	边框大小
        /// </summary>
        [XmlFieldSerializable("borderSize")]
        protected float borderSize;

        #endregion Fields

        #region Properties
        /// <summary>
        /// 标题栏
        /// </summary>
        [Category("Windows")]
        public UICaptionBar CaptionBar
        {
            get { return captionBar; }
        }
        /// <summary>
        /// 是否可缩放大小
        /// </summary>
        [Category("Data")]
        public bool SizingEnabled
        {
            get
            {
                return isSizingEnabled;
            }
            set
            {
                isSizingEnabled = value;
            }
        }

        /// <summary>
        /// 边框大小
        /// </summary>
        [Category("Data")]
        public float BorderSize
        {
            get
            {
                return borderSize;
            }
            set
            {
                borderSize = value;
            }
        }
        #endregion Properties

        #region public methods
        public UIFrameWindow() { } //default ctor not valid - we want to enforce initializing our data

        public UIFrameWindow(string name)
            : base(name)
        {
            this.isSizingEnabled = true;
            this.borderSize = DefaultSizingBorderSize;


            captionBar = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UICaptionBar", name + "_caption", this) as UICaptionBar;

        }
        #endregion public methods

        #region Overridden Trigger Methods

        protected SizingLocation GetSizingBorderAtPoint(Vector2 point)
        {
            Rect frameRect = this.Rect;

            // we can only size if the frame is enabled and sizing is on
            if (SizingEnabled)
            {
                // point must be inside the outer edge
                if (frameRect.IsPointInRect(point))
                {
                    // adjust rect to get inner edge
                    frameRect.left += borderSize;
                    frameRect.top += borderSize;
                    frameRect.right -= borderSize;
                    frameRect.bottom -= borderSize;

                    // detect which edges we are on
                    bool top = (point.y < frameRect.top);
                    bool bottom = (point.y >= frameRect.bottom);
                    bool left = (point.x < frameRect.left);
                    bool right = (point.x >= frameRect.right);

                    // return appropriate 'SizingLocation' value
                    if (top && left)
                    {
                        return SizingLocation.TopLeft;
                    }
                    else if (top && right)
                    {
                        return SizingLocation.TopRight;
                    }
                    else if (bottom && left)
                    {
                        return SizingLocation.BottomLeft;
                    }
                    else if (bottom && right)
                    {
                        return SizingLocation.BottomRight;
                    }
                    else if (top)
                    {
                        return SizingLocation.Top;
                    }
                    else if (bottom)
                    {
                        return SizingLocation.Bottom;
                    }
                    else if (left)
                    {
                        return SizingLocation.Left;
                    }
                    else if (right)
                    {
                        return SizingLocation.Right;
                    }
                }
            }

            // default value
            return SizingLocation.None;
        }

        protected internal override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseButtonDown(e);
            
            if( e.Button == GUISystem.ClickButton 
                && this.isSizingEnabled == true )
            {
                sizingLocation = GetSizingBorderAtPoint(e.Position);
                if( sizingLocation != SizingLocation.None )
                {
                    // ensure all inputs come to us for now
                    CaptureInput();

                    // setup the 'dragging' state variables
                    isBeingSized = true;
                    dragStartPosition = e.Position;

                    return true;
                }
            }
            return ret;
        }

        protected internal override bool OnMouseButtonUp(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseButtonUp(e);
            
            if( e.Button == GUISystem.ClickButton )
            {
                // release our capture on the input data
                ReleaseInput();
                isBeingSized = false;
                return true;
            }
            return ret;
        }

        protected internal override void OnCaptureLost(GUIEventArgs e)
        {
            base.OnCaptureLost(e);
            isBeingSized = false;
            sizingLocation = SizingLocation.None;
        }

        protected internal override bool OnMouseMove(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseMove(e);

            if (SizingEnabled && isBeingSized && sizingLocation != SizingLocation.None)
            {
                Vector2 deltaVector = e.Position - dragStartPosition;
                dragStartPosition = e.Position;

                // 左边
                UIDim xOffest = new UIDim(0, deltaVector.x);
                UIDim yOffest = new UIDim(0, deltaVector.y);
                if( sizingLocation == SizingLocation.Left
                    || sizingLocation == SizingLocation.TopLeft
                    || sizingLocation == SizingLocation.BottomLeft )
                {
                    this.X += xOffest;
                    this.Width -= xOffest;
                }
                // 右边
                if (sizingLocation == SizingLocation.Right
                    || sizingLocation == SizingLocation.TopRight
                    || sizingLocation == SizingLocation.BottomRight)
                {
                    this.Width += xOffest;
                }
                // 上方
                if (sizingLocation == SizingLocation.Top
                        || sizingLocation == SizingLocation.TopLeft
                        || sizingLocation == SizingLocation.TopRight)
                {
                    this.Y += yOffest;
                    this.Height -= yOffest;
                }
                // 下方
                if (sizingLocation == SizingLocation.Bottom
                        || sizingLocation == SizingLocation.BottomLeft
                        || sizingLocation == SizingLocation.BottomRight)
                {
                    this.Height += yOffest;
                }

                return true;
            }
            return ret;
        }
        #endregion Overridden Trigger Methods

        #region Serialization

        [Browsable(false)]
        [XmlFieldSerializable("CaptionBarHelper")]
        public string CaptionBarHelper
        {
            get { return captionBar.Name;  }
            set 
            {
               this.captionBar = children[value] as UICaptionBar;
            }
        }

        #endregion

    } // end of class UIFrameWindow
} // end of namespace Nexus.GUI.Widgets
