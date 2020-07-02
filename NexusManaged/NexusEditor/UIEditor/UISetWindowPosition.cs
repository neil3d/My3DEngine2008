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
    public partial class UISetWindowPosition : Form
    {
        public UISetWindowPosition()
        {
            InitializeComponent();
        }

        public float LeftPosition
        {
            get { return float.Parse(textBoxLeft.Text.Trim()); }
            set
            {
                textBoxLeft.Text = value.ToString();
            }
        }

        public float TopPosition
        {
            get { return float.Parse(textBoxTop.Text.Trim()); }
            set
            {
                textBoxTop.Text = value.ToString();
            }
        }
    }
}
