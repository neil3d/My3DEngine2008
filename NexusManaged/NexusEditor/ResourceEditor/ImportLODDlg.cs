using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 导入Static Mesh LOD的对话框
    /// </summary>
    public partial class ImportLODDlg : Form
    {
        private string m_fileFilter;

        public string FileFilter
        {
            get { return m_fileFilter; }
            set { m_fileFilter = value; }
        }

        public ImportLODDlg(int maxLOD)
        {
            InitializeComponent();
            for (int i = 0; i <= maxLOD; i++)
            {
                this.comboBoxLOD.Items.Add(i);
            }
            this.comboBoxLOD.SelectedIndex = 0;
        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = m_fileFilter;
            dlg.RestoreDirectory = true;
            if (dlg.ShowDialog(this) == DialogResult.OK)
            {
                this.textBoxPath.Text = dlg.FileName;                
            }
        }

        public int SelectedLOD
        {
            get
            {
                return this.comboBoxLOD.SelectedIndex;
            }
        }

        public string SelectedFile
        {
            get 
            {
                return this.textBoxPath.Text;
            }
        }
    }
}
