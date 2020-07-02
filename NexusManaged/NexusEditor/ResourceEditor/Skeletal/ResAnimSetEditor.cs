using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;
using NexusEditor.EditorMain;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 骨骼动画集编辑器
    /// </summary>
    public partial class ResAnimSetEditor : Form
    {
        #region Fields
        /// <summary>
        /// 动画预览控件
        /// </summary>
        private ResResourceAnimSetPreview previewControl;

        /// <summary>
        /// 预览的骨骼模型资源
        /// </summary>
        private NResourceSkeletalMesh skeletalMeshPreview;
        private ResSkeletalMeshProperty skeletalMeshProperty;

        private NSkeletalAnimPlayer animPlayer;

        /// <summary>
        /// 预览的动画集资源
        /// </summary>
        private NResourceSkeletalAnimSet animSetPreview;

        private bool finalClose;

        /// <summary>
        ///  update timer
        /// </summary>
        private Timer updateTimer = new Timer();
        #endregion


        public ResAnimSetEditor()
        {
            InitializeComponent();

            finalClose = false;
            this.Load += new EventHandler(ResAnimSetEditor_Load);
            this.FormClosing += new FormClosingEventHandler(ResAnimSetEditor_FormClosing);

            previewControl = new ResResourceAnimSetPreview("ResAnimSetPreview_LV");
            this.splitContainerLeft.Panel1.Controls.Add(previewControl);

        }

        #region Properties
        public NResourceSkeletalMesh SkeletalMeshPreview
        {
            get { return skeletalMeshPreview; }
            set
            {
                skeletalMeshPreview = value;

                // 必须重置Preview
                previewControl.ResetSkeletalMeshResource(skeletalMeshPreview);

                // 重置动画播放器
                if (skeletalMeshPreview != null)
                {
                    animPlayer = previewControl.SkeletalMeshComponentPreview.ResetSkeletalAnimPlayer();
                    if (animPlayer != null && AnimSetPreview != null)
                    {
                        animPlayer.Init(AnimSetPreview, false);
                    }
                    skeletalMeshProperty = new ResSkeletalMeshProperty(skeletalMeshPreview);
                }
                else
                {
                    animPlayer = null;
                    skeletalMeshProperty = null;
                }

                // 更新界面
                RefreshLOD();
            }
        }

        public NResourceSkeletalAnimSet AnimSetPreview
        {
            get { return animSetPreview; }
            set
            {
                animSetPreview = value;

                if (animPlayer != null)
                {
                    animPlayer.Init(animSetPreview, false);
                }

                // Sequence list刷新界面
                RefreshAnimSequeneList();

                RefreshSkeletonTree();
            }
        }
        #endregion

        public new void Close()
        {
            if (updateTimer != null)
            {
                // close refresh timer
                updateTimer.Stop();
                updateTimer.Dispose();
                updateTimer = null;
            }

            if (SkeletalMeshPreview != null)
                SkeletalMeshPreview.Dispose();
            if (AnimSetPreview != null)
                AnimSetPreview.Dispose();
            previewControl.Dispose();

            finalClose = true;
            base.Close();
        }

        private void ResAnimSetEditor_Load(object sender, EventArgs e)
        {
            updateTimer.Interval = 10;
            updateTimer.Tick += new EventHandler(updateTimer_Tick);
            updateTimer.Start();

            NLevelEditorEngine.Instance.CurrentFolderChanged
                += new EventHandler(SaveLocationChanged);
            SaveLocationChanged(this, null);
        }

        private void SaveLocationChanged(object sender, EventArgs e)
        {
            NResourceLoc loc = NLevelEditorEngine.Instance.CurrentFolder;
            this.statusSaveLoc.Text = string.Format("Save Location [{0}]", loc.ToString());
        }

        private void ResAnimSetEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!finalClose)
            {
                e.Cancel = true;
                this.Hide();
            }
        }

        /// <summary>
        /// When the timer fires, refresh control
        /// </summary>
        private void updateTimer_Tick(object sender, EventArgs e)
        {
            previewControl.Invalidate();
        }

        private void ResetResource(bool showCheckDlg)
        {
            if (showCheckDlg &&
                DialogResult.OK !=
                MessageBox.Show(this,
                NexusEditor.Properties.Resources.ResetChecking,
                "Please Confirm", MessageBoxButtons.OKCancel))
            {
                return;
            }

            previewControl.Reset();

            // 重置当前资源
            if (AnimSetPreview != null)
            {
                AnimSetPreview.Dispose();
                AnimSetPreview = null;
            }
            if (SkeletalMeshPreview != null)
            {
                SkeletalMeshPreview.Dispose();
                SkeletalMeshPreview = null;
            }
            skeletalMeshProperty = null;

            // 重置当前界面
            this.propertyGridMesh.SelectedObject = null;
            this.propertyGridMtl.SelectedObject = null;
            this.comboBoxLOD.Items.Clear();
            this.comboBoxSection.Items.Clear();
            this.listBoxAnim.Items.Clear();
            this.propertyGridSeq.SelectedObject = null;
        }

        /// <summary>
        /// Sequence list刷新界面
        /// </summary>
        private void RefreshAnimSequeneList()
        {
            this.listBoxAnim.Items.Clear();
            if (AnimSetPreview == null)
                return;

            this.listBoxAnim.BeginUpdate();
            UInt32 numSequence = AnimSetPreview.SequenceCount;
            for (UInt32 i = 0; i < numSequence; i++)
            {
                NSkeletalAnimSequence sequence = AnimSetPreview.FindSequence(i);
                if (sequence == null)
                    continue;

                ListViewItem lvi = new ListViewItem(sequence.ToString());
                lvi.Tag = sequence;
                this.listBoxAnim.Items.Add(lvi);
            }
            this.listBoxAnim.EndUpdate();
        }

        private void RefreshSkeletonTree()
        {
            this.treeViewSkeletal.BeginUpdate();
            this.treeViewSkeletal.Nodes.Clear();

            if (AnimSetPreview != null)
            {
                TreeNode rootNode = this.treeViewSkeletal.Nodes.Add("root");
                int numBone = AnimSetPreview.GetBoneCount();
                for (int i = 0; i < numBone; i++ )
                {
                    string boneName = AnimSetPreview.GetBoneName(i);
                    string parentName = AnimSetPreview.GetBoneParentName(i);
                    TreeNode parentNode = FindTreeNode(rootNode, parentName);
                    if (parentNode != null)
                        parentNode.Nodes.Add(boneName);
                    else
                        rootNode.Nodes.Add(boneName);
                }
            }

            this.treeViewSkeletal.ExpandAll();
            this.treeViewSkeletal.EndUpdate();
        }

        private TreeNode FindTreeNode(TreeNode node, string txt)
        {
            if (node.Text == txt)
                return node;
            foreach (TreeNode child in node.Nodes)
            {
                TreeNode ret =FindTreeNode(child, txt);
                if (null != ret)
                    return ret;
            }

            return null;
        }

        private void RefreshLOD()
        {            
            this.propertyGridMesh.SelectedObject = skeletalMeshProperty;

            this.comboBoxLOD.Items.Clear();
            if (SkeletalMeshPreview != null
                && SkeletalMeshPreview.GetNumLOD() > 0)
            {
                //-- LOD ComboBox   
                for (int i = 0; i < SkeletalMeshPreview.GetNumLOD(); i++)
                    this.comboBoxLOD.Items.Add(i.ToString());
                this.comboBoxLOD.SelectedIndex = 0;
            }// end of if
        }

        private void SaveCurrentSkeletalMesh()
        {
            //-- 检测资源是否可用
            if (SkeletalMeshPreview == null)
            {
                NexusEditor.Program.ShowError("Skeletal Mesh 资源尚未创建!");
                return;
            }

            using (VirtualFileDialog dlg = new VirtualFileDialog(false, "", "skm"))
            {
                //-- 设置dialogue属性
                dlg.SetResourceLocation(NLevelEditorEngine.Instance.CurrentFolder);
                dlg.Text = "保存Skeletal Mesh ...";
                int i = skeletalMeshProperty.Name.LastIndexOf('\\');
                if (i == -1)
                {
                    i = skeletalMeshProperty.Name.LastIndexOf('/');
                }
                if (i >= 0)
                {
                    dlg.SetFileName(skeletalMeshProperty.Name.Substring(i + 1, skeletalMeshProperty.Name.Length - i - 1));
                }
                else
                {
                    dlg.SetFileName(skeletalMeshProperty.Name);
                }

                //-- 请用户确认
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    NResourceLoc loc = dlg.GetResourceLocation();

                    using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                        SkeletalMeshPreview.SaveToFile(loc, this.toolBtnXML.Checked);
                }

            }// end of using(dg)         
        }

        private void SaveCurrentAnimSet()
        {
            if (AnimSetPreview == null)
            {
                NexusEditor.Program.ShowError("Anim Set 资源尚未创建!");
            }

            using (VirtualFileDialog dlg = new VirtualFileDialog(false, "", "animset"))
            {
                //-- 设置dialogue属性
                dlg.SetResourceLocation(NLevelEditorEngine.Instance.CurrentFolder);
                dlg.Text = "保存Anim Set ...";

                //-- 请用户确认
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    NResourceLoc loc = dlg.GetResourceLocation();

                    using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                        AnimSetPreview.SaveToFile(loc, this.toolBtnXML.Checked);
                }
            }            
        }

        private void ImportPSK()
        {
            try
            {
                int maxLOD = 0;
                if (SkeletalMeshPreview != null)
                {
                    maxLOD = SkeletalMeshPreview.GetNumLOD();
                }

                ImportLODDlg dlg = new ImportLODDlg(maxLOD);
                dlg.Text = "Import Skeletal Mesh LOD";
                dlg.FileFilter = "ActorX Skeletal Mesh file(*.PSK)|*.PSK";
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    if (SkeletalMeshPreview == null)
                    {
                        // 产生一个不重复的名称
                        string resName = "NewSkeletalMesh";
                        resName += DateTime.Now.ToFileTime();

                        // 创建空白资源和内部数据
                        SkeletalMeshPreview = NResourceManager.Instance.NewSkeletalMesh(resName);
                    }

                    //-- 使得Resource的名称与LOD 0的文件名相同
                    if (skeletalMeshProperty != null && dlg.SelectedLOD == 0)
                    {
                        NFileEntity fileName = new NFileEntity();
                        fileName.path = dlg.SelectedFile;
                        fileName.path = fileName.path.Replace('\\', '/');

                        skeletalMeshProperty.Name = fileName.FileNameNoExtension;
                    }

                    using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                    {
                        try
                        {
                            SkeletalMeshPreview.ImportSkeletalMeshLOD(dlg.SelectedLOD, dlg.SelectedFile);
                            SkeletalMeshPreview.PostEditChange(true);
                            // 必须重置Preview
                            previewControl.ResetSkeletalMeshResource(skeletalMeshPreview);
                        }
                        catch (System.Exception ex)
                        {
                            NexusEditor.Program.ShowException(ex, "Import Skeletal Mesh LOD FAILED!");
                        }

                        // 更新界面
                        RefreshLOD();
                    }
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Import Skeletal Mesh LOD FAILED!");
            }
        }

        private void ImportPSA()
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "ActorX PSA File(*.PSA)|*.PSA";
            dlg.RestoreDirectory = true;
            if (dlg.ShowDialog(this) == DialogResult.OK)
            {
                if (AnimSetPreview == null)
                {
                    // 产生一个不重复的名称
                    string resName = "NewAnimSet";
                    resName += DateTime.Now.ToFileTime();

                    // 创建空白资源和内部数据
                    AnimSetPreview = NResourceManager.Instance.NewSkeletalAnimSet(resName);
                }

                using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                {
                    try
                    {
                        AnimSetPreview.ImportSkeletalAnimSequence(dlg.FileName);
                        AnimSetPreview.PostEditChange(true);

                        if (animPlayer != null)
                        {
                            animPlayer.Init(AnimSetPreview, true);
                        }
                    }
                    catch (System.Exception ex)
                    {
                        NexusEditor.Program.ShowException(ex, "Import Anim Set LOD FAILED!");
                    }

                    // 更新界面
                    RefreshAnimSequeneList();
                    RefreshSkeletonTree();
                }
            }
        }

        public void LoadAnimSet(string pkg, string file)
        {
            try
            {
                using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                {
                    NResourceLoc resLoc = new NResourceLoc(pkg, file);
                    AnimSetPreview = NResourceManager.Instance.LoadSkeletalAnimSet(
                        resLoc,
                        EResourceIOMode.Block,
                        EResourceIOPriority.Normal
                        );
                    if (animPlayer != null)
                    {
                        animPlayer.Init(AnimSetPreview, true);
                    }
                }
            }
            catch (System.Exception ex)
            {
                Program.ShowException(ex, "Load AnimSet Failed!");
            }
        }

        public void LoadSkeletalMesh(string pkg, string file)
        {
            try
            {
                using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                {
                    NResourceLoc resLoc = new NResourceLoc(pkg, file);
                    SkeletalMeshPreview = NResourceManager.Instance.LoadSkeletalMesh(
                        resLoc,
                        EResourceIOMode.Block,
                        EResourceIOPriority.Normal
                        );
                }
            }
            catch (System.Exception ex)
            {
                Program.ShowException(ex, "Load Skeletal Mesh Failed!");
            }
        }


        private void toolStripButtonRest_Click(object sender, EventArgs e)
        {
            ResetResource(true);
        }

        private void toolStripButtonSaveMesh_Click(object sender, EventArgs e)
        {
            SaveCurrentSkeletalMesh();
        }

        private void toolStripButtonSaveAnim_Click(object sender, EventArgs e)
        {
            SaveCurrentAnimSet();
        }

        private void toolStripImportPSK_Click(object sender, EventArgs e)
        {
            ImportPSK();
        }

        private void toolStripImportPSA_Click(object sender, EventArgs e)
        {
            ImportPSA();
        }

        private void toolBtnLit_Click(object sender, EventArgs e)
        {
            previewControl.ViewRenderMode = ERenderMode.Lit;
        }

        private void toolBtnUnlit_Click(object sender, EventArgs e)
        {
            previewControl.ViewRenderMode = ERenderMode.Unlit;
        }

        private void toolBtnWireframe_Click(object sender, EventArgs e)
        {
            previewControl.ViewRenderMode = ERenderMode.Wireframe;
        }

        private void toolBtnZoomExtents_Click(object sender, EventArgs e)
        {
            previewControl.ZoomExtents();
        }

        private void toolBtnXML_Click(object sender, EventArgs e)
        {
            this.toolBtnXML.Checked = !this.toolBtnXML.Checked;
        }

        private void PlayAnimByCurrentPosition()
        {
            if (animPlayer == null)
                return;

            // 设置起始时间
            float rate = 1.0f;
            try
            {
                rate = (float)(this.trackBarTime.Value - this.trackBarTime.Minimum) / (float)(this.trackBarTime.Maximum - this.trackBarTime.Minimum);
            }
            finally
            {
            }
            animPlayer.PlayAnim(this.checkBoxLoop.Checked, 1.0f, rate * animPlayer.CurrentAnimLength);
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

            PlayAnimByCurrentPosition();
        }

        private void btnPlay_Click(object sender, EventArgs e)
        {
            if (this.btnPlay.ImageKey.Equals("play"))
            {
                // 设置起始时间
                PlayAnimByCurrentPosition();

                this.btnPlay.ImageKey = "stop";
            }
            else
            {
                this.btnPlay.ImageKey = "play";

                if (animPlayer != null)
                {
                    this.animPlayer.StopAnim();
                }
            }
        }

        private void trackBarTime_Scroll(object sender, EventArgs e)
        {
            if (animPlayer == null)
                return;

            float rate = 1.0f;
            try
            {
                rate = (float)(this.trackBarTime.Value - this.trackBarTime.Minimum) / (float)(this.trackBarTime.Maximum - this.trackBarTime.Minimum);
            }
            finally
            {
                //rate *= 2.0f;
            }
            animPlayer.SetPosition(rate * animPlayer.CurrentAnimLength, false);
        }

        private void listBoxAnim_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (animPlayer == null)
                return;

            ListView.SelectedListViewItemCollection selectedItems = this.listBoxAnim.SelectedItems;
            if (selectedItems.Count == 0)
            {
                return;
            }
            ListViewItem item = selectedItems[0];
            this.propertyGridSeq.SelectedObject = item.Tag;

            NSkeletalAnimSequence sequence = item.Tag as NSkeletalAnimSequence;
            if (sequence != null)
            {
                animPlayer.SetAnim(sequence.Name);

                // 设置trackBar的值
                this.trackBarTime.Minimum = 0;
                this.trackBarTime.Maximum = (int)(animPlayer.CurrentAnimLength * 1000);
                this.trackBarTime.Enabled = true;
            }
        }

        private void importMeshLODToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ImportPSK();
        }

        private void importPSAToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ImportPSA();
        }

        private void loadSkeletalMeshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (LevelLocDlg dlg = new LevelLocDlg())
            {
                dlg.Text = "Load SkeletalMesh ...";
                dlg.SetResourceLocation(NLevelEditorEngine.Instance.LevelLoc);
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                    {
                        NResourceLoc loc = dlg.ResourceLocation;
                        LoadSkeletalMesh(loc.pkgName, loc.fileName);
                    }
                }
            }
        }

        private void saveSkeletalMeshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveCurrentSkeletalMesh();
        }

        private void loadAnimSetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (LevelLocDlg dlg = new LevelLocDlg())
            {
                dlg.Text = "Load AnimSet ...";
                dlg.SetResourceLocation(NLevelEditorEngine.Instance.LevelLoc);
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                    {
                        NResourceLoc loc = dlg.ResourceLocation;
                        LoadAnimSet(loc.pkgName, loc.fileName);
                    }
                }
            }
        }

        private void saveAnimMeshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveCurrentAnimSet();
        }

        private void btnTrnRemoveSeq_Click(object sender, EventArgs e)
        {
            // TODO:
        }

        private void comboBoxLOD_SelectedIndexChanged(object sender, EventArgs e)
        {
            int lod = this.comboBoxLOD.SelectedIndex;
            this.comboBoxSection.Items.Clear();
            int numSec = SkeletalMeshPreview.GetNumSection(lod);

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

            SkeletalMeshMaterialProperty mtlProp = new SkeletalMeshMaterialProperty(SkeletalMeshPreview, lod, sec);

            this.propertyGridMtl.SelectedObject = mtlProp;
        }

        private void propertyGridMtl_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            previewControl.ResetSkeletalMeshResource(skeletalMeshPreview);
            previewControl.Refresh();
        }
    }
}
