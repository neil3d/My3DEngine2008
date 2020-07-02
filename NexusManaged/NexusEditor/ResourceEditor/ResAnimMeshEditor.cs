using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 显示/编辑顶点变形动画Mesh资源
    /// </summary>
    public partial class ResAnimMeshEditor : Form
    {
        private ResourcePreview m_preview;  // 预览窗口控件
        private NResourceAnimMesh m_resAnimMesh;
        private ResAnimMeshProperty m_prop;
        private bool m_animPlaying;
        private bool m_finalClose;

        public ResAnimMeshEditor()
        {
            InitializeComponent();
            m_finalClose = false;

            m_preview = new ResourcePreview("ResAnimMeshPreview_LV");
            this.splitContainerLeft.Panel1.Controls.Add(m_preview);

            m_animPlaying = false;
        }

        public new void Close()
        {
            if (m_resAnimMesh != null)
                m_resAnimMesh.Dispose();
            m_preview.Dispose();

            m_finalClose = true;
            base.Close();
        }

        private void ResAnimMeshEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_finalClose)
            {
                e.Cancel = true;
                this.Hide();
            }
        }

        private bool ResetResource(string confirmTxt)
        {
            if (m_resAnimMesh != null
                && confirmTxt.Length > 0)
            {
                if (DialogResult.OK !=
                    MessageBox.Show(this, confirmTxt, "Please Confirm", MessageBoxButtons.OKCancel))
                    return false;
            }

            //--
            m_preview.Reset();

            if (m_resAnimMesh != null)
            {
                m_resAnimMesh.Dispose();
                m_resAnimMesh = null;
            }
            m_prop = null;
            this.propertyGridMtl.SelectedObject = null;
            this.propertyGridRes.SelectedObject = null;
            this.comboBoxLOD.Items.Clear();
            this.comboBoxSection.Items.Clear();
            this.listBoxAnim.Items.Clear();
            this.propertyGridSeq.SelectedObject = null;

            return true;
        }

        private void CreateResource()
        {
            ResetResource("");
            // 产生一个不重复的名称
            string resName = "NewAnimMesh";
            resName += DateTime.Now.ToFileTime();

            // 创建空白资源和内部数据
            m_resAnimMesh = NResourceManager.Instance.NewAnimMesh(resName);
            m_prop = new ResAnimMeshProperty(m_resAnimMesh);

            // 更新界面
            RefreshLOD();            
        }

        private void toolStripButtonRest_Click(object sender, EventArgs e)
        {
            this.ResetResource("Are You Sure RESET?");
        }

        private void toolStripButtonNew_Click(object sender, EventArgs e)
        {
            if (m_resAnimMesh != null)
            {
                if (DialogResult.OK !=
                    MessageBox.Show(this, "Are You Sure Create a NEW Anim Mesh?", "Please Confirm", MessageBoxButtons.OKCancel))
                    return;
            }
            this.CreateResource();
        }

        private void toolStripButtonSave_Click(object sender, EventArgs e)
        {
            if (m_resAnimMesh != null)
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

                    string saveFileName = string.Format("{0}/{1}.nam",
                        loc.FileName, m_prop.Name);
                    loc.FileName = saveFileName;
                }

                string confirmTxt = string.Format("Save As [{1}]",
                    m_resAnimMesh.Name, loc.ToString());

                if (MessageBox.Show(this, confirmTxt, "Save Anim Mesh", MessageBoxButtons.OKCancel)
                    == DialogResult.OK)
                {
                    using(NWaitCursor wc = new NWaitCursor(this))
                        m_resAnimMesh.SaveToFile(loc, this.toolBtnXML.Checked);                    
                }
            }// end of if
            else
            {
                NexusEditor.Program.ShowError("Anim Mesh Resource Save FAILED!");
            }
        }

        private void toolStripButtonImport_Click(object sender, EventArgs e)
        {
            try
            {
                int maxLOD = 0;
                if (m_resAnimMesh != null)
                {
                    maxLOD = m_resAnimMesh.GetNumLOD();
                }

                ImportLODDlg dlg = new ImportLODDlg(maxLOD);
                dlg.Text = "Import Anim Mesh LOD";
                dlg.FileFilter = "All Support Files(*.fak;*.md2)|*.fak;*.md2|F3D Anim Mesh file(*.fak)|*.fak|Quake2 Anim Mesh file(*.md2)|*.md2";
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    if (m_resAnimMesh == null)
                    {
                        CreateResource();
                    }

                    //-- 使得Resource的名称与LOD 0的文件名相同
                    if (dlg.SelectedLOD == 0)
                    {
                        NFileEntity fileName = new NFileEntity();
                        fileName.path = dlg.SelectedFile;
                        fileName.path = fileName.path.Replace('\\', '/');

                        m_prop.Name = fileName.FileNameNoExtension;
                    }

                    using (NWaitCursor wc = new NWaitCursor(this))
                    {
                        m_resAnimMesh.ImportLOD(dlg.SelectedLOD, dlg.SelectedFile);
                        RefreshLOD();
                    }
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Import Static Mesh LOD FAILED!");
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

        private void RefreshLOD()
        {
            this.propertyGridRes.SelectedObject = m_prop;
            ShowCurrentAnim(); // 必须重置Preview

            this.comboBoxLOD.Items.Clear();            
            if (m_resAnimMesh != null
                && m_resAnimMesh.GetNumLOD() > 0)
            {
                //-- LOD ComboBox   
                for (int i = 0; i < m_resAnimMesh.GetNumLOD(); i++)
                    this.comboBoxLOD.Items.Add(i.ToString());
                this.comboBoxLOD.SelectedIndex = 0;

                //-- Sequence list
                this.listBoxAnim.Items.Clear();
                int numSeq = m_resAnimMesh.GetNumSequence();
                float seqLen;
                string seqName;
                for (int i = 0; i < numSeq; i++ )
                {
                    seqName = m_resAnimMesh.GetSequenceInfo(i, out seqLen);
                    this.listBoxAnim.Items.Add(seqName);
                }
                this.listBoxAnim.SelectedIndex = 0;
                
            }// end of if
        }

        private void comboBoxLOD_SelectedIndexChanged(object sender, EventArgs e)
        {
            int lod = this.comboBoxLOD.SelectedIndex;
            this.comboBoxSection.Items.Clear();
            int numSec = m_resAnimMesh.GetNumSection(lod);

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

            NMaterial mtl = m_resAnimMesh.GetMaterial(lod, sec) as NMaterial;

            MaterialProperty mtlProp = new MaterialProperty(mtl);

            //-- apply default material setting
            if (mtl == null)
            {
                mtlProp.Name = string.Format("Mtl_L{0}S{1}", lod, sec);
                //mtlProp.TemplateLoc = m_importDefault.MaterialTemplateRes;
            }

            this.propertyGridMtl.SelectedObject = mtlProp;
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
                    m_resAnimMesh.ImportSetMaterial(lod, sec, mtl);

                    mtlProp.BindMaterial(mtl);
                    this.propertyGridMtl.SelectedObject = mtlProp;
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Create Material FAILED");
            }   
        }

        private void buttonApplyMtlProperty_Click(object sender, EventArgs e)
        {
            MaterialProperty mtlProp = this.propertyGridMtl.SelectedObject as MaterialProperty;
            if (mtlProp != null)
            {
                mtlProp.ApplyChange();
            }
        }

        private void ResAnimMeshEditor_Load(object sender, EventArgs e)
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

        public void LoadAnimMesh(string pkg, string file)
        {
            try
            {
                using (NWaitCursor wc = new NWaitCursor(this))
                {
                    NResourceLoc resLoc = new NResourceLoc(pkg, file);
                    m_resAnimMesh = NResourceManager.Instance.LoadAnimMesh(
                        resLoc,
                        EResourceIOMode.Block,
                        EResourceIOPriority.Normal
                        );
                    m_prop = new ResAnimMeshProperty(m_resAnimMesh);

                    this.propertyGridRes.SelectedObject = m_prop;
                    ShowCurrentAnim();

                    RefreshLOD();
                }
            }
            catch (System.Exception ex)
            {
                Program.ShowException(ex, "Load Anim Mesh FAILED!");
            }            
        }

        private void listBoxAnim_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_resAnimMesh != null
                && m_resAnimMesh.GetNumLOD() > 0)
            {
                float len;
                string name = m_resAnimMesh.GetSequenceInfo(
                    this.listBoxAnim.SelectedIndex, out len);
                this.propertyGridSeq.SelectedObject = new AnimSequenceInfo(name, len);

                ShowCurrentAnim();
            }
        }

        private void timerTick_Tick(object sender, EventArgs e)
        {
            if (m_animPlaying)
                m_preview.Refresh();
        }

        private void btnPlay_Click(object sender, EventArgs e)
        {
            m_animPlaying = !m_animPlaying;
            this.timerTick.Enabled = m_animPlaying;

            if (m_animPlaying)
                this.btnPlay.ImageKey = "play";
            else
                this.btnPlay.ImageKey = "stop";

            ShowCurrentAnim();
        }

        private void splitContainerLeft_Panel2_Resize(object sender, EventArgs e)
        {
            Size oldSize = this.trackBarTime.Size;
            Size panelSize = this.splitContainerLeft.Panel2.Size;

            oldSize.Width = panelSize.Width - 26 * 2 - 16;
            this.trackBarTime.Size = oldSize;
        }

        private void ShowCurrentAnim()
        {
            string seqName = "";
            bool loop = this.checkBoxLoop.Checked;

            if (this.listBoxAnim.SelectedItem != null)
            {
                seqName = this.listBoxAnim.SelectedItem as string;
            }

            this.m_preview.ShowAnimMesh(m_resAnimMesh, seqName, loop);
        }

        private void checkBoxLoop_CheckedChanged(object sender, EventArgs e)
        {
            if (this.checkBoxLoop.Checked)
            {
                this.checkBoxLoop.ImageKey = "loop";
            }
            else
            {
                this.checkBoxLoop.ImageKey = "no_loop";
            }
        }

        private void btnTrnRemoveSeq_Click(object sender, EventArgs e)
        {
            if (this.listBoxAnim.SelectedIndex < 0)
                return;

            string seqName = this.listBoxAnim.SelectedItem as string;
            string text = string.Format("Remve Animation Sequence [{0}]?", seqName);
            if (MessageBox.Show(this, text, "Please Confirm", MessageBoxButtons.OKCancel)
                == DialogResult.OK)
            {
            }
            
        }

        private void toolBtnXML_Click(object sender, EventArgs e)
        {
            this.toolBtnXML.Checked = !this.toolBtnXML.Checked;
        }

        private void propertyGridMtl_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            m_preview.Refresh();
        }
    }
}
