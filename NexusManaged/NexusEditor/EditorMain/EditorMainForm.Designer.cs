namespace NexusEditor
{
    partial class EditorMainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
                //this.m_resPreview.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditorMainForm));
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem("test1");
            System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem("test2");
            System.Windows.Forms.Label label7;
            System.Windows.Forms.Label label6;
            System.Windows.Forms.Label label5;
            System.Windows.Forms.Label label4;
            System.Windows.Forms.Label label3;
            System.Windows.Forms.Label label2;
            System.Windows.Forms.Label label1;
            this.menuMainForm = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveCurrentLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem5 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripMenuExportLevel = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.MTU = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.CopyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.CutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.PasteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem30 = new System.Windows.Forms.ToolStripSeparator();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuItemSelSameResource = new System.Windows.Forms.ToolStripMenuItem();
            this.menuItemAllInLayer = new System.Windows.Forms.ToolStripMenuItem();
            this.menuItemSelOthers = new System.Windows.Forms.ToolStripMenuItem();
            this.menuItemUnselAll = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.resetLayoutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showResourceManagerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showLayerManagerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showLogToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.activeViewOnlyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fourViewsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.twoViewportsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fitViewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.showToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.hideAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showSelOnlyToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem29 = new System.Windows.Forms.ToolStripSeparator();
            this.hideTerrainsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem32 = new System.Windows.Forms.ToolStripMenuItem();
            this.showNavigationMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.navigationHeightMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.navigationWalkableMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.levelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createTerrainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.actorListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.globalSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuAbout = new System.Windows.Forms.ToolStripMenuItem();
            this.statusMainForm = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLableNote = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusSelectionLock = new System.Windows.Forms.ToolStripStatusLabel();
            this.CameraSpeed = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelAutoSave = new System.Windows.Forms.ToolStripStatusLabel();
            this.m_MoveAlignStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.m_RotateAlignStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.m_ScaleAlignStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.splitMainForm = new System.Windows.Forms.SplitContainer();
            this.splitContainerViewH = new System.Windows.Forms.SplitContainer();
            this.splitContainerViewUp = new System.Windows.Forms.SplitContainer();
            this.splitContainerViewDown = new System.Windows.Forms.SplitContainer();
            this.tabControlRight = new System.Windows.Forms.TabControl();
            this.tabPageActor = new System.Windows.Forms.TabPage();
            this.tabPageTerrain = new System.Windows.Forms.TabPage();
            this.imageListTrnTarget = new System.Windows.Forms.ImageList(this.components);
            this.tabPageRes = new System.Windows.Forms.TabPage();
            this.imageListResource = new System.Windows.Forms.ImageList(this.components);
            this.tabFactory = new System.Windows.Forms.TabPage();
            this.listViewClass = new System.Windows.Forms.ListView();
            this.className = new System.Windows.Forms.ColumnHeader();
            this.nativeClassName = new System.Windows.Forms.ColumnHeader();
            this.imageListTab = new System.Windows.Forms.ImageList(this.components);
            this.imageListCtrlMode = new System.Windows.Forms.ImageList(this.components);
            this.imageListCommand = new System.Windows.Forms.ImageList(this.components);
            this.toolMainForm = new System.Windows.Forms.ToolStrip();
            this.toolBtnUndo = new System.Windows.Forms.ToolStripButton();
            this.toolBtnRedo = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolBtnPick = new System.Windows.Forms.ToolStripButton();
            this.toolBtnCamera = new System.Windows.Forms.ToolStripButton();
            this.toolBtnTerrainEd = new System.Windows.Forms.ToolStripButton();
            this.toolBtnNavEditor = new System.Windows.Forms.ToolStripButton();
            this.toolBtnTranslate = new System.Windows.Forms.ToolStripButton();
            this.toolBtnRotate = new System.Windows.Forms.ToolStripButton();
            this.toolBtnScale = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripReplace = new System.Windows.Forms.ToolStripButton();
            this.toolSpaceList = new System.Windows.Forms.ToolStripComboBox();
            this.toolBtnPanView = new System.Windows.Forms.ToolStripButton();
            this.toolBtnRotateView = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolBtnActorList = new System.Windows.Forms.ToolStripButton();
            this.toolBtnMaxView = new System.Windows.Forms.ToolStripButton();
            this.toolBtnCoord = new System.Windows.Forms.ToolStripSplitButton();
            this.viewToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.screenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.worldToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.localToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolLevelLayers = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolBtnShowResMgr = new System.Windows.Forms.ToolStripButton();
            this.toolBtnShowLog = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.PIE = new System.Windows.Forms.ToolStripButton();
            this.cameraSight = new System.Windows.Forms.TrackBar();
            this.autoSaveTimer = new System.Windows.Forms.Timer(this.components);
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.autoSaveTimeSetting5 = new System.Windows.Forms.ToolStripMenuItem();
            this.autoSaveTimeSetting10 = new System.Windows.Forms.ToolStripMenuItem();
            this.autoSaveTimeSetting15 = new System.Windows.Forms.ToolStripMenuItem();
            this.autoSaveTimeSetting20 = new System.Windows.Forms.ToolStripMenuItem();
            this.closeAutoSave = new System.Windows.Forms.ToolStripMenuItem();
            this.MoveAlignMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItem6 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem7 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem8 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem9 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem20 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem21 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem22 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem23 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem24 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem25 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem26 = new System.Windows.Forms.ToolStripMenuItem();
            this.RotateAlignMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItem10 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem11 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem12 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem13 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem14 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem27 = new System.Windows.Forms.ToolStripMenuItem();
            this.ScaleAlignMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItem15 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem16 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem17 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem18 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem19 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem28 = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStripViewport = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.addActorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
            this.playFromHereToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groupActorProp = new NexusEditor.NControl.CollapsibleGroupBox();
            this.propertyGridActor = new System.Windows.Forms.PropertyGrid();
            this.groupActorComp = new NexusEditor.NControl.CollapsibleGroupBox();
            this.treeViewActor = new System.Windows.Forms.TreeView();
            this.groupTrnUtil = new NexusEditor.NControl.CollapsibleGroupBox();
            this.btnTrnTools = new System.Windows.Forms.Button();
            this.checkBoxShowChunk = new System.Windows.Forms.CheckBox();
            this.buttonTrnNoise = new System.Windows.Forms.Button();
            this.btnTrnMtl = new System.Windows.Forms.Button();
            this.groupTrnLayers = new NexusEditor.NControl.CollapsibleGroupBox();
            this.btnTrnAddLayer = new System.Windows.Forms.Button();
            this.btnTrnLayerDown = new System.Windows.Forms.Button();
            this.btnTrnRemoveLayer = new System.Windows.Forms.Button();
            this.btnTrnLayerUp = new System.Windows.Forms.Button();
            this.listViewTrnTarget = new System.Windows.Forms.ListView();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.comboBoxTrnTarget = new System.Windows.Forms.ComboBox();
            this.groupBrush = new NexusEditor.NControl.CollapsibleGroupBox();
            this.numericUpDownStrength = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownOutter = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownInner = new System.Windows.Forms.NumericUpDown();
            this.textBoxTrnFreq = new System.Windows.Forms.TextBox();
            this.comboBoxTrnLerp = new System.Windows.Forms.ComboBox();
            this.comboBoxTrnShape = new System.Windows.Forms.ComboBox();
            this.comboBoxTrnOp = new System.Windows.Forms.ComboBox();
            this.collapsibleGroupBox2 = new NexusEditor.NControl.CollapsibleGroupBox();
            this.pictureBoxResource = new System.Windows.Forms.PictureBox();
            this.collapsibleGroupBox1 = new NexusEditor.NControl.CollapsibleGroupBox();
            this.listViewResource = new System.Windows.Forms.ListView();
            label7 = new System.Windows.Forms.Label();
            label6 = new System.Windows.Forms.Label();
            label5 = new System.Windows.Forms.Label();
            label4 = new System.Windows.Forms.Label();
            label3 = new System.Windows.Forms.Label();
            label2 = new System.Windows.Forms.Label();
            label1 = new System.Windows.Forms.Label();
            this.menuMainForm.SuspendLayout();
            this.statusMainForm.SuspendLayout();
            this.splitMainForm.Panel1.SuspendLayout();
            this.splitMainForm.Panel2.SuspendLayout();
            this.splitMainForm.SuspendLayout();
            this.splitContainerViewH.Panel1.SuspendLayout();
            this.splitContainerViewH.Panel2.SuspendLayout();
            this.splitContainerViewH.SuspendLayout();
            this.splitContainerViewUp.SuspendLayout();
            this.splitContainerViewDown.SuspendLayout();
            this.tabControlRight.SuspendLayout();
            this.tabPageActor.SuspendLayout();
            this.tabPageTerrain.SuspendLayout();
            this.tabPageRes.SuspendLayout();
            this.tabFactory.SuspendLayout();
            this.toolMainForm.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.cameraSight)).BeginInit();
            this.contextMenuStrip1.SuspendLayout();
            this.MoveAlignMenu.SuspendLayout();
            this.RotateAlignMenu.SuspendLayout();
            this.ScaleAlignMenu.SuspendLayout();
            this.contextMenuStripViewport.SuspendLayout();
            this.groupActorProp.SuspendLayout();
            this.groupActorComp.SuspendLayout();
            this.groupTrnUtil.SuspendLayout();
            this.groupTrnLayers.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBrush.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownStrength)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownOutter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownInner)).BeginInit();
            this.collapsibleGroupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxResource)).BeginInit();
            this.collapsibleGroupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuMainForm
            // 
            this.menuMainForm.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.selectToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.levelToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuMainForm.Location = new System.Drawing.Point(0, 0);
            this.menuMainForm.Name = "menuMainForm";
            this.menuMainForm.Size = new System.Drawing.Size(1029, 25);
            this.menuMainForm.TabIndex = 0;
            this.menuMainForm.Text = "menuMainForm";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newLevelToolStripMenuItem,
            this.loadLevelToolStripMenuItem,
            this.saveCurrentLevelToolStripMenuItem,
            this.toolStripMenuItem5,
            this.toolStripMenuExportLevel,
            this.toolStripMenuItem2,
            this.MTU,
            this.toolStripMenuItem4,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(39, 21);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newLevelToolStripMenuItem
            // 
            this.newLevelToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.NewLevel;
            this.newLevelToolStripMenuItem.Name = "newLevelToolStripMenuItem";
            this.newLevelToolStripMenuItem.Size = new System.Drawing.Size(183, 22);
            this.newLevelToolStripMenuItem.Text = "New Level";
            this.newLevelToolStripMenuItem.Click += new System.EventHandler(this.newLevelToolStripMenuItem_Click);
            // 
            // loadLevelToolStripMenuItem
            // 
            this.loadLevelToolStripMenuItem.Name = "loadLevelToolStripMenuItem";
            this.loadLevelToolStripMenuItem.Size = new System.Drawing.Size(183, 22);
            this.loadLevelToolStripMenuItem.Text = "Load Level";
            this.loadLevelToolStripMenuItem.Click += new System.EventHandler(this.loadLevelToolStripMenuItem_Click);
            // 
            // saveCurrentLevelToolStripMenuItem
            // 
            this.saveCurrentLevelToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.Save;
            this.saveCurrentLevelToolStripMenuItem.Name = "saveCurrentLevelToolStripMenuItem";
            this.saveCurrentLevelToolStripMenuItem.Size = new System.Drawing.Size(183, 22);
            this.saveCurrentLevelToolStripMenuItem.Text = "Save Current Level";
            this.saveCurrentLevelToolStripMenuItem.Click += new System.EventHandler(this.saveCurrentLevelToolStripMenuItem_Click);
            // 
            // toolStripMenuItem5
            // 
            this.toolStripMenuItem5.Name = "toolStripMenuItem5";
            this.toolStripMenuItem5.Size = new System.Drawing.Size(180, 6);
            // 
            // toolStripMenuExportLevel
            // 
            this.toolStripMenuExportLevel.Name = "toolStripMenuExportLevel";
            this.toolStripMenuExportLevel.Size = new System.Drawing.Size(183, 22);
            this.toolStripMenuExportLevel.Text = "Export...";
            this.toolStripMenuExportLevel.Click += new System.EventHandler(this.toolStripMenuExportLevel_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(180, 6);
            // 
            // MTU
            // 
            this.MTU.Name = "MTU";
            this.MTU.Size = new System.Drawing.Size(183, 22);
            this.MTU.Text = "最近使用的文件";
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(180, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(183, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.CopyToolStripMenuItem,
            this.CutToolStripMenuItem,
            this.PasteToolStripMenuItem,
            this.toolStripMenuItem30,
            this.optionsToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(42, 21);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // CopyToolStripMenuItem
            // 
            this.CopyToolStripMenuItem.Name = "CopyToolStripMenuItem";
            this.CopyToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.CopyToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.CopyToolStripMenuItem.Text = "复制";
            this.CopyToolStripMenuItem.Click += new System.EventHandler(this.CopyToolStripMenuItem_Click);
            // 
            // CutToolStripMenuItem
            // 
            this.CutToolStripMenuItem.Name = "CutToolStripMenuItem";
            this.CutToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.CutToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.CutToolStripMenuItem.Text = "剪切";
            this.CutToolStripMenuItem.Click += new System.EventHandler(this.CutToolStripMenuItem_Click);
            // 
            // PasteToolStripMenuItem
            // 
            this.PasteToolStripMenuItem.Name = "PasteToolStripMenuItem";
            this.PasteToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.PasteToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.PasteToolStripMenuItem.Text = "粘贴";
            this.PasteToolStripMenuItem.Click += new System.EventHandler(this.PasteToolStripMenuItem_Click);
            // 
            // toolStripMenuItem30
            // 
            this.toolStripMenuItem30.Name = "toolStripMenuItem30";
            this.toolStripMenuItem30.Size = new System.Drawing.Size(142, 6);
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.optionsToolStripMenuItem.Text = "Options";
            this.optionsToolStripMenuItem.Click += new System.EventHandler(this.optionsToolStripMenuItem_Click);
            // 
            // selectToolStripMenuItem
            // 
            this.selectToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuItemSelSameResource,
            this.menuItemAllInLayer,
            this.menuItemSelOthers,
            this.menuItemUnselAll});
            this.selectToolStripMenuItem.Name = "selectToolStripMenuItem";
            this.selectToolStripMenuItem.Size = new System.Drawing.Size(54, 21);
            this.selectToolStripMenuItem.Text = "Select";
            // 
            // menuItemSelSameResource
            // 
            this.menuItemSelSameResource.Name = "menuItemSelSameResource";
            this.menuItemSelSameResource.Size = new System.Drawing.Size(205, 22);
            this.menuItemSelSameResource.Text = "所有同类对象";
            this.menuItemSelSameResource.Click += new System.EventHandler(this.menuItemSelSameResource_Click);
            // 
            // menuItemAllInLayer
            // 
            this.menuItemAllInLayer.Name = "menuItemAllInLayer";
            this.menuItemAllInLayer.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
            this.menuItemAllInLayer.Size = new System.Drawing.Size(205, 22);
            this.menuItemAllInLayer.Text = "当前层所有对象";
            this.menuItemAllInLayer.Click += new System.EventHandler(this.menuItemAllInLayer_Click);
            // 
            // menuItemSelOthers
            // 
            this.menuItemSelOthers.Name = "menuItemSelOthers";
            this.menuItemSelOthers.Size = new System.Drawing.Size(205, 22);
            this.menuItemSelOthers.Text = "反向选择";
            this.menuItemSelOthers.Click += new System.EventHandler(this.menuItemSelOthers_Click);
            // 
            // menuItemUnselAll
            // 
            this.menuItemUnselAll.Name = "menuItemUnselAll";
            this.menuItemUnselAll.Size = new System.Drawing.Size(205, 22);
            this.menuItemUnselAll.Text = "取消选择";
            this.menuItemUnselAll.Click += new System.EventHandler(this.menuItemUnselAll_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.resetLayoutToolStripMenuItem,
            this.showResourceManagerToolStripMenuItem,
            this.showLayerManagerToolStripMenuItem,
            this.showLogToolStripMenuItem,
            this.toolStripMenuItem1,
            this.activeViewOnlyToolStripMenuItem,
            this.fourViewsToolStripMenuItem,
            this.twoViewportsToolStripMenuItem,
            this.fitViewToolStripMenuItem,
            this.toolStripSeparator6,
            this.showToolStripMenuItem,
            this.showNavigationMapToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(47, 21);
            this.viewToolStripMenuItem.Text = "View";
            // 
            // resetLayoutToolStripMenuItem
            // 
            this.resetLayoutToolStripMenuItem.Name = "resetLayoutToolStripMenuItem";
            this.resetLayoutToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.resetLayoutToolStripMenuItem.Text = "Reset Layout";
            this.resetLayoutToolStripMenuItem.Click += new System.EventHandler(this.resetLayoutToolStripMenuItem_Click);
            // 
            // showResourceManagerToolStripMenuItem
            // 
            this.showResourceManagerToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.MediaFolder;
            this.showResourceManagerToolStripMenuItem.Name = "showResourceManagerToolStripMenuItem";
            this.showResourceManagerToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.showResourceManagerToolStripMenuItem.Text = "Show Resource Manager";
            this.showResourceManagerToolStripMenuItem.Click += new System.EventHandler(this.showResourceManagerToolStripMenuItem_Click);
            // 
            // showLayerManagerToolStripMenuItem
            // 
            this.showLayerManagerToolStripMenuItem.Name = "showLayerManagerToolStripMenuItem";
            this.showLayerManagerToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.showLayerManagerToolStripMenuItem.Text = "Show Layer Manager";
            this.showLayerManagerToolStripMenuItem.Click += new System.EventHandler(this.toolLevelLayers_Click);
            // 
            // showLogToolStripMenuItem
            // 
            this.showLogToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.Console;
            this.showLogToolStripMenuItem.Name = "showLogToolStripMenuItem";
            this.showLogToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.showLogToolStripMenuItem.Text = "Show Log";
            this.showLogToolStripMenuItem.Click += new System.EventHandler(this.toolBtnShowLog_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(219, 6);
            // 
            // activeViewOnlyToolStripMenuItem
            // 
            this.activeViewOnlyToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.MaxView;
            this.activeViewOnlyToolStripMenuItem.Name = "activeViewOnlyToolStripMenuItem";
            this.activeViewOnlyToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.activeViewOnlyToolStripMenuItem.Text = "Active View Only";
            this.activeViewOnlyToolStripMenuItem.Click += new System.EventHandler(this.activeViewOnlyToolStripMenuItem_Click);
            // 
            // fourViewsToolStripMenuItem
            // 
            this.fourViewsToolStripMenuItem.Name = "fourViewsToolStripMenuItem";
            this.fourViewsToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.fourViewsToolStripMenuItem.Text = "Four Viewports";
            this.fourViewsToolStripMenuItem.Click += new System.EventHandler(this.fourViewsToolStripMenuItem_Click);
            // 
            // twoViewportsToolStripMenuItem
            // 
            this.twoViewportsToolStripMenuItem.Name = "twoViewportsToolStripMenuItem";
            this.twoViewportsToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.twoViewportsToolStripMenuItem.Text = "Two Viewports";
            this.twoViewportsToolStripMenuItem.Click += new System.EventHandler(this.twoViewportsToolStripMenuItem_Click);
            // 
            // fitViewToolStripMenuItem
            // 
            this.fitViewToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.FitView;
            this.fitViewToolStripMenuItem.Name = "fitViewToolStripMenuItem";
            this.fitViewToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.fitViewToolStripMenuItem.Text = "Fit View";
            this.fitViewToolStripMenuItem.Click += new System.EventHandler(this.fitViewToolStripMenuItem_Click);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(219, 6);
            // 
            // showToolStripMenuItem
            // 
            this.showToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showAllToolStripMenuItem,
            this.hideAllToolStripMenuItem,
            this.showSelOnlyToolStripMenuItem1,
            this.toolStripMenuItem29,
            this.hideTerrainsToolStripMenuItem,
            this.toolStripMenuItem32});
            this.showToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.ArrowList;
            this.showToolStripMenuItem.Name = "showToolStripMenuItem";
            this.showToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.showToolStripMenuItem.Text = "Show Actors";
            this.showToolStripMenuItem.DropDownOpening += new System.EventHandler(this.showToolStripMenuItem_DropDownOpening);
            // 
            // showAllToolStripMenuItem
            // 
            this.showAllToolStripMenuItem.Name = "showAllToolStripMenuItem";
            this.showAllToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.showAllToolStripMenuItem.Text = "Show All";
            this.showAllToolStripMenuItem.Click += new System.EventHandler(this.showAllToolStripMenuItem_Click);
            // 
            // hideAllToolStripMenuItem
            // 
            this.hideAllToolStripMenuItem.Name = "hideAllToolStripMenuItem";
            this.hideAllToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.hideAllToolStripMenuItem.Text = "Hide All";
            this.hideAllToolStripMenuItem.Click += new System.EventHandler(this.hideAllToolStripMenuItem_Click);
            // 
            // showSelOnlyToolStripMenuItem1
            // 
            this.showSelOnlyToolStripMenuItem1.Name = "showSelOnlyToolStripMenuItem1";
            this.showSelOnlyToolStripMenuItem1.Size = new System.Drawing.Size(190, 22);
            this.showSelOnlyToolStripMenuItem1.Text = "Show Selected Only";
            this.showSelOnlyToolStripMenuItem1.Click += new System.EventHandler(this.showSelOnlyToolStripMenuItem1_Click);
            // 
            // toolStripMenuItem29
            // 
            this.toolStripMenuItem29.Name = "toolStripMenuItem29";
            this.toolStripMenuItem29.Size = new System.Drawing.Size(187, 6);
            // 
            // hideTerrainsToolStripMenuItem
            // 
            this.hideTerrainsToolStripMenuItem.Name = "hideTerrainsToolStripMenuItem";
            this.hideTerrainsToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.hideTerrainsToolStripMenuItem.Text = "Hide Terrains";
            this.hideTerrainsToolStripMenuItem.CheckedChanged += new System.EventHandler(this.showTerrainsToolStripMenuItem_CheckedChanged);
            this.hideTerrainsToolStripMenuItem.Click += new System.EventHandler(this.showTerrainsToolStripMenuItem_Click);
            // 
            // toolStripMenuItem32
            // 
            this.toolStripMenuItem32.Name = "toolStripMenuItem32";
            this.toolStripMenuItem32.Size = new System.Drawing.Size(190, 22);
            this.toolStripMenuItem32.Text = " ";
            // 
            // showNavigationMapToolStripMenuItem
            // 
            this.showNavigationMapToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.navigationHeightMapToolStripMenuItem,
            this.navigationWalkableMapToolStripMenuItem});
            this.showNavigationMapToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.nav;
            this.showNavigationMapToolStripMenuItem.Name = "showNavigationMapToolStripMenuItem";
            this.showNavigationMapToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.showNavigationMapToolStripMenuItem.Text = "Show Navigation Map";
            this.showNavigationMapToolStripMenuItem.DropDownOpening += new System.EventHandler(this.showNavigationMapToolStripMenuItem_DropDownOpening);
            // 
            // navigationHeightMapToolStripMenuItem
            // 
            this.navigationHeightMapToolStripMenuItem.Name = "navigationHeightMapToolStripMenuItem";
            this.navigationHeightMapToolStripMenuItem.Size = new System.Drawing.Size(228, 22);
            this.navigationHeightMapToolStripMenuItem.Text = "Navigation Height Map";
            this.navigationHeightMapToolStripMenuItem.CheckedChanged += new System.EventHandler(this.navigationHeightMapToolStripMenuItem_CheckedChanged);
            this.navigationHeightMapToolStripMenuItem.Click += new System.EventHandler(this.navigationHeightMapToolStripMenuItem_Click);
            // 
            // navigationWalkableMapToolStripMenuItem
            // 
            this.navigationWalkableMapToolStripMenuItem.Name = "navigationWalkableMapToolStripMenuItem";
            this.navigationWalkableMapToolStripMenuItem.Size = new System.Drawing.Size(228, 22);
            this.navigationWalkableMapToolStripMenuItem.Text = "Navigation Walkable Map";
            this.navigationWalkableMapToolStripMenuItem.CheckedChanged += new System.EventHandler(this.navigationWalkableMapToolStripMenuItem_CheckedChanged);
            this.navigationWalkableMapToolStripMenuItem.Click += new System.EventHandler(this.navigationWalkableMapToolStripMenuItem_Click);
            // 
            // levelToolStripMenuItem
            // 
            this.levelToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createTerrainToolStripMenuItem,
            this.toolStripSeparator2,
            this.actorListToolStripMenuItem,
            this.globalSettingsToolStripMenuItem});
            this.levelToolStripMenuItem.Name = "levelToolStripMenuItem";
            this.levelToolStripMenuItem.Size = new System.Drawing.Size(49, 21);
            this.levelToolStripMenuItem.Text = "Level";
            // 
            // createTerrainToolStripMenuItem
            // 
            this.createTerrainToolStripMenuItem.Name = "createTerrainToolStripMenuItem";
            this.createTerrainToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.createTerrainToolStripMenuItem.Text = "New Terrain ...";
            this.createTerrainToolStripMenuItem.Click += new System.EventHandler(this.newTerrainToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(157, 6);
            // 
            // actorListToolStripMenuItem
            // 
            this.actorListToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.ArrowList;
            this.actorListToolStripMenuItem.Name = "actorListToolStripMenuItem";
            this.actorListToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.actorListToolStripMenuItem.Text = "Actor List ...";
            this.actorListToolStripMenuItem.Click += new System.EventHandler(this.actorListToolStripMenuItem_Click);
            // 
            // globalSettingsToolStripMenuItem
            // 
            this.globalSettingsToolStripMenuItem.Name = "globalSettingsToolStripMenuItem";
            this.globalSettingsToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.globalSettingsToolStripMenuItem.Text = "全局效果设置";
            this.globalSettingsToolStripMenuItem.Click += new System.EventHandler(this.globalSettingsToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuAbout});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(47, 21);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // menuAbout
            // 
            this.menuAbout.Name = "menuAbout";
            this.menuAbout.Size = new System.Drawing.Size(111, 22);
            this.menuAbout.Text = "&About";
            this.menuAbout.Click += new System.EventHandler(this.menuAbout_Click);
            // 
            // statusMainForm
            // 
            this.statusMainForm.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLableNote,
            this.statusSelectionLock,
            this.CameraSpeed,
            this.toolStripStatusLabelAutoSave,
            this.m_MoveAlignStatusLabel,
            this.m_RotateAlignStatusLabel,
            this.m_ScaleAlignStatusLabel});
            this.statusMainForm.Location = new System.Drawing.Point(0, 746);
            this.statusMainForm.Name = "statusMainForm";
            this.statusMainForm.Size = new System.Drawing.Size(1029, 26);
            this.statusMainForm.TabIndex = 2;
            this.statusMainForm.Text = "statusMainForm";
            // 
            // toolStripStatusLableNote
            // 
            this.toolStripStatusLableNote.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
            this.toolStripStatusLableNote.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenInner;
            this.toolStripStatusLableNote.Name = "toolStripStatusLableNote";
            this.toolStripStatusLableNote.Size = new System.Drawing.Size(48, 21);
            this.toolStripStatusLableNote.Text = "Ready";
            // 
            // statusSelectionLock
            // 
            this.statusSelectionLock.BorderStyle = System.Windows.Forms.Border3DStyle.Bump;
            this.statusSelectionLock.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.statusSelectionLock.Image = global::NexusEditor.Properties.Resources.unlock;
            this.statusSelectionLock.ImageTransparentColor = System.Drawing.Color.White;
            this.statusSelectionLock.Name = "statusSelectionLock";
            this.statusSelectionLock.Size = new System.Drawing.Size(16, 21);
            // 
            // CameraSpeed
            // 
            this.CameraSpeed.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
            this.CameraSpeed.BorderStyle = System.Windows.Forms.Border3DStyle.Etched;
            this.CameraSpeed.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.CameraSpeed.Name = "CameraSpeed";
            this.CameraSpeed.Size = new System.Drawing.Size(96, 21);
            this.CameraSpeed.Text = "摄像机操作速度";
            // 
            // toolStripStatusLabelAutoSave
            // 
            this.toolStripStatusLabelAutoSave.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
            this.toolStripStatusLabelAutoSave.BorderStyle = System.Windows.Forms.Border3DStyle.Etched;
            this.toolStripStatusLabelAutoSave.Name = "toolStripStatusLabelAutoSave";
            this.toolStripStatusLabelAutoSave.Size = new System.Drawing.Size(60, 21);
            this.toolStripStatusLabelAutoSave.Text = "自动保存";
            this.toolStripStatusLabelAutoSave.Click += new System.EventHandler(this.toolStripStatusLabel1_Click);
            // 
            // m_MoveAlignStatusLabel
            // 
            this.m_MoveAlignStatusLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
            this.m_MoveAlignStatusLabel.BorderStyle = System.Windows.Forms.Border3DStyle.Etched;
            this.m_MoveAlignStatusLabel.Name = "m_MoveAlignStatusLabel";
            this.m_MoveAlignStatusLabel.Size = new System.Drawing.Size(60, 21);
            this.m_MoveAlignStatusLabel.Text = "移动对齐";
            this.m_MoveAlignStatusLabel.Click += new System.EventHandler(this.toolStripStatusLabel1_Click_1);
            // 
            // m_RotateAlignStatusLabel
            // 
            this.m_RotateAlignStatusLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
            this.m_RotateAlignStatusLabel.BorderStyle = System.Windows.Forms.Border3DStyle.Etched;
            this.m_RotateAlignStatusLabel.Name = "m_RotateAlignStatusLabel";
            this.m_RotateAlignStatusLabel.Size = new System.Drawing.Size(60, 21);
            this.m_RotateAlignStatusLabel.Text = "旋转对齐";
            this.m_RotateAlignStatusLabel.Click += new System.EventHandler(this.toolStripStatusLabel2_Click);
            // 
            // m_ScaleAlignStatusLabel
            // 
            this.m_ScaleAlignStatusLabel.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
            this.m_ScaleAlignStatusLabel.BorderStyle = System.Windows.Forms.Border3DStyle.Etched;
            this.m_ScaleAlignStatusLabel.Name = "m_ScaleAlignStatusLabel";
            this.m_ScaleAlignStatusLabel.Size = new System.Drawing.Size(60, 21);
            this.m_ScaleAlignStatusLabel.Text = "缩放对齐";
            this.m_ScaleAlignStatusLabel.Click += new System.EventHandler(this.toolStripStatusLabel3_Click);
            // 
            // splitMainForm
            // 
            this.splitMainForm.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitMainForm.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitMainForm.IsSplitterFixed = true;
            this.splitMainForm.Location = new System.Drawing.Point(0, 56);
            this.splitMainForm.Name = "splitMainForm";
            // 
            // splitMainForm.Panel1
            // 
            this.splitMainForm.Panel1.Controls.Add(this.splitContainerViewH);
            this.splitMainForm.Panel1MinSize = 32;
            // 
            // splitMainForm.Panel2
            // 
            this.splitMainForm.Panel2.Controls.Add(this.tabControlRight);
            this.splitMainForm.Size = new System.Drawing.Size(1029, 690);
            this.splitMainForm.SplitterDistance = 782;
            this.splitMainForm.SplitterWidth = 2;
            this.splitMainForm.TabIndex = 3;
            // 
            // splitContainerViewH
            // 
            this.splitContainerViewH.AllowDrop = true;
            this.splitContainerViewH.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitContainerViewH.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerViewH.Location = new System.Drawing.Point(0, 0);
            this.splitContainerViewH.Name = "splitContainerViewH";
            this.splitContainerViewH.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerViewH.Panel1
            // 
            this.splitContainerViewH.Panel1.Controls.Add(this.splitContainerViewUp);
            // 
            // splitContainerViewH.Panel2
            // 
            this.splitContainerViewH.Panel2.Controls.Add(this.splitContainerViewDown);
            this.splitContainerViewH.Size = new System.Drawing.Size(782, 690);
            this.splitContainerViewH.SplitterDistance = 311;
            this.splitContainerViewH.SplitterWidth = 2;
            this.splitContainerViewH.TabIndex = 0;
            // 
            // splitContainerViewUp
            // 
            this.splitContainerViewUp.AllowDrop = true;
            this.splitContainerViewUp.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitContainerViewUp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerViewUp.Location = new System.Drawing.Point(0, 0);
            this.splitContainerViewUp.Name = "splitContainerViewUp";
            // 
            // splitContainerViewUp.Panel1
            // 
            this.splitContainerViewUp.Panel1.AllowDrop = true;
            // 
            // splitContainerViewUp.Panel2
            // 
            this.splitContainerViewUp.Panel2.AllowDrop = true;
            this.splitContainerViewUp.Size = new System.Drawing.Size(782, 311);
            this.splitContainerViewUp.SplitterDistance = 291;
            this.splitContainerViewUp.SplitterWidth = 2;
            this.splitContainerViewUp.TabIndex = 0;
            // 
            // splitContainerViewDown
            // 
            this.splitContainerViewDown.AllowDrop = true;
            this.splitContainerViewDown.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitContainerViewDown.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerViewDown.Location = new System.Drawing.Point(0, 0);
            this.splitContainerViewDown.Name = "splitContainerViewDown";
            // 
            // splitContainerViewDown.Panel1
            // 
            this.splitContainerViewDown.Panel1.AllowDrop = true;
            // 
            // splitContainerViewDown.Panel2
            // 
            this.splitContainerViewDown.Panel2.AllowDrop = true;
            this.splitContainerViewDown.Size = new System.Drawing.Size(782, 377);
            this.splitContainerViewDown.SplitterDistance = 291;
            this.splitContainerViewDown.SplitterWidth = 2;
            this.splitContainerViewDown.TabIndex = 0;
            // 
            // tabControlRight
            // 
            this.tabControlRight.Controls.Add(this.tabPageActor);
            this.tabControlRight.Controls.Add(this.tabPageTerrain);
            this.tabControlRight.Controls.Add(this.tabPageRes);
            this.tabControlRight.Controls.Add(this.tabFactory);
            this.tabControlRight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlRight.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tabControlRight.ImageList = this.imageListTab;
            this.tabControlRight.Location = new System.Drawing.Point(0, 0);
            this.tabControlRight.Multiline = true;
            this.tabControlRight.Name = "tabControlRight";
            this.tabControlRight.SelectedIndex = 0;
            this.tabControlRight.ShowToolTips = true;
            this.tabControlRight.Size = new System.Drawing.Size(245, 690);
            this.tabControlRight.SizeMode = System.Windows.Forms.TabSizeMode.FillToRight;
            this.tabControlRight.TabIndex = 0;
            // 
            // tabPageActor
            // 
            this.tabPageActor.Controls.Add(this.groupActorProp);
            this.tabPageActor.Controls.Add(this.groupActorComp);
            this.tabPageActor.ImageKey = "user_suit.gif";
            this.tabPageActor.Location = new System.Drawing.Point(4, 42);
            this.tabPageActor.Name = "tabPageActor";
            this.tabPageActor.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageActor.Size = new System.Drawing.Size(237, 644);
            this.tabPageActor.TabIndex = 3;
            this.tabPageActor.Text = "Actor";
            this.tabPageActor.UseVisualStyleBackColor = true;
            // 
            // tabPageTerrain
            // 
            this.tabPageTerrain.Controls.Add(this.groupTrnUtil);
            this.tabPageTerrain.Controls.Add(this.groupTrnLayers);
            this.tabPageTerrain.Controls.Add(this.groupBrush);
            this.tabPageTerrain.ImageKey = "world.gif";
            this.tabPageTerrain.Location = new System.Drawing.Point(4, 42);
            this.tabPageTerrain.Name = "tabPageTerrain";
            this.tabPageTerrain.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTerrain.Size = new System.Drawing.Size(237, 644);
            this.tabPageTerrain.TabIndex = 2;
            this.tabPageTerrain.Text = "Terrain";
            this.tabPageTerrain.UseVisualStyleBackColor = true;
            // 
            // imageListTrnTarget
            // 
            this.imageListTrnTarget.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListTrnTarget.ImageStream")));
            this.imageListTrnTarget.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListTrnTarget.Images.SetKeyName(0, "Heightmap");
            this.imageListTrnTarget.Images.SetKeyName(1, "NoTexture");
            // 
            // tabPageRes
            // 
            this.tabPageRes.Controls.Add(this.collapsibleGroupBox2);
            this.tabPageRes.Controls.Add(this.collapsibleGroupBox1);
            this.tabPageRes.ImageKey = "photo.gif";
            this.tabPageRes.Location = new System.Drawing.Point(4, 42);
            this.tabPageRes.Name = "tabPageRes";
            this.tabPageRes.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageRes.Size = new System.Drawing.Size(237, 644);
            this.tabPageRes.TabIndex = 4;
            this.tabPageRes.Text = "Resource";
            this.tabPageRes.UseVisualStyleBackColor = true;
            // 
            // imageListResource
            // 
            this.imageListResource.ColorDepth = System.Windows.Forms.ColorDepth.Depth24Bit;
            this.imageListResource.ImageSize = new System.Drawing.Size(48, 48);
            this.imageListResource.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // tabFactory
            // 
            this.tabFactory.Controls.Add(this.listViewClass);
            this.tabFactory.ImageKey = "factory.bmp";
            this.tabFactory.Location = new System.Drawing.Point(4, 42);
            this.tabFactory.Name = "tabFactory";
            this.tabFactory.Padding = new System.Windows.Forms.Padding(3);
            this.tabFactory.Size = new System.Drawing.Size(237, 644);
            this.tabFactory.TabIndex = 5;
            this.tabFactory.Text = "Factory";
            this.tabFactory.UseVisualStyleBackColor = true;
            // 
            // listViewClass
            // 
            this.listViewClass.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.listViewClass.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.className,
            this.nativeClassName});
            this.listViewClass.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewClass.FullRowSelect = true;
            this.listViewClass.GridLines = true;
            this.listViewClass.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1,
            listViewItem2});
            this.listViewClass.Location = new System.Drawing.Point(3, 3);
            this.listViewClass.MultiSelect = false;
            this.listViewClass.Name = "listViewClass";
            this.listViewClass.Size = new System.Drawing.Size(231, 638);
            this.listViewClass.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.listViewClass.TabIndex = 1;
            this.listViewClass.UseCompatibleStateImageBehavior = false;
            this.listViewClass.View = System.Windows.Forms.View.Tile;
            // 
            // className
            // 
            this.className.Text = "Class";
            // 
            // nativeClassName
            // 
            this.nativeClassName.Text = "NativeClass";
            // 
            // imageListTab
            // 
            this.imageListTab.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListTab.ImageStream")));
            this.imageListTab.TransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(185)))), ((int)(((byte)(185)))), ((int)(((byte)(185)))));
            this.imageListTab.Images.SetKeyName(0, "world.gif");
            this.imageListTab.Images.SetKeyName(1, "user_suit.gif");
            this.imageListTab.Images.SetKeyName(2, "photo.gif");
            this.imageListTab.Images.SetKeyName(3, "factory.bmp");
            // 
            // imageListCtrlMode
            // 
            this.imageListCtrlMode.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListCtrlMode.ImageStream")));
            this.imageListCtrlMode.TransparentColor = System.Drawing.Color.Magenta;
            this.imageListCtrlMode.Images.SetKeyName(0, "Camera.bmp");
            this.imageListCtrlMode.Images.SetKeyName(1, "TerrainMode.bmp");
            this.imageListCtrlMode.Images.SetKeyName(2, "ActorMode.bmp");
            // 
            // imageListCommand
            // 
            this.imageListCommand.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListCommand.ImageStream")));
            this.imageListCommand.TransparentColor = System.Drawing.Color.Magenta;
            this.imageListCommand.Images.SetKeyName(0, "Pick");
            this.imageListCommand.Images.SetKeyName(1, "Translate");
            this.imageListCommand.Images.SetKeyName(2, "Rotate");
            this.imageListCommand.Images.SetKeyName(3, "Camera");
            this.imageListCommand.Images.SetKeyName(4, "Scale");
            this.imageListCommand.Images.SetKeyName(5, "TerrainMode");
            this.imageListCommand.Images.SetKeyName(6, "PanView");
            this.imageListCommand.Images.SetKeyName(7, "RotateView");
            // 
            // toolMainForm
            // 
            this.toolMainForm.BackgroundImage = global::NexusEditor.Properties.Resources.gray;
            this.toolMainForm.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.toolMainForm.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolBtnUndo,
            this.toolBtnRedo,
            this.toolStripSeparator1,
            this.toolBtnPick,
            this.toolBtnCamera,
            this.toolBtnTerrainEd,
            this.toolBtnNavEditor,
            this.toolBtnTranslate,
            this.toolBtnRotate,
            this.toolBtnScale,
            this.toolStripSeparator7,
            this.toolStripReplace,
            this.toolSpaceList,
            this.toolBtnPanView,
            this.toolBtnRotateView,
            this.toolStripSeparator3,
            this.toolBtnActorList,
            this.toolBtnMaxView,
            this.toolBtnCoord,
            this.toolLevelLayers,
            this.toolStripSeparator4,
            this.toolBtnShowResMgr,
            this.toolBtnShowLog,
            this.toolStripSeparator5,
            this.PIE});
            this.toolMainForm.Location = new System.Drawing.Point(0, 25);
            this.toolMainForm.Name = "toolMainForm";
            this.toolMainForm.Size = new System.Drawing.Size(1029, 31);
            this.toolMainForm.TabIndex = 1;
            this.toolMainForm.Text = "toolMainForm";
            // 
            // toolBtnUndo
            // 
            this.toolBtnUndo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnUndo.Image = global::NexusEditor.Properties.Resources.Undo;
            this.toolBtnUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnUndo.Name = "toolBtnUndo";
            this.toolBtnUndo.Size = new System.Drawing.Size(28, 28);
            this.toolBtnUndo.Text = "Undo";
            this.toolBtnUndo.Click += new System.EventHandler(this.toolBtnUndo_Click);
            // 
            // toolBtnRedo
            // 
            this.toolBtnRedo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnRedo.Image = global::NexusEditor.Properties.Resources.Redo;
            this.toolBtnRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnRedo.Name = "toolBtnRedo";
            this.toolBtnRedo.Size = new System.Drawing.Size(28, 28);
            this.toolBtnRedo.Text = "Redo";
            this.toolBtnRedo.Click += new System.EventHandler(this.toolBtnRedo_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 31);
            // 
            // toolBtnPick
            // 
            this.toolBtnPick.Checked = true;
            this.toolBtnPick.CheckState = System.Windows.Forms.CheckState.Checked;
            this.toolBtnPick.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnPick.Image = global::NexusEditor.Properties.Resources.Pick;
            this.toolBtnPick.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnPick.Name = "toolBtnPick";
            this.toolBtnPick.Size = new System.Drawing.Size(28, 28);
            this.toolBtnPick.Text = "Select Object";
            // 
            // toolBtnCamera
            // 
            this.toolBtnCamera.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnCamera.Image = ((System.Drawing.Image)(resources.GetObject("toolBtnCamera.Image")));
            this.toolBtnCamera.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnCamera.Name = "toolBtnCamera";
            this.toolBtnCamera.Size = new System.Drawing.Size(28, 28);
            this.toolBtnCamera.Text = "Free Camera";
            this.toolBtnCamera.MouseUp += new System.Windows.Forms.MouseEventHandler(this.toolBtnCamera_MouseUp);
            // 
            // toolBtnTerrainEd
            // 
            this.toolBtnTerrainEd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnTerrainEd.Image = global::NexusEditor.Properties.Resources.TerrainMode;
            this.toolBtnTerrainEd.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnTerrainEd.Name = "toolBtnTerrainEd";
            this.toolBtnTerrainEd.Size = new System.Drawing.Size(28, 28);
            this.toolBtnTerrainEd.Text = "Terrain Editor Mode";
            // 
            // toolBtnNavEditor
            // 
            this.toolBtnNavEditor.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnNavEditor.Image = global::NexusEditor.Properties.Resources.nav;
            this.toolBtnNavEditor.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnNavEditor.Name = "toolBtnNavEditor";
            this.toolBtnNavEditor.Size = new System.Drawing.Size(28, 28);
            this.toolBtnNavEditor.Text = "Navigate Map Editor Mode";
            // 
            // toolBtnTranslate
            // 
            this.toolBtnTranslate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnTranslate.Image = global::NexusEditor.Properties.Resources.Translate;
            this.toolBtnTranslate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnTranslate.Name = "toolBtnTranslate";
            this.toolBtnTranslate.Size = new System.Drawing.Size(28, 28);
            this.toolBtnTranslate.Text = "Select and Move";
            // 
            // toolBtnRotate
            // 
            this.toolBtnRotate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnRotate.Image = global::NexusEditor.Properties.Resources.Rotate;
            this.toolBtnRotate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnRotate.Name = "toolBtnRotate";
            this.toolBtnRotate.Size = new System.Drawing.Size(28, 28);
            this.toolBtnRotate.Text = "Select and Rotate";
            // 
            // toolBtnScale
            // 
            this.toolBtnScale.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnScale.Image = global::NexusEditor.Properties.Resources.Scale;
            this.toolBtnScale.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnScale.Name = "toolBtnScale";
            this.toolBtnScale.Size = new System.Drawing.Size(28, 28);
            this.toolBtnScale.Text = "Select and Scale";
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 31);
            // 
            // toolStripReplace
            // 
            this.toolStripReplace.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripReplace.Image = global::NexusEditor.Properties.Resources.Sort;
            this.toolStripReplace.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripReplace.Name = "toolStripReplace";
            this.toolStripReplace.Size = new System.Drawing.Size(28, 28);
            this.toolStripReplace.Text = "替换";
            this.toolStripReplace.ToolTipText = "用Resource中的对象替换选中对象";
            this.toolStripReplace.Click += new System.EventHandler(this.toolStripReplace_Click);
            // 
            // toolSpaceList
            // 
            this.toolSpaceList.AutoSize = false;
            this.toolSpaceList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.toolSpaceList.Items.AddRange(new object[] {
            "World",
            "Local"});
            this.toolSpaceList.Name = "toolSpaceList";
            this.toolSpaceList.Size = new System.Drawing.Size(80, 25);
            // 
            // toolBtnPanView
            // 
            this.toolBtnPanView.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnPanView.Image = global::NexusEditor.Properties.Resources.PanView;
            this.toolBtnPanView.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnPanView.Name = "toolBtnPanView";
            this.toolBtnPanView.Size = new System.Drawing.Size(28, 28);
            this.toolBtnPanView.Text = "Pan View";
            // 
            // toolBtnRotateView
            // 
            this.toolBtnRotateView.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnRotateView.Image = global::NexusEditor.Properties.Resources.RotateView;
            this.toolBtnRotateView.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnRotateView.Name = "toolBtnRotateView";
            this.toolBtnRotateView.Size = new System.Drawing.Size(28, 28);
            this.toolBtnRotateView.Text = "Arc Rotate View";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.BackColor = System.Drawing.Color.SkyBlue;
            this.toolStripSeparator3.ForeColor = System.Drawing.Color.DodgerBlue;
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 31);
            // 
            // toolBtnActorList
            // 
            this.toolBtnActorList.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnActorList.Image = global::NexusEditor.Properties.Resources.ArrowList;
            this.toolBtnActorList.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnActorList.Name = "toolBtnActorList";
            this.toolBtnActorList.Size = new System.Drawing.Size(28, 28);
            this.toolBtnActorList.Text = "Select by Name";
            // 
            // toolBtnMaxView
            // 
            this.toolBtnMaxView.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnMaxView.Image = global::NexusEditor.Properties.Resources.MaxView;
            this.toolBtnMaxView.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnMaxView.Name = "toolBtnMaxView";
            this.toolBtnMaxView.Size = new System.Drawing.Size(28, 28);
            this.toolBtnMaxView.Text = "Maximize Viewport";
            // 
            // toolBtnCoord
            // 
            this.toolBtnCoord.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnCoord.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.viewToolStripMenuItem1,
            this.screenToolStripMenuItem,
            this.worldToolStripMenuItem,
            this.localToolStripMenuItem});
            this.toolBtnCoord.Image = global::NexusEditor.Properties.Resources.XY;
            this.toolBtnCoord.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnCoord.Name = "toolBtnCoord";
            this.toolBtnCoord.Size = new System.Drawing.Size(40, 28);
            this.toolBtnCoord.Text = "Reference Coordinates";
            this.toolBtnCoord.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // viewToolStripMenuItem1
            // 
            this.viewToolStripMenuItem1.Name = "viewToolStripMenuItem1";
            this.viewToolStripMenuItem1.Size = new System.Drawing.Size(115, 22);
            this.viewToolStripMenuItem1.Text = "View";
            // 
            // screenToolStripMenuItem
            // 
            this.screenToolStripMenuItem.Name = "screenToolStripMenuItem";
            this.screenToolStripMenuItem.Size = new System.Drawing.Size(115, 22);
            this.screenToolStripMenuItem.Text = "Screen";
            // 
            // worldToolStripMenuItem
            // 
            this.worldToolStripMenuItem.Name = "worldToolStripMenuItem";
            this.worldToolStripMenuItem.Size = new System.Drawing.Size(115, 22);
            this.worldToolStripMenuItem.Text = "World";
            // 
            // localToolStripMenuItem
            // 
            this.localToolStripMenuItem.Name = "localToolStripMenuItem";
            this.localToolStripMenuItem.Size = new System.Drawing.Size(115, 22);
            this.localToolStripMenuItem.Text = "Local";
            // 
            // toolLevelLayers
            // 
            this.toolLevelLayers.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolLevelLayers.Image = global::NexusEditor.Properties.Resources.WLayers;
            this.toolLevelLayers.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolLevelLayers.Name = "toolLevelLayers";
            this.toolLevelLayers.Size = new System.Drawing.Size(28, 28);
            this.toolLevelLayers.Text = "LevelLayers";
            this.toolLevelLayers.Click += new System.EventHandler(this.toolLevelLayers_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 31);
            // 
            // toolBtnShowResMgr
            // 
            this.toolBtnShowResMgr.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnShowResMgr.Image = global::NexusEditor.Properties.Resources.MediaFolder;
            this.toolBtnShowResMgr.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnShowResMgr.Name = "toolBtnShowResMgr";
            this.toolBtnShowResMgr.Size = new System.Drawing.Size(28, 28);
            this.toolBtnShowResMgr.Text = "Resource Manager";
            this.toolBtnShowResMgr.Click += new System.EventHandler(this.showResourceManagerToolStripMenuItem_Click);
            // 
            // toolBtnShowLog
            // 
            this.toolBtnShowLog.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnShowLog.Image = global::NexusEditor.Properties.Resources.Console;
            this.toolBtnShowLog.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnShowLog.Name = "toolBtnShowLog";
            this.toolBtnShowLog.Size = new System.Drawing.Size(28, 28);
            this.toolBtnShowLog.Text = "显示Log窗口";
            this.toolBtnShowLog.Click += new System.EventHandler(this.toolBtnShowLog_Click);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 31);
            // 
            // PIE
            // 
            this.PIE.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.PIE.ForeColor = System.Drawing.Color.Transparent;
            this.PIE.Image = global::NexusEditor.Properties.Resources.PIE;
            this.PIE.ImageTransparentColor = System.Drawing.Color.Black;
            this.PIE.Name = "PIE";
            this.PIE.Size = new System.Drawing.Size(28, 28);
            this.PIE.Text = "Play In Editor";
            this.PIE.Click += new System.EventHandler(this.PIE_Click);
            // 
            // cameraSight
            // 
            this.cameraSight.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cameraSight.AutoSize = false;
            this.cameraSight.Location = new System.Drawing.Point(882, 28);
            this.cameraSight.Maximum = 100;
            this.cameraSight.Name = "cameraSight";
            this.cameraSight.Size = new System.Drawing.Size(143, 22);
            this.cameraSight.TabIndex = 4;
            this.cameraSight.TickStyle = System.Windows.Forms.TickStyle.None;
            this.cameraSight.Value = 25;
            this.cameraSight.ValueChanged += new System.EventHandler(this.cameraSight_ValueChanged);
            // 
            // autoSaveTimer
            // 
            this.autoSaveTimer.Interval = 60000;
            this.autoSaveTimer.Tick += new System.EventHandler(this.autoSaveTimer_Tick);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.autoSaveTimeSetting5,
            this.autoSaveTimeSetting10,
            this.autoSaveTimeSetting15,
            this.autoSaveTimeSetting20,
            this.closeAutoSave});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(149, 114);
            this.contextMenuStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.contextMenuStrip1_ItemClicked);
            // 
            // autoSaveTimeSetting5
            // 
            this.autoSaveTimeSetting5.Name = "autoSaveTimeSetting5";
            this.autoSaveTimeSetting5.Size = new System.Drawing.Size(148, 22);
            this.autoSaveTimeSetting5.Tag = "5";
            this.autoSaveTimeSetting5.Text = "5分钟";
            // 
            // autoSaveTimeSetting10
            // 
            this.autoSaveTimeSetting10.Name = "autoSaveTimeSetting10";
            this.autoSaveTimeSetting10.Size = new System.Drawing.Size(148, 22);
            this.autoSaveTimeSetting10.Tag = "10";
            this.autoSaveTimeSetting10.Text = "10分钟";
            // 
            // autoSaveTimeSetting15
            // 
            this.autoSaveTimeSetting15.Name = "autoSaveTimeSetting15";
            this.autoSaveTimeSetting15.Size = new System.Drawing.Size(148, 22);
            this.autoSaveTimeSetting15.Tag = "15";
            this.autoSaveTimeSetting15.Text = "15分钟";
            // 
            // autoSaveTimeSetting20
            // 
            this.autoSaveTimeSetting20.Name = "autoSaveTimeSetting20";
            this.autoSaveTimeSetting20.Size = new System.Drawing.Size(148, 22);
            this.autoSaveTimeSetting20.Tag = "20";
            this.autoSaveTimeSetting20.Text = "20分钟";
            // 
            // closeAutoSave
            // 
            this.closeAutoSave.Checked = true;
            this.closeAutoSave.CheckOnClick = true;
            this.closeAutoSave.CheckState = System.Windows.Forms.CheckState.Checked;
            this.closeAutoSave.Name = "closeAutoSave";
            this.closeAutoSave.Size = new System.Drawing.Size(148, 22);
            this.closeAutoSave.Tag = "-1";
            this.closeAutoSave.Text = "关闭自动保存";
            // 
            // MoveAlignMenu
            // 
            this.MoveAlignMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem6,
            this.toolStripMenuItem7,
            this.toolStripMenuItem8,
            this.toolStripMenuItem9,
            this.toolStripMenuItem20,
            this.toolStripMenuItem21,
            this.toolStripMenuItem22,
            this.toolStripMenuItem23,
            this.toolStripMenuItem24,
            this.toolStripMenuItem25,
            this.toolStripMenuItem26});
            this.MoveAlignMenu.Name = "contextMenuStrip1";
            this.MoveAlignMenu.Size = new System.Drawing.Size(105, 246);
            this.MoveAlignMenu.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.MoveAlignMenu_ItemClicked);
            // 
            // toolStripMenuItem6
            // 
            this.toolStripMenuItem6.Name = "toolStripMenuItem6";
            this.toolStripMenuItem6.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem6.Tag = "1";
            this.toolStripMenuItem6.Text = "1";
            // 
            // toolStripMenuItem7
            // 
            this.toolStripMenuItem7.Name = "toolStripMenuItem7";
            this.toolStripMenuItem7.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem7.Tag = "2";
            this.toolStripMenuItem7.Text = "2";
            // 
            // toolStripMenuItem8
            // 
            this.toolStripMenuItem8.Name = "toolStripMenuItem8";
            this.toolStripMenuItem8.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem8.Tag = "4";
            this.toolStripMenuItem8.Text = "4";
            // 
            // toolStripMenuItem9
            // 
            this.toolStripMenuItem9.Name = "toolStripMenuItem9";
            this.toolStripMenuItem9.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem9.Tag = "8";
            this.toolStripMenuItem9.Text = "8";
            // 
            // toolStripMenuItem20
            // 
            this.toolStripMenuItem20.Name = "toolStripMenuItem20";
            this.toolStripMenuItem20.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem20.Tag = "16";
            this.toolStripMenuItem20.Text = "16";
            // 
            // toolStripMenuItem21
            // 
            this.toolStripMenuItem21.Name = "toolStripMenuItem21";
            this.toolStripMenuItem21.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem21.Tag = "32";
            this.toolStripMenuItem21.Text = "32";
            // 
            // toolStripMenuItem22
            // 
            this.toolStripMenuItem22.Name = "toolStripMenuItem22";
            this.toolStripMenuItem22.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem22.Tag = "64";
            this.toolStripMenuItem22.Text = "64";
            // 
            // toolStripMenuItem23
            // 
            this.toolStripMenuItem23.Name = "toolStripMenuItem23";
            this.toolStripMenuItem23.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem23.Tag = "128";
            this.toolStripMenuItem23.Text = "128";
            // 
            // toolStripMenuItem24
            // 
            this.toolStripMenuItem24.Name = "toolStripMenuItem24";
            this.toolStripMenuItem24.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem24.Tag = "256";
            this.toolStripMenuItem24.Text = "256";
            // 
            // toolStripMenuItem25
            // 
            this.toolStripMenuItem25.Name = "toolStripMenuItem25";
            this.toolStripMenuItem25.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem25.Tag = "512";
            this.toolStripMenuItem25.Text = "512";
            // 
            // toolStripMenuItem26
            // 
            this.toolStripMenuItem26.Name = "toolStripMenuItem26";
            this.toolStripMenuItem26.Size = new System.Drawing.Size(104, 22);
            this.toolStripMenuItem26.Tag = "1024";
            this.toolStripMenuItem26.Text = "1024";
            // 
            // RotateAlignMenu
            // 
            this.RotateAlignMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem10,
            this.toolStripMenuItem11,
            this.toolStripMenuItem12,
            this.toolStripMenuItem13,
            this.toolStripMenuItem14,
            this.toolStripMenuItem27});
            this.RotateAlignMenu.Name = "contextMenuStrip1";
            this.RotateAlignMenu.Size = new System.Drawing.Size(103, 136);
            this.RotateAlignMenu.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.RotateAlignMenu_ItemClicked);
            // 
            // toolStripMenuItem10
            // 
            this.toolStripMenuItem10.Name = "toolStripMenuItem10";
            this.toolStripMenuItem10.Size = new System.Drawing.Size(102, 22);
            this.toolStripMenuItem10.Tag = "3";
            this.toolStripMenuItem10.Text = "3度";
            // 
            // toolStripMenuItem11
            // 
            this.toolStripMenuItem11.Name = "toolStripMenuItem11";
            this.toolStripMenuItem11.Size = new System.Drawing.Size(102, 22);
            this.toolStripMenuItem11.Tag = "6";
            this.toolStripMenuItem11.Text = "6度";
            // 
            // toolStripMenuItem12
            // 
            this.toolStripMenuItem12.Name = "toolStripMenuItem12";
            this.toolStripMenuItem12.Size = new System.Drawing.Size(102, 22);
            this.toolStripMenuItem12.Tag = "15";
            this.toolStripMenuItem12.Text = "15度";
            // 
            // toolStripMenuItem13
            // 
            this.toolStripMenuItem13.Name = "toolStripMenuItem13";
            this.toolStripMenuItem13.Size = new System.Drawing.Size(102, 22);
            this.toolStripMenuItem13.Tag = "30";
            this.toolStripMenuItem13.Text = "30度";
            // 
            // toolStripMenuItem14
            // 
            this.toolStripMenuItem14.Name = "toolStripMenuItem14";
            this.toolStripMenuItem14.Size = new System.Drawing.Size(102, 22);
            this.toolStripMenuItem14.Tag = "45";
            this.toolStripMenuItem14.Text = "45度";
            // 
            // toolStripMenuItem27
            // 
            this.toolStripMenuItem27.Name = "toolStripMenuItem27";
            this.toolStripMenuItem27.Size = new System.Drawing.Size(102, 22);
            this.toolStripMenuItem27.Tag = "90";
            this.toolStripMenuItem27.Text = "90度";
            // 
            // ScaleAlignMenu
            // 
            this.ScaleAlignMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem15,
            this.toolStripMenuItem16,
            this.toolStripMenuItem17,
            this.toolStripMenuItem18,
            this.toolStripMenuItem19,
            this.toolStripMenuItem28});
            this.ScaleAlignMenu.Name = "contextMenuStrip1";
            this.ScaleAlignMenu.Size = new System.Drawing.Size(102, 136);
            this.ScaleAlignMenu.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.ScaleAlignMenu_ItemClicked);
            // 
            // toolStripMenuItem15
            // 
            this.toolStripMenuItem15.Name = "toolStripMenuItem15";
            this.toolStripMenuItem15.Size = new System.Drawing.Size(101, 22);
            this.toolStripMenuItem15.Tag = "1";
            this.toolStripMenuItem15.Text = "1%";
            // 
            // toolStripMenuItem16
            // 
            this.toolStripMenuItem16.Name = "toolStripMenuItem16";
            this.toolStripMenuItem16.Size = new System.Drawing.Size(101, 22);
            this.toolStripMenuItem16.Tag = "2";
            this.toolStripMenuItem16.Text = "2%";
            // 
            // toolStripMenuItem17
            // 
            this.toolStripMenuItem17.Name = "toolStripMenuItem17";
            this.toolStripMenuItem17.Size = new System.Drawing.Size(101, 22);
            this.toolStripMenuItem17.Tag = "5";
            this.toolStripMenuItem17.Text = "5%";
            // 
            // toolStripMenuItem18
            // 
            this.toolStripMenuItem18.Name = "toolStripMenuItem18";
            this.toolStripMenuItem18.Size = new System.Drawing.Size(101, 22);
            this.toolStripMenuItem18.Tag = "10";
            this.toolStripMenuItem18.Text = "10%";
            // 
            // toolStripMenuItem19
            // 
            this.toolStripMenuItem19.Name = "toolStripMenuItem19";
            this.toolStripMenuItem19.Size = new System.Drawing.Size(101, 22);
            this.toolStripMenuItem19.Tag = "25";
            this.toolStripMenuItem19.Text = "25%";
            // 
            // toolStripMenuItem28
            // 
            this.toolStripMenuItem28.Name = "toolStripMenuItem28";
            this.toolStripMenuItem28.Size = new System.Drawing.Size(101, 22);
            this.toolStripMenuItem28.Tag = "50";
            this.toolStripMenuItem28.Text = "50%";
            // 
            // contextMenuStripViewport
            // 
            this.contextMenuStripViewport.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem3,
            this.toolStripSeparator8,
            this.addActorToolStripMenuItem,
            this.toolStripSeparator9,
            this.playFromHereToolStripMenuItem});
            this.contextMenuStripViewport.Name = "contextMenuStripViewport";
            this.contextMenuStripViewport.Size = new System.Drawing.Size(166, 82);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(165, 22);
            this.toolStripMenuItem3.Text = "Delete";
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(162, 6);
            // 
            // addActorToolStripMenuItem
            // 
            this.addActorToolStripMenuItem.Name = "addActorToolStripMenuItem";
            this.addActorToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.addActorToolStripMenuItem.Text = "Add Actor";
            this.addActorToolStripMenuItem.DropDownOpening += new System.EventHandler(this.addActorToolStripMenuItem_DropDownOpening);
            // 
            // toolStripSeparator9
            // 
            this.toolStripSeparator9.Name = "toolStripSeparator9";
            this.toolStripSeparator9.Size = new System.Drawing.Size(162, 6);
            // 
            // playFromHereToolStripMenuItem
            // 
            this.playFromHereToolStripMenuItem.Name = "playFromHereToolStripMenuItem";
            this.playFromHereToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.playFromHereToolStripMenuItem.Text = "Play From Here";
            this.playFromHereToolStripMenuItem.Click += new System.EventHandler(this.playFromHereToolStripMenuItem_Click);
            // 
            // groupActorProp
            // 
            this.groupActorProp.Controls.Add(this.propertyGridActor);
            this.groupActorProp.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupActorProp.Location = new System.Drawing.Point(3, 199);
            this.groupActorProp.Name = "groupActorProp";
            this.groupActorProp.Size = new System.Drawing.Size(231, 381);
            this.groupActorProp.TabIndex = 6;
            this.groupActorProp.TabStop = false;
            this.groupActorProp.Text = "Properties";
            // 
            // propertyGridActor
            // 
            this.propertyGridActor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridActor.Location = new System.Drawing.Point(3, 16);
            this.propertyGridActor.Name = "propertyGridActor";
            this.propertyGridActor.Size = new System.Drawing.Size(225, 362);
            this.propertyGridActor.TabIndex = 4;
            // 
            // groupActorComp
            // 
            this.groupActorComp.Controls.Add(this.treeViewActor);
            this.groupActorComp.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupActorComp.Location = new System.Drawing.Point(3, 3);
            this.groupActorComp.Name = "groupActorComp";
            this.groupActorComp.Size = new System.Drawing.Size(231, 196);
            this.groupActorComp.TabIndex = 5;
            this.groupActorComp.TabStop = false;
            this.groupActorComp.Text = "Components";
            // 
            // treeViewActor
            // 
            this.treeViewActor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewActor.Location = new System.Drawing.Point(3, 16);
            this.treeViewActor.Name = "treeViewActor";
            this.treeViewActor.Size = new System.Drawing.Size(225, 177);
            this.treeViewActor.TabIndex = 5;
            // 
            // groupTrnUtil
            // 
            this.groupTrnUtil.Controls.Add(this.btnTrnTools);
            this.groupTrnUtil.Controls.Add(this.checkBoxShowChunk);
            this.groupTrnUtil.Controls.Add(this.buttonTrnNoise);
            this.groupTrnUtil.Controls.Add(this.btnTrnMtl);
            this.groupTrnUtil.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupTrnUtil.Location = new System.Drawing.Point(3, 535);
            this.groupTrnUtil.Name = "groupTrnUtil";
            this.groupTrnUtil.Size = new System.Drawing.Size(231, 125);
            this.groupTrnUtil.TabIndex = 5;
            this.groupTrnUtil.TabStop = false;
            this.groupTrnUtil.Text = "Util";
            // 
            // btnTrnTools
            // 
            this.btnTrnTools.Location = new System.Drawing.Point(7, 102);
            this.btnTrnTools.Name = "btnTrnTools";
            this.btnTrnTools.Size = new System.Drawing.Size(92, 23);
            this.btnTrnTools.TabIndex = 6;
            this.btnTrnTools.Text = "Tools";
            this.btnTrnTools.UseVisualStyleBackColor = true;
            // 
            // checkBoxShowChunk
            // 
            this.checkBoxShowChunk.AutoSize = true;
            this.checkBoxShowChunk.Checked = true;
            this.checkBoxShowChunk.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxShowChunk.Location = new System.Drawing.Point(7, 20);
            this.checkBoxShowChunk.Name = "checkBoxShowChunk";
            this.checkBoxShowChunk.Size = new System.Drawing.Size(134, 18);
            this.checkBoxShowChunk.TabIndex = 6;
            this.checkBoxShowChunk.Text = "显示地形分块边界线";
            this.checkBoxShowChunk.UseVisualStyleBackColor = true;
            // 
            // buttonTrnNoise
            // 
            this.buttonTrnNoise.Location = new System.Drawing.Point(7, 44);
            this.buttonTrnNoise.Name = "buttonTrnNoise";
            this.buttonTrnNoise.Size = new System.Drawing.Size(92, 23);
            this.buttonTrnNoise.TabIndex = 5;
            this.buttonTrnNoise.Text = "Noise";
            this.buttonTrnNoise.UseVisualStyleBackColor = true;
            // 
            // btnTrnMtl
            // 
            this.btnTrnMtl.Location = new System.Drawing.Point(7, 73);
            this.btnTrnMtl.Name = "btnTrnMtl";
            this.btnTrnMtl.Size = new System.Drawing.Size(92, 23);
            this.btnTrnMtl.TabIndex = 5;
            this.btnTrnMtl.Text = "材质";
            this.btnTrnMtl.UseVisualStyleBackColor = true;
            // 
            // groupTrnLayers
            // 
            this.groupTrnLayers.Controls.Add(this.btnTrnAddLayer);
            this.groupTrnLayers.Controls.Add(this.btnTrnLayerDown);
            this.groupTrnLayers.Controls.Add(this.btnTrnRemoveLayer);
            this.groupTrnLayers.Controls.Add(this.btnTrnLayerUp);
            this.groupTrnLayers.Controls.Add(this.listViewTrnTarget);
            this.groupTrnLayers.Controls.Add(this.pictureBox1);
            this.groupTrnLayers.Controls.Add(this.comboBoxTrnTarget);
            this.groupTrnLayers.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupTrnLayers.Location = new System.Drawing.Point(3, 210);
            this.groupTrnLayers.Name = "groupTrnLayers";
            this.groupTrnLayers.Size = new System.Drawing.Size(231, 325);
            this.groupTrnLayers.TabIndex = 4;
            this.groupTrnLayers.TabStop = false;
            this.groupTrnLayers.Text = "Layers";
            // 
            // btnTrnAddLayer
            // 
            this.btnTrnAddLayer.Image = global::NexusEditor.Properties.Resources.Add;
            this.btnTrnAddLayer.Location = new System.Drawing.Point(41, 276);
            this.btnTrnAddLayer.Name = "btnTrnAddLayer";
            this.btnTrnAddLayer.Size = new System.Drawing.Size(30, 30);
            this.btnTrnAddLayer.TabIndex = 13;
            this.btnTrnAddLayer.UseVisualStyleBackColor = false;
            // 
            // btnTrnLayerDown
            // 
            this.btnTrnLayerDown.Image = global::NexusEditor.Properties.Resources.Down;
            this.btnTrnLayerDown.Location = new System.Drawing.Point(147, 276);
            this.btnTrnLayerDown.Name = "btnTrnLayerDown";
            this.btnTrnLayerDown.Size = new System.Drawing.Size(30, 30);
            this.btnTrnLayerDown.TabIndex = 16;
            this.btnTrnLayerDown.UseVisualStyleBackColor = false;
            // 
            // btnTrnRemoveLayer
            // 
            this.btnTrnRemoveLayer.Image = global::NexusEditor.Properties.Resources.Del;
            this.btnTrnRemoveLayer.Location = new System.Drawing.Point(77, 276);
            this.btnTrnRemoveLayer.Name = "btnTrnRemoveLayer";
            this.btnTrnRemoveLayer.Size = new System.Drawing.Size(30, 30);
            this.btnTrnRemoveLayer.TabIndex = 14;
            this.btnTrnRemoveLayer.UseVisualStyleBackColor = false;
            // 
            // btnTrnLayerUp
            // 
            this.btnTrnLayerUp.Image = global::NexusEditor.Properties.Resources.Up;
            this.btnTrnLayerUp.Location = new System.Drawing.Point(113, 276);
            this.btnTrnLayerUp.Name = "btnTrnLayerUp";
            this.btnTrnLayerUp.Size = new System.Drawing.Size(30, 30);
            this.btnTrnLayerUp.TabIndex = 15;
            this.btnTrnLayerUp.UseVisualStyleBackColor = false;
            // 
            // listViewTrnTarget
            // 
            this.listViewTrnTarget.FullRowSelect = true;
            this.listViewTrnTarget.GridLines = true;
            this.listViewTrnTarget.HideSelection = false;
            this.listViewTrnTarget.LargeImageList = this.imageListTrnTarget;
            this.listViewTrnTarget.Location = new System.Drawing.Point(6, 47);
            this.listViewTrnTarget.MultiSelect = false;
            this.listViewTrnTarget.Name = "listViewTrnTarget";
            this.listViewTrnTarget.Size = new System.Drawing.Size(207, 223);
            this.listViewTrnTarget.SmallImageList = this.imageListTrnTarget;
            this.listViewTrnTarget.TabIndex = 12;
            this.listViewTrnTarget.TileSize = new System.Drawing.Size(200, 42);
            this.listViewTrnTarget.UseCompatibleStateImageBehavior = false;
            this.listViewTrnTarget.View = System.Windows.Forms.View.Tile;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::NexusEditor.Properties.Resources.Layers;
            this.pictureBox1.Location = new System.Drawing.Point(33, 15);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(24, 26);
            this.pictureBox1.TabIndex = 11;
            this.pictureBox1.TabStop = false;
            // 
            // comboBoxTrnTarget
            // 
            this.comboBoxTrnTarget.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTrnTarget.FormattingEnabled = true;
            this.comboBoxTrnTarget.Location = new System.Drawing.Point(63, 19);
            this.comboBoxTrnTarget.Name = "comboBoxTrnTarget";
            this.comboBoxTrnTarget.Size = new System.Drawing.Size(121, 22);
            this.comboBoxTrnTarget.TabIndex = 10;
            // 
            // groupBrush
            // 
            this.groupBrush.Controls.Add(this.numericUpDownStrength);
            this.groupBrush.Controls.Add(this.numericUpDownOutter);
            this.groupBrush.Controls.Add(this.numericUpDownInner);
            this.groupBrush.Controls.Add(label7);
            this.groupBrush.Controls.Add(label6);
            this.groupBrush.Controls.Add(label5);
            this.groupBrush.Controls.Add(this.textBoxTrnFreq);
            this.groupBrush.Controls.Add(label4);
            this.groupBrush.Controls.Add(this.comboBoxTrnLerp);
            this.groupBrush.Controls.Add(label3);
            this.groupBrush.Controls.Add(this.comboBoxTrnShape);
            this.groupBrush.Controls.Add(label2);
            this.groupBrush.Controls.Add(label1);
            this.groupBrush.Controls.Add(this.comboBoxTrnOp);
            this.groupBrush.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBrush.Location = new System.Drawing.Point(3, 3);
            this.groupBrush.Name = "groupBrush";
            this.groupBrush.Size = new System.Drawing.Size(231, 207);
            this.groupBrush.TabIndex = 3;
            this.groupBrush.TabStop = false;
            this.groupBrush.Text = "Brush";
            // 
            // numericUpDownStrength
            // 
            this.numericUpDownStrength.Location = new System.Drawing.Point(87, 174);
            this.numericUpDownStrength.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericUpDownStrength.Name = "numericUpDownStrength";
            this.numericUpDownStrength.Size = new System.Drawing.Size(99, 20);
            this.numericUpDownStrength.TabIndex = 36;
            this.numericUpDownStrength.Value = new decimal(new int[] {
            30,
            0,
            0,
            0});
            // 
            // numericUpDownOutter
            // 
            this.numericUpDownOutter.Location = new System.Drawing.Point(87, 144);
            this.numericUpDownOutter.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericUpDownOutter.Name = "numericUpDownOutter";
            this.numericUpDownOutter.Size = new System.Drawing.Size(99, 20);
            this.numericUpDownOutter.TabIndex = 36;
            this.numericUpDownOutter.Value = new decimal(new int[] {
            20,
            0,
            0,
            0});
            // 
            // numericUpDownInner
            // 
            this.numericUpDownInner.Location = new System.Drawing.Point(87, 118);
            this.numericUpDownInner.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericUpDownInner.Name = "numericUpDownInner";
            this.numericUpDownInner.Size = new System.Drawing.Size(99, 20);
            this.numericUpDownInner.TabIndex = 36;
            this.numericUpDownInner.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Location = new System.Drawing.Point(14, 173);
            label7.Name = "label7";
            label7.Size = new System.Drawing.Size(21, 14);
            label7.TabIndex = 33;
            label7.Text = "Str";
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Location = new System.Drawing.Point(14, 146);
            label6.Name = "label6";
            label6.Size = new System.Drawing.Size(37, 14);
            label6.TabIndex = 32;
            label6.Text = "Outter";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new System.Drawing.Point(14, 124);
            label5.Name = "label5";
            label5.Size = new System.Drawing.Size(31, 14);
            label5.TabIndex = 30;
            label5.Text = "Inner";
            // 
            // textBoxTrnFreq
            // 
            this.textBoxTrnFreq.Location = new System.Drawing.Point(87, 91);
            this.textBoxTrnFreq.Name = "textBoxTrnFreq";
            this.textBoxTrnFreq.Size = new System.Drawing.Size(99, 20);
            this.textBoxTrnFreq.TabIndex = 29;
            this.textBoxTrnFreq.Text = "8";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new System.Drawing.Point(13, 97);
            label4.Name = "label4";
            label4.Size = new System.Drawing.Size(56, 14);
            label4.TabIndex = 28;
            label4.Text = "NoiseFreq";
            // 
            // comboBoxTrnLerp
            // 
            this.comboBoxTrnLerp.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTrnLerp.FormattingEnabled = true;
            this.comboBoxTrnLerp.Location = new System.Drawing.Point(88, 65);
            this.comboBoxTrnLerp.Name = "comboBoxTrnLerp";
            this.comboBoxTrnLerp.Size = new System.Drawing.Size(99, 22);
            this.comboBoxTrnLerp.TabIndex = 27;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new System.Drawing.Point(13, 73);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(29, 14);
            label3.TabIndex = 26;
            label3.Text = "Lerp";
            // 
            // comboBoxTrnShape
            // 
            this.comboBoxTrnShape.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTrnShape.FormattingEnabled = true;
            this.comboBoxTrnShape.Location = new System.Drawing.Point(88, 39);
            this.comboBoxTrnShape.Name = "comboBoxTrnShape";
            this.comboBoxTrnShape.Size = new System.Drawing.Size(99, 22);
            this.comboBoxTrnShape.TabIndex = 25;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new System.Drawing.Point(13, 47);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(38, 14);
            label2.TabIndex = 24;
            label2.Text = "Shape";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(13, 22);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(54, 14);
            label1.TabIndex = 22;
            label1.Text = "Operation";
            // 
            // comboBoxTrnOp
            // 
            this.comboBoxTrnOp.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTrnOp.FormattingEnabled = true;
            this.comboBoxTrnOp.Location = new System.Drawing.Point(87, 14);
            this.comboBoxTrnOp.Name = "comboBoxTrnOp";
            this.comboBoxTrnOp.Size = new System.Drawing.Size(100, 22);
            this.comboBoxTrnOp.TabIndex = 23;
            // 
            // collapsibleGroupBox2
            // 
            this.collapsibleGroupBox2.Controls.Add(this.pictureBoxResource);
            this.collapsibleGroupBox2.Dock = System.Windows.Forms.DockStyle.Top;
            this.collapsibleGroupBox2.Location = new System.Drawing.Point(3, 369);
            this.collapsibleGroupBox2.Name = "collapsibleGroupBox2";
            this.collapsibleGroupBox2.Size = new System.Drawing.Size(231, 262);
            this.collapsibleGroupBox2.TabIndex = 1;
            this.collapsibleGroupBox2.TabStop = false;
            this.collapsibleGroupBox2.Text = "Preview";
            // 
            // pictureBoxResource
            // 
            this.pictureBoxResource.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxResource.Location = new System.Drawing.Point(3, 16);
            this.pictureBoxResource.Name = "pictureBoxResource";
            this.pictureBoxResource.Size = new System.Drawing.Size(225, 243);
            this.pictureBoxResource.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBoxResource.TabIndex = 0;
            this.pictureBoxResource.TabStop = false;
            // 
            // collapsibleGroupBox1
            // 
            this.collapsibleGroupBox1.Controls.Add(this.listViewResource);
            this.collapsibleGroupBox1.Dock = System.Windows.Forms.DockStyle.Top;
            this.collapsibleGroupBox1.Location = new System.Drawing.Point(3, 3);
            this.collapsibleGroupBox1.Name = "collapsibleGroupBox1";
            this.collapsibleGroupBox1.Size = new System.Drawing.Size(231, 366);
            this.collapsibleGroupBox1.TabIndex = 0;
            this.collapsibleGroupBox1.TabStop = false;
            this.collapsibleGroupBox1.Text = "List";
            // 
            // listViewResource
            // 
            this.listViewResource.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewResource.LargeImageList = this.imageListResource;
            this.listViewResource.Location = new System.Drawing.Point(3, 16);
            this.listViewResource.MultiSelect = false;
            this.listViewResource.Name = "listViewResource";
            this.listViewResource.Size = new System.Drawing.Size(225, 347);
            this.listViewResource.SmallImageList = this.imageListResource;
            this.listViewResource.TabIndex = 0;
            this.listViewResource.TileSize = new System.Drawing.Size(168, 49);
            this.listViewResource.UseCompatibleStateImageBehavior = false;
            this.listViewResource.View = System.Windows.Forms.View.Tile;
            // 
            // EditorMainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(185)))), ((int)(((byte)(185)))), ((int)(((byte)(185)))));
            this.ClientSize = new System.Drawing.Size(1029, 772);
            this.Controls.Add(this.cameraSight);
            this.Controls.Add(this.splitMainForm);
            this.Controls.Add(this.statusMainForm);
            this.Controls.Add(this.toolMainForm);
            this.Controls.Add(this.menuMainForm);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuMainForm;
            this.Name = "EditorMainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Nexus Editor";
            this.Load += new System.EventHandler(this.EditorMainForm_Load);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.EditorMainForm_FormClosed);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.EditorMainForm_FormClosing);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.EditorMainForm_KeyDown);
            this.menuMainForm.ResumeLayout(false);
            this.menuMainForm.PerformLayout();
            this.statusMainForm.ResumeLayout(false);
            this.statusMainForm.PerformLayout();
            this.splitMainForm.Panel1.ResumeLayout(false);
            this.splitMainForm.Panel2.ResumeLayout(false);
            this.splitMainForm.ResumeLayout(false);
            this.splitContainerViewH.Panel1.ResumeLayout(false);
            this.splitContainerViewH.Panel2.ResumeLayout(false);
            this.splitContainerViewH.ResumeLayout(false);
            this.splitContainerViewUp.ResumeLayout(false);
            this.splitContainerViewDown.ResumeLayout(false);
            this.tabControlRight.ResumeLayout(false);
            this.tabPageActor.ResumeLayout(false);
            this.tabPageTerrain.ResumeLayout(false);
            this.tabPageRes.ResumeLayout(false);
            this.tabFactory.ResumeLayout(false);
            this.toolMainForm.ResumeLayout(false);
            this.toolMainForm.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.cameraSight)).EndInit();
            this.contextMenuStrip1.ResumeLayout(false);
            this.MoveAlignMenu.ResumeLayout(false);
            this.RotateAlignMenu.ResumeLayout(false);
            this.ScaleAlignMenu.ResumeLayout(false);
            this.contextMenuStripViewport.ResumeLayout(false);
            this.groupActorProp.ResumeLayout(false);
            this.groupActorComp.ResumeLayout(false);
            this.groupTrnUtil.ResumeLayout(false);
            this.groupTrnUtil.PerformLayout();
            this.groupTrnLayers.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBrush.ResumeLayout(false);
            this.groupBrush.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownStrength)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownOutter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownInner)).EndInit();
            this.collapsibleGroupBox2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxResource)).EndInit();
            this.collapsibleGroupBox1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuMainForm;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusMainForm;
		private System.Windows.Forms.SplitContainer splitMainForm;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem menuAbout;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem levelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createTerrainToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainerViewH;
        private System.Windows.Forms.SplitContainer splitContainerViewUp;
        private System.Windows.Forms.SplitContainer splitContainerViewDown;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem resetLayoutToolStripMenuItem;
        private System.Windows.Forms.ImageList imageListCtrlMode;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem activeViewOnlyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fourViewsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem twoViewportsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveCurrentLevelToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem newLevelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showResourceManagerToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLableNote;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem actorListToolStripMenuItem;
        private System.Windows.Forms.ImageList imageListCommand;
        private System.Windows.Forms.ImageList imageListTab;
        private System.Windows.Forms.ToolStripMenuItem fitViewToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolMainForm;
        private System.Windows.Forms.ToolStripButton toolBtnUndo;
        private System.Windows.Forms.ToolStripButton toolBtnRedo;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolBtnPick;
        private System.Windows.Forms.ToolStripButton toolBtnCamera;
        private System.Windows.Forms.ToolStripButton toolBtnTerrainEd;
        private System.Windows.Forms.ToolStripButton toolBtnTranslate;
        private System.Windows.Forms.ToolStripButton toolBtnRotate;
        private System.Windows.Forms.ToolStripButton toolBtnScale;
        private System.Windows.Forms.ToolStripButton toolBtnPanView;
        private System.Windows.Forms.ToolStripButton toolBtnRotateView;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton toolBtnActorList;
        private System.Windows.Forms.ToolStripButton toolBtnMaxView;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
		private System.Windows.Forms.ToolStripButton toolBtnShowResMgr;
        private System.Windows.Forms.ToolStripSplitButton toolBtnCoord;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem screenToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem worldToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem localToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolLevelLayers;
		private System.Windows.Forms.ImageList imageListTrnTarget;
        private System.Windows.Forms.ToolStripComboBox toolSpaceList;
        private System.Windows.Forms.ToolStripStatusLabel statusSelectionLock;
        private System.Windows.Forms.ToolStripButton toolBtnShowLog;
        private System.Windows.Forms.ToolStripMenuItem showLogToolStripMenuItem;
		private System.Windows.Forms.ImageList imageListResource;
        private System.Windows.Forms.ToolStripMenuItem loadLevelToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showLayerManagerToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
		private System.Windows.Forms.ToolStripButton PIE;
        private System.Windows.Forms.ToolStripButton toolBtnNavEditor;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripMenuItem showToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showNavigationMapToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem navigationHeightMapToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem navigationWalkableMapToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem hideTerrainsToolStripMenuItem;
		private System.Windows.Forms.ToolStripStatusLabel CameraSpeed;
		private System.Windows.Forms.TrackBar cameraSight;
		private System.Windows.Forms.ToolStripMenuItem MTU;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelAutoSave;
		private System.Windows.Forms.Timer autoSaveTimer;
		private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
		private System.Windows.Forms.ToolStripMenuItem autoSaveTimeSetting5;
		private System.Windows.Forms.ToolStripMenuItem autoSaveTimeSetting10;
		private System.Windows.Forms.ToolStripMenuItem autoSaveTimeSetting15;
		private System.Windows.Forms.ToolStripMenuItem autoSaveTimeSetting20;
		private System.Windows.Forms.ToolStripMenuItem closeAutoSave;
		private System.Windows.Forms.ToolStripStatusLabel m_MoveAlignStatusLabel;
		private System.Windows.Forms.ToolStripStatusLabel m_RotateAlignStatusLabel;
		private System.Windows.Forms.ToolStripStatusLabel m_ScaleAlignStatusLabel;
		private System.Windows.Forms.ContextMenuStrip MoveAlignMenu;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem6;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem7;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem8;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem9;
		private System.Windows.Forms.ContextMenuStrip RotateAlignMenu;
		private System.Windows.Forms.ContextMenuStrip ScaleAlignMenu;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem20;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem21;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem22;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem23;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem24;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem25;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem26;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem10;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem11;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem12;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem13;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem14;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem27;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem15;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem16;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem17;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem18;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem19;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem28;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
		private System.Windows.Forms.ToolStripButton toolStripReplace;
		private System.Windows.Forms.ContextMenuStrip contextMenuStripViewport;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem3;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
		private System.Windows.Forms.ToolStripMenuItem addActorToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
		private System.Windows.Forms.ToolStripMenuItem playFromHereToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem selectToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem menuItemSelSameResource;
		private System.Windows.Forms.ToolStripMenuItem menuItemAllInLayer;
		private System.Windows.Forms.ToolStripMenuItem menuItemSelOthers;
		private System.Windows.Forms.ToolStripMenuItem menuItemUnselAll;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuExportLevel;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem5;
		private System.Windows.Forms.ToolStripMenuItem showAllToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem hideAllToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem29;
		private System.Windows.Forms.ToolStripMenuItem CopyToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem CutToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem PasteToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem30;
		private System.Windows.Forms.ToolStripMenuItem showSelOnlyToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem32;
		private System.Windows.Forms.TabControl tabControlRight;
		private System.Windows.Forms.TabPage tabPageActor;
		private NexusEditor.NControl.CollapsibleGroupBox groupActorProp;
		private System.Windows.Forms.PropertyGrid propertyGridActor;
		private NexusEditor.NControl.CollapsibleGroupBox groupActorComp;
		private System.Windows.Forms.TreeView treeViewActor;
		private System.Windows.Forms.TabPage tabPageTerrain;
		private NexusEditor.NControl.CollapsibleGroupBox groupTrnUtil;
		private System.Windows.Forms.Button btnTrnTools;
		private System.Windows.Forms.CheckBox checkBoxShowChunk;
		private System.Windows.Forms.Button buttonTrnNoise;
		private System.Windows.Forms.Button btnTrnMtl;
		private NexusEditor.NControl.CollapsibleGroupBox groupTrnLayers;
		private System.Windows.Forms.Button btnTrnAddLayer;
		private System.Windows.Forms.Button btnTrnLayerDown;
		private System.Windows.Forms.Button btnTrnRemoveLayer;
		private System.Windows.Forms.Button btnTrnLayerUp;
		private System.Windows.Forms.ListView listViewTrnTarget;
		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.ComboBox comboBoxTrnTarget;
        private NexusEditor.NControl.CollapsibleGroupBox groupBrush;
		private System.Windows.Forms.TextBox textBoxTrnFreq;
		private System.Windows.Forms.ComboBox comboBoxTrnLerp;
		private System.Windows.Forms.ComboBox comboBoxTrnShape;
		private System.Windows.Forms.ComboBox comboBoxTrnOp;
		private System.Windows.Forms.TabPage tabPageRes;
		private NexusEditor.NControl.CollapsibleGroupBox collapsibleGroupBox2;
		private System.Windows.Forms.PictureBox pictureBoxResource;
		private NexusEditor.NControl.CollapsibleGroupBox collapsibleGroupBox1;
		private System.Windows.Forms.ListView listViewResource;
		private System.Windows.Forms.TabPage tabFactory;
		private System.Windows.Forms.ListView listViewClass;
		private System.Windows.Forms.ColumnHeader className;
		private System.Windows.Forms.ColumnHeader nativeClassName;
		private System.Windows.Forms.ToolStripMenuItem globalSettingsToolStripMenuItem;
        private System.Windows.Forms.NumericUpDown numericUpDownStrength;
        private System.Windows.Forms.NumericUpDown numericUpDownOutter;
        private System.Windows.Forms.NumericUpDown numericUpDownInner;

    }
}

