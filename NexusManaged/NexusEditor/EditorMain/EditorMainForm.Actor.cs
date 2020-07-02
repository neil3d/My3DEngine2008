using System;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor
{
    /// <summary>
    /// 实现主窗口中Actor Tab Page相关的功能
    /// </summary>
    partial class EditorMainForm
    {
        private EditorMain.ActorListDlg m_actorList;
        
        /// <summary>
        /// 初始化, 绑定一些事件
        /// </summary>
        private void ActorPageLoad()
        {
            m_actorList = new EditorMain.ActorListDlg();

            NLevelEditorEngine.Instance.ActorEd.ActorSelected
                += new EventHandler(this.ActorSelectedHandler);
            NLevelEditorEngine.Instance.SelectionLockTag 
                += new EventHandler(this.EditorSelectionLock);

            this.treeViewActor.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeViewActor_AfterSelect);
            this.toolBtnActorList.Click += new System.EventHandler(this.toolBtnActorList_Click);
            this.propertyGridActor.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGridActor_PropertyValueChanged);
        }

        /// <summary>
        /// 响应引擎的Selection Lock事件,更新Status Bar上的小图标
        /// </summary>        
        private void EditorSelectionLock(object sender, EventArgs e)
        {
            if (NLevelEditorEngine.Instance.SelectionLocked)
                this.statusSelectionLock.Image = Properties.Resources.locked;
            else
                this.statusSelectionLock.Image = Properties.Resources.unlock;
        }

        /// <summary>
        /// 响应引擎的"选中Actor"事件, 更新Actor的树控件和属性控件
        /// </summary>        
        private void ActorSelectedHandler(object sender, EventArgs e)
        {
            ActorSelectedArgs ae = e as ActorSelectedArgs;
            NActor selActor = ae.SelectedActor;

            this.treeViewActor.BeginUpdate();

            //-- clear old            
            this.treeViewActor.Nodes.Clear();
            this.propertyGridActor.SelectedObject = null;

            if (selActor == null)
            {
                TreeNode actorNode = new TreeNode("None");
                this.treeViewActor.Nodes.Add(actorNode);
                this.treeViewActor.SelectedNode = actorNode;
            }
            else
            {
                TreeNode actorNode = new TreeNode(selActor.Name);
                actorNode.Tag = selActor;
                this.treeViewActor.Nodes.Add(actorNode);

                for (int i = 0; i < selActor.NumComponents; i++)
                {
                    NActorComponent comp = selActor.GetComponent(i);

                    if (comp.Editable)
                    {
                        TreeNode compNode = new TreeNode(comp.Name);
                        compNode.Tag = comp;
                        actorNode.Nodes.Add(compNode);
                    }
                }

                this.treeViewActor.SelectedNode = actorNode;
                this.treeViewActor.ExpandAll();

                //--
                this.propertyGridActor.SelectedObject = selActor;
            }


            //--
            this.treeViewActor.EndUpdate();
        }

        /// <summary>
        /// 在Actor Components树上选择一个Node,则显示对应的对象属性
        /// </summary>        
        private void treeViewActor_AfterSelect(object sender, TreeViewEventArgs e)
        {
            this.propertyGridActor.SelectedObject = e.Node.Tag;
        }

        /// <summary>
        /// 显示关卡中所有Actor的列表窗体
        /// </summary>        
        private void toolBtnActorList_Click(object sender, EventArgs e)
        {
            m_actorList.ShowLevel(NLevelEditorEngine.Instance.MainLevel);
            m_actorList.Show(this);
        }

        /// <summary>
        /// Actor/Actor Component属性编辑之后, 刷新窗口显示
        /// </summary>        
        private void propertyGridActor_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            foreach (NEViewport vp in m_viewports)
            {
                vp.Refresh();
            }            
        }
    }
}