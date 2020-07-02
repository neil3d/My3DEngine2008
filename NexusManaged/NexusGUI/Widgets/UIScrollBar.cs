using System;
using System.ComponentModel;
using NexusEngine;
using Nexus.GUI.Components;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("UIScrollBar",true)]
    public abstract class UIScrollBar : Nexus.GUI.Window
    {
        #region Event Handler
        public event EventHandler<WindowEventArgs> ScrollPositionChanged;
        #endregion Event Handler


        #region Fields
        /// <summary>
        /// 文档大小
        /// </summary>
        [XmlFieldSerializable("documentSize")]
        protected float documentSize;
        /// <summary>
        /// 单页大小
        /// </summary>
        [XmlFieldSerializable("pageSize")]
        protected float pageSize;
        /// <summary>
        /// 页与页之间的重叠区域大小，单次导航时滚动的大小为(pageSize-overlapSize)
        /// </summary>
        [XmlFieldSerializable("overlapSize")]
        protected float overlapSize;
        /// <summary>
        /// 步进大小
        /// </summary>
        [XmlFieldSerializable("stepSize")]
        protected float stepSize;
        /// <summary>
        /// 当前位置
        /// </summary>
        protected float position;

        /// <summary>
        /// 滚动条背景图片渲染
        /// </summary>
        [XmlFieldSerializable("background")]
        protected UIComp_DrawTextureAtlas background; 

        /// <summary>
        /// 向下步进按钮
        /// </summary>
        protected UIButton increaseButton;
        /// <summary>
        /// 向上步进按钮
        /// </summary>
        protected UIButton decreaseButton;
        /// <summary>
        /// 导航按钮
        /// </summary>
        protected UIScrollThumb thumb;
        #endregion Fields

        [Category("Data")]
        public float DocumentSize
        {
            get
            {
                return documentSize;
            }
            set
            {
                if (value > 0.0f && documentSize != value)
                {
                    documentSize = value;
                    if (position > documentSize)
                    {
                        ScrollPosition = documentSize;
                    }
                    else
                    {
                        UpdateThumb();
                    }
                }
            }
        }

        [Category("Data")]
        public float PageSize
        {
            get
            {
                return pageSize;
            }
            set
            {
                if (value > 0.0f && pageSize != value)
                {
                    pageSize = value;
                    UpdateThumb();
                }
            }
        }

        [Category("Data")]
        public float OverlapSize
        {
            get
            {
                return overlapSize;
            }
            set
            {
                overlapSize = value;
            }
        }

        [Category("Data")]
        public float ScrollPosition
        {
            get
            {
                return position;
            }
            set
            {
                float oldPos = position;

                // max position is (docSize - pageSize), but must be at least 0 (in case doc size is very small)
                float maxPos = Math.Max((documentSize - pageSize), 0.0f);

                // limit position to valid range:  0 <= position <= max_pos
                position = (value >= 0) ? ((value <= maxPos) ? value : maxPos) : 0.0f;

                // notification if required
                if (position != oldPos)
                {
                    UpdateThumb();
                    OnScrollPositionChanged(new WindowEventArgs(this));
                }
            }
        }

        [Category("Data")]
        public float StepSize
        {
            get
            {
                return stepSize;
            }
            set
            {
                if (stepSize > 0.0f)
                {
                    stepSize = value;
                }
            }
        }

        [Category("Component")]
        [Description("Display some image.")]
        public UIComp_DrawTextureAtlas BackgroundComponent
        {
            get { return background; }
        }

        [Category("Data")]
        public float ThumbVerticalMin
        {
            get { return thumb.VerticalMin; }
        }

        [Category("Data")]
        public float ThumbVerticalMax
        {
            get { return thumb.VerticalMax; }
        }

        [Category("Data")]
        public float ThumbHorizontalMin
        {
            get { return thumb.HorizontalMin; }
        }

        [Category("Data")]
        public float ThumbHorizontalMax
        {
            get { return thumb.HorizontalMax; }
        }

        #region Properties

        #endregion Properties

        #region Constructor
        protected UIScrollBar() { } //default ctor not valid - we want to enforce initializing our data

        protected UIScrollBar(string name)
            : base(name)
        {
            this.documentSize = 1.0f;
            this.pageSize = 1.0f;
            this.stepSize = 1.0f;

            background = new UIComp_DrawTextureAtlas(this);

            background.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            background.AtlasItemName = "0";

            increaseButton = CreateDecreaseButton();
            decreaseButton = CreateIncreaseButton();
            thumb = CreateThumb();

            InitEventHandler();

        }
        #endregion Constructor

        #region Abstract Members
        /// <summary>
        /// 创建步进按钮
        /// </summary>
        /// <returns></returns>
        protected abstract UIButton CreateDecreaseButton();
        protected abstract UIButton CreateIncreaseButton();
        /// <summary>
        /// 创建导航按钮
        /// </summary>
        /// <returns></returns>
        protected abstract UIScrollThumb CreateThumb();

        /// <summary>
        /// 更新导航按钮的大小和位置等
        /// </summary>
        protected abstract void UpdateThumb();

        /// <summary>
        ///	获取当前位置位于滚动条的方向
        /// </summary>
        /// <returns>
        ///		- -1 滚动条需要向下滚动.
        ///		-  0 不需要滚动
        ///		- +1 滚动条需要向上滚动
        /// </returns>
        protected abstract float GetAdjustDirectionFromPoint(Vector2 point);

        /// <summary>
        /// 获取当前滚动条导航位置
        /// </summary>
        protected abstract float GetPositionFromThumb();

        #endregion Abstract Members

        #region Trigger Event Methods
        /// <summary>
        /// 触发滚动条位置变化事件
        /// </summary>
        /// <param name="e"></param>
        protected internal void OnScrollPositionChanged(WindowEventArgs e)
        {
            if (ScrollPositionChanged != null)
            {
                ScrollPositionChanged(this, e);
            }
        }

        protected internal override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            bool ret = base.OnMouseButtonDown(e);
            
            if( e.Button == GUISystem.ClickButton )
            {
                float adj = GetAdjustDirectionFromPoint(e.Position);

				// adjust scroll bar position in whichever direction as required.
				if (adj != 0) 
                {
					ScrollPosition = (position + ((pageSize - overlapSize) * adj));
				}
                return true;
            }
            return ret;
        }

        private void InitEventHandler()
        {
            increaseButton.Clicked += new EventHandler<GUIEventArgs>(increaseButton_Clicked);
            decreaseButton.Clicked += new EventHandler<GUIEventArgs>(decreaseButton_Clicked);
            thumb.PositionChanged += new EventHandler<WindowEventArgs>(thumb_PositionChanged);
        }

        private void thumb_PositionChanged(object sender,WindowEventArgs e)
        {
            // adjust scroll bar position as required.
            ScrollPosition = GetPositionFromThumb();
        }

        private void decreaseButton_Clicked(object sender, GUIEventArgs e)
        {
            ScrollPosition = position - stepSize;
        }

        private void increaseButton_Clicked(object sender, GUIEventArgs e)
        {
            ScrollPosition = position + stepSize;
        }

        #endregion Trigger Event Methods

        #region Public Members
        /// <summary>
        /// 重载基类绘制方法
        /// </summary>
        protected override void DrawSelf()
        {
            if (background!=null)
            {
                background.Draw();
            }
        }
        #endregion Public  Members

        #region Serialization
        protected override void OnStringDictionaryChanged()
        {
        }

        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            this.background.PostCustomSerial(this);
        }

        [Browsable(false)]
        [XmlFieldSerializable("IncreaseButtonHelper")]
        public string IncreaseButtonHelper
        {
            get
            {
                return increaseButton.Name;
            }
            set
            {
                this.increaseButton = children[value] as UIButton;
                this.increaseButton.Clicked += new EventHandler<GUIEventArgs>(increaseButton_Clicked);
            }
        }
        [XmlFieldSerializable("DecreaseButtonHelper")]
        public string DecreaseButtonHelper
        {
            get
            {
                return decreaseButton.Name;
            }
            set
            {
                this.decreaseButton = children[value] as UIButton;
                this.decreaseButton.Clicked += new EventHandler<GUIEventArgs>(decreaseButton_Clicked);
            }
        }
        [XmlFieldSerializable("ThumbHelper")]
        public string ThumbHelper
        {
            get
            {
                return thumb.Name;
            }
            set
            {
                this.thumb = children[value] as UIScrollThumb;
                this.thumb.PositionChanged += new EventHandler<WindowEventArgs>(thumb_PositionChanged);
            }
        }
        #endregion

    }
}
