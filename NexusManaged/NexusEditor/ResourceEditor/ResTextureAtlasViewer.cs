using System;
using System.Windows.Forms;
using System.Drawing;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    public partial class ResTextureAtlasViewer : Form
    {
        private Size m_imgSize;
        private NTexAtlasItem m_selectedItem;
        private NResourceTextureAtlas m_resource;
        private bool m_finalClose;

        public ResTextureAtlasViewer()
        {
            InitializeComponent();

            m_finalClose = false;

            m_selectedItem = new NTexAtlasItem();
            this.propertyGridItem.SelectedObject = m_selectedItem;
        }

        public void LoadTextureAtlas(string pkgName, string fileName)
        {
            try
            {
                //-- Load资源对象
                NResourceLoc loc = new NResourceLoc(pkgName, fileName);
                m_resource = NResourceManager.Instance.LoadTextureAtlas(loc,
                        EResourceIOMode.Block, EResourceIOPriority.Normal);

                //-- Load贴图文件
                fileName = fileName.Replace(".txa", ".tga");//找到对应的贴图文件
                IntPtr hbmp = NLevelEditorEngine.Instance.LoadTextureAsBitmap(
                        new NResourceLoc(pkgName, fileName)
                        );

                //-- 更新控件显示
                Image img = Image.FromHbitmap(hbmp);
                m_imgSize = img.Size;
                this.pictureBoxMain.Image = img;

                //-- 显示Item List
                UpdateAtlasList();
            }
            catch (System.Exception e)
            {
                Program.ShowException(e, "Texture Atlas Load Failed");
            }            
        }

        private void UpdateAtlasList()
        {
            string filterString = this.textBoxSearch.Text.Trim();
            bool nofilter = string.IsNullOrEmpty(filterString);

            this.listViewAtlas.BeginUpdate();
            this.listViewAtlas.Items.Clear();
            NTexAtlasItem item = new NTexAtlasItem();
            bool next = m_resource.FirstItem(ref item);
            System.Windows.Rect itemRc = new System.Windows.Rect();
            while (next)
            {
                Vector2 itemSize = item.uvEnd - item.uvStart;
                itemRc.X = item.uvStart.x * m_imgSize.Width;
                itemRc.Y = item.uvStart.y * m_imgSize.Height;
                itemRc.Width = itemSize.x * m_imgSize.Width;
                itemRc.Height = itemSize.y * m_imgSize.Height;

                if (nofilter || item.Name.IndexOf(filterString, StringComparison.OrdinalIgnoreCase) != -1)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = string.Format("{0} [{1},{2}]", item.Name, itemRc.Width, itemRc.Height);
                    listItem.Tag = item;
                    this.listViewAtlas.Items.Add(listItem);
                }

                next = m_resource.NextItem(ref item);
            }// end of while()
            this.listViewAtlas.EndUpdate();
        }

        private void pictureBoxMain_Paint(object sender, PaintEventArgs e)
        {
            if (!string.IsNullOrEmpty(m_selectedItem.name))
            {                
                Vector2 itemSize = m_selectedItem.uvEnd-m_selectedItem.uvStart;
                itemSize.x *= m_imgSize.Width;
                itemSize.y *= m_imgSize.Height;

                Graphics g = e.Graphics;
                Pen redPen = new Pen(Color.OrangeRed, 2);
                float rcX = m_selectedItem.uvStart.x * m_imgSize.Width;
                float rcY = m_selectedItem.uvStart.y * m_imgSize.Height;
                g.DrawRectangle(redPen, rcX, rcY,
                    itemSize.x, itemSize.y);

                //Brush bsh = new SolidBrush(Color.IndianRed);
                //g.DrawString(m_selectedItem.Name, this.Font, bsh, rcX + 2, rcY + 2);
            }            
        }

        private void pictureBoxMain_MouseClick(object sender, MouseEventArgs e)
        {
            if(m_resource==null)
                return;

            System.Windows.Point clickPt = new System.Windows.Point(e.X, e.Y);
            System.Windows.Rect itemRc = new System.Windows.Rect();
            NTexAtlasItem item = new NTexAtlasItem();
            

            bool next = m_resource.FirstItem(ref item);
            while (next)
            {
                Vector2 itemSize = item.uvEnd-item.uvStart;

                itemRc.X = item.uvStart.x * m_imgSize.Width;
                itemRc.Y = item.uvStart.y * m_imgSize.Height;
                itemRc.Width = itemSize.x * m_imgSize.Width;
                itemRc.Height = itemSize.y * m_imgSize.Height;

                if (itemRc.Contains(clickPt))
                {
                    m_selectedItem = item;
                    this.propertyGridItem.SelectedObject = m_selectedItem;
                    this.pictureBoxMain.Invalidate();
                    return;
                }
                next = m_resource.NextItem(ref item);
            }// end of while()

            m_selectedItem = new NTexAtlasItem();
        }

        private void ResTextureAtlasViewer_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_finalClose)
            {
                e.Cancel = true;
                this.Hide();
            }
        }

        public new void Close()
        {
            if (m_resource != null)
            {
                m_resource.Dispose();
                m_resource = null;
            }            
            m_finalClose = true;
            base.Close();
        }

        private void listViewAtlas_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.listViewAtlas.SelectedItems.Count != 1) return;

            ListViewItem item = this.listViewAtlas.SelectedItems[0];
            if (item.Tag is NTexAtlasItem)
            {
                m_selectedItem = (NTexAtlasItem)item.Tag;
                this.propertyGridItem.SelectedObject = m_selectedItem;
                this.pictureBoxMain.Invalidate();
            }
        }

        private void textBoxSearch_TextChanged(object sender, EventArgs e)
        {
            UpdateAtlasList();
        }
    }
}
