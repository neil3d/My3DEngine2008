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
    /// 为地形对象创建分层材质
    /// </summary>
    public partial class TerrainSplattingMtlDlg : Form
    {        
        public TerrainSplattingMtlDlg()
        {
            InitializeComponent();
        }

        /// <summary>
        /// [Material]创建Texture Splatting材质按钮事件
        /// </summary> 
        private void btnCreateSplat_Click(object sender, EventArgs e)
        {
            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            if (trnEd.Empty())
            {
                NexusEditor.Program.ShowError("Terrain Actor NOT Bind.");
                return;
            }

            int alphaW = int.Parse(this.textBoxAlphaW.Text);
            int alphaH = int.Parse(this.textBoxAlphaH.Text);

            trnEd.CreateTextureSplatting(alphaW, alphaH);            
        }
    }
}
