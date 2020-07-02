using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace NexusEditor.EditorMain
{
    /// <summary>
    /// 将引擎的Log信息显示到一个窗口中
    /// </summary>
    public partial class EditorLogForm : Form
    {
        public EditorLogForm()
        {
            InitializeComponent();
        }

        public void WriteString(NexusEngine.LogType t, string txt)
        {
            try
            {
                string log = string.Format("{0} : {1}{2}", t.ToString(), txt, Environment.NewLine);
                this.textBoxLog.AppendText(log);
            }
            finally
            {

            }
        }

        private void EditorLogForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                this.Hide();
            }
        }
    }
}
