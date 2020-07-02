using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using NexusEngine;
using Nexus.GameFramework;
using System.Reflection;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 资源编辑器主窗口
    /// </summary>
    public partial class ResourceEditorForm : Form
    {
        protected ResStaticMeshEditor m_staticMeshEditor;
		protected MaterialEditor m_MaterialEditor;
        protected ResAnimMeshEditor m_animMeshEditor;
        protected ResAnimSetEditor m_animSetEditor;
        protected SpeedTreeViewer m_sptViewer;
        protected ResTextureAtlasViewer m_texAtlasViewer;
        protected NexusEditor.UIEditor.UIEditor m_guiEditor;
        protected ResSFXEditor m_sfxEditor;

        protected ListView m_backEndList;   // 为了实现Search功能,使用以后后台列表, 根据search情况,显示到前台列表中
        protected ResourceListFilter m_filter;
        private bool m_finalClose;
		protected List<NResourceLoc> m_PathList;	//前进后退使用的列表
		int m_PathPointer;

        public ResourceEditorForm()
        {
            InitializeComponent();

            m_finalClose = false;

			m_PathList = new List<NResourceLoc>();
			m_PathPointer = -1;
            //-- build tree
            NEFileTreeBuilder tb = new NEFileTreeBuilder(this.treeViewFileSys);
            tb.BuildTree();

            //-- create sub editors
            m_staticMeshEditor = new ResStaticMeshEditor();
            m_animMeshEditor = new ResAnimMeshEditor();
            m_animSetEditor = new ResAnimSetEditor();
            m_sptViewer = new SpeedTreeViewer();
            m_texAtlasViewer = new ResTextureAtlasViewer();
            m_guiEditor = new NexusEditor.UIEditor.UIEditor();
			m_sfxEditor = new ResSFXEditor();
			m_MaterialEditor = new MaterialEditor();

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
            m_animSetEditor.Close();
            m_sptViewer.Close();
            m_guiEditor.Close();
            m_sfxEditor.Close();
			m_MaterialEditor.Close();
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
                using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                {
                    NEFileTag tag = (NEFileTag)e.Node.Tag;
                    NLevelEditorEngine.Instance.CurrentFolder = new NResourceLoc(tag.pkgName, tag.path);
					m_PathList.Add(NLevelEditorEngine.Instance.CurrentFolder);
					if (m_PathList.Count ==11)	//10次回溯
					{
						m_PathList.RemoveAt(0);
					}
					m_PathPointer = m_PathList.Count - 1;
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
            using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
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
                    case "skm":
                        m_animSetEditor.LoadSkeletalMesh(nfile.PackageName, nfile.FilePath);
                        m_animSetEditor.Show();
                        break;
                    case "animset":
                        m_animSetEditor.LoadAnimSet(nfile.PackageName, nfile.FilePath);
                        m_animSetEditor.Show();
                        break;
                    case "txa":
                        m_texAtlasViewer.LoadTextureAtlas(nfile.pkg, nfile.path);
                        m_texAtlasViewer.Show(this);
                        break;
                    case "nui":
                        m_guiEditor.LoadUI(nfile.pkg, nfile.path);
                        if (!m_guiEditor.Visible)
                            m_guiEditor.Show();
                        break;
					case "mtl":
						m_MaterialEditor.LoadMaterial(nfile.PackageName, nfile.FilePath);
						m_MaterialEditor.Show();
						break;
                    case "att":
                        ResAttributeInfoEditor infoEditor = new ResAttributeInfoEditor();
                        infoEditor.ResetAttributeInfo(nfile.PackageName, nfile.FilePath);
                        infoEditor.Show();
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
				case 5:
					if (chk)
						m_filter.EnableFileExt("mtl");
					else
						m_filter.DisableFileExt("mtl");
                    break;
            }// end of switch

            RefreshFileList();
        }

        private void makeTextureAtlasHereToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode sel = this.treeViewFileSys.SelectedNode;
            if (sel.Tag == null)
                return;

            NEFileTag tag = (NEFileTag)sel.Tag;
            NResourceLoc loc = new NResourceLoc(tag.pkgName, tag.path);

            using (MakeTextureAtlasDlg dlg = new MakeTextureAtlasDlg())
            {
                dlg.SetInputLoc(loc);
                loc.fileName += "/Atlas";
                dlg.SetOutputLoc(loc);
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    dlg.MakeIt();
                }// end of if()
            }// end of using()
        }

        private void showGUIEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_guiEditor.Show();
        }

        private void showSFXEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_sfxEditor.Show();
        }

        private void externalToolsToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //新建目录
            //首先要获取当前目录
            TreeNode sel = this.treeViewFileSys.SelectedNode;
            if (sel.Tag == null)
                return;

            NEFileTag tag = (NEFileTag)sel.Tag;
            String path = tag.path + "/";
            uint i = 1;
            String name;
            String full_path;
            while (true)
            {
                name = "NewFolder" + i.ToString();
                full_path = path + name;
                if (!NLevelEditorEngine.Instance.FileSystem.IsFileExist(tag.pkgName, full_path))
                {
                    break;
                }
                ++i;
            }
            NLevelEditorEngine.Instance.FileSystem.CreateDirectory(tag.pkgName, full_path);

            //更新目录树
            TreeNode folderNode = sel.Nodes.Add(name);
            folderNode.SelectedImageIndex = 1;
            folderNode.ImageIndex = 0;

            //-- set tag
            NEFileTag newtag;
            newtag.pkgName = tag.pkgName;
            newtag.path = full_path;
            folderNode.Tag = newtag;
            sel.Expand();
            folderNode.BeginEdit();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //删除当前选中目录
            TreeNode sel = this.treeViewFileSys.SelectedNode;
            if (sel == null || sel.Tag == null)
                return;
            NEFileTag tag = (NEFileTag)sel.Tag;
            if (tag.path.Length == 0) return;

            DialogResult rst = MessageBox.Show("删除文件夹会删除文件夹内的所有文件，并且该操作不可撤销，\n是否继续？", "Nexus Editor", MessageBoxButtons.YesNo);
            if (rst == DialogResult.Yes)
            {
                NLevelEditorEngine.Instance.FileSystem.DeleteDirectory(tag.pkgName, tag.path);
                //更新视图
                this.treeViewFileSys.Nodes.Remove(sel);
            }
        }

        private void treeViewFileSys_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            //重命名的处理
            if (e.Label != null)
            {
                if (e.Label.Length > 0)
                {
                    if (e.Label.IndexOfAny(new char[] { '@', '.', ',', '!' }) == -1)
                    {
                        //完成重命名
                        TreeNode sel = e.Node;
                        if (sel.Tag == null)
                        {
                            //如果没有关联文件夹，则取消并返回
                            e.CancelEdit = true;
                            e.Node.EndEdit(true);
                            return;
                        }
                        NEFileTag tag = (NEFileTag)sel.Tag;
                        if (tag.path.Length == 0)
                        {
                            //不能编辑package
                            e.CancelEdit = true;
                            e.Node.EndEdit(true);
                            return;
                        }
                        String oldpath = tag.path;
                        int len = oldpath.LastIndexOf('/');
                        if (len == -1) len = oldpath.Length;
                        String newpath = oldpath.Substring(0, len) + "/" + e.Label;
                        if (NLevelEditorEngine.Instance.FileSystem.RenameDirectory(tag.pkgName, oldpath, newpath))
                        {
                            tag.path = tag.path.Substring(0, tag.path.LastIndexOf('/')) + "/" + e.Label;
                            e.Node.Tag = tag;
                            e.Node.EndEdit(false);
                        }
                        else
                        {
                            e.CancelEdit = true;
                            MessageBox.Show("重命名文件夹时发生错误", "Nexus Editor");
                            e.Node.BeginEdit();
                        }
                    }
                    else
                    {
                        /* Cancel the label edit action, inform the user, and 
                           place the node in edit mode again. */
                        e.CancelEdit = true;
                        MessageBox.Show("文件夹的名字格式不正确。文件夹的名字里面不能包含有这些字符: '@','.', ',', '!'", "Nexus Editor");
                        e.Node.BeginEdit();
                    }
                }
                else
                {
                    e.CancelEdit = true;
                    MessageBox.Show("文件夹的名字不能为空.\n请重新输入一个名字.", "Nexus Editor");
                    e.Node.BeginEdit();
                }
            }
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            TreeNode sel = this.treeViewFileSys.SelectedNode;
            if (sel == null || sel.Tag == null)
                return;
            NEFileTag tag = (NEFileTag)sel.Tag;
            if (tag.path.Length == 0) return;
            sel.BeginEdit();
        }

        private void toolStripButtonRefresh_Click(object sender, EventArgs e)
        {
            RefreshFileList();
        }

        private void refreshDirectoryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NEFileTreeBuilder tb = new NEFileTreeBuilder(this.treeViewFileSys);
            tb.BuildTree();
        }

		private void toolStripButtonStaticMeshEd_Click(object sender, EventArgs e)
		{
			m_MaterialEditor.Show();
		}

		private void toolStripButton2_Click(object sender, EventArgs e)
		{
			m_staticMeshEditor.Show();
		}

		private void toolStripButton1_Click(object sender, EventArgs e)
		{
			if (m_PathList.Count == 0 || m_PathPointer <= 0) return;
			--m_PathPointer;
			NLevelEditorEngine.Instance.CurrentFolder = m_PathList[m_PathPointer];
			RefreshFileList();
		}

		private void toolStripButton3_Click(object sender, EventArgs e)
		{
			if (m_PathList.Count == 0 || m_PathPointer >= m_PathList.Count-1) return;
			++m_PathPointer;
			NLevelEditorEngine.Instance.CurrentFolder = m_PathList[m_PathPointer];
			RefreshFileList();
		}

        private void toolStripAnimSet_Click(object sender, EventArgs e)
        {
            m_animSetEditor.Show();
        }

        private void importToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            this.importToolStripMenuItem.DropDownItems.Clear();

            Assembly assem = Assembly.GetExecutingAssembly();
            foreach (Type item in assem.GetTypes())
            {
                if( !item.IsAbstract
                    && item.IsSubclassOf(typeof(GameAttributeImportorBase)) )
                {
                    ToolStripMenuItem addActorTypeMenu = new ToolStripMenuItem(item.Name, null, new EventHandler(GameAttributeImportorMenu_Click));
                    addActorTypeMenu.Tag = Activator.CreateInstance(item);
				    this.importToolStripMenuItem.DropDownItems.Add(addActorTypeMenu);
                }
            }
        }

        private void GameAttributeImportorMenu_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = sender as ToolStripMenuItem;
            GameAttributeImportorBase importor = item.Tag as GameAttributeImportorBase;
            if(importor != null)
            {
                importor.ImportFromFile();
            }
            else
            {
                NLogger.Instance.WriteString(LogType.Error,"Unkown attribute import type!");
            }
        }

    }
}
