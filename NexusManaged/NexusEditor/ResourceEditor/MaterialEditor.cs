using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEditor.EditorMain;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
	public partial class MaterialEditor : Form
	{
		private NResourceLoc m_Location;					//文件的保存路径
		private ResourcePreview			m_preview;			// 预览窗口控件
		private NResourceStaticMesh		m_resStaticMesh;    // 用于材质预览的ResStaticMesh
		private NMtlBase m_Material;		//编辑用材质
		private NShapeComponent m_Sphere;	//预览用球体
		private bool m_finalClose;
		private bool m_realTime;

		public MaterialEditor()
		{
			InitializeComponent();

			m_finalClose = false;
			m_realTime = false;

			m_Location = new NResourceLoc("","");

			m_preview = new ResourcePreview("MaterialPreview_LV");
			this.splitContainer1.Panel1.Controls.Add(m_preview);

			//新建材质
			NResourceLoc resLoc = new NResourceLoc("engine_data", "material/standard.hlsl");
			NMtlStatic mtl = new NMtlStatic(resLoc.ToString());
			mtl.DefaultCreate(resLoc);
			m_Material = mtl;
			MaterialProperty mtlProp = new MaterialProperty(m_Material);
			mtlProp.ApplyChange();
			this.propertyGridMtl.SelectedObject = mtlProp;
			LoadSphere();
		}

		public new void Close()
		{
			if (m_resStaticMesh != null)
			{
				m_resStaticMesh.Dispose();
				m_resStaticMesh = null;
			}
			m_Sphere = null;
			m_preview.Dispose();
			m_finalClose = true;
			base.Close();
		}

		private void timerViewUpdate_Tick(object sender, EventArgs e)
		{
			if (m_preview.ViewRenderMode == ERenderMode.Lit)
			{
				m_preview.AnimLight();
			}
			else
			{
				if (m_realTime)
				{
					m_preview.Refresh();
				}
			}			
		}

		private void MaterialEditor_FormClosing(object sender, FormClosingEventArgs e)
		{
			if (!m_finalClose)
			{
				e.Cancel = true;

				this.Hide();
			}
		}

		public void LoadMaterial(string pkg, string file)
		{
			m_Location = new NResourceLoc(pkg, file);
			m_Material = NMtlBase.FromFile(m_Location);
			if (m_Material == null)
			{
				this.propertyGridMtl.SelectedObject = null;
				return;
			}
			m_Material.Name = m_Location.ToString();

			MaterialProperty mtlProp = new MaterialProperty(m_Material);
			mtlProp.ApplyChange();
			this.propertyGridMtl.SelectedObject = mtlProp;
			if(m_Sphere==null)
			{
				LoadSphere();
			}
			if (m_Material != null) m_Sphere.SetMaterial(m_Material);
			m_preview.Refresh();
		}

		//加载静态模型并将当前材质赋给模型
		public void LoadStaticMesh(string pkg, string file)
		{
			try
			{
				using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
				{
					NResourceLoc resLoc = new NResourceLoc(pkg, file);
					m_resStaticMesh = NResourceManager.Instance.LoadStaticMesh(
						resLoc,
						EResourceIOMode.Block,
						EResourceIOPriority.Normal
						);
					for (int i = 0; i < m_resStaticMesh.GetNumLOD(); ++i)
					{
						for (int j = 0; j < m_resStaticMesh.GetNumSection(i); ++j)
						{
							m_resStaticMesh.ImportSetMaterial(i, j, m_Material);
						}
					}
					m_preview.ShowStaticMesh(m_resStaticMesh);
					m_Sphere = null;
				}
			}
			catch (System.Exception ex)
			{
				NexusEditor.Program.ShowException(ex, "Load Static Mesh FAILED");
			}
		}

		public void	LoadSphere()
		{			
			m_Sphere = m_preview.ShowSphereShapeComponent(500, 18, 18);
			if(m_Material!=null)	m_Sphere.SetMaterial(m_Material);
			m_resStaticMesh = null;
		}

		private void buttonApplyMtlProperty_Click(object sender, EventArgs e)
		{
			MaterialProperty mtlProp = this.propertyGridMtl.SelectedObject as MaterialProperty;
			if (mtlProp != null)
			{
				mtlProp.ApplyChange();
				m_preview.Refresh();
			}
		}

		private void btnCreateMtl_Click(object sender, EventArgs e)
		{
			try
			{
				MaterialProperty mtlProp = this.propertyGridMtl.SelectedObject as MaterialProperty;
				if (mtlProp != null)
				{
					NMtlStatic mtl = new NMtlStatic(mtlProp.Name);
					mtl.DefaultCreate(mtlProp.ShaderLoc);

					mtlProp.BindMaterial(mtl);
					this.propertyGridMtl.SelectedObject = mtlProp;
					m_Material = mtl;

					if (m_Sphere == null)
					{
						LoadSphere();
					}
					if (m_Material != null) m_Sphere.SetMaterial(m_Material);
					m_preview.Refresh();
				}
			}
			catch (System.Exception ex)
			{
				NexusEditor.Program.ShowException(ex, "Create Material FAILED");
			}    
		}

		private void toolStripButton1_Click(object sender, EventArgs e)
		{
			//新建材质
			NResourceLoc resLoc = new NResourceLoc("engine_data", "material/standard.hlsl");
			NMtlStatic mtl = new NMtlStatic(resLoc.ToString());
			mtl.DefaultCreate(resLoc);
			m_Material = mtl;
			MaterialProperty mtlProp = new MaterialProperty(m_Material);
			mtlProp.ApplyChange();
			this.propertyGridMtl.SelectedObject = mtlProp;
			LoadSphere();
		}

		private void toolStripButtonSave_Click(object sender, EventArgs e)
		{
			//另存材质
			if (m_Material == null) return;
			using (VirtualFileDialog dlg = new VirtualFileDialog(false, "", "mtl"))
			{
				dlg.Text = "Save Material ...";
				if (dlg.ShowDialog(this) == DialogResult.OK)
				{
					m_Location = dlg.GetResourceLocation();
					m_Material.SaveToFile(m_Location);
				}
			}
		}

		private void toolStripButton2_Click(object sender, EventArgs e)
		{
			//打开现有的材质文件
			using (VirtualFileDialog dlg = new VirtualFileDialog(true,"","mtl"))
			{
				dlg.Text = "Load Material ...";
				if (dlg.ShowDialog(this) == DialogResult.OK)
				{
					m_Location = dlg.GetResourceLocation();
					LoadMaterial(m_Location.PackageName, m_Location.FileName);
				}
			}
		}

		private void propertyGridMtl_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
		{
			if (e.ChangedItem.Label == "ShaderLoc") return;
			MaterialProperty mtlProp = this.propertyGridMtl.SelectedObject as MaterialProperty;
			if (mtlProp != null)
			{
				mtlProp.ApplyChange();
				m_preview.Refresh();
			}
		}

		private void toolBtnLit_Click(object sender, EventArgs e)
		{
			m_preview.ViewRenderMode = ERenderMode.Lit;
		}

		private void toolBtnUnlit_Click(object sender, EventArgs e)
		{
			m_preview.ViewRenderMode = ERenderMode.Unlit;
		}

		private void toolBtnWireframe_Click(object sender, EventArgs e)
		{
			m_preview.ViewRenderMode = ERenderMode.Wireframe;
		}

		private void toolBtnZoomExtents_Click(object sender, EventArgs e)
		{
			m_preview.ZoomExtents();
		}

		private void toolStripButton3_Click(object sender, EventArgs e)
		{
			try
			{
				LoadStaticMesh(NLevelEditorEngine.Instance.CurrentResource.PackageName,NLevelEditorEngine.Instance.CurrentResource.FileName);
			}
            catch (System.Exception ex)
			{
                Program.ShowException(ex, "Load Static Mesh Error!");
			}
		}

		private void realTimeMode_Click(object sender, EventArgs e)
		{
			m_realTime = realTimeMode.Checked;
		}

		private void toolStripButton4_Click(object sender, EventArgs e)
		{
			//保存到文件
			if(m_Location.IsValid())
			{
				m_Material.SaveToFile(m_Location);
			}
			else
			{
				toolStripButtonSave_Click(sender, e);
			}
		}
	}
}
