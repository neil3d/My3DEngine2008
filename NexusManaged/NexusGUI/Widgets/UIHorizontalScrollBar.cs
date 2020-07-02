using System;
using System.ComponentModel;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    /// <summary>
    /// 实现水平滚动条
    /// </summary>
    [XmlClassSerializable("UIHorizontalScrollBar", true)]
    public class UIHorizontalScrollBar : UIScrollBar
    {
        #region Constructor
        public UIHorizontalScrollBar() { } //default ctor not valid - we want to enforce initializing our data
        public UIHorizontalScrollBar(string name)
            : base(name)
        {
        }
        #endregion Constructor

        protected override UIButton CreateIncreaseButton()
        {
            UIButton button = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", name + "_increase",this) as UIButton;
            button.Height = new UIDim(1,0);
            button.Width = new UIDim(0, 24);
            return button;
        }

        protected override UIButton CreateDecreaseButton()
        {
            UIButton button = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", name + "_decrease", this) as UIButton;
            button.Height = new UIDim(1, 0);
            button.Width = new UIDim(0, 24);
            button.X = new UIDim(1, -button.Width.offest);
            return button;
        }

        protected override UIScrollThumb CreateThumb()
        {
            UIScrollThumb thumb = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIScrollThumb", name + "_thumb", this) as UIScrollThumb;
            thumb.Height = new UIDim(1, 0);
            thumb.Width = new UIDim(0, 24);
            thumb.Horizontal = true;
            return thumb;
        }

        protected override void UpdateThumb()
        {
            thumb.HorizontalMin = decreaseButton.Rect.Width;
            thumb.HorizontalMax = this.Rect.Width - decreaseButton.Rect.Width - increaseButton.Rect.Width;

            float sizeHorizontal = thumb.HorizontalMax - thumb.HorizontalMin;
            float maxPos = Math.Max((documentSize - pageSize), 0.0f);
            float rate = maxPos > 0 ? position / maxPos : 0;
            if (rate > 1.0f) rate = 1.0f;
            thumb.X = new UIDim(0, (decreaseButton.Rect.Width + sizeHorizontal * rate));
        }

        protected override float GetAdjustDirectionFromPoint(Vector2 point)
        {
            if (this.Rect.IsPointInRect(point))
            {
                if (point.x < thumb.Rect.left)
                {
                    return -1;
                }
                if (point.x > thumb.Rect.right)
                {
                    return 1;
                }
            }
            return 0;
        }

        protected override float GetPositionFromThumb()
        {
            float thumbRange = thumb.HorizontalMax - thumb.HorizontalMin;
            if (thumbRange < 0) return 0;

            float maxPos = Math.Max((documentSize - pageSize), 0.0f);
            return (thumb.X.offest - thumb.HorizontalMin) / thumbRange * maxPos;
        }

        protected internal override void OnSized(GUIEventArgs e)
        {
            base.OnSized(e);

            UpdateThumb();
        }

    }
}
