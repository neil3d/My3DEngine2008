using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EXControls;
using NexusEngine;

namespace NexusEditor.EditorMain
{
    public partial class LayerManagerDlg : Form
    {
        public LayerManagerDlg()
        {
            InitializeComponent();
            LayerList.SetItemHeight(24);
            EXColumnHeader layer_icon = new EXColumnHeader("", 36);
            LayerList.Columns.Add(layer_icon);
            LayerList.Columns.Add(new EXEditableColumnHeader("工作层", 300));
            EXBoolColumnHeader active_icon = new EXBoolColumnHeader("激活", 48);
            active_icon.Editable = true;
            active_icon.TrueImage = NexusEditor.Properties.Resources.active;
            active_icon.FalseImage = NexusEditor.Properties.Resources.disactive;
            LayerList.Columns.Add(active_icon);
            EXBoolColumnHeader vis_icon = new EXBoolColumnHeader("渲染", 48);
            vis_icon.Editable = true;
            vis_icon.TrueImage = NexusEditor.Properties.Resources.visible;
            vis_icon.FalseImage = NexusEditor.Properties.Resources.disactive;
            LayerList.Columns.Add(vis_icon);
            EXBoolColumnHeader lock_icon = new EXBoolColumnHeader("冻结", 48);
            lock_icon.Editable = true;
            lock_icon.TrueImage = NexusEditor.Properties.Resources.lock_layer;
            lock_icon.FalseImage = NexusEditor.Properties.Resources.disactive;
            LayerList.Columns.Add(lock_icon);
        }

        public void OnLevelChanged()
        {
            LayerList.BeginUpdate();
            LayerList.Clear();
            string active_layer = Program.engine.MainLevel.GetActiveLayer();
            for (int i = 0; i < Program.engine.MainLevel.GetLayerCount();++i )
            {
                string text = Program.engine.MainLevel.GetLayerName(i);
                EXListViewItem item = new EXImageListViewItem(NexusEditor.Properties.Resources.layer);
                EXListViewSubItem name = new EXListViewSubItem(text);
                name.OnItemValueChanged = this.OnLayerNameChanged; name.UserData = item;
                item.AddSubItem(name);
                EXBoolListViewSubItem active = new EXBoolListViewSubItem(active_layer==text);
                active.OnItemValueChanged = this.OnActiveLayer; active.UserData = item;
                item.AddSubItem(active);
                EXBoolListViewSubItem show = new EXBoolListViewSubItem(Program.engine.MainLevel.IsLayerVisible(text));
                show.OnItemValueChanged = this.OnShowLayer; show.UserData = item;
                item.AddSubItem(show);
                EXBoolListViewSubItem islock = new EXBoolListViewSubItem(Program.engine.MainLevel.IsLayerLocked(text));
                islock.OnItemValueChanged = this.OnLockLayer; islock.UserData = item;
                item.AddSubItem(islock);
                LayerList.AddItem(item);
            }
			LayerList.EndUpdate();
			//在右侧的列表中显示当前激活层中的所有actor
			UpdateActorList(Program.engine.MainLevel.GetActiveLayer());
			OperationTooltip.Text = "就绪";
        }

		private void UpdateActorList(string layerName)
		{
			NLevel lv = NLevelEditorEngine.Instance.MainLevel;
			NActor edAct = NLevelEditorEngine.Instance.ActorEd.SelectedActor;
			this.m_ActorList.BeginUpdate();
			this.m_ActorList.Items.Clear();

			int i = 0;
			NActor act = lv.FirstActor();
			while (act != null)
			{
				if (act.Layer != layerName)
				{
					act = lv.NextActor();
					continue;
				}
				ListViewItem listItem = new ListViewItem(act.Name);
				listItem.Tag = act;
				this.m_ActorList.Items.Add(listItem);

				if (edAct == act)
				{
					this.m_ActorList.SelectedIndices.Add(i);
				}

				act = lv.NextActor();
				++i;
			}

			this.m_ActorList.EndUpdate();
		}

