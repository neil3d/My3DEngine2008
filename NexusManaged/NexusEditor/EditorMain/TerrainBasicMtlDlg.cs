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
    /// 为地形对象创建基本材质
    /// </summary>
    public partial class TerrainBasicMtlDlg : Form
    {
        private NETerrainBasicMtlProp m_prop;

        public TerrainBasicMtlDlg()
        {
            InitializeComponent();
        }

        private void buttonCreate_Click(object sender, EventArgs e)
        {
            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            if (trnEd.Empty())
            {
                NexusEditor.Program.ShowError("Terrain Actor NOT Bind.");
                return;
            }

            if (!m_prop.DiffuseTextureRes.IsValid())
            {
                NexusEditor.Program.ShowError("Please input texture location for terrain basic material.");
                return;
            }

            try
            {
                trnEd.CreateMaterialBasic(m_prop.DiffuseTextureRes);
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Create Basic Terrain Material FAILED!");
            }            
        }

        private void TerrainBasicMtlDlg_Load(object sender, EventArgs e)
        {
            m_prop = new NETerrainBasicMtlProp();
            this.propertyGridMtl.SelectedObject = m_prop;
        }
    }
}
