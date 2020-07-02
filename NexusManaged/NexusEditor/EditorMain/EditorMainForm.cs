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

namespace NexusEditor
{
    /// <summary>
    /// 编辑器主窗口
    /// </summary>
    public partial class EditorMainForm : Form
    {        
        private List<NEMainViewport> m_viewports;
        private EditorLogForm m_logForm;

        /// <summary>
        /// 构造函数，创建窗体和子编辑器窗体
        /// </summary>
        public EditorMainForm()
        {
            InitializeComponent();
            this.toolSpaceList.SelectedIndex = 0;
        }

        public void CreateLogForm()
        {
            m_logForm = new EditorLogForm();
            NLevelEditorEngine.Instance.RedirectLog(m_logForm);
            m_logForm.Hide();
        }

        public void CreateEditorForms()
        {            
            NLevelEditorEngine.Instance.LevelChanged += new EventHandler(Editor_LevelChanged);            
        }

        void Editor_LevelChanged(object sender, EventArgs e)
        {
            this.Text = "Nexus Editor - " + NLevelEditorEngine.Instance.LevelLoc.ToString();

            //string toolTxt = "Save Level to " + NLevelEditorEngine.Instance.LevelLoc.ToString();
            //this.toolStripButtonSave.Text = toolTxt;
            //this.toolStripButtonSave.ToolTipText = toolTxt;
        }

        /// <summary>
        /// 获得一个窗口句柄用来初始化引擎
        /// </summary>
        /// <returns>窗口句柄</returns>
        public IntPtr RenderPanelHandle
        {
            get { return this.splitMainForm.Panel1.Handle; }
        }

        /// <summary>
        /// 相应About菜单消息
        /// </summary>        
        private void menuAbout_Click(object sender, EventArgs e)
        {
            EditorMain.AboutBoxNexus dlg = new EditorMain.AboutBoxNexus();            
            dlg.ShowDialog();
        }

        /// <summary>
        /// 自动调节所有viewport的大小，使其均匀分布
        /// </summary>
        private void AutoViewportSize()
        {
            this.splitContainerViewH.SplitterDistance = this.splitContainerViewH.Size.Height / 2;
            this.splitContainerViewUp.SplitterDistance = this.splitContainerViewUp.Size.Width / 2;
            this.splitContainerViewDown.SplitterDistance = this.splitContainerViewDown.Size.Width / 2;
        }

        /// <summary>
        /// 创建viewport窗口，因为创建窗口即刻需要渲染(Paint事件)，所以独立成一个函数，在引擎和level创建后才调用。        
        /// </summary>
        public void CreateViewport()
        {
            AutoViewportSize();

            List<NEMainViewport> vpList = new List<NEMainViewport>();
            NEMainViewport vp1 = new NEMainViewport();
            vp1.SetCameraMode(EditorCameraMode.Top);
            this.splitContainerViewUp.Panel1.Controls.Add(vp1);
            vpList.Add(vp1);

            NEMainViewport vp2 = new NEMainViewport();
            vp2.SetCameraMode(EditorCameraMode.Front);
            this.splitContainerViewUp.Panel2.Controls.Add(vp2);
            vpList.Add(vp2);

            NEMainViewport vp3 = new NEMainViewport();
            vp3.SetCameraMode(EditorCameraMode.Side);
            this.splitContainerViewDown.Panel1.Controls.Add(vp3);
            vpList.Add(vp3);

            NEMainViewport vp4 = new NEMainViewport();
            vp4.SetCameraMode(EditorCameraMode.Perspective);
            this.splitContainerViewDown.Panel2.Controls.Add(vp4);
            vpList.Add(vp4);

            NLevelEditorEngine.Instance.ActorEd.ActorSelected
                += new EventHandler(this.ActorSelectedHandler);
            
            //--
            m_viewports = vpList;
            foreach (NEMainViewport vp in m_viewports)
            {
                vp.DoubleClick += new EventHandler(viewport_DoubleClick);
            }
        }

        void viewport_DoubleClick(object sender, EventArgs e)
        {
            AtiveViewOnlyMode();
        }

        /// <summary>
        /// 窗口关闭事件
        /// </summary>        
        private void EditorMainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        /// <summary>
        /// 响应Exit菜单
        /// </summary>        
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        

        /// <summary>
        /// new terrain菜单响应
        /// </summary>        
        /// <remarks>
        /// 向Level中创建一个新的Terrrain，并自动打开Terrain编辑模式
        /// </remarks>
        private void newTerrainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewTerrainDlg dlg = new NewTerrainDlg();
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                NETerrainProp trnProp = dlg.TerrainProperty;

