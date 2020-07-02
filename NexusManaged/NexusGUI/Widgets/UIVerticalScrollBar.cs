using System;
using System.ComponentModel;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    /// <summary>
    /// 实现垂直滚动条
    /// </summary>
    [XmlClassSerializable("UIVerticalScrollBar", true)]
    public class UIVerticalScrollBar : UIScrollBar
    {
        #region Constructor
        public UIVerticalScrollBar() { } //default ctor not valid - we want to enforce initializing our data
        public UIVerticalScrollBar(string name)
            : base(name)
        {
        }
        #endregion Constructor

        protected override UIButton CreateIncreaseButton()
        {
            UIButton button = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", name + "_increase", this) as UIButton;
            button.Width = new UIDim(1, 0);
            button.Height = new UIDim(0, 24);
            return button;
        }

        protected override UIButton CreateDecreaseButton()
        {
            UIButton button = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", name + "_decrease", this) as UIButton;
            button.Width = new UIDim(1, 0);
            button.Height = new UIDim(0, 24);
            button.Y = new UIDim(1, -button.Height.offest);
            return button;
        }

        protected override UIScrollThumb CreateThumb()
        {
            UIScrollThumb thumb = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIScrollThumb", name + "_thumb", this) as UIScrollThumb;
            thumb.Width = new UIDim(1, 0);
            thumb.Height = new UIDim(0, 24);
            thumb.Vertical = true;
            return thumb;
        }
        
        /// <summary>
        /// 更新导航按钮的大小和位置等
        /// </summary>
        protected override void UpdateThumb()
        {
            thumb.VerticalMin = decreaseButton.Rect.Height;
            thumb.VerticalMax = this.Rect.Height - decreaseButton.Rect.Height - increaseButton.Rect.Height;

            float sizeVertical = thumb.VerticalMax - thumb.VerticalMin;
            float maxPos = Math.Max((documentSize - pageSize), 0.0f);
            float rate = maxPos > 0 ? position / maxPos : 0;
            if (rate > 1.0f) rate = 1.0f;
            thumb.Y = new UIDim(0, (decreaseButton.Rect.Height + sizeVertical * rate));
        }

        protected override float GetAdjustDirectionFromPoint(Vector2 point)
        {
            if( this.Rect.IsPointInRect(point) )
            {
                if( point.y < thumb.Rect.top )
                {
                    return -1;
                }
                if( point.y > thumb.Rect.bottom )
                {
                    return 1;
                }
            }
            return 0;
        }

        protected override float GetPositionFromThumb()
        {
            float thumbRange = thumb.VerticalMax - thumb.VerticalMin;
            if (thumbRange < 0) return 0;

            float maxPos = Math.Max((documentSize - pageSize), 0.0f);
            return (thumb.Y.offest - thumb.VerticalMin) / thumbRange * maxPos;
        }

        protected internal override void OnSized(GUIEventArgs e)
        {
            base.OnSized(e);

            UpdateThumb();
        }

    }
}
