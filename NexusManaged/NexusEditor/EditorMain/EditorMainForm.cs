using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using NexusEditor.EditorMain;
using NexusEditor.ResourceEditor;
using NexusEngine;
using System.Diagnostics;
using NexusEngineExtension;
using Nexus.GameFramework;

namespace NexusEditor
{
    /// <summary>
    /// 编辑器主窗口
    /// </summary>
    public partial class EditorMainForm : Form
    {
        private List<NEMainViewport> m_viewports;
        private EditorLogForm m_logForm;
        private LayerManagerDlg m_layerDlg;
		private RecentFile m_RecentFile;
		private GlobalSettingForm m_GlobalSettingForm;

		protected float m_basicMoveFactor;
		protected float m_basicRotateFactor;
		protected float m_basicWheelFactor;
		protected int m_basicCameraSpeed;
		//摄像机的操作倍率
		static float[] m_cameraOperateScale = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f };

		protected int m_AutoSaveInterval;		//自动保存间隔，初始时默认关闭，<=0表示关闭
		protected int m_TimerTickCount;			//自动保存定时器的触发计数

        /// <summary>
        /// 构造函数，创建窗体和子编辑器窗体
        /// </summary>
        public EditorMainForm()
        {
            InitializeComponent();

            this.toolSpaceList.SelectedIndex = 0;

            // short cut key需要
            this.KeyPreview = true;

			//摄像机操作器的初始速度
			m_basicMoveFactor = 4.0f;
			m_basicRotateFactor = 0.005f;
			m_basicWheelFactor = 0.5f;
			//摄像机操作器的初始倍率
			m_basicCameraSpeed = 9;
        }

		/// <summary>
		/// 返回当前激活的Viewport
		/// </summary>
		public NEMainViewport ActiveViewport
		{
			get
			{
				foreach (NEMainViewport vp in m_viewports)
				{
					if (vp.Focused)
						return vp;
				}
				return null;
			}
		}

		private void ReadSettings()
		{
			//默认15分钟自动保存一次，每次设置后自动保存
			m_AutoSaveInterval = Convert.ToInt32(Settings.ReadSetting("Editor", "AutoSaveInterval", -1).ToString());
			m_TimerTickCount = 0;
			autoSaveTimer.Enabled = (m_AutoSaveInterval > 0);
			foreach (ToolStripItem _item in contextMenuStrip1.Items)
			{
				ToolStripMenuItem item = (ToolStripMenuItem)_item;
				int i = Convert.ToInt32(item.Tag.ToString());
				item.Checked = (i == m_AutoSaveInterval);
			}
			if (m_AutoSaveInterval > 0)
			{
				toolStripStatusLabelAutoSave.Text = "自动保存间隔" + m_AutoSaveInterval.ToString() + "分钟";
			}
			else
			{
				toolStripStatusLabelAutoSave.Text = "关闭自动保存";
			}
			//移动对齐粒度
			float align = Convert.ToSingle(Settings.ReadSetting("Editor", "MoveAlign", 1.0f).ToString());
			foreach (ToolStripItem _item in MoveAlignMenu.Items)
			{
				ToolStripMenuItem item = _item as ToolStripMenuItem;
				if (item.Tag.ToString() == align.ToString())
				{
					item.Checked = true;
				}
				else
				{
					item.Checked = false;
				}
			}
			NLevelEditorEngine.Instance.ActorEd.MoveAlign = align;
			m_MoveAlignStatusLabel.Text = "移动对齐粒度:" + align.ToString();

			//旋转对齐粒度
			align = Convert.ToSingle(Settings.ReadSetting("Editor", "RotateAlign", 3.0f).ToString());
			foreach (ToolStripItem _item in RotateAlignMenu.Items)
			{
				ToolStripMenuItem item = _item as ToolStripMenuItem;
				if (item.Tag.ToString() == align.ToString())
				{
					item.Checked = true;
				}
				else
				{
					item.Checked = false;
				}
			}
			m_RotateAlignStatusLabel.Text = "旋转对齐粒度:" + align.ToString() + "度";	
			align *= (float)Math.PI / 180.0f;
			NLevelEditorEngine.Instance.ActorEd.RotateAlign = align;

			//缩放对齐粒度
			//m_ScaleAlignStatusLabel
			align = Convert.ToSingle(Settings.ReadSetting("Editor", "ScaleAlign", 1.0f).ToString());
			foreach (ToolStripItem _item in ScaleAlignMenu.Items)
			{
				ToolStripMenuItem item = _item as ToolStripMenuItem;
				if (item.Tag.ToString() == align.ToString())
				{
					item.Checked = true;
				}
				else
				{
					item.Checked = false;
				}
			}
			m_ScaleAlignStatusLabel.Text = "缩放对齐粒度:" + align.ToString() + "%";
			align /= 100.0f;
			NLevelEditorEngine.Instance.ActorEd.ScaleAlign = align;
		}