                NLevel mainLv = NLevelEditorEngine.Instance.MainLevel;
                NActor newActor = mainLv.CreateActor(trnProp.Name, "nterrain_actor");
                NTerrainActor newTrn = newActor as NTerrainActor;

                newTrn.Create(trnProp.Width, trnProp.Height, trnProp.Init, trnProp.ChunkSize);
                newTrn.Move(trnProp.Position, trnProp.Scale);                

                //-- 自动打开地形编辑器模式
                NLevelEditorEngine.Instance.ActorEd.SelectedActor = newTrn;
                //NLevelEditorEngine.Instance.OpenTerrainEditor();          
                
            }// end of if
        }        

        /// <summary>
        /// 响应Viewport->Reset Layout菜单
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void resetLayoutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AutoViewportSize();
        }

        private void ExpandAllViewportSplit()
        {
            this.splitContainerViewH.Panel1Collapsed = false;
            this.splitContainerViewH.Panel2Collapsed = false;

            this.splitContainerViewUp.Panel1Collapsed = false;
            this.splitContainerViewUp.Panel2Collapsed = false;

            this.splitContainerViewDown.Panel1Collapsed = false;
            this.splitContainerViewDown.Panel2Collapsed = false;
        }

        private void AtiveViewOnlyMode()
        {
            int activeVp = 0;
            foreach (NEMainViewport vp in m_viewports)
            {
                if (vp.Focused)
                    break;
                activeVp++;
            }
            if (activeVp > 3)
                activeVp = 3;

            this.SuspendLayout();
            ExpandAllViewportSplit();

            switch (activeVp)
            {
                case 0:                    
                    this.splitContainerViewH.Panel2Collapsed = true;
                    this.splitContainerViewUp.Panel2Collapsed = true;
                    break;
                case 1:
                    this.splitContainerViewH.Panel2Collapsed = true;
                    this.splitContainerViewUp.Panel1Collapsed = true;
                    break;
                case 2:
                    this.splitContainerViewH.Panel1Collapsed = true;
                    this.splitContainerViewDown.Panel2Collapsed = true;
                    break;
                case 3:
                    this.splitContainerViewH.Panel1Collapsed = true;
                    this.splitContainerViewDown.Panel1Collapsed = true;
                    break;
            }
            this.ResumeLayout();
        }

        private void activeViewOnlyToolStripMenuItem_Click(object sender, EventArgs e)
        {            
            AtiveViewOnlyMode();
        }       
        

        private void showResourceManagerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NexusEditor.Program.resourceForm.Show();
            NexusEditor.Program.resourceForm.BringToFront();
        }

        private void newLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NLevelEditorEngine.Instance.CreateMainLevel("DefaultLevelName");
            this.toolStripStatusLableNote.Text = "Level created";
        }

        private void saveCurrentLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NResourceLoc loc = NLevelEditorEngine.Instance.LevelLoc;
            if (loc.IsValid())
            {
                NLevelEditorEngine.Instance.SaveMainLevel(loc);
                this.toolStripStatusLableNote.Text = "Level saved to " + loc.ToString();
            }
            else 
            {
                NexusEditor.Program.ShowError("Level save location no specified.");
            }
        }

        private void EditorMainForm_Load(object sender, EventArgs e)
        {
            this.TerrainPageLoad();
            this.ActorPageLoad();
            this.CommandToolStripLoad();
            this.ResourcePageLoad();
        }

        private void toolBtnCamera_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                NEMainViewport activeVp = null;
                foreach (NEMainViewport vp in m_viewports)
                {
                    if (vp.Focused)
                        activeVp = vp;
                }

                if (activeVp == null)
                {
                    NexusEditor.Program.ShowError("Not active viewport found.");
                    return;
                }

                using (EditorMain.CameraCtrlSettingDlg dlg = new CameraCtrlSettingDlg())
                {
                    dlg.SetCameraCtrl(activeVp.CameraCtrl);
                    dlg.ShowDialog(this);
                }
            }
        }

        private void toolBtnShowLog_Click(object sender, EventArgs e)
        {
            m_logForm.Show();
        }

        private void fourViewsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.SuspendLayout();
            ExpandAllViewportSplit();
            this.ResumeLayout();
        }

        private void twoViewportsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.SuspendLayout();
            ExpandAllViewportSplit();
            this.splitContainerViewUp.Panel2Collapsed = true;
            this.splitContainerViewDown.Panel1Collapsed = true;
            this.ResumeLayout();
        }

        private void fitViewToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

    }// end of class EditorMainForm
}
