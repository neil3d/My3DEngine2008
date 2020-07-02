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
    partial class StaticMeshEdOptionsDlg : Form
    {
        public StaticMeshEdOptionsDlg(ResStaticMeshImportDefault importDefault)
        {
            InitializeComponent();

            this.propertyGridImportDefault.SelectedObject = importDefault;
        }

        private void StaticMeshEdOptionsDlg_Load(object sender, EventArgs e)
        {

        }
    }
}
