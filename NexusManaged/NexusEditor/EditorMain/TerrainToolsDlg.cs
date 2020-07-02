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
    /// 地形工具对话框
    /// </summary>
    public partial class TerrainToolsDlg : Form
    {
        public TerrainToolsDlg()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Heightmap Import按钮事件
        /// </summary>        
        private void btnImportHmap_Click(object sender, EventArgs e)
        {
            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            if (trnEd.Empty())
            {
                NexusEditor.Program.ShowError("Terrain Actor NOT Bind.");
                return;
            }

            try
            {
                string fileName = this.textBoxHMapFile.Text;
                if (fileName.Length > 0)
                {
                    trnEd.ImportHeightmap(fileName);
                }
                else
                {
                    NexusEditor.Program.ShowError("Please input heightmap file name.");
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Import Hiehgtmap FAILED!");
            }
        }

        private void btnHMapExport_Click(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Heightmap Import/export文件浏览按钮事件
        /// </summary>        
        private void btnBrowseHmapFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "TGA File(*.tga)|*.tga|JPEG file(*.jpg)|*.jpg|Windows Bitmap File(*.bmp)|*.bmp";
            dlg.RestoreDirectory = true;
            if (dlg.ShowDialog(this) == DialogResult.OK)
            {
                this.textBoxHMapFile.Text = dlg.FileName;
            }
        }
    }
}
