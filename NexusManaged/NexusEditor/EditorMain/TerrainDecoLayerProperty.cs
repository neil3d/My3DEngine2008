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
	public partial class TerrainDecoLayerProperty : Form
	{
		public TerrainDecoLayerProperty()
		{
			InitializeComponent();
		}

		public PropertyGrid PropertyView
		{
			get
			{
				return propertyGrid;
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
			Close();
		}
	}
}
