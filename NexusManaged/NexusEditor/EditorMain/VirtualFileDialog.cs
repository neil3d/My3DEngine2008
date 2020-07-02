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
	public partial class VirtualFileDialog : Form
	{
		private bool m_OpenFileMode;
		private string m_DefaultExtName;

		public VirtualFileDialog(bool OpenFileMode,string DefaultFileName,string DefaultExtName)
		{
			m_OpenFileMode = OpenFileMode;
			m_DefaultExtName = DefaultExtName;
			InitializeComponent();
			textFile.Text = DefaultFileName;
		}

		public VirtualFileDialog()
		{
			m_OpenFileMode = true;
			m_DefaultExtName = "";
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

		public void SetFileName(string filename)
		{
			textFile.Text = filename;
		}

		public NResourceLoc GetResourceLocation()
		{
			string fileName = textFile.Text;
			if (m_DefaultExtName.Length > 0 && fileName.LastIndexOf('.') == -1)
			{
				fileName = fileName + "." + m_DefaultExtName;
			}
			NResourceLoc loc = new NResourceLoc(txtPackage.Text, txtFolder.Text + "\\" + fileName);
			return loc;
		}

		private void resMgrLink_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
		{
			SetResourceLocation(NLevelEditorEngine.Instance.CurrentFolder);
		}

		private void VirtualFileDialog_Load(object sender, EventArgs e)
		{
			this.resMgrLink.Text = "Current Folder : " +
				NLevelEditorEngine.Instance.CurrentFolder.ToString();
		}

		private void btnOK_Click(object sender, EventArgs e)
		{
			if(!m_OpenFileMode)
			{
				NResourceLoc loc = GetResourceLocation();
				if(NEngine.Instance().FileSystem.IsFileExist(loc.PackageName,loc.FileName))
				{
					DialogResult rst = MessageBox.Show("当前名称的文件已经存在，是否覆盖？", "警告", MessageBoxButtons.YesNo);
					if(rst!=DialogResult.Yes)
					{
						return;
					}
					else
					{
						DialogResult = DialogResult.OK;
						this.Close();
					}
				}
				else
				{
					DialogResult = DialogResult.OK;
					this.Close();
				}
			}
			else
			{
				DialogResult = DialogResult.OK;
				this.Close();
			}
		}
	}
}
