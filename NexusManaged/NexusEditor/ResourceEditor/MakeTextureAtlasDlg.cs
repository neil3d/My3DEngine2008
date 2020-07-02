using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    public partial class MakeTextureAtlasDlg : Form
    {    
        public MakeTextureAtlasDlg()
        {
            InitializeComponent();
        }

        public void SetInputLoc(NResourceLoc loc)
        {
            textBoxFolder.Text = loc.ToString();
        }
        public void SetOutputLoc(NResourceLoc loc)
        {
            textBoxOutputFolder.Text = loc.ToString();
        }

        public void MakeIt()
        {
            NResourceLoc inputLoc = new NResourceLoc(textBoxFolder.Text);
            NResourceLoc outputLoc = new NResourceLoc(textBoxOutputFolder.Text);

            NLevelEditorEngine.Instance.MakeTextureAtlas(
                inputLoc,
                outputLoc,
                int.Parse(txtWidth.Text),
                int.Parse(txtHeight.Text)
                );
        }
    }
}
