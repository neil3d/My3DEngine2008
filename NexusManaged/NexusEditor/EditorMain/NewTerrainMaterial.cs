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
    public partial class NewTerrainMaterial : Form
    {
        private NETerrainMaterialProp m_prop;
        private NTerrainActor m_trn;

        public NewTerrainMaterial(NTerrainActor trn)
        {
            m_trn = trn;
            InitializeComponent();
        }

        private void NewTerrainMaterial_Load(object sender, EventArgs e)
        {
            m_prop = new NETerrainMaterialProp();
            this.propertyGridTrn.SelectedObject = m_prop;
        }

        public NETerrainMaterialProp TerrainProperty
        {
            get { return m_prop; }
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            NTerrainMtlSetup mtl = m_trn.GetMaterial();

            //-- 检测所有输入数据，是否存在错误
            bool ok = true;
            string errorMsg = "error : ";
            if (!m_prop.BasicTexture.IsValid())
            {
                errorMsg += "基本材质模式贴图路径为空; ";
                ok = false;
            }

            //--
            if (ok)
            {
                mtl.Create(m_prop.BasicTexture, m_prop.AlphaMapWidth, m_prop.AlphaMapHeight);

                this.Close();
            }
            else
            {
                NexusEditor.Program.ShowError(errorMsg);
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
