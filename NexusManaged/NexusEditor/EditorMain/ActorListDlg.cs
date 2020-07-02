using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.EditorMain
{
    /// <summary>
    /// 显示Level中所有的Actor, 用于实现Level Editor中的"按名称选择对象"
    /// </summary>
    public partial class ActorListDlg : Form
    {
        private NLevel m_Level;                     //保存的当前level指针
        private List<string> m_VisibleLayerList;    //过滤器中选中的Layer列表,虽然不是必须，但是多少可以提高一些效率

        //判断指定名字的层是否被过滤
        bool    IsLayerVisible(string layerName)
        {
            if(m_VisibleLayerList.IndexOf(layerName)==-1)
            {
                return false;
            }
            return true;
        }

        public ActorListDlg()
        {            
            m_VisibleLayerList=new List<string>();
            InitializeComponent();
        }

        /// <summary>
        /// 更新Actor列表
        /// </summary>
        /// <param name="lv">需要显示的Level</param>
        void UpdateActorList(NLevel lv)
        {
            NActor edAct = NLevelEditorEngine.Instance.ActorEd.SelectedActor;
            this.listViewActors.BeginUpdate();
            this.listViewActors.Items.Clear();

            int i = 0;
            string filterSearch = this.textBoxSearch.Text.Trim();
            bool needFilter = !string.IsNullOrEmpty(filterSearch);
            StringComparison comparison = this.checkBoxCase.Checked ? StringComparison.Ordinal : StringComparison.OrdinalIgnoreCase;
            NActor act = lv.FirstActor();
            while (act != null)
            {
                if (!IsLayerVisible(act.Layer))
                {
                    act = lv.NextActor();
                    continue;
                }

                if (needFilter
                    && act.Name.IndexOf(filterSearch, comparison) == -1)
                {
                    act = lv.NextActor();
                    continue;
                }

                ListViewItem listItem = new ListViewItem(act.Name);
                listItem.Tag = act;
                this.listViewActors.Items.Add(listItem);

                if (edAct == act)
                {
                    this.listViewActors.SelectedIndices.Add(i);
                }

                act = lv.NextActor();
                i++;
            }

            this.listViewActors.EndUpdate();
        }

        /// <summary>
        /// 更新分层列表
        /// </summary>
        /// <param name="lv">需要显示的Level</param>
        public void ShowLevel(NLevel lv)
        {
            m_Level = lv;
            m_VisibleLayerList.Clear();
            layerFitter.BeginUpdate();
            layerFitter.Items.Clear();
            for (int i = 0; i < lv.GetLayerCount(); ++i)
            {
                string text = lv.GetLayerName(i);
                int item=layerFitter.Items.Add(text);
                bool vis = lv.IsLayerVisible(text);
                layerFitter.SetItemChecked(item,vis);
            }
            layerFitter.EndUpdate();

            UpdateActorList(lv);
        }

        /// <summary>
        /// 点击Cancel按钮, 隐藏窗口
        /// </summary>
        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        /// <summary>
        /// 鼠标双击列表,则在关卡编辑器中选中该Actor
        /// </summary>        
        private void listViewActors_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            SelectedActor();
        }

        private void btnSelect_Click(object sender, EventArgs e)
        {
            SelectedActor();
        }

        private void SelectedActor()
        {
            if (1 == this.listViewActors.SelectedItems.Count)
            {
                ListViewItem selItem = this.listViewActors.SelectedItems[0];
                NActor ac = selItem.Tag as NActor;
                NLevelEditorEngine.Instance.ActorEd.BindActor(ac);
            }
        }

        private void ActorListDlg_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                this.Hide();
            }
        }

        private void layerFitter_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (m_Level == null || m_VisibleLayerList==null ) return;
            string name = m_Level.GetLayerName(e.Index);
            if(e.NewValue==CheckState.Checked)
            {
                if(m_VisibleLayerList.IndexOf(name)==-1)
                {
                    m_VisibleLayerList.Add(name);
                }
            }
            else
            {
                m_VisibleLayerList.Remove(name);
            }
            UpdateActorList(m_Level);
        }

        private void textBoxSearch_TextChanged(object sender, EventArgs e)
        {
            if (m_Level == null) return;
            UpdateActorList(m_Level);
        }

        private void checkBoxCase_CheckedChanged(object sender, EventArgs e)
        {
            if (m_Level == null) return;
            UpdateActorList(m_Level);
        }

    }
}
