using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
using NexusEngine;
using NexusEditor.ResourceEditor;

namespace NexusEditor
{
    /// <summary>
    /// 处理主窗口中Resource Tab Page相关功能
    /// </summary>
    partial class EditorMainForm
    {
        //private ResourcePreview m_resPreview; // preview太小,不实用
        private ImageList m_resBigImgList;
        private ResourceListFilter m_meshFileFilter;

        //-- drag - drop
        private Rectangle m_dragBox;
        private ListViewItem m_itemUnderDrag;

        /// <summary>
        /// 初始化,绑定事件
        /// </summary>
        private void ResourcePageLoad()
        {
            this.listViewResource.SelectedIndexChanged += new System.EventHandler(this.listViewResource_SelectedIndexChanged);

            NLevelEditorEngine.Instance.CurrentFolderChanged
                += new EventHandler(Engine_CurrentFolderChanged);
            //m_resPreview = new ResourcePreview("LevelDesignResourcePreveiw");
            //this.panelResPreview.Controls.Add(m_resPreview);

            m_resBigImgList = new ImageList();
            m_resBigImgList.ColorDepth = ColorDepth.Depth24Bit;
            m_resBigImgList.ImageSize = new System.Drawing.Size(256,256);

            m_meshFileFilter = new ResourceListFilter();
            m_meshFileFilter.EnableFileExt("nmdl");
            m_meshFileFilter.EnableFileExt("nam");
            m_meshFileFilter.EnableFileExt("spt");

            //-- 拖放相关操作
            this.listViewResource.MouseUp += new System.Windows.Forms.MouseEventHandler(this.listViewResource_MouseUp);
            this.listViewResource.MouseMove += new System.Windows.Forms.MouseEventHandler(this.listViewResource_MouseMove);
            this.listViewResource.MouseDown += new System.Windows.Forms.MouseEventHandler(this.listViewResource_MouseDown);
            
        }

        /// <summary>
        /// 响应引擎当前工作目录更改时间, 把当前工作目录中的Mesh资源文件显示到列表中
        /// </summary>        
        void Engine_CurrentFolderChanged(object sender, EventArgs e)
        {
            NResourceLoc folder = NLevelEditorEngine.Instance.CurrentFolder;

            NEFileListBuilder lb = new NEFileListBuilder(
                this.listViewResource, m_resBigImgList, this.imageListResource,
                folder.PackageName, folder.FileName);
            lb.SetFilter(m_meshFileFilter);
            lb.BuildList();
        }

        /// <summary>
        /// 响应Resource列表选中事件, 将选中的文件的缩略图显示到列表下方的PictureBox上
        /// </summary>        
        private void listViewResource_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection selItems = this.listViewResource.SelectedItems;
            if (selItems.Count <= 0)
                return;

            ListViewItem sel = selItems[0];
            NFileEntity nfile = (NFileEntity)sel.Tag;
            NResourceLoc resLoc = new NResourceLoc(nfile.PackageName, nfile.FilePath);
            //m_resPreview.ShowResource(resLoc);
            //m_resPreview.ZoomExtents();

            int i = m_resBigImgList.Images.IndexOfKey(resLoc.ToString());
            if (i == -1)
                this.pictureBoxResource.Image = null;
            else
                this.pictureBoxResource.Image = m_resBigImgList.Images[i];
        }

        /// <summary>
        /// 鼠标在List上按下, 准备拖放相关数据
        /// </summary>        
        private void listViewResource_MouseDown(object sender, MouseEventArgs e)
        {
            if (this.listViewResource.SelectedItems.Count > 0)
            {
                m_itemUnderDrag = this.listViewResource.GetItemAt(e.X, e.Y);
                Size dragSize = SystemInformation.DragSize;
                m_dragBox = new Rectangle(new Point(e.X - dragSize.Width / 2, e.Y - dragSize.Height / 2),
                    dragSize);
            }
            else 
            {
                m_dragBox = Rectangle.Empty;
                m_itemUnderDrag = null;
            }
        }

        /// <summary>
        /// 检测, 并开始拖放
        /// </summary>        
        private void listViewResource_MouseMove(object sender, MouseEventArgs e)
        {
            if ((e.Button&MouseButtons.Left)==MouseButtons.Left)
            {
                if (m_dragBox != Rectangle.Empty
                    && !m_dragBox.Contains(e.X, e.Y)
                    && m_itemUnderDrag!=null)
                {
                    DragDropEffects eft = DragDropEffects.None;
                    eft = this.listViewResource.DoDragDrop(m_itemUnderDrag.Tag, DragDropEffects.Copy);
                }
            }
        }

        /// <summary>
        /// 清空拖放状态数据
        /// </summary>        
        private void listViewResource_MouseUp(object sender, MouseEventArgs e)
        {
            m_dragBox = Rectangle.Empty;
        }
    }
}