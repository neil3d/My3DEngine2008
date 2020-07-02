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
    /// <summary>
    /// Static Mesh资源编辑器
    /// </summary>
    public partial class ResStaticMeshEditor : Form
    {
        private ResourcePreview m_preview;  // 预览窗口控件
        private ResStaticMeshProperty m_prop;   // 显示ResStaticMesh对象的数据到PropGrid
        private ResStaticMeshImportDefault m_importDefault; // 创建ResStaticMesh的一些默认设置,简化操作
        private NResourceStaticMesh m_resStaticMesh;    // 正在编辑的ResStaticMesh
        private bool m_finalClose;

        public ResStaticMeshEditor()
        {
            InitializeComponent();

            m_finalClose = false;

            m_importDefault = new ResStaticMeshImportDefault();
            m_preview = new ResourcePreview("ResStaticMeshPreview_LV");
            this.splitContainer1.Panel1.Controls.Add(m_preview);            
        }

        public new void Close()
        {
            if (m_resStaticMesh != null)
            {
                m_resStaticMesh.Dispose();
                m_resStaticMesh = null;
            }
            m_preview.Dispose();
            m_finalClose = true;
            base.Close();
        }

        private void ResStaticMeshEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_finalClose)
            {
                e.Cancel = true;

                if (ResetResource("Are You Sure CLOSE StaticMeshEd?"))
                    this.Hide();
            }
        }

        private void importToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                int maxLOD = 0;
                if (m_resStaticMesh != null)
                {
                    maxLOD = m_resStaticMesh.GetNumLOD();
                }

                ImportLODDlg dlg = new ImportLODDlg(maxLOD);
                dlg.Text = "Import Static Mesh LOD";
                dlg.FileFilter = "All Support Files(*.obj;*.fsm;*.fscn)|*.obj;*.fsm;*.fscn|Wavefront OBJ file(*.obj)|*.obj|F3D Static mesh file(*.fsm)|*.fsm|F3D Scene mesh file(*.fscn)|*.fscn";
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    if (m_resStaticMesh == null)
                    {
                        CreateResource();
                    }

                    //-- 使得Resource的名称与LOD 0的文件名相同
                    if( dlg.SelectedLOD == 0)
                    {
                        NFileEntity fileName = new NFileEntity();
                        fileName.path = dlg.SelectedFile;
                        fileName.path = fileName.path.Replace('\\', '/');

                        m_prop.Name = fileName.FileNameNoExtension;
                    }

                    using (NWaitCursor wc = new NWaitCursor(this))
                    {
                        m_resStaticMesh.ImportLOD(dlg.SelectedLOD, dlg.SelectedFile);
                        m_preview.ShowStaticMesh(m_resStaticMesh);
                        RefreshLOD();
                    }
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Import Static Mesh LOD FAILED!");
            }            
        }

        private void RefreshLOD()
        {
            this.propertyGridRes.SelectedObject = m_prop;

            this.comboBoxLOD.Items.Clear();            
            if (m_resStaticMesh != null
                && m_resStaticMesh.GetNumLOD() > 0)
            {
                //-- LOD ComboBox   
                for (int i = 0; i < m_resStaticMesh.GetNumLOD(); i++)
                    this.comboBoxLOD.Items.Add(i.ToString());
                this.comboBoxLOD.SelectedIndex = 0;              
            }// end of if
        }

        private void CreateResource()
        {
            ResetResource("");

            // 产生一个不重复的名称
            string resName = "NewStaticMesh";
            resName += DateTime.Now.ToFileTime();

            // 创建空白资源和内部数据
            m_resStaticMesh = NResourceManager.Instance.NewStaticMesh(resName);
            m_prop = new ResStaticMeshProperty(m_resStaticMesh);

            // 更新界面
            RefreshLOD();
            m_preview.ShowStaticMesh(m_resStaticMesh);
        }

        public void LoadStaticMesh(string pkg, string file)
        {
            try
            {
                using (NWaitCursor wc = new NWaitCursor(this))
                {
                    NResourceLoc resLoc = new NResourceLoc(pkg, file);
                    m_resStaticMesh = NResourceManager.Instance.LoadStaticMesh(
                        resLoc,
                        EResourceIOMode.Block,
                        EResourceIOPriority.Normal
                        );
                    m_prop = new ResStaticMeshProperty(m_resStaticMesh);

                    this.propertyGridRes.SelectedObject = m_prop;
                    m_preview.ShowStaticMesh(m_resStaticMesh);

                    RefreshLOD();
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Load Static Mesh FAILED");
            }            
        }

        private void btnCreateMtl_Click(object sender, EventArgs e)
        {
            try
            {
                MaterialProperty mtlProp = this.propertyGridMtl.SelectedObject as MaterialProperty;
                if (mtlProp != null)
                {
                    NResourceLoc tempLoc = mtlProp.TemplateLoc;                    
                    NMaterial mtl = new NMaterial(mtlProp.Name);
                    mtl.Create(tempLoc);

                    int lod = this.comboBoxLOD.SelectedIndex;
                    int sec = this.comboBoxSection.SelectedIndex;
                    m_resStaticMesh.ImportSetMaterial(lod, sec, mtl);

                    mtlProp.BindMaterial(mtl);
                    this.propertyGridMtl.SelectedObject = mtlProp;
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Create Material FAILED");
            }            
        }

        private void comboBoxLOD_SelectedIndexChanged(object sender, EventArgs e)
        {
            int lod = this.comboBoxLOD.SelectedIndex;
            this.comboBoxSection.Items.Clear();
            int numSec = m_resStaticMesh.GetNumSection(lod);

            for (int i = 0; i < numSec; i++)
            {
                this.comboBoxSection.Items.Add(i.ToString());
            }
            this.comboBoxSection.SelectedIndex = 0;
        }

        private void comboBoxSection_SelectedIndexChanged(object sender, EventArgs e)
        {
            int lod = this.comboBoxLOD.SelectedIndex;
            int sec = this.comboBoxSection.SelectedIndex;
            
            NMaterial mtl = m_resStaticMesh.GetMaterial(lod, sec) as NMaterial;

            MaterialProperty mtlProp = new MaterialProperty(mtl);

            //-- apply default material setting
            if (mtl == null)
            {
                mtlProp.Name = string.Format("Mtl_L{0}S{1}", lod, sec);
                mtlProp.TemplateLoc = m_importDefault.MaterialTemplateRes;
            }

            this.propertyGridMtl.SelectedObject = mtlProp;
        }
        

        private void buttonApplyMtlProperty_Click(object sender, EventArgs e)
        {
            MaterialProperty mtlProp = this.propertyGridMtl.SelectedObject as MaterialProperty;
            if (mtlProp != null)
            {
                mtlProp.ApplyChange();                
            }
        }

        private bool ResetResource(string confirmTxt)
        {
            if (m_resStaticMesh != null
                && confirmTxt.Length > 0)
            {
                if (DialogResult.OK !=
                    MessageBox.Show(this, confirmTxt, "Please Confirm", MessageBoxButtons.OKCancel))
                    return false;
            }

            //--
            m_preview.Reset();

            if (m_resStaticMesh != null)
            {
                m_resStaticMesh.Dispose();
                m_resStaticMesh = null;
            }
            m_prop = null;
            this.propertyGridMtl.SelectedObject = null;
            this.propertyGridRes.SelectedObject = null;
            this.comboBoxLOD.Items.Clear();
            this.comboBoxSection.Items.Clear();

            return true;
        }

        private void resetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.ResetResource("Are You Sure RESET?");
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_resStaticMesh != null)
            {
                NResourceLoc loc;

                if (m_prop.Name == m_prop.Location)
                {
                    loc = new NResourceLoc(m_prop.Name);
                }
                else
                {
                    loc = NLevelEditorEngine.Instance.CurrentFolder;
                    if (!loc.IsValid())
                    {
                        NexusEditor.Program.ShowError("Please Select a Folder in Resource Manager File System Tree!");
                        return;
                    }

                    string saveFileName = string.Format("{0}/{1}.nmdl",
                        loc.FileName, m_prop.Name);
                    loc.FileName = saveFileName;
                }

                string confirmTxt = string.Format("Save As [{1}]",
                    m_resStaticMesh.Name, loc.ToString());
                
                if (MessageBox.Show(this, confirmTxt, "Save Static Mesh", MessageBoxButtons.OKCancel)
                    == DialogResult.OK)
                {
                    using (NWaitCursor wc = new NWaitCursor(this))
                        m_resStaticMesh.SaveToFile(loc, this.toolBtnXML.Checked);
                }
            }// end of if
            else
            {
                NexusEditor.Program.ShowError("Static Mesh Resource Save FAILED!");                
            }
        }


        private void ResStaticMeshEditor_Load(object sender, EventArgs e)
        {
            NLevelEditorEngine.Instance.CurrentFolderChanged
                += new EventHandler(SaveLocationChanged);
            SaveLocationChanged(this, null);
        }


        private void SaveLocationChanged(object sender, EventArgs e)
        {
            NResourceLoc loc = NLevelEditorEngine.Instance.CurrentFolder;
            this.statusSaveLoc.Text = string.Format("Save Location [{0}]", loc.ToString());
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_resStaticMesh != null)
            {
                if (DialogResult.OK !=
                    MessageBox.Show(this, "Are You Sure Create a NEW Static Mesh?", "Please Confirm", MessageBoxButtons.OKCancel))
                    return;
            }

            CreateResource();
        }

        private void importDefautsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (StaticMeshEdOptionsDlg dlg = new StaticMeshEdOptionsDlg(m_importDefault))
            {
                dlg.ShowDialog(this);
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

        private void timerViewUpdate_Tick(object sender, EventArgs e)
        {
            if (m_preview.ViewRenderMode == ERenderMode.Lit)
            {
                m_preview.AnimLight();
            }
        }

        private void propertyGridMtl_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            m_preview.Refresh();
        }
    }
}
