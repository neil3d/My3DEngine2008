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
        public ActorListDlg()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 创建Actor列表
        /// </summary>
        /// <param name="lv">需要显示的Level</param>
        public void ShowLevel(NLevel lv)
        {
            NActor edAct = NLevelEditorEngine.Instance.ActorEd.SelectedActor;
            this.listViewActors.BeginUpdate();
            this.listViewActors.Items.Clear();

            int i = 0;
            NActor act = lv.FirstActor();
            while (act!=null)
            {
                ListViewItem listItem = new ListViewItem(act.Name);
                listItem.Tag = act;
                this.listViewActors.Items.Add(listItem);

                if (edAct == act)
                    this.listViewActors.SelectedIndices.Add(i);
                
                act = lv.NextActor();
                i++;
            }

            this.listViewActors.EndUpdate();
        }

        /// <summary>
        /// 点击Cancel按钮, 隐藏窗口
        /// </summary>        
        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void btnSelect_Click(object sender, EventArgs e)
        {            
        }

        /// <summary>
        /// 鼠标双击列表,则在关卡编辑器中选中该Actor
        /// </summary>        
        private void listViewActors_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (1 == this.listViewActors.SelectedItems.Count)
            {
                ListViewItem selItem = this.listViewActors.SelectedItems[0];
                NActor ac = selItem.Tag as NActor;
                NLevelEditorEngine.Instance.ActorEd.BindActor(ac);
                this.Hide();
            }            
        }
    }
}
