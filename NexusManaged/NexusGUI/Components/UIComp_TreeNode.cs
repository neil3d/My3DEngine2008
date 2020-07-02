using System;
using System.Collections.Generic;
using System.Text;
using NexusEngine;
using System.ComponentModel;
using Nexus.GUI.Widgets;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    [XmlClassSerializable("UIComp_TreeNode", true)]
    public class UIComp_TreeNode : UIComp_TreeNodeBase
    {
        internal const float DefaultTreeNodeIconHeight = 15.0f;
        internal const float DefaultTreeNodeTextHeight = 24.0f;
        internal const float DefaultIndentation = 14;

        /// <summary>
        /// 树节点的图标
        /// </summary>
        [XmlFieldSerializable("nodeIcon")]
        protected UIComp_DrawTextureAtlas nodeIcon;
        /// <summary>
        /// 树节点文字显示
        /// </summary>
        [XmlFieldSerializable("nodeText")]
        protected UIComp_DrawText nodeText;
        /// <summary>
        /// 展开 / 折叠
        /// </summary>
        [XmlFieldSerializable("expand")]
        protected bool expand;

        /// <summary>
        /// 展开 / 折叠 图标区域
        /// </summary>
        [XmlFieldSerializable("expandIconVisiable")]
        protected bool expandIconVisiable;

        protected Rect expandIconRect;
        UIComp_DrawTextureAtlas collapseComp;
        UIComp_DrawTextureAtlas expandComp;
        UIComp_DrawTextureAtlas currentExpandComp;
        /// <summary>
        /// 选择光标
        /// </summary>
        UIComp_DrawTextureAtlas stickComp;

        public UIComp_TreeNode() { } //default ctor not valid - we want to enforce initializing our data

        protected UIComp_TreeNode(Window owner)
            : base(owner)
        {
        }

        public UIComp_TreeNode(Window owner, UIComp_TreeNodeBase parent)
            : base(owner)
        {
            this.parentNode = parent;
            this.expand = true;
            this.indentation = DefaultIndentation;

            this.nodeIcon = new UIComp_DrawTextureAtlas(owner);
            this.nodeText = new UIComp_DrawText(owner);

            this.nodeIcon.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            this.nodeIcon.AtlasItemName = "0";
            this.nodeIcon.Height = new UIDim(0, DefaultTreeNodeIconHeight);
            this.nodeIcon.Width = new UIDim(0, DefaultTreeNodeIconHeight);

            this.nodeText.TextFormat = DrawTextFormat.Left | DrawTextFormat.VerticalCenter | DrawTextFormat.NoClip;
            this.nodeText.Height = new UIDim(0, DefaultTreeNodeTextHeight);
            this.nodeText.Width = new UIDim(0, DefaultTreeNodeTextHeight);
            this.nodeText.TextChanged += new EventHandler<GUIEventArgs>(nodeText_TextChanged);
            this.nodeText.Text = "Tree Item";

            CacheOwnerData();
        }

        [Category("Component")]
        public UIComp_DrawTextureAtlas IconComponent
        {
            get { return nodeIcon; }
        }

        [Category("Component")]
        public UIComp_DrawText TextComponent
        {
            get { return nodeText; }
        }

        [Category("Component")]
        public override string Text
        {
            get { return nodeText.Text; }
            set
            {
                nodeText.Text = value;

                OnTextChanged(new TreeNodeEventArg(this));
            }
        }
        /// <summary>
        /// 文本显示大小
        /// </summary>
        [Browsable(false)]
        public Rect TextRect
        {
            get { return this.nodeText.TargetRect; }
        }

        /// <summary>
        /// 像素大小
        /// </summary>
        [Browsable(false)]
        public override Vector2 PixelSize
        {
            get
            {
                return new Vector2((this.nodeIcon.TargetRect.Width + this.nodeText.TargetRect.Width),
                    Math.Max(this.nodeIcon.TargetRect.Height, this.nodeText.TargetRect.Height));
            }
        }

        /// <summary>
        /// 像素矩形范围
        /// </summary>
        [Browsable(false)]
        public Rect PixelRect
        {
            get
            {
                return new Rect(
                    Math.Min(this.nodeIcon.TargetRect.left, this.nodeText.TargetRect.left),
                    Math.Min(this.nodeIcon.TargetRect.top, this.nodeText.TargetRect.top),
                    Math.Max(this.nodeIcon.TargetRect.right, this.nodeText.TargetRect.right),
                    Math.Max(this.nodeIcon.TargetRect.bottom, this.nodeText.TargetRect.bottom));
            }
        }

        [Browsable(false)]
        public override Vector2 TotalPixelSize
        {
            get
            {
                Vector2 size = new Vector2(0, 0);

                Vector2 itemSize = this.PixelSize;
                size.x = Math.Max(size.x, itemSize.x + IndentationSum);
                size.y += itemSize.y;

                if (expand)
                {
                    for (int i = 0; i < NodeCount; ++i)
                    {
                        Vector2 childPixelSize = treeNodes[i].TotalPixelSize;
                        size.x = Math.Max(size.x, childPixelSize.x);
                        size.y += childPixelSize.y;
                    }
                }
                return size;
            }
        }

        [Browsable(false)]
        public override float IndentationOffest
        {
            get
            {
                float offest = 0.0f;
                if (parentNode != null)
                {
                    offest += parentNode.IndentationOffest + indentation;
                }
                return offest;
            }
        }

        [Browsable(false)]
        public override float IndentationSum
        {
            get
            {
                float offest = 0.0f;
                if (parentNode != null)
                {
                    offest += parentNode.IndentationSum + indentation;
                }
                return offest;
            }
        }

        [Category("Data")]
        public override bool Expand
        {
            get { return expand; }
            set
            {
                if (expand != value)
                {
                    expand = value;

                    if (expand)
                    {
                        this.currentExpandComp = this.collapseComp;
                        OnAfterExpand(new TreeNodeEventArg(this));
                    }
                    else
                    {
                        this.currentExpandComp = this.expandComp;
                        OnAfterCollapse(new TreeNodeEventArg(this));
                    }
                }
            }
        }

        public override void DrawTreeNode()
        {
            base.DrawTreeNode();

            // 渲染选中光标
            if (stickComp != null && this.selected)
            {
                stickComp.TargetRect = this.TextRect;
                stickComp.Draw();
            }

            // 画是否显示 折叠/展开 图标
            if (expandIconVisiable && this.currentExpandComp != null)
            {
                expandIconRect.left = this.nodeIcon.TargetRect.left - indentation;
                expandIconRect.top = this.nodeText.TargetRect.top + (this.nodeText.TargetRect.Height - expandComp.Height.offest) * 0.5f;
                expandIconRect.right = expandIconRect.left + expandComp.Width.offest;
                expandIconRect.bottom = expandIconRect.top + expandComp.Height.offest;
                this.currentExpandComp.TargetRect = expandIconRect;
                this.currentExpandComp.Draw();
            }

            nodeIcon.Draw();
            nodeText.Draw();

            // 渲染子节点
            if (expand)
            {
                foreach (UIComp_TreeNodeBase node in treeNodes)
                {
                    node.DrawTreeNode();
                }
            }
        }

        public override UIComp_TreeNodeBase CreateDefaultTreeNode()
        {
            return new UIComp_TreeNode(owner, this);
        }

        public override void LayoutComponents(ref Vector2 offest)
        {
            // 增加缩进
            float indentOffest = IndentationOffest;
            this.nodeText.X = new UIDim(0, indentOffest + this.nodeIcon.Width.offest + 2);
            this.nodeText.Y = new UIDim(0, offest.y);

            // 图标垂直居中对齐
            this.nodeIcon.X = new UIDim(0, indentOffest);
            this.nodeIcon.Y = new UIDim(0, offest.y + (this.nodeText.TargetRect.Height - this.nodeIcon.Height.offest) * 0.5f);


            // 增加Y方向上的偏移
            float maxH = Math.Max(this.nodeIcon.TargetRect.Height, this.nodeText.TargetRect.Height);
            offest.y += maxH;

            if (expand)
            {
                foreach (UIComp_TreeNodeBase item in treeNodes)
                {
                    if (item != null)
                    {
                        item.LayoutComponents(ref offest);
                    }
                }
            }

            expandIconVisiable = (treeNodes.Count > 0);
        }

        public override void ClearAllSelections()
        {
            this.Selected = false;
            foreach (UIComp_TreeNodeBase node in treeNodes)
            {
                node.ClearAllSelections();
            }
        }

        public override void CollapseOrExpandAll(bool expand)
        {
            this.Expand = Expand;
            foreach (UIComp_TreeNodeBase node in treeNodes)
            {
                node.CollapseOrExpandAll(expand);
            }
        }

        public virtual bool IsHit(Vector2 point)
        {
            return PixelRect.IsPointInRect(point);
        }

        public override UIComp_TreeNodeBase GetNodeAtPoint(Vector2 point)
        {
            if (IsHit(point))
            {
                return this;
            }

            // 迭代所有子项
            if (expand)
            {
                foreach (UIComp_TreeNodeBase node in treeNodes)
                {
                    UIComp_TreeNodeBase ret = node.GetNodeAtPoint(point);
                    if (ret != null)
                    {
                        return ret;
                    }
                }
            }
            return null;
        }

        public override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            if (e.Button == GUISystem.ClickButton
                && expandIconVisiable
                && expandIconRect.IsPointInRect(e.Position)
                )
            {
                this.Expand = !this.expand;
                return true;
            }
            else
            {
                foreach (UIComp_TreeNodeBase node in treeNodes)
                {
                    if (node.OnMouseButtonDown(e))
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        protected override void OnOwnerChanged(GUIEventArgs e)
        {
            base.OnOwnerChanged(e);

            CacheOwnerData();
            if (this.nodeText != null)
            {
                this.nodeText.Owner = owner;
            }
            if (this.nodeIcon != null)
            {
                this.nodeIcon.Owner = owner;
            }
        }

        private void nodeText_TextChanged(object sender, GUIEventArgs e)
        {
            Rect textRect = this.nodeText.Font.MeasureString(this.nodeText.Text, this.nodeText.TextFormat);
            this.nodeText.Width = new UIDim(0, textRect.Width);
        }

        private void CacheOwnerData()
        {
            UITreeView treeView = this.owner as UITreeView;
            if (treeView != null)
            {
                if (treeView.ResourceImages != null && treeView.ResourceImages.Count >= 2)
                {
                    this.expandComp = treeView.ResourceImages[(int)UITreeView.ResourceImageType.ExpandIcon];
                    this.collapseComp = treeView.ResourceImages[(int)UITreeView.ResourceImageType.CollapseIcon];
                    this.currentExpandComp = this.expand ? this.collapseComp : this.expandComp;
                }
                this.stickComp = treeView.StickRenderComp;
            }
        }

        #region Serialization

        internal override void PostCustomSerial(Window _owner)
        {
            base.PostCustomSerial(_owner);

            this.nodeText.PostCustomSerial(_owner);
            this.nodeIcon.PostCustomSerial(_owner);
        }

        internal override void OnStringDictionaryChanged()
        {
            this.nodeText.OnStringDictionaryChanged();
            base.OnStringDictionaryChanged();
        }
        #endregion

    }
}
