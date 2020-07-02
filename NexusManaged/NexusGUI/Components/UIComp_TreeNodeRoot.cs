using System;
using System.Collections.Generic;
using NexusEngine;
using System.ComponentModel;
using Nexus.GUI.Widgets;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    [XmlClassSerializable("UIComp_TreeNodeRoot", true)]
    public class UIComp_TreeNodeRoot : UIComp_TreeNodeBase
    {
        public UIComp_TreeNodeRoot() { } //default ctor not valid - we want to enforce initializing our data

        public UIComp_TreeNodeRoot(Window owner)
            :base(owner)
        {
            this.indentation = 2;
        }

        [Category("Component")]
        public override string Text
        {
            get { return string.Empty; }
            set { }
        }

        /// <summary>
        /// 像素大小
        /// </summary>
        public override Vector2 PixelSize
        {
            get
            {
                return new Vector2(0,0);
            }
        }

        public override Vector2 TotalPixelSize
        {
            get
            {
                Vector2 size = new Vector2(0, 0);

                for (int i = 0; i < NodeCount; ++i)
                {
                    Vector2 childPixelSize = treeNodes[i].TotalPixelSize;
                    size.x = Math.Max(size.x, childPixelSize.x);
                    size.y += childPixelSize.y;
                }
                size.x += indentation * 2;
                return size;
            }
        }

        public override float IndentationOffest
        {
            get
            {
                UITreeView treeView = owner as UITreeView;
                if (treeView!=null)
                {
                    return treeView.IndentationOffest + indentation;
                }
                return indentation;
            }
        }
        public override float IndentationSum
        {
            get
            {
                return indentation;
            }
        }

        public override bool Expand
        {
            get { return true; }
            set { }
        }

        public override void DrawTreeNode()
        {
            base.DrawTreeNode();

            // 渲染子节点
            foreach (UIComp_TreeNodeBase node in treeNodes)
            {
                node.DrawTreeNode();
            }
        }

        public override UIComp_TreeNodeBase CreateDefaultTreeNode()
        {
            return new UIComp_TreeNode(owner, this);
        }

        public override void LayoutComponents(ref Vector2 offest)
        {
            // 增加缩进
            foreach (UIComp_TreeNodeBase node in treeNodes)
            {
                node.LayoutComponents(ref offest);
            }
        }

        public override void ClearAllSelections()
        {
            foreach (UIComp_TreeNodeBase node in treeNodes)
            {
                node.ClearAllSelections();
            }
        }

        public override void CollapseOrExpandAll(bool expand)
        {
            foreach (UIComp_TreeNodeBase node in treeNodes)
            {
                node.CollapseOrExpandAll(expand);
            }
        }

        public override UIComp_TreeNodeBase GetNodeAtPoint(Vector2 point)
        {
            foreach (UIComp_TreeNodeBase node in treeNodes)
            {
                UIComp_TreeNodeBase ret = node.GetNodeAtPoint(point);
                if(ret!=null)
                {
                    return ret;
                }
            }
            return null;
        }

        public override bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            foreach (UIComp_TreeNodeBase node in treeNodes)
            {
                if(node.OnMouseButtonDown(e))
                {
                    return true;
                }
            }
            return false;
        }

        #region Serialization
  
        #endregion Serialization

    }
}
