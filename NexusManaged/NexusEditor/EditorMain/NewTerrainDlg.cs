using System;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.EditorMain
{
    /// <summary>
    /// 创建Terrain时，设定创建参数的对话框
    /// </summary>
    public partial class NewTerrainDlg : Form
    {
        private NETerrainProp m_prop;

        public NewTerrainDlg()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 响应Form Load事件，指定参数对象
        /// </summary>        
        private void NewTerrainDlg_Load(object sender, EventArgs e)
        {
            m_prop = new NETerrainProp();
            this.propertyGridTrn.SelectedObject = m_prop;
        }

        /// <summary>
        /// 参数对象属性
        /// </summary>
        public NETerrainProp TerrainProperty
        {
            get { return m_prop; }
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            if (checkBoxCenter.Checked)
            {
                float trnW = m_prop.Width * m_prop.Scale.x;
                float trnH = m_prop.Height * m_prop.Scale.z;
                Vector3 pos = new Vector3(-trnW * 0.5f, m_prop.Position.y, -trnH * 0.5f);
                m_prop.Position = pos;
            }
        }
    }

}
