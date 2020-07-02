using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 资源编辑器主窗口
    /// </summary>
    public partial class ResourceEditorForm : Form
    {
        protected ResStaticMeshEditor m_staticMeshEditor;
        protected ResAnimMeshEditor m_animMeshEditor;
        protected SpeedTreeViewer m_sptViewer;
        protected ListView m_backEndList;   // 为了实现Search功能,使用以后后台列表, 根据search情况,显示到前台列表中
        protected ResourceListFilter m_filter;
        private bool m_finalClose;

        public ResourceEditorForm()
        {
            InitializeComponent();

            m_finalClose = false;

            //-- build tree
            NEFileTreeBuilder tb = new NEFileTreeBuilder(this.treeViewFileSys);
            tb.BuildTree();

            //-- create sub editors
            m_staticMeshEditor = new ResStaticMeshEditor();
            m_animMeshEditor = new ResAnimMeshEditor();
            m_sptViewer = new SpeedTreeViewer();

            //--
            m_backEndList = new ListView();

            m_filter = new ResourceListFilter();
            m_filter.EnableAll = true;
            this.listFilter.SetItemChecked(0, true);
        }

        public new void Close()
        {
            m_finalClose = true;
            m_staticMeshEditor.Close();
            m_animMeshEditor.Close();
            m_sptViewer.Close();
            base.Close();
        }

        private void ResourceEditorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_finalClose)
            {
                e.Cancel = true;
                this.Hide();
            }            
        }

        private void RefreshFileList()
        {
            NResourceLoc curFolder = NLevelEditorEngine.Instance.CurrentFolder;

            if (curFolder.IsValid())
            {
                NEFileListBuilder lb = new NEFileListBuilder(m_backEndList,
                        this.imageListForList, this.imageListForListSmall,
                        curFolder.PackageName, curFolder.FileName);
                lb.SetFilter(m_filter);
                lb.BuildList();

                SearchFliterResourceList();
            }                            
        }

        private void treeViewFileSys_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Tag != null)
            {
                using (NWaitCursor wc = new NWaitCursor(this))
                {
                    NEFileTag tag = (NEFileTag)e.Node.Tag;
                    NLevelEditorEngine.Instance.CurrentFolder = new NResourceLoc(tag.pkgName, tag.path);
                    RefreshFileList();
                }
            }
        }

        private void showStaticMeshEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_staticMeshEditor.Show();
        }

        private void listViewResource_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            ListView.SelectedListViewItemCollection selItems = this.listViewResource.SelectedItems;
            if (selItems.Count <= 0)
                return;

            ListViewItem sel = selItems[0];

            NFileEntity nfile = (NFileEntity)sel.Tag;
            using (NWaitCursor wc = new NWaitCursor(this))
            {
                switch (nfile.FileExtension)
                {
                    case "spt":
                        m_sptViewer.ShowSpt(nfile.pkg, nfile.path);
                        m_sptViewer.Show(this);
                        break;
                    case "nmdl":
                        m_staticMeshEditor.LoadStaticMesh(nfile.PackageName, nfile.FilePath);
                        m_staticMeshEditor.Show();
                        break;
                    case "nam":
                        m_animMeshEditor.LoadAnimMesh(nfile.PackageName, nfile.FilePath);
                        m_animMeshEditor.Show();
                        break;
                }
            }
        }

        private void treeViewFileSys_Click(object sender, EventArgs e)
        {
            MouseEventArgs me = e as MouseEventArgs;
            if (me.Button == MouseButtons.Right)
            {
                Point pt = me.Location;
                this.contextMenuStripTree.Show(this.treeViewFileSys, pt);
            }
        }

        private void saveCurrentLevelHereToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode sel = this.treeViewFileSys.SelectedNode;
            if (sel.Tag != null)
            {
                NEFileTag tag = (NEFileTag)sel.Tag;
                NResourceLoc loc = new NResourceLoc(tag.pkgName, tag.path);
                NLevelEditorEngine.Instance.SaveMainLevel(loc);
            }
        }

        private void loadLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode sel = this.treeViewFileSys.SelectedNode;
            if (sel.Tag != null)
            {
                NEFileTag tag = (NEFileTag)sel.Tag;
                NResourceLoc loc = new NResourceLoc(tag.pkgName, tag.path);
                NLevelEditorEngine.Instance.LoadMainLevel(loc);
            }
        }

        private void listViewResource_KeyPress(object sender, KeyPressEventArgs e)
        {
        }

        private void listViewResource_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (e.IsSelected)
            {
                object itemTag = e.Item.Tag;
                NFileEntity file = (NFileEntity)itemTag;

                NLevelEditorEngine.Instance.CurrentResource =
                    new NResourceLoc(file.PackageName, file.FilePath);
            }
        }

        private void toolTextSearch_TextChanged(object sender, EventArgs e)
        {
            SearchFliterResourceList();
        }

        private void SearchFliterResourceList()
        {
            string searchTxt = this.toolTextSearch.Text;

            this.listViewResource.BeginUpdate();
            this.listViewResource.Items.Clear();
            if (searchTxt.Length <= 0)
            {
                foreach (ListViewItem item in this.m_backEndList.Items)
                    this.listViewResource.Items.Add(item.Clone() as ListViewItem);
            }
            else
            {
                foreach (ListViewItem item in this.m_backEndList.Items)
                {
                    // 只显示搜索的资源
                    string itemTxt = item.Text;

                    if (itemTxt.Contains(searchTxt))
                        this.listViewResource.Items.Add(item.Clone() as ListViewItem);
                }
            }
            this.listViewResource.EndUpdate();
        }

        private void showAnimMeshEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_animMeshEditor.Show();
        }

        private void iconsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.listViewResource.View = View.SmallIcon;
        }

        private void detailsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.listViewResource.View = View.Details;
        }

        private void thumbnialsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.listViewResource.View = View.LargeIcon;
        }

        private void listFilter_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            bool chk = e.NewValue == CheckState.Checked;

            switch (e.Index)
            {
                case 0:
                    m_filter.EnableAll = chk;
                    break;
                case 1:
                    if (chk)
                        m_filter.EnableFileExt("nmdl");
                    else
                        m_filter.DisableFileExt("nmdl");
                    break;
                case 2:
                    if (chk)
                        m_filter.EnableFileExt("todo");
                    else
                        m_filter.DisableFileExt("todo");
                    break;
                case 3:
                    if (chk)
                        m_filter.EnableFileExt("nam");
                    else
                        m_filter.DisableFileExt("name");
                    break;
                case 4:
                    if (chk)
                    {
                        m_filter.EnableFileExt("dds");
                        m_filter.EnableFileExt("tga");
                        m_filter.EnableFileExt("bmp");
                        m_filter.EnableFileExt("jpg");
                    }
                    else
                    {
                        m_filter.DisableFileExt("dds");
                        m_filter.DisableFileExt("tga");
                        m_filter.DisableFileExt("bmp");
                        m_filter.DisableFileExt("jpg");
                    }
                    break;
            }// end of switch

            RefreshFileList();                   
        }
    }
}