        private void OnLayerNameChanged(EXListViewSubItemBase _item, string oldText, string newText)
        {
            if (oldText == newText) return;
            if(Program.engine.MainLevel.IsLayerExist(newText))
            {
                MessageBox.Show("作业层的名字不能重复，请重新设定.","错误");
                _item.ItemValue=oldText;
                return;
            }
            Program.engine.MainLevel.RenameLayer(oldText, newText);
			OperationTooltip.Text = "就绪";
        }

        private void OnActiveLayer(EXListViewSubItemBase _item, string oldText, string newText)
        {
            EXBoolListViewSubItem item = (EXBoolListViewSubItem)_item;
            if (item.BoolValue)
            {
                //解除其它层的激活
                for (int i = 0; i < LayerList.ItemList.Count; ++i)
                {
                    EXBoolListViewSubItem iter = (EXBoolListViewSubItem)LayerList.ItemList[i].SubItemList[1];
                    if (iter != item && iter.BoolValue)
                    {
                        iter.SetValue(false);
                        LayerList.Invalidate(iter.Bounds);
                    }
                }

                // 激活编辑器中的对应层
                ListView.SelectedListViewItemCollection sel = LayerList.SelectedItems;
                if (sel.Count > 0)
                {
                    Program.engine.MainLevel.SetActiveLayer(sel[0].SubItems[1].Text);
                }

				//在右侧的列表中显示当前激活层中的所有actor
				UpdateActorList(sel[0].SubItems[1].Text);
            }
            else
            {
                //不能直接关闭激活层的激活，只能通过激活其它层来替换当前激活层
                item.BoolValue = true;
                LayerList.Invalidate(item.Bounds);
            }
			OperationTooltip.Text = "就绪";
        }

        private void OnShowLayer(EXListViewSubItemBase _item, string oldText, string newText)
        {
            //隐藏或者显示编辑器中对应的层
            EXBoolListViewSubItem item = (EXBoolListViewSubItem)_item;
            ListView.SelectedListViewItemCollection sel = LayerList.SelectedItems;
            if (sel.Count > 0)
            {
                Program.engine.MainLevel.ShowLayer(sel[0].SubItems[1].Text,item.BoolValue);
            }
			OperationTooltip.Text = "就绪";
        }

        private void OnLockLayer(EXListViewSubItemBase _item, string oldText, string newText)
        {
            //锁定或者解锁编辑器中对应的层
            EXBoolListViewSubItem item = (EXBoolListViewSubItem)_item;
            ListView.SelectedListViewItemCollection sel = LayerList.SelectedItems;
            if (sel.Count > 0)
            {
                Program.engine.MainLevel.LockLayer(sel[0].SubItems[1].Text, item.BoolValue);
            }
			OperationTooltip.Text = "就绪";
        }

        private void LayerManagerDlg_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(e.CloseReason==CloseReason.UserClosing)
            {
                e.Cancel = true;
                this.Hide();
            }
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            //添加新层
            //确定可用的默认层名称
            string text;
            for(int i=1;;++i)
            {
                text = "新作业层" + i.ToString();
                if (!Program.engine.MainLevel.IsLayerExist(text))
                {
                    break;
                }                
            }

