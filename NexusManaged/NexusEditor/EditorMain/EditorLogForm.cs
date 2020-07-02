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
    public partial class EditorLogForm : Form, NexusEngine.NEditorLog
    {
        public EditorLogForm()
        {
            InitializeComponent();
        }

        public void WriteString(string txt)
        {
            this.textBoxLog.AppendText(txt);            
        }

        public void WriteString(NexusEngine.ELogType t, string txt)
        {
            string log = string.Format("{0} : {1}",
                t.ToString(), txt);
            this.textBoxLog.AppendText(log);
        }

        private void EditorLogForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }
    }
}
