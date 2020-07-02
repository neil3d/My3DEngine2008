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
	public partial class CreateDecoLayerDialog : Form
	{
		public CreateDecoLayerDialog()
		{
			InitializeComponent();
			comboBox1.SelectedIndex = 0;
			NTerrainDecoSetup deco= NLevelEditorEngine.Instance.TerrainEd.GetTerrainBinded().GetDeco();
			for(int i=1;i<1024;++i)
			{
				string name = "DecoLayer_" + i.ToString();
				if (deco.GetLayer(name) != null) continue;
				textBox2.Text = name;
				break;
			}
		}

		public DecoLayerType LayerType
		{
			get
			{
				if(comboBox1.SelectedIndex==0)
				{
					return DecoLayerType.GrassDecoLayer;
				}
				else
				{
					return DecoLayerType.StaticMeshDecoLayer;
				}
			}
		}

		public uint GridSize
		{
			get
			{
				return Convert.ToUInt32(textBox1.Text);
			}
		}

		public string LayerName
		{
			get
			{
				return textBox2.Text;
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
			NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
			if(trnEd.GetTerrainBinded().GetDeco().GetLayer(textBox2.Text)!=null)
			{
				MessageBox.Show("当前名称的图层已经存在，请重新输入.","错误");
				textBox2.SelectAll();
			}
			else
			{
				DialogResult = DialogResult.OK;
				this.Close();
			}
		}
	}
}
