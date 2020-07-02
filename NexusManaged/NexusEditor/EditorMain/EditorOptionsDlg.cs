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
    public partial class EditorOptionsDlg : Form
    {
        public EditorOptionsDlg()
        {
            InitializeComponent();

            EditorOptionsBasic basicOp = new EditorOptionsBasic();
            this.propertyGridBasic.SelectedObject = basicOp;

            this.propertyGridEngine.SelectedObject = NLevelEditorEngine.Instance.Config;
        }
    }
}
