using System;
using System.ComponentModel;
using Nexus.GUI.Components;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Widgets
{
    /// <summary>
    /// 树状控件
    /// </summary>
    [XmlClassSerializable("UITreeView", true)]
    public class UITreeView : Nexus.GUI.Window
    {
        #region Fields
        /// <summary>
        /// 垂直滚动条
        /// </summary>
        protected UIVerticalScrollBar verticalScrollBar;
        /// <summary>
        ///		true if vertical scrollbar should always be displayed
        /// </summary>
        [XmlFieldSerializable("forceVerticalScrollBar")]
        protected bool forceVerticalScrollBar;
        /// <summary>
        /// 水平滚动条
        /// </summary>
        protected UIHorizontalScrollBar horizontalScrollBar;
        /// <summary>
        ///		true if horizontal scrollbar should always be displayed
        /// </summary>
        [XmlFieldSerializable("forceHorizontalScrollBar")]
        protected bool forceHorizontalScrollBar;

        /// <summary>
        ///		Allow multiple selections.
        /// </summary>
        [XmlFieldSerializable("multiSelect")]
        protected bool multiSelect;

        [XmlFieldSerializable("backgroundRenderComp")]
        protected UIComp_DrawTextureAtlas backgroundRenderComp;
        /// <summary>
        /// 渲染选中光标
        /// </summary>
        [XmlFieldSerializable("stickRenderComp")]
        protected UIComp_DrawTextureAtlas stickRenderComp;

        /// <summary>
        /// 树控件提供的资源图片类型，作为resourceImages的下标索引
        /// </summary>
        public enum ResourceImageType : int
        {
            /// <summary>
            /// 折叠图标
            /// </summary>
            CollapseIcon,
            /// <summary>
            /// 展开图标
            /// </summary>
            ExpandIcon,
        };
        /// <summary>
        /// 提供的资源图片
        /// </summary>
        [XmlFieldSerializable("resourceImages")]
        protected DrawTextureAtlasCollection resourceImages;

        /// <summary>
        /// 根节点
        /// </summary>
        [XmlFieldSerializable("rootNode")]
        protected UIComp_TreeNodeRoot rootNode;

        /// <summary>
        /// 禁用任何树视图重绘，避免一次加入、删除、修改多个树节点时多次更新树视图。
        /// </summary>
        protected bool diableUpdateLayout;

        #endregion Fields

        #region Constructor
        /// <summary>
        /// default ctor not valid - we want to enforce initializing our data
        /// </summary>
        public UITreeView() 
        {
        } 
        public UITreeView(string name)
            : base(name)
        {
            // 创建缺省的根节点
            this.rootNode = new UIComp_TreeNodeRoot(this);
            this.rootNode.EndUpdated += new EventHandler<TreeNodeEventArg>(rootNode_LayoutChanged);

            this.backgroundRenderComp = new UIComp_DrawTextureAtlas(this);

            this.backgroundRenderComp.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            this.backgroundRenderComp.AtlasItemName = "0";

            this.stickRenderComp = new UIComp_DrawTextureAtlas(this);
            this.stickRenderComp.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            this.stickRenderComp.AtlasItemName = @"N_sys-gb-a";

            this.resourceImages = new DrawTextureAtlasCollection();
            this.resourceImages.Add(new UIComp_DrawTextureAtlas(this));
            this.resourceImages.Add(new UIComp_DrawTextureAtlas(this));
            this.resourceImages[(int)ResourceImageType.CollapseIcon].ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            this.resourceImages[(int)ResourceImageType.CollapseIcon].AtlasItemName = @"N_help-button-d";
            this.resourceImages[(int)ResourceImageType.ExpandIcon].ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            this.resourceImages[(int)ResourceImageType.ExpandIcon].AtlasItemName = @"N_help-button-c";
            this.resourceImages[(int)ResourceImageType.CollapseIcon].Width = new UIDim(0,9);
            this.resourceImages[(int)ResourceImageType.CollapseIcon].Height = new UIDim(0, 9);
            this.resourceImages[(int)ResourceImageType.ExpandIcon].Width = new UIDim(0, 9);
            this.resourceImages[(int)ResourceImageType.ExpandIcon].Height = new UIDim(0, 9);


            // 创建子控件
            this.verticalScrollBar = CreateVerticalScrollBar();
            this.horizontalScrollBar = CreateHorizontalScrollBar();

            InitEventHandler();
      }

        #endregion Constructor

        #region Properties
        /// <summary>
        /// 总是显示垂直滚动条
        /// </summary>
        [Category("Data")]
        public bool AlwaysShowVerticalScrollBar
        {
            get
            {
                return forceVerticalScrollBar;
            }

            set
            {
                if (forceVerticalScrollBar != value)
                {
                    forceVerticalScrollBar = value;

                    ConfigureScrollBars();
                }
            }
        }

        /// <summary>
        /// 总是显示水平滚动条
        /// </summary>
        [Category("Data")]
        public bool AlwaysShowHorizontalScrollBar
        {
            get
            {
                return forceHorizontalScrollBar;
            }

            set
            {
                if (forceHorizontalScrollBar != value)
                {
                    forceHorizontalScrollBar = value;

                    ConfigureScrollBars();
                }
            }
        }

        [TypeConverter(typeof(DrawTextureAtlasCollectionConverter))]
        public DrawTextureAtlasCollection ResourceImages
        {
            get { return resourceImages; }
        }

        [TypeConverter(typeof(TreeNodeCollectionConverter))]
        public UIComp_TreeNodeRoot RootNode
        {
            get { return rootNode; }
        }
        /// <summary>
        /// Get the pixel extent of all items
        /// </summary>
        [Category("Data")]
        public Vector2 TotalPixelSize
        {
            get
            {
                return rootNode.TotalPixelSize;
            }
        }

        [Browsable(false)]
        public float IndentationOffest
        {
            get { return -horizontalScrollBar.ScrollPosition; }
        }

        [Category("Action")]
        public bool MultiSelect
        {
            get { return multiSelect; }
            set { multiSelect = value; }
        }

        [Category("Component")]
        [Description("Display some image.")]
        public UIComp_DrawTextureAtlas BackgroundRenderComp
        {
            get { return backgroundRenderComp; }
        }


        [Category("Component")]
        [Description("Display stick image.")]
        public UIComp_DrawTextureAtlas StickRenderComp
        {
            get { return stickRenderComp; }
        }
        #endregion Properties


        #region Virtual Methods

        protected virtual UIHorizontalScrollBar CreateHorizontalScrollBar()
        {
            UIHorizontalScrollBar hsb = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIHorizontalScrollBar", name + "_HSB", this) as UIHorizontalScrollBar;

            hsb.Height = new UIDim(0, 28);
            hsb.Width = new UIDim(1.0f, 0);
            hsb.Y = new UIDim(1, 0);

            return hsb;
        }

        protected virtual UIVerticalScrollBar CreateVerticalScrollBar()
        {
            UIVerticalScrollBar vsb = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIVerticalScrollBar", name + "_VSB", this) as UIVerticalScrollBar;

            vsb.Width = new UIDim(0, 28);
            vsb.Height = new UIDim(1.0f, 0);
            vsb.X = new UIDim(1, 0);

            return vsb;
        }

        protected override void PostDrawSelf()
        {
            base.PostDrawSelf();
            GUISystem.Instance.Renderer.PopScissorRect();
        }

        protected override void DrawSelf()
        {
            // 渲染背景图片
            backgroundRenderComp.Draw();

            // 设置裁剪区域
            GUISystem.Instance.Renderer.PushScissorRect(this.Rect);

            // 渲染子节点
            rootNode.DrawTreeNode();
        }

        /// <summary>
        ///		display required integrated scroll bars according to current state of the list box and update their values.
        /// </summary>
        protected void ConfigureScrollBars()
        {
            Vector2 totalSize = TotalPixelSize;

            //
            // First show or hide the scroll bars as needed (or requested)
            //
            // show or hide vertical scroll bar as required (or as specified by option)
            Rect ListRenderArea = this.Rect;
            if ((totalSize.y > ListRenderArea.Height) || forceVerticalScrollBar)
            {
                verticalScrollBar.Show();

                // show or hide horizontal scroll bar as required (or as specified by option)
                if ((totalSize.x > ListRenderArea.Width) || forceHorizontalScrollBar)
                {
                    horizontalScrollBar.Show();
                }
                else
                {
                    horizontalScrollBar.Hide();
                }
            }
            else
            {
                // show or hide horizontal scroll bar as required (or as specified by option)
                if ((totalSize.x > ListRenderArea.Width) || forceHorizontalScrollBar)
                {
                    horizontalScrollBar.Show();

                    // show or hide vertical scroll bar as required (or as specified by option)
                    if ((totalSize.y > ListRenderArea.Height) || forceVerticalScrollBar)
                    {
                        verticalScrollBar.Show();
                    }
                    else
                    {
                        verticalScrollBar.Hide();
                    }
                }
                else
                {
                    verticalScrollBar.Hide();
                    horizontalScrollBar.Hide();
                }
            }

            //
            // Set up scroll bar values
            //
            Rect renderArea = ListRenderArea;

            verticalScrollBar.DocumentSize = totalSize.y;
            verticalScrollBar.PageSize = renderArea.Height;
            verticalScrollBar.StepSize = Math.Max(1.0f, verticalScrollBar.PageSize * 0.1f);
            verticalScrollBar.ScrollPosition = verticalScrollBar.ScrollPosition;

            horizontalScrollBar.DocumentSize = totalSize.x;
            horizontalScrollBar.PageSize = renderArea.Width;
            horizontalScrollBar.StepSize = Math.Max(1.0f, horizontalScrollBar.PageSize * 0.1f);
            horizontalScrollBar.ScrollPosition = horizontalScrollBar.ScrollPosition;
        }

        /// <summary>
        /// 重排控件布局
        /// </summary>
        protected void LayoutComponentWidgets()
        {
            Vector2 offest = new Vector2(-horizontalScrollBar.ScrollPosition, -verticalScrollBar.ScrollPosition);
            rootNode.LayoutComponents(ref offest);

            RequestRedraw();
        }

        protected internal override void OnMouseWheel(GUIMouseEventArgs e)
        {
            // base class processing
            base.OnMouseWheel(e);

            if (verticalScrollBar.Visible && (verticalScrollBar.DocumentSize > verticalScrollBar.PageSize))
            {
                verticalScrollBar.ScrollPosition = verticalScrollBar.ScrollPosition + verticalScrollBar.StepSize * (e.Delta > 0 ? -1 : 1);
            }
            else if (horizontalScrollBar.Visible && (horizontalScrollBar.DocumentSize > horizontalScrollBar.PageSize))
            {
                horizontalScrollBar.ScrollPosition = horizontalScrollBar.ScrollPosition + horizontalScrollBar.StepSize * (e.Delta > 0 ? -1 : 1);
            }
        }

        protected internal override void OnSized(GUIEventArgs e)
        {
            base.OnSized(e);

            ConfigureScrollBars();
            //LayoutComponentWidgets();
        }

        #endregion Methods

        #region Public Methods
        /// <summary>
        /// 清空所有子节点
        /// </summary>
        public void ClearTreeNodes()
        {
            rootNode.ClearTreeNodes();
        }

        /// <summary>
        /// 往指定的父节点中追加一个缺省的子节点
        /// </summary>
        /// <param name="parent">parent 要追加子节点的节点</param>
        public UIComp_TreeNodeBase AppendTreeNode(UIComp_TreeNodeBase parent)
        {
            if (parent == null)
            {
                return rootNode.AppendTreeNode();
            }
            else
            {
                return parent.AppendTreeNode();
            }
        }

        /// <summary>
        /// 插入一个新的项
        /// </summary>
        public UIComp_TreeNodeBase InsertTreeNode(int position)
        {
            return rootNode.InsertTreeNode(position);
        }

        /// <summary>
        /// 插入一个新的元素
        /// </summary>
        public UIComp_TreeNodeBase InsertTreeNode(int position, UIComp_TreeNodeBase node, UIComp_TreeNodeBase parent)
        {
            if (parent == null)
            {
                return rootNode.InsertTreeNode(position, node);
            }
            else
            {
                return parent.InsertTreeNode(position, node);
            }
        }

        /// <summary>
        /// 移除特定对象的第一个匹配项。
        /// </summary>
        /// <returns>如果成功移除 node，则为 true；否则为 false。</returns>
        public bool RemoveTreeNode(UIComp_TreeNodeBase node)
        {
            if (node == rootNode)
            {
                return false;
            }
            return rootNode.RemoveTreeNode(node);
        }

        /// <summary>
        /// 移除指定位置的元素
        /// </summary>
        public bool RemoveTreeNode(int position, UIComp_TreeNodeBase parent)
        {
            return parent.RemoveTreeNode(position);
        }

        /// <summary>
        /// 取对应位置的元素
        /// </summary>
        public UIComp_TreeNodeBase GetTreeNode(UIComp_TreeNodeBase parent, int position)
        {
            if (position < 0 || position >= parent.NodeCount)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "Invalid index {0} of treeNodes", position));
                return null;
            }
            return parent.GetTreeNode(position);
        }

        /// <summary>
        /// 禁用任何树视图重绘。
        /// </summary>
        public virtual void BeginUpdate()
        {
            diableUpdateLayout = true;
        }

        /// <summary>
        /// 启用树视图的重绘。
        /// </summary>
        public virtual void EndUpdate()
        {
            diableUpdateLayout = false;

            LayoutComponentWidgets();
            ConfigureScrollBars();
        }

        /// <summary>
        /// 取消所有选中项
        /// </summary>
        public void ClearAllSelections()
        {
            BeginUpdate();
            rootNode.ClearAllSelections();
            EndUpdate();
        }

        /// <summary>
        /// 折叠所有树节点。
        /// </summary>
        public void CollapseAll()
        {
            BeginUpdate();
            rootNode.CollapseOrExpandAll(false);
            EndUpdate();
        }

        /// <summary>
        /// 展开所有树节点。
        /// </summary>
        public void ExpandAll()
        {
            BeginUpdate();
            rootNode.CollapseOrExpandAll(true);
            EndUpdate();
        }

        public UIComp_TreeNodeBase GetNodeAtPoint(Vector2 point)
        {
            return rootNode.GetNodeAtPoint(point);
        }
        #endregion

        #region override input event

        protected internal override bool OnMouseDoubleClicked(GUIMouseEventArgs e)
        {
            base.OnMouseDoubleClicked(e);

            ClearAllSelections();

            // get mouse position as a local pixel value
            UIComp_TreeNodeBase node = GetNodeAtPoint(e.Position);
            if(node!=null)
            {
                BeginUpdate();
                node.Selected = true;
                node.Expand = !node.Expand;
                EndUpdate();
            }

            return true;
        }

        protected internal override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            base.OnMouseButtonDown(e);

            if(e.Button == GUISystem.ClickButton
                && !rootNode.OnMouseButtonDown(e) )
            {
                // clear old selections if no control key is pressed or if not multi-select
                if (!(GUISystem.Instance.Modifiers == System.Windows.Forms.Keys.Control) || !multiSelect)
                {
                    ClearAllSelections();
                }

                // get mouse position as a local pixel value
                UIComp_TreeNodeBase node = GetNodeAtPoint(e.Position);
                if (node != null)
                {
                    node.Selected = true;
                }
            }

            return true;
        }

        #endregion

        #region delegate Methods
        private void InitEventHandler()
        {
            this.verticalScrollBar.ScrollPositionChanged += new EventHandler<WindowEventArgs>(verticalScrollBar_PositionChanged);
            this.horizontalScrollBar.ScrollPositionChanged += new EventHandler<WindowEventArgs>(horizontalScrollBar_PositionChanged);
        }

        private void rootNode_LayoutChanged(object sender, TreeNodeEventArg e)
        {
            if (!diableUpdateLayout)
            {
                EndUpdate();
            }
        }

        private void verticalScrollBar_PositionChanged(object sender, WindowEventArgs e)
        {
            LayoutComponentWidgets();
        }
        private void horizontalScrollBar_PositionChanged(object sender, WindowEventArgs e)
        {
            LayoutComponentWidgets();
        }
        #endregion

        #region Serialization
        protected override void OnStringDictionaryChanged()
        {
            this.rootNode.OnStringDictionaryChanged();
        }

        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            this.backgroundRenderComp.PostCustomSerial(this);
            this.stickRenderComp.PostCustomSerial(this);

            foreach (var item in resourceImages)
            {
                item.PostCustomSerial(this);
            }

            this.rootNode.PostCustomSerial(this);
            this.rootNode.EndUpdated += new EventHandler<TreeNodeEventArg>(rootNode_LayoutChanged);
            InitEventHandler();

            LayoutComponentWidgets();
        }

        [Browsable(false)]
        [XmlFieldSerializable("VerticalScrollBarHelper")]
        private string VerticalScrollBarHelper
        {
            get
            {
                return this.verticalScrollBar.Name;
            }
            set
            {
                this.verticalScrollBar = Children[value] as UIVerticalScrollBar;
            }
        }

        [Browsable(false)]
        [XmlFieldSerializable("HorizontalScrollBarHelper")]
        private string HorizontalScrollBarHelper
        {
            get
            {
                return this.horizontalScrollBar.Name;
            }
            set
            {
                this.horizontalScrollBar = Children[value] as UIHorizontalScrollBar;
            }
        }
        #endregion
    }
}