		public void OnLoadRecentFile(object sender, EventArgs e)
		{
			ToolStripMenuItem item = (ToolStripMenuItem)sender;
			NResourceLoc loc = new NResourceLoc(item.Text);
			if(loc.IsValid())
			{
				using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
				{
					NLevelEditorEngine.Instance.LoadMainLevel(loc);
					this.toolStripStatusLableNote.Text = "Level Load from " + loc.ToString();
					this.RefreshViewports();
				}
			}
		}

        public void CreateLogForm()
        {
            m_logForm = new EditorLogForm();
            NLogger.Instance.AddLogListener(new LogTextFormOutput(m_logForm));
            NLogger.Instance.WriteString(LogType.Info, "Add Editor Log Form OK.");
            m_logForm.Hide();
        }

        public void CreateEditorForms()
        {
            NLevelEditorEngine.Instance.LevelChanged += new EventHandler(Editor_LevelChanged);
            NEditorCommandManager cmdMgr = NLevelEditorEngine.Instance.CommandMgr;
            cmdMgr.UndoStackChanged += new EventHandler(this.UndoStackChangedEvent);
            cmdMgr.RedoStackChanged += new EventHandler(this.RedoStackChangedEvent);

            m_layerDlg = new LayerManagerDlg();

			m_GlobalSettingForm = new GlobalSettingForm();

			//加载最近用过的文件
			m_RecentFile = new RecentFile();
			m_RecentFile.Load();
			for (int i = 0; i < m_RecentFile.FileCount; ++i)
			{
				NResourceLoc loc = m_RecentFile.GetFile(i);
				if (loc.IsValid())
				{
					ToolStripItem item = MTU.DropDown.Items.Add(loc.ToString());
					if (item != null)
					{
						item.Click += this.OnLoadRecentFile;
					}
				}
			}

			//读取各种设置
			ReadSettings();
        }

		protected void OnCameraOperateFactorChange(int delta)
		{
			if (delta < 0)
			{
				--m_basicCameraSpeed;
				if (m_basicCameraSpeed < 0) m_basicCameraSpeed = 0;
				for (int i = 0; i < m_viewports.Count; ++i)
				{
					m_viewports[i].CameraCtrl.MoveFactor = m_basicMoveFactor * m_cameraOperateScale[m_basicCameraSpeed];
					m_viewports[i].CameraCtrl.RotateFactor = m_basicRotateFactor * m_cameraOperateScale[m_basicCameraSpeed];
					m_viewports[i].CameraCtrl.WheelFactor = m_basicWheelFactor * m_cameraOperateScale[m_basicCameraSpeed];
				}
			}
			else
			{
				++m_basicCameraSpeed;
				if (m_basicCameraSpeed >= m_cameraOperateScale.Length) m_basicCameraSpeed = m_cameraOperateScale.Length-1;

				for (int i = 0; i < m_viewports.Count; ++i)
				{
					m_viewports[i].CameraCtrl.MoveFactor = m_basicMoveFactor * m_cameraOperateScale[m_basicCameraSpeed];
					m_viewports[i].CameraCtrl.RotateFactor = m_basicRotateFactor * m_cameraOperateScale[m_basicCameraSpeed];
					m_viewports[i].CameraCtrl.WheelFactor = m_basicWheelFactor * m_cameraOperateScale[m_basicCameraSpeed];
				}
			}

			if (m_viewports.Count > 0)
			{
				statusMainForm.Items[2].Text = "摄像机" + m_cameraOperateScale[m_basicCameraSpeed].ToString() + "倍速操作";

			}
		}

