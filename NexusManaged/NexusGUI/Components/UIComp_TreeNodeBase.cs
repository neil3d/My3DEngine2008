using System;
using System.Collections.Generic;
using System.ComponentModel;
using NexusEngine;
using System.Security.Permissions;
using System.Runtime.Serialization;
using NexusEngineExtension;

namespace Nexus.GUI.Components
{
    /// <summary>
    /// 树节点，用于组合TreeView
    /// </summary>
    [XmlClassSerializable("UIComp_TreeNodeBase", true)]
    abstract public class UIComp_TreeNodeBase : UIComponent
    {
        #region event-delegate
        /// <summary>
        /// 子节点发生变化
        /// </summary>
        public EventHandler<TreeNodeEventArg> TreeNodesChanged;
        public EventHandler<TreeNodeEventArg> TextChanged;
        public EventHandler<TreeNodeEventArg> IndentationChanged;
        public EventHandler<TreeNodeEventArg> SelectChanged;
        public EventHandler<TreeNodeEventArg> EndUpdated;

        public EventHandler<TreeNodeEventArg> AfterCollapse;
        public EventHandler<TreeNodeEventArg> AfterExpand;
        #endregion

        #region Fields
        /// <summary>
        /// 父节点
        /// </summary>
        protected UIComp_TreeNodeBase parentNode;
        /// <summary>
        /// 子节点容器
        /// </summary>
        [XmlFieldSerializable("treeNodes")]
        protected TreeNodeCollection treeNodes = new TreeNodeCollection();

        /// <summary>
        /// 缩进
        /// </summary>
        [XmlFieldSerializable("indentation")]
        protected float indentation;

        /// <summary>
        /// 应用程序标记
        /// </summary>
        protected object tagObj;
        /// <summary>
        /// 是否被选中
        /// </summary>
        protected bool selected;

        /// <summary>
        /// 禁用任何树视图重绘。
        /// </summary>
        protected bool diableUpdateLayout;
        #endregion

        protected UIComp_TreeNodeBase() { } //default ctor not valid - we want to enforce initializing our data

        protected UIComp_TreeNodeBase(Window owner)
            : base(owner)
        {
            this.parentNode = null;
        }

        protected UIComp_TreeNodeBase(Window owner, UIComp_TreeNodeBase parent)
            : base(owner)
        {
            this.parentNode = parent;
        }

        [Browsable(false)]
        public UIComp_TreeNodeBase Parent
        {
            get { return parentNode; }
        }

        [Category("Data")]
        public int NodeCount
        {
            get { return treeNodes.Count; }
        }

        [TypeConverter(typeof(TreeNodeCollectionConverter))]
        public TreeNodeCollection TreeNodes
        {
            get { return treeNodes; }
        }

        [Category("Data")]
        public float Indentation
        {
            get { return indentation; }
            set 
            {
                if (indentation!=value)
                {
                    indentation = value;

                    OnIndentationChanged(new TreeNodeEventArg(this));
                }
            }
        }

        /// <summary>
        /// 是否被选中
        /// </summary>
        public bool Selected
        {
            get { return selected; }
            set 
            {
                if(selected != value)
                {
                   selected = value;
                   OnSelectChanged(new TreeNodeEventArg(this));
                }
            }
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

        #region abstract porperties
        [Category("Component")]
        abstract public string Text
        {
            get;
            set;
        }

        /// <summary>
        /// 像素大小
        /// </summary>
        [Category("Data")]
        abstract public Vector2 PixelSize
        {
            get;
        }

        /// <summary>
        /// 总像素大小
        /// </summary>
        [Category("Data")]
        abstract public Vector2 TotalPixelSize
        {
            get;
        }

        /// <summary>
        /// 缩进量，该节点的子节点最终偏移量为(IndentationOffest + Indentation)
        /// </summary>
        [Category("Data")]
        abstract public float IndentationOffest
        {
            get;
        }

        /// <summary>
        /// 缩进量，不计算滚动条的影响
        /// </summary>
        abstract public float IndentationSum
        {
            get;
        }

        /// <summary>
        /// 展开 / 折叠
        /// </summary>
        abstract public bool Expand
        {
            get;
            set;
        }
        #endregion

        /// <summary>
        /// 重新布局组件
        /// </summary>
        abstract public void LayoutComponents(ref Vector2 offest);

        /// <summary>
        /// 创建缺省指定的树节点
        /// </summary>
        abstract public UIComp_TreeNodeBase CreateDefaultTreeNode();

        /// <summary>
        /// 取消所有选中项
        /// </summary>
        abstract public void ClearAllSelections();

        /// <summary>
        /// 折叠或者展开所有的节点
        /// </summary>
        /// <param name="expand">true:展开 false:折叠</param>
        abstract public void CollapseOrExpandAll(bool expand);
        /// <summary>
        /// 返回给定像素点下的节点
        /// </summary>
        abstract public UIComp_TreeNodeBase GetNodeAtPoint(Vector2 point);

        /// <summary>
        /// 处理鼠标输入
        /// </summary>
        abstract public bool OnMouseButtonDown(GUIMouseEventArgs e);

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

            if(parentNode!=null)
            {
                parentNode.EndUpdate();
            }

            OnEndUpdated(new TreeNodeEventArg(this));
        }

