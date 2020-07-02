using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    public partial class ResAttributeInfoEditor : Form
    {
        private AttributeInfoEditorBase editInfo;

        public ResAttributeInfoEditor()
        {
            InitializeComponent();
        }

        public void ResetAttributeInfo(string pkg, string file)
        {
            if(string.IsNullOrEmpty(file))
                return;

            try
            {
                this.Text = "ResAttributeInfoEditor - " + file;

                this.listViewInfo.BeginUpdate();
                this.listViewInfo.Items.Clear();

                // TODO: 优化代码组织
                // 下面这段代码使用文件名字作为打开那个编辑器的Key，有点出于无赖
                if (file.IndexOf("nspell_info_dictionary") != -1)
                {
                    editInfo = new NSpellInfoEditor();
                    editInfo.ReloadFromFile();

                    editInfo.ShowToListView(ref this.listViewInfo, this.toolTextSearch.Text.Trim());
                }
                else if (file.IndexOf("nspell_effect_info_dictionary") != -1)
                {
                    editInfo = new NSpellEffectInfoEditor();
                    editInfo.ReloadFromFile();

                    editInfo.ShowToListView(ref this.listViewInfo, this.toolTextSearch.Text.Trim());
                }
                else if (file.IndexOf("nspell_aura_info_dictionary") != -1)
                {
                    editInfo = new NSpellAuraInfoEditor();
                    editInfo.ReloadFromFile();

                    editInfo.ShowToListView(ref this.listViewInfo, this.toolTextSearch.Text.Trim());
                }
                else if (file.IndexOf("nlevelup_info_dictionary") != -1)
                {
                    editInfo = new NLevelupInfoEditor();
                    editInfo.ReloadFromFile();

                    editInfo.ShowToListView(ref this.listViewInfo, this.toolTextSearch.Text.Trim());
                }
                else if (file.IndexOf("nspell_talent_table_dictionary") != -1)
                {
                    editInfo = new NSpellTalentTableEditor();
                    editInfo.ReloadFromFile();

                    editInfo.ShowToListView(ref this.listViewInfo, this.toolTextSearch.Text.Trim());
                }
                this.listViewInfo.EndUpdate();
            }
            catch(Exception e)
            {
                Program.ShowException(e, "Load attribute info error!");
            }
        }

        private void listViewInfo_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection infos =
                        this.listViewInfo.SelectedItems;

            List<Object> currentSelObjs = new List<Object>();
            foreach (ListViewItem item in infos)
            {
                currentSelObjs.Add(item.Tag);
            }
            this.propertyGridInfo.SelectedObjects = currentSelObjs.ToArray();
        }

        private void listViewInfo_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            this.listViewInfo.ListViewItemSorter = new ListViewItemComparer(e.Column);
        }

        // Implements the manual sorting of items by columns.
        class ListViewItemComparer : System.Collections.IComparer
        {
            private int col;
            public ListViewItemComparer()
            {
                col = 0;
            }
            public ListViewItemComparer(int column)
            {
                col = column;
            }
            public int Compare(object x, object y)
            {
                return String.Compare(((ListViewItem)x).SubItems[col].Text, ((ListViewItem)y).SubItems[col].Text);
            }
        }

        private void toolStripButtonSave_Click(object sender, EventArgs e)
        {
            if(editInfo != null)
            {
                editInfo.SaveToFile();
            }
        }

        private void toolStripButtonNew_Click(object sender, EventArgs e)
        {
            if (editInfo != null)
            {
                editInfo.NewInfo();
                // 刷新界面
                editInfo.ShowToListView(ref this.listViewInfo, this.toolTextSearch.Text.Trim());
            }
        }

        private void toolStripButtonDelete_Click(object sender, EventArgs e)
        {
            if (editInfo == null)
                return;

             ListView.SelectedListViewItemCollection infos =
                        this.listViewInfo.SelectedItems;

            foreach (ListViewItem item in infos)
            {
                editInfo.RemoveInfo(item.Tag);
            }

            // 刷新界面
            editInfo.ShowToListView(ref this.listViewInfo, this.toolTextSearch.Text.Trim());
        }

        private void toolStripButtonClear_Click(object sender, EventArgs e)
        {
            if (editInfo != null)
            {
                editInfo.ClearInfo();

                // 刷新界面
                editInfo.ShowToListView(ref this.listViewInfo, this.toolTextSearch.Text.Trim());
            }
        }

        private void toolTextSearch_TextChanged(object sender, EventArgs e)
        {
            if (editInfo != null)
            {
                // 刷新界面
                editInfo.ShowToListView(ref this.listViewInfo, this.toolTextSearch.Text.Trim());
            }
        }
    }
}
