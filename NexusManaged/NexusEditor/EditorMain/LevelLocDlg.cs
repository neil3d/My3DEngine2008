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
    public partial class LevelLocDlg : Form
    {
        public LevelLocDlg()
        {
            InitializeComponent();
        }

        public void SetResourceLocation(NResourceLoc loc)
        {
            if (loc.IsValid())
            {
                this.txtPackage.Text = loc.PackageName;
                this.txtFolder.Text = loc.FileName;
            }
            else
            {
                this.txtPackage.Text = "";
                this.txtFolder.Text = "";
            }
        }

        public NResourceLoc ResourceLocation
        {
			get
			{
				NResourceLoc loc = new NResourceLoc(txtPackage.Text, txtFolder.Text);
				return loc;
			}
        }

        private void resMgrLink_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            SetResourceLocation(NLevelEditorEngine.Instance.CurrentFolder);
        }

        private void LevelLocDlg_Load(object sender, EventArgs e)
        {
            this.resMgrLink.Text = "Current Folder : " +
                NLevelEditorEngine.Instance.CurrentFolder.ToString();
        }

    }
}