        /// <summary>
        /// 移除指定位置的元素
        /// </summary>
        public bool RemoveTreeNode(int position)
        {
            return RemoveTreeNode(GetTreeNode(position));
        }
        /// <summary>
        /// 移除特定对象的第一个匹配项。
        /// </summary>
        /// <returns>如果成功移除 node，则为 true；否则为 false。</returns>
        public bool RemoveTreeNode(UIComp_TreeNodeBase node)
        {
            if(node==null)
            {
                return false;
            }

            bool ret = treeNodes.Remove(node);
            if (!ret)
            {
                // 迭代所有子节点
                foreach (UIComp_TreeNodeBase child in treeNodes )
                {
                    ret = child.RemoveTreeNode(node);
                    if(ret)
                        break;
                }
            }
            if(ret)
            {
                OnTreeNodesChanged(new TreeNodeEventArg(this));
            }
            return ret;
        }

        /// <summary>
        /// 清空所有子项
        /// </summary>
        public void ClearTreeNodes()
        {
            foreach (UIComp_TreeNodeBase child in treeNodes )
            {
                child.ClearTreeNodes();
            }
            treeNodes.Clear();

            OnTreeNodesChanged(new TreeNodeEventArg(this));
        }


        /// <summary>
        /// 插入一个新的元素
        /// </summary>
        public UIComp_TreeNodeBase InsertTreeNode(int position, UIComp_TreeNodeBase item)
        {
            if (item == null)
            {
                throw new InvalidRequestException("Invalid insert item");
            }

            if (position < 0 || position >= treeNodes.Count)
            {
                treeNodes.Add(item);
            }
            else
            {
                treeNodes.Insert(position, item);
            }

            OnTreeNodesChanged(new TreeNodeEventArg(item));

            return item;
        }

        /// <summary>
        /// 插入一个新的项
        /// </summary>
        public UIComp_TreeNodeBase InsertTreeNode(int position)
        {
            return InsertTreeNode(position, CreateDefaultTreeNode());
        }

        /// <summary>
        ///  追加一项
        /// </summary>
        public UIComp_TreeNodeBase AppendTreeNode()
        {
            return InsertTreeNode(treeNodes.Count, CreateDefaultTreeNode());
        }

        /// <summary>
        /// 取对应位置的元素
        /// </summary>
        public UIComp_TreeNodeBase GetTreeNode(int position)
        {
            if (position < 0 || position >= treeNodes.Count)
            {
                NLogger.Instance.WriteString(LogType.Info, string.Format(System.Globalization.CultureInfo.CurrentCulture, "Invalid index {0} of treeItems", position));
                return null;
            }
            return treeNodes[position];
        }

        protected virtual void OnEndUpdated(TreeNodeEventArg e)
        {
            if(EndUpdated!=null)
            {
                EndUpdated(owner, e);
            }
        }

        protected virtual void OnSelectChanged(TreeNodeEventArg e)
        {
            if( SelectChanged!=null)
            {
                SelectChanged(owner, e);
            }
        }

        protected virtual void OnAfterCollapse(TreeNodeEventArg e)
        {
            if (AfterCollapse != null)
            {
                AfterCollapse(this, e);
            }
            if (!diableUpdateLayout)
            {
                EndUpdate();
            }
        }

        protected virtual void OnAfterExpand(TreeNodeEventArg e)
        {
            if (AfterExpand != null)
            {
                AfterExpand(this, e);
            }
            if (!diableUpdateLayout)
            {
                EndUpdate();
            }
        }

        /// <summary>
        /// 缩进发生变化
        /// </summary>
        protected virtual void OnIndentationChanged(TreeNodeEventArg e)
        {
            if (IndentationChanged != null)
            {
                IndentationChanged(owner, e);
            }
            if (!diableUpdateLayout)
            {
                EndUpdate();
            }
        }

        protected virtual void OnTreeNodesChanged(TreeNodeEventArg e)
        {
            if (TreeNodesChanged != null)
            {
                TreeNodesChanged(owner, e);
            }

            if(!diableUpdateLayout)
            {
                EndUpdate();
            }
        }

        protected virtual void OnTextChanged(TreeNodeEventArg e)
        {
            if (TextChanged != null)
            {
                TextChanged(owner, e);
            }
            if (!diableUpdateLayout)
            {
                EndUpdate();
            }
        }

        protected override void OnOwnerChanged(GUIEventArgs e)
        {
            base.OnOwnerChanged(e);

            foreach (var item in treeNodes)
            {
                item.Owner = this.Owner;
            }
        }

        /// <summary>
        /// 渲染节点
        /// </summary>
        public virtual void DrawTreeNode(){}


        #region Serialization

        internal override void PostCustomSerial(Window _owner)
        {
            base.PostCustomSerial(_owner);

            foreach (var node in this.treeNodes)
            {
                node.parentNode = this;

                node.PostCustomSerial(this.Owner);
            }
        }


        internal virtual void OnStringDictionaryChanged()
        {
            foreach (var node in this.treeNodes)
            {
                node.OnStringDictionaryChanged();
            }
        }
        #endregion Serialization

    }
}