        void Editor_LevelChanged(object sender, EventArgs e)
        {
            this.Text = "Nexus Editor - " + NLevelEditorEngine.Instance.LevelLoc.ToString();
			m_RecentFile.Add(NLevelEditorEngine.Instance.LevelLoc);
			m_RecentFile.Save();
            m_layerDlg.OnLevelChanged();
            if (m_actorList.Visible)
            {
                //m_actorList.ShowLevel(NLevelEditorEngine.Instance.MainLevel);
            }
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
				vp.MouseClick += this.viewport_MouseClick;
				vp.CameraOperateFactorChangeEvent += this.OnCameraOperateFactorChange;
            }

			if (m_viewports.Count > 0)
			{

				statusMainForm.Items[2].Text = "摄像机正常速度";

			}
        }

        private void viewport_DoubleClick(object sender, EventArgs e)
        {
            AtiveViewOnlyMode();
        }

		private void viewport_MouseClick(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Right)
			{
				NEMainViewport activeViewport = this.ActiveViewport;
				if (activeViewport != null
                    && NLevelEditorEngine.Instance.Command != EditorCommand.TerrainMode
                    && NLevelEditorEngine.Instance.Command != EditorCommand.NavigateMapMode
					&& activeViewport.RightMouseButtenDownPosition == e.Location)
				{
					this.contextMenuStripViewport.Show(activeViewport, e.Location);
				}
			}
		}

        /// <summary>
        /// 窗口关闭事件
        /// </summary>        
        private void EditorMainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
			m_RecentFile.Save();
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

                //-- 创建材质
                NewTerrainMaterial mtlDlg = new NewTerrainMaterial(newTrn);
                mtlDlg.Show(this);
                
                //-- 自动选中地形对象
                NLevelEditorEngine.Instance.ActorEd.SelectedActor = newTrn;
                
                this.RefreshViewports();
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
            if (MessageBox.Show(this, NexusEditor.Properties.Resources.NewLevelChecking,
                NexusEditor.Properties.Resources.Ok, MessageBoxButtons.OKCancel) == DialogResult.OK)
            {
                NLevelEditorEngine.Instance.CreateMainLevel("DefaultLevelName");
                this.toolStripStatusLableNote.Text = "Level created";
                this.RefreshViewports();
            }            
        }

        private void saveCurrentLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (LevelLocDlg dlg = new LevelLocDlg())
            {
                dlg.Text = "Save Level ...";
                dlg.SetResourceLocation(NLevelEditorEngine.Instance.LevelLoc);
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    NResourceLoc loc = dlg.ResourceLocation;
                    using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                    {
                        NLevelEditorEngine.Instance.SaveMainLevel(loc);
                        this.toolStripStatusLableNote.Text = "Level saved to " + loc.ToString();
						m_RecentFile.Add(loc);
						m_RecentFile.Save();
                    }
                }
            }
        }

        private void loadLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (LevelLocDlg dlg = new LevelLocDlg())
            {
                dlg.Text = "Load Level ...";
                dlg.SetResourceLocation(NLevelEditorEngine.Instance.LevelLoc);
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    NResourceLoc loc = dlg.ResourceLocation;
                    using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
                    {
                        NLevelEditorEngine.Instance.LoadMainLevel(loc);
                        this.toolStripStatusLableNote.Text = "Level Load from " + loc.ToString();
                        this.RefreshViewports();
                    }
                }
            }
        }

        private void EditorMainForm_Load(object sender, EventArgs e)
        {
            this.TerrainPageLoad();
            this.ActorPageLoad();
            this.CommandToolStripLoad();
            this.ResourcePageLoad();
			this.InitActorFactoryList();
        }

		private void InitActorFactoryList()
        {
            List<ObjectCreator> factoryList = NLevelEditorEngine.Instance.ActorFactoryCreatorList;
            this.listViewClass.Clear();
            this.listViewClass.SuspendLayout();
            foreach (var item in factoryList)
            {
                ListViewItem viewItem = new ListViewItem();
                viewItem.Text = item.ObjectType.Name;
                viewItem.Tag = item;
                this.listViewClass.Items.Add(viewItem);
            }
            this.listViewClass.ResumeLayout();
        }

        private void toolBtnCamera_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
				NEMainViewport activeVp = ActiveViewport;
				
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

        public void OnApplicationIdle(Object sender, EventArgs e)
        {
            this.splitMainForm.Panel1.Refresh();
        }

        public void RefreshViewports()
        {
            this.splitMainForm.Panel1.Refresh();            
        }

        private void toolBtnUndo_Click(object sender, EventArgs e)
        {
            NLevelEditorEngine.Instance.CommandMgr.Undo();
            this.RefreshViewports();
        }

        private void toolBtnRedo_Click(object sender, EventArgs e)
        {
            NLevelEditorEngine.Instance.CommandMgr.Redo();
            this.RefreshViewports();
        }

        private void actorListToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_actorList.ShowLevel(NLevelEditorEngine.Instance.MainLevel);
            m_actorList.Show(this);
        }

        private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (EditorOptionsDlg dlg = new EditorOptionsDlg())
            {
                dlg.ShowDialog(this);
            }
        }

        /// <summary>
        /// 响应command mgr的undo stack changed时间，更新tool bar信息
        /// </summary>        
        private void UndoStackChangedEvent(object sender, EventArgs e)
        {
            NEditorCommandManager cmdMgr = NLevelEditorEngine.Instance.CommandMgr;
            if (cmdMgr.UndoStackTop != null)
                this.toolBtnUndo.Text = "Undo " + cmdMgr.UndoStackTop.Name;
            else
                this.toolBtnUndo.Text = "Undo";
        }

        private void RedoStackChangedEvent(object sender, EventArgs e)
        {
            NEditorCommandManager cmdMgr = NLevelEditorEngine.Instance.CommandMgr;
            if (cmdMgr.RedoStackTop != null)
                this.toolBtnRedo.Text = "Redo " + cmdMgr.RedoStackTop.Name;
            else
                this.toolBtnRedo.Text = "Redo";
        }

        private void EditorMainForm_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Z:
                    if (e.Control)
                    {
                        NLevelEditorEngine.Instance.CommandMgr.Undo();
                        this.RefreshViewports();
                    }
                    break;
                case Keys.Y:
                    if (e.Control)
                    {
                        NLevelEditorEngine.Instance.CommandMgr.Redo();
                        this.RefreshViewports();
                    }
                    break;
                case Keys.Q:
                    this.CheckCommandButton(this.toolBtnPick);
                    this.RefreshViewports();
                    break;
                case Keys.W:
                    this.CheckCommandButton(this.toolBtnTranslate);
                    this.RefreshViewports();
                    break;
                case Keys.E:
                    this.CheckCommandButton(this.toolBtnRotate);
                    this.RefreshViewports();
                    break;
                case Keys.R:
                    this.CheckCommandButton(this.toolBtnScale);
                    this.RefreshViewports();
                    break;
                case Keys.C:
                    this.CheckCommandButton(this.toolBtnCamera);
                    this.RefreshViewports();
                    break;
                case Keys.T:
                    this.CheckCommandButton(this.toolBtnTerrainEd);
                    this.RefreshViewports();
                    break;				
            }
        }

        private void toolLevelLayers_Click(object sender, EventArgs e)
        {
            if(!m_layerDlg.Visible)
            {
                m_layerDlg.Show(this);
            }            
        }

        private void EditorMainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show(this, NexusEditor.Properties.Resources.EditorClosing, NexusEditor.Properties.Resources.Ok, MessageBoxButtons.YesNo)
                == DialogResult.No)
            {
                e.Cancel = true;
            }

        }

        private void PIE_Click(object sender, EventArgs e)
        {
            Vector3 startPosition = Vector3.Zero;
            NActor actor = NLevelEditorEngine.Instance.MainLevel.FirstActor();
            while (actor != null)
            {
                NStartPointActor startPointActor = actor as NStartPointActor;
                if (startPointActor != null)
                {
                    startPosition = startPointActor.Location;
                    break;
                }
                actor = NLevelEditorEngine.Instance.MainLevel.NextActor();
            }
            PlayInEditor(startPosition);
        }

        private void PlayInEditor(Vector3 startPosition)
        {
            try
            {
                if (MessageBox.Show(NexusEditor.Properties.Resources.PreviewLevel,
                                    NexusEditor.Properties.Resources.Ok,
                                    MessageBoxButtons.OKCancel) == DialogResult.OK)
                {
                    // 通过命令行参数启动客户端，预览当前场景
                    ProcessStartInfo startInfo = new ProcessStartInfo("NexusGameClient.exe");
                    startInfo.Arguments = "--LoadLevel=" + "\"" + NLevelEditorEngine.Instance.LevelLoc.ToString() + "\""
                        + " "
                        + "--StartPosition=" + startPosition.ToString();
                    NLogger.Instance.WriteString(LogType.Info, startInfo.FileName+startInfo.Arguments);
                    Process.Start(startInfo);
                }
            }
            catch (System.Exception ex)
            {
                Program.ShowException(ex, "Failed Play In Editor!");
            }
        }

        private void showNavigationMapToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            this.navigationHeightMapToolStripMenuItem.Checked = NLevelEditorEngine.Instance.NavigateMapEd.NavigationMapVisible;
            this.navigationWalkableMapToolStripMenuItem.Checked = NLevelEditorEngine.Instance.NavigateMapEd.WalkableMapVisible;
        }

        private void navigationHeightMapToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            NLevelEditorEngine.Instance.NavigateMapEd.NavigationMapVisible = this.navigationHeightMapToolStripMenuItem.Checked;
        }

        private void navigationWalkableMapToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            NLevelEditorEngine.Instance.NavigateMapEd.WalkableMapVisible = this.navigationWalkableMapToolStripMenuItem.Checked;
        }

        private void navigationWalkableMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.navigationWalkableMapToolStripMenuItem.Checked = !this.navigationWalkableMapToolStripMenuItem.Checked;
        }

        private void navigationHeightMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.navigationHeightMapToolStripMenuItem.Checked = !this.navigationHeightMapToolStripMenuItem.Checked;
        }

        private void showTerrainsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.hideTerrainsToolStripMenuItem.Checked = !this.hideTerrainsToolStripMenuItem.Checked;
        }

        private void showTerrainsToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            NLevelEditorEngine.Instance.TerrainsVisible = !this.hideTerrainsToolStripMenuItem.Checked;
        }

        private void showToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            this.hideTerrainsToolStripMenuItem.Checked = !NLevelEditorEngine.Instance.TerrainsVisible;
        }

		private void cameraSight_ValueChanged(object sender, EventArgs e)
		{
			foreach (NEMainViewport vp in m_viewports)
			{
				vp.Sight = (float)(cameraSight.Value-cameraSight.Minimum) / (cameraSight.Maximum-cameraSight.Minimum);
			}
		}

		private void toolStripStatusLabel1_Click(object sender, EventArgs e)
		{
			//显示自动保存间隔时间设置菜单
			{
				Point pt = Cursor.Position;
				this.contextMenuStrip1.Show(pt);
			}
		}

		private void autoSaveTimer_Tick(object sender, EventArgs e)
		{
			++m_TimerTickCount;
			if(m_TimerTickCount>=m_AutoSaveInterval)
			{
				m_TimerTickCount = 0;
				NResourceLoc loc = NLevelEditorEngine.Instance.LevelLoc;
				if (!loc.IsValid()) return;
				//自动保存到当前level所在文件夹下面的"auto_save+日期+时间"子文件夹
				NResourceLoc auto_save=new NResourceLoc();
				auto_save.PackageName = loc.PackageName;
				auto_save.FileName=loc.FileName+"\\auto_save";
				NLevelEditorEngine.Instance.FileSystem.CreateDirectory(auto_save.PackageName, auto_save.FileName);
				using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
				{
					NLevelEditorEngine.Instance.SaveMainLevelAs(auto_save);
					this.toolStripStatusLableNote.Text = "Level auto saved to " + auto_save.ToString();
				}
			}
		}

		private void toolStripStatusLabel1_Click_1(object sender, EventArgs e)
		{
			//移动对齐
			Point pt = Cursor.Position;
			this.MoveAlignMenu.Show(pt);
		}

		private void toolStripStatusLabel2_Click(object sender, EventArgs e)
		{
			//旋转对齐
			Point pt = Cursor.Position;
			this.RotateAlignMenu.Show(pt);
		}

		private void toolStripStatusLabel3_Click(object sender, EventArgs e)
		{
			//缩放对齐
			Point pt = Cursor.Position;
			this.ScaleAlignMenu.Show(pt);
		}

		private void MoveAlignMenu_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
		{
			foreach (ToolStripItem _item in e.ClickedItem.Owner.Items)
			{
				ToolStripMenuItem item = _item as ToolStripMenuItem;
				item.Checked = false;
			}
			ToolStripMenuItem clicked = e.ClickedItem as ToolStripMenuItem;
			clicked.Checked = true;
			string text = clicked.Tag.ToString();
			NLevelEditorEngine.Instance.ActorEd.MoveAlign = Convert.ToSingle(text);
			m_MoveAlignStatusLabel.Text = "移动对齐粒度:"+text;
			Settings.WriteSetting("Editor", "MoveAlign", NLevelEditorEngine.Instance.ActorEd.MoveAlign);
		}

		private void contextMenuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
		{
			//设置保存间隔为20分钟
			m_AutoSaveInterval = Convert.ToInt32(e.ClickedItem.Tag.ToString());
			Settings.WriteSetting("Editor", "AutoSaveInterval", m_AutoSaveInterval);
			autoSaveTimer.Enabled = (m_AutoSaveInterval > 0);
			foreach (ToolStripItem _item in closeAutoSave.Owner.Items)
			{
				ToolStripMenuItem item = (ToolStripMenuItem)_item;
				item.Checked = false;
			}
			ToolStripMenuItem clieked = e.ClickedItem as ToolStripMenuItem;
			clieked.Checked = true;

			if (m_AutoSaveInterval > 0)
			{
				toolStripStatusLabelAutoSave.Text = "自动保存间隔" + m_AutoSaveInterval.ToString() + "分钟";
			}
			else
			{
				toolStripStatusLabelAutoSave.Text = "关闭自动保存";
			}
		}

		private void RotateAlignMenu_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
		{
			foreach (ToolStripItem _item in e.ClickedItem.Owner.Items)
			{
				ToolStripMenuItem item = _item as ToolStripMenuItem;
				item.Checked = false;
			}
			ToolStripMenuItem clicked = e.ClickedItem as ToolStripMenuItem;
			clicked.Checked = true;
			string text = clicked.Tag.ToString();
			NLevelEditorEngine.Instance.ActorEd.RotateAlign = Convert.ToSingle(text)*(float)Math.PI/180.0f;
			m_RotateAlignStatusLabel.Text = "旋转对齐粒度:" + text + "度";
			Settings.WriteSetting("Editor", "RotateAlign", text);
		}

		private void ScaleAlignMenu_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
		{
			foreach (ToolStripItem _item in e.ClickedItem.Owner.Items)
			{
				ToolStripMenuItem item = _item as ToolStripMenuItem;
				item.Checked = false;
			}
			ToolStripMenuItem clicked = e.ClickedItem as ToolStripMenuItem;
			clicked.Checked = true;
			string text = clicked.Tag.ToString();
			NLevelEditorEngine.Instance.ActorEd.ScaleAlign = Convert.ToSingle(text) / 100.0f;
			m_ScaleAlignStatusLabel.Text = "缩放对齐粒度:" + text + "%";
			Settings.WriteSetting("Editor", "ScaleAlign", text);
		}

		private void toolStripReplace_Click(object sender, EventArgs e)
		{
			//用资源列表中选定的对象替换当前level中选定的所有对象
			if (listViewResource.SelectedItems.Count == 0)
			{
				toolStripStatusLableNote.Text = "资源列表中没有选中的对象";
				return;
			}
			ListViewItem item = listViewResource.SelectedItems[0];
			NFileEntity nfile = (NFileEntity)item.Tag;
			NResourceLoc resLoc = new NResourceLoc(nfile.PackageName, nfile.FilePath);
			if (ResourceEditor.ResourceActorCreator.AcceptResoruceFile(resLoc))
			{
				List<NActor> actors = NLevelEditorEngine.Instance.ActorEd.GetSelectedActors();
				List<NActor> newActors = new List<NActor>();
				NActor activeActor = NLevelEditorEngine.Instance.ActorEd.GetActor();
				for(int i=0;i<actors.Count;++i)
				{
					NActor act = actors[i];
					if (act.Frozen ) continue;
					NActor newActor = ResourceEditor.ResourceActorCreator.CreateActor(resLoc);
					if(newActor!=null)
					{
						newActor.Layer = act.Layer;
						newActor.SpaceData = act.SpaceData;
						newActors.Add(newActor);
					}
				}
				NLevelEditorEngine.Instance.ActorEd.DeleteSelectedActors();
				for (int i = 0; i < newActors.Count; ++i)
				{
					NLevelEditorEngine.Instance.ActorEd.AddSelectedActor(newActors[i]);
				}
			}
		}

		private void addActorToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
		{
			// 显示ActorFactory菜单
			List<ObjectCreator> factoryList = NLevelEditorEngine.Instance.ActorFactoryCreatorList;
			this.addActorToolStripMenuItem.DropDownItems.Clear();
			foreach (var item in factoryList)
			{
				//TODO： 检测当前哪些是可以Placeable的
				ToolStripMenuItem addActorTypeMenu = new ToolStripMenuItem(item.ObjectType.Name, null, new EventHandler(addActorTypeMenu_Click));
				addActorTypeMenu.Tag = item;
				this.addActorToolStripMenuItem.DropDownItems.Add(addActorTypeMenu);
			}
		}

		private void addActorTypeMenu_Click(object sender, EventArgs e)
		{
			ToolStripMenuItem item = sender as ToolStripMenuItem;
			ObjectCreator creator = item.Tag as ObjectCreator;
			if (creator != null)
			{
				try
				{
					NCheckResult hit;
					NEMainViewport activeViewport = this.ActiveViewport;
					if (activeViewport != null)
					{
						activeViewport.MouseRayCheck(out hit, activeViewport.RightMouseButtenDownPosition, 512 * 1024, LineCheckType.All);
						Point pt = activeViewport.Viewport.Camera.WorldToScreen(hit.location);
						ActorFactory factory = creator.CreateInstance() as ActorFactory;
						factory.CreateActor(NLevelEditorEngine.Instance.MainLevel, hit.location, Vector3.Zero);
					}
				}
				catch (System.Exception ex)
				{
					Program.ShowException(ex, "ActorFactory Create Error!");
				}
			}
		}

		private void toolStripMenuExportLevel_Click(object sender, EventArgs e)
		{
			using (LevelLocDlg dlg = new LevelLocDlg())
			{
				dlg.Text = "Export Level ...";
				dlg.SetResourceLocation(NLevelEditorEngine.Instance.LevelLoc);
				if (dlg.ShowDialog(this) == DialogResult.OK)
				{
					NResourceLoc loc = dlg.ResourceLocation;
					using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
					{
						NLevelEditorEngine.Instance.ExportMainLevel(loc);
						this.toolStripStatusLableNote.Text = "Level Export to " + loc.ToString();
					}
				}
			}
		}

		private void menuItemSelSameResource_Click(object sender, EventArgs e)
		{
			//选择引用了相同模型资源的所有对象
			NLevelEditorEngine.Instance.ActorEd.SelectSameTypeActors();
		}

		private void menuItemAllInLayer_Click(object sender, EventArgs e)
		{
			//选择当前层内的所有对象
			NLevelEditorEngine.Instance.ActorEd.SelectAllActorsInLayer();
		}

		private void menuItemSelOthers_Click(object sender, EventArgs e)
		{
			//以层为单位反向选择
			NLevelEditorEngine.Instance.ActorEd.SelectOtherActors();
		}

		private void menuItemUnselAll_Click(object sender, EventArgs e)
		{
			//取消所有选择
			NLevelEditorEngine.Instance.ActorEd.BindActor(null);
		}

		private void showAllToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//显示所有对象
			NLevelEditorEngine.Instance.ActorEd.VisibleAllActors(true);
		}

		private void hideAllToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//隐藏所有对象
			NLevelEditorEngine.Instance.ActorEd.VisibleAllActors(false);
		}

		private void showSelOnlyToolStripMenuItem1_Click(object sender, EventArgs e)
		{
			//只显示被选中的对象，隐藏所有其他的对象
			NLevelEditorEngine.Instance.ActorEd.VisibleSelectedActorsOnly();
		}

		private void CopyToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//复制选中对象
			NLevelEditorEngine.Instance.ActorEd.CopySelectedActors(false);
		}

		private void CutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//剪切选中对象
			NLevelEditorEngine.Instance.ActorEd.CopySelectedActors(true);
		}

		private void PasteToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//在当前激活视图的中心粘贴已经复制或者剪切的对象
			int x = ActiveViewport.Width / 2;
			int y = ActiveViewport.Height / 2;
			NCamera camera = ActiveViewport.Viewport.Camera;
			NLevelEditorEngine.Instance.ActorEd.PasteCopyedActors(camera, x, y);
		}

        /// <summary>
        /// play from here
        /// </summary>
        private void playFromHereToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 检测鼠标射线
			NCheckResult hit;
            hit.location = Vector3.Zero;
			NEMainViewport activeViewport = this.ActiveViewport;
			if (activeViewport != null)
			{
                activeViewport.MouseRayCheck(out hit, activeViewport.RightMouseButtenDownPosition, 1024 * 1024, LineCheckType.All);
			}

            PlayInEditor(hit.location);
        }

		private void globalSettingsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//打开全局效果设置窗体
			if (!m_GlobalSettingForm.Visible)
			{
				m_GlobalSettingForm.Show(this);
			}
		}
    }// end of class EditorMainForm
}
