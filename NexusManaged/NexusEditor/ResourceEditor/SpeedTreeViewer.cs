using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// SpeedTree模型查看窗口
    /// </summary>
    public partial class SpeedTreeViewer : Form
    {
        private ResourcePreview m_preview;
        private bool m_finalClose;

        public SpeedTreeViewer()
        {
            InitializeComponent();

            m_finalClose = false;

            //--
            m_preview = new ResourcePreview("SpeedTreePreview_LV");
            this.splitContainer1.Panel1.Controls.Add(m_preview);
        }

        public void ShowSpt(string pkg, string path)
        {
            NResourceLoc loc = new NResourceLoc(pkg, path);
            m_preview.LoadSpeedTree(loc);            
        }

        private void SpeedTreeViewer_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_finalClose)
            {
                e.Cancel = true;
                this.Hide();
            }
        }

        public new void Close()
        {         
            m_preview.Dispose();
            this.FormClosing -= new System.Windows.Forms.FormClosingEventHandler(this.SpeedTreeViewer_FormClosing);
            base.Close();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            // show speed wind effect
            m_preview.Refresh();
        }

        private void SpeedTreeViewer_VisibleChanged(object sender, EventArgs e)
        {
            // 窗口可见时启动timer，更新动画
            this.timer1.Enabled = this.Visible;
        }

        private void toolStripTextBoxLOD_Leave(object sender, EventArgs e)
        {
            string valTxt = this.toolStripTextBoxLOD.Text;
            if (valTxt.Length > 0)
            {
                m_preview.SetViewLODBias(float.Parse(valTxt));
            }
        }
    }
}
