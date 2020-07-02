using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace NexusEditor.UIEditor
{
    public partial class UISetWindowSize : Form
    {
        public UISetWindowSize()
        {
            InitializeComponent();
        }

        public float WindowWidth
        {
            get { return float.Parse(textBoxWidth.Text.Trim()); }
            set
            {
                textBoxWidth.Text = value.ToString();
            }
        }

        public float WindowHeight
        {
            get { return float.Parse(textBoxHeight.Text.Trim()); }
            set
            {
                textBoxHeight.Text = value.ToString();
            }
        }
    }
}