            EXListViewItem item = new EXImageListViewItem(NexusEditor.Properties.Resources.layer);
            EXListViewSubItem name = new EXListViewSubItem(text);
            name.OnItemValueChanged = this.OnLayerNameChanged; name.UserData = item;
            item.AddSubItem(name);
            EXBoolListViewSubItem active = new EXBoolListViewSubItem(false);
            active.OnItemValueChanged = this.OnActiveLayer; active.UserData = item;
            item.AddSubItem(active);
            EXBoolListViewSubItem show = new EXBoolListViewSubItem(true);
            show.OnItemValueChanged = this.OnShowLayer; show.UserData = item;
            item.AddSubItem(show);
            EXBoolListViewSubItem islock = new EXBoolListViewSubItem(true);
            islock.OnItemValueChanged = this.OnLockLayer; islock.UserData = item;
            item.AddSubItem(islock);
            LayerList.AddItem(item);
            // 在编辑器中对应的增加一层
            Program.engine.MainLevel.CreateNewLayer(text);
			OperationTooltip.Text = "就绪";
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            //删除当前选中的层(问题：当前激活的层是否可以删除？)
            ListView.SelectedIndexCollection sel= LayerList.SelectedIndices;
            if (sel.Count > 0)
            {
                if(DialogResult.Yes==MessageBox.Show("删除工作层时会删除层中的所有对象，是否确定要删除?", "警告", MessageBoxButtons.YesNo))
                {
                    // 删除编辑器中对应的层
                    EXListViewSubItemBase item=LayerList.ItemList[sel[0]].SubItemList[0];
                    string name = item.Text;
                    Program.engine.MainLevel.RemoveLayer(name);
                    LayerList.RemoveItem(sel[0]);
                }
                
            }
			OperationTooltip.Text = "就绪";
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            //显示或者隐藏全部的层
            //只有当前层全部都显示的时候，才设置为全部不显示，否则全部显示
            bool s = true;
            for (int i = 0; i < LayerList.ItemList.Count; ++i)
            {
                EXBoolListViewSubItem item = (EXBoolListViewSubItem)LayerList.Items[i].SubItems[3];
                if (item != null)
                {
                    if (!item.BoolValue)
                    {
                        s = false;
                    }
                }
            }
            for (int i = 0; i < LayerList.ItemList.Count; ++i)
            {
                EXBoolListViewSubItem item = (EXBoolListViewSubItem)LayerList.Items[i].SubItems[3];
                if (item != null)
                {
                    item.BoolValue = !s;
                    string layer = LayerList.Items[i].SubItems[1].Text;
                    Program.engine.MainLevel.ShowLayer(layer,!s);
                }
            }

            LayerList.Invalidate();
			OperationTooltip.Text = "就绪";
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            //锁定或者解除锁定所有的层
            //只有当前层全部都冻结的时候，才设置为全部不冻结，否则全部冻结
            bool s = true;
            for (int i = 0; i < LayerList.ItemList.Count; ++i)
            {
                EXBoolListViewSubItem item = (EXBoolListViewSubItem)LayerList.Items[i].SubItems[4];
                if (item != null)
                {
                    if(!item.BoolValue)
                    {
                        s = false;
                    }
                }
            }

            for (int i = 0; i < LayerList.ItemList.Count; ++i)
            {
                EXBoolListViewSubItem item = (EXBoolListViewSubItem)LayerList.Items[i].SubItems[4];
                if (item != null)
                {
                    item.BoolValue = !s;
                    string layer = LayerList.Items[i].SubItems[1].Text;
                    Program.engine.MainLevel.LockLayer(layer, !s);
                }
            }

            LayerList.Invalidate();
			OperationTooltip.Text = "就绪";
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            //将当前选定对象移动到当前激活的图层
            NexusEngine.NActor actor = Program.engine.ActorEd.SelectedActor;
            if(actor!=null)
            {
                string name = actor.Name;
                Program.engine.MainLevel.MoveActorToActiveLayer(name);
				OperationTooltip.Text = "对象移动完成";
            }
        }

        private void LayerManagerDlg_Load(object sender, EventArgs e)
        {
            OnLevelChanged();
			OperationTooltip.Text = "就绪";
        }

		private void m_ActorList_DoubleClick(object sender, EventArgs e)
		{
			//选中关联的actor
			ListViewItem selItem = this.m_ActorList.SelectedItems[0];
			NActor ac = selItem.Tag as NActor;
			NLevelEditorEngine.Instance.ActorEd.BindActor(ac);
		}
    }
}
