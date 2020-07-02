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
    }

}
