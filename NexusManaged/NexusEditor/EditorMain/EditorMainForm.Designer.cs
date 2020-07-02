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
            System.Windows.Forms.Label label7;
            System.Windows.Forms.Label label6;
            System.Windows.Forms.Label label5;
            System.Windows.Forms.Label label4;
            System.Windows.Forms.Label label3;
            System.Windows.Forms.Label label2;
            System.Windows.Forms.Label label1;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditorMainForm));
            this.menuMainForm = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveCurrentLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.resetLayoutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showResourceManagerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showLogToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.activeViewOnlyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fourViewsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.twoViewportsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fitViewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.levelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createTerrainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.actorListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuAbout = new System.Windows.Forms.ToolStripMenuItem();
            this.statusMainForm = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLableNote = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusSelectionLock = new System.Windows.Forms.ToolStripStatusLabel();
            this.splitMainForm = new System.Windows.Forms.SplitContainer();
            this.splitContainerViewH = new System.Windows.Forms.SplitContainer();
            this.splitContainerViewUp = new System.Windows.Forms.SplitContainer();
            this.splitContainerViewDown = new System.Windows.Forms.SplitContainer();
            this.tabControlRight = new System.Windows.Forms.TabControl();
            this.tabPageActor = new System.Windows.Forms.TabPage();
            this.groupActorProp = new NControl.CollapsibleGroupBox();
            this.propertyGridActor = new System.Windows.Forms.PropertyGrid();
            this.groupActorComp = new NControl.CollapsibleGroupBox();
            this.treeViewActor = new System.Windows.Forms.TreeView();
            this.tabPageTerrain = new System.Windows.Forms.TabPage();
            this.groupTrnUtil = new NControl.CollapsibleGroupBox();
            this.btnTrnSplatting = new System.Windows.Forms.Button();
            this.btnTrnBasicMtl = new System.Windows.Forms.Button();
            this.btnTrnTools = new System.Windows.Forms.Button();
            this.buttonTrnNoise = new System.Windows.Forms.Button();
            this.groupTrnLayers = new NControl.CollapsibleGroupBox();
            this.btnTrnAddLayer = new System.Windows.Forms.Button();
            this.btnTrnLayerDown = new System.Windows.Forms.Button();
            this.btnTrnRemoveLayer = new System.Windows.Forms.Button();
            this.btnTrnLayerUp = new System.Windows.Forms.Button();
            this.listViewTrnTarget = new System.Windows.Forms.ListView();
            this.imageListTrnTarget = new System.Windows.Forms.ImageList(this.components);
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.comboBoxTrnTarget = new System.Windows.Forms.ComboBox();
            this.groupBrush = new NControl.CollapsibleGroupBox();
            this.trackBarTrnStrength = new System.Windows.Forms.TrackBar();
            this.trackBarTrnOutter = new System.Windows.Forms.TrackBar();
            this.trackBarTrnInner = new System.Windows.Forms.TrackBar();
            this.textBoxTrnFreq = new System.Windows.Forms.TextBox();
            this.comboBoxTrnLerp = new System.Windows.Forms.ComboBox();
            this.comboBoxTrnShape = new System.Windows.Forms.ComboBox();
            this.comboBoxTrnOp = new System.Windows.Forms.ComboBox();
            this.tabPageRes = new System.Windows.Forms.TabPage();
            this.collapsibleGroupBox2 = new NControl.CollapsibleGroupBox();
            this.pictureBoxResource = new System.Windows.Forms.PictureBox();
            this.collapsibleGroupBox1 = new NControl.CollapsibleGroupBox();
            this.listViewResource = new System.Windows.Forms.ListView();
            this.imageListResource = new System.Windows.Forms.ImageList(this.components);
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
            this.toolBtnTranslate = new System.Windows.Forms.ToolStripButton();
            this.toolBtnRotate = new System.Windows.Forms.ToolStripButton();
            this.toolBtnScale = new System.Windows.Forms.ToolStripButton();
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
            this.groupActorProp.SuspendLayout();
            this.groupActorComp.SuspendLayout();
            this.tabPageTerrain.SuspendLayout();
            this.groupTrnUtil.SuspendLayout();
            this.groupTrnLayers.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBrush.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTrnStrength)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTrnOutter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTrnInner)).BeginInit();
            this.tabPageRes.SuspendLayout();
            this.collapsibleGroupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxResource)).BeginInit();
            this.collapsibleGroupBox1.SuspendLayout();
            this.toolMainForm.SuspendLayout();
            this.SuspendLayout();
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
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new System.Drawing.Point(13, 97);
            label4.Name = "label4";
            label4.Size = new System.Drawing.Size(56, 14);
            label4.TabIndex = 28;
            label4.Text = "NoiseFreq";
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
            // menuMainForm
            // 
            this.menuMainForm.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.levelToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuMainForm.Location = new System.Drawing.Point(0, 0);
            this.menuMainForm.Name = "menuMainForm";
            this.menuMainForm.Size = new System.Drawing.Size(1016, 24);
            this.menuMainForm.TabIndex = 0;
            this.menuMainForm.Text = "menuMainForm";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newLevelToolStripMenuItem,
            this.saveCurrentLevelToolStripMenuItem,
            this.toolStripMenuItem2,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newLevelToolStripMenuItem
            // 
            this.newLevelToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.NewLevel;
            this.newLevelToolStripMenuItem.Name = "newLevelToolStripMenuItem";
            this.newLevelToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.newLevelToolStripMenuItem.Text = "New Level";
            this.newLevelToolStripMenuItem.Click += new System.EventHandler(this.newLevelToolStripMenuItem_Click);
            // 
            // saveCurrentLevelToolStripMenuItem
            // 
            this.saveCurrentLevelToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.Save;
            this.saveCurrentLevelToolStripMenuItem.Name = "saveCurrentLevelToolStripMenuItem";
            this.saveCurrentLevelToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.saveCurrentLevelToolStripMenuItem.Text = "Save Current Level";
            this.saveCurrentLevelToolStripMenuItem.Click += new System.EventHandler(this.saveCurrentLevelToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(163, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.resetLayoutToolStripMenuItem,
            this.showResourceManagerToolStripMenuItem,
            this.showLogToolStripMenuItem,
            this.toolStripMenuItem1,
            this.activeViewOnlyToolStripMenuItem,
            this.fourViewsToolStripMenuItem,
            this.twoViewportsToolStripMenuItem,
            this.fitViewToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.viewToolStripMenuItem.Text = "View";
            // 
            // resetLayoutToolStripMenuItem
            // 
            this.resetLayoutToolStripMenuItem.Name = "resetLayoutToolStripMenuItem";
            this.resetLayoutToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.resetLayoutToolStripMenuItem.Text = "Reset Layout";
            this.resetLayoutToolStripMenuItem.Click += new System.EventHandler(this.resetLayoutToolStripMenuItem_Click);
            // 
            // showResourceManagerToolStripMenuItem
            // 
            this.showResourceManagerToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.MediaFolder;
            this.showResourceManagerToolStripMenuItem.Name = "showResourceManagerToolStripMenuItem";
            this.showResourceManagerToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.showResourceManagerToolStripMenuItem.Text = "Show Resource Manager";
            this.showResourceManagerToolStripMenuItem.Click += new System.EventHandler(this.showResourceManagerToolStripMenuItem_Click);
            // 
            // showLogToolStripMenuItem
            // 
            this.showLogToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.Console;
            this.showLogToolStripMenuItem.Name = "showLogToolStripMenuItem";
            this.showLogToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.showLogToolStripMenuItem.Text = "Show Log";
            this.showLogToolStripMenuItem.Click += new System.EventHandler(this.toolBtnShowLog_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(190, 6);
            // 
            // activeViewOnlyToolStripMenuItem
            // 
            this.activeViewOnlyToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.MaxView;
            this.activeViewOnlyToolStripMenuItem.Name = "activeViewOnlyToolStripMenuItem";
            this.activeViewOnlyToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.activeViewOnlyToolStripMenuItem.Text = "Active View Only";
            this.activeViewOnlyToolStripMenuItem.Click += new System.EventHandler(this.activeViewOnlyToolStripMenuItem_Click);
            // 
            // fourViewsToolStripMenuItem
            // 
            this.fourViewsToolStripMenuItem.Name = "fourViewsToolStripMenuItem";
            this.fourViewsToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.fourViewsToolStripMenuItem.Text = "Four Viewports";
            this.fourViewsToolStripMenuItem.Click += new System.EventHandler(this.fourViewsToolStripMenuItem_Click);
            // 
            // twoViewportsToolStripMenuItem
            // 
            this.twoViewportsToolStripMenuItem.Name = "twoViewportsToolStripMenuItem";
            this.twoViewportsToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.twoViewportsToolStripMenuItem.Text = "Two Viewports";
            this.twoViewportsToolStripMenuItem.Click += new System.EventHandler(this.twoViewportsToolStripMenuItem_Click);
            // 
            // fitViewToolStripMenuItem
            // 
            this.fitViewToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.FitView;
            this.fitViewToolStripMenuItem.Name = "fitViewToolStripMenuItem";
            this.fitViewToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.fitViewToolStripMenuItem.Text = "Fit View";
            this.fitViewToolStripMenuItem.Click += new System.EventHandler(this.fitViewToolStripMenuItem_Click);
            // 
            // levelToolStripMenuItem
            // 
            this.levelToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createTerrainToolStripMenuItem,
            this.toolStripSeparator2,
            this.actorListToolStripMenuItem});
            this.levelToolStripMenuItem.Name = "levelToolStripMenuItem";
            this.levelToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.levelToolStripMenuItem.Text = "Level";
            // 
            // createTerrainToolStripMenuItem
            // 
            this.createTerrainToolStripMenuItem.Name = "createTerrainToolStripMenuItem";
            this.createTerrainToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.createTerrainToolStripMenuItem.Text = "New Terrain ...";
            this.createTerrainToolStripMenuItem.Click += new System.EventHandler(this.newTerrainToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(144, 6);
            // 
            // actorListToolStripMenuItem
            // 
            this.actorListToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.ArrowList;
            this.actorListToolStripMenuItem.Name = "actorListToolStripMenuItem";
            this.actorListToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.actorListToolStripMenuItem.Text = "Actor List ...";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuAbout});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // menuAbout
            // 
            this.menuAbout.Name = "menuAbout";
            this.menuAbout.Size = new System.Drawing.Size(103, 22);
            this.menuAbout.Text = "&About";
            this.menuAbout.Click += new System.EventHandler(this.menuAbout_Click);
            // 
            // statusMainForm
            // 
            this.statusMainForm.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLableNote,
            this.statusSelectionLock});
            this.statusMainForm.Location = new System.Drawing.Point(0, 719);
            this.statusMainForm.Name = "statusMainForm";
            this.statusMainForm.Size = new System.Drawing.Size(1016, 22);
            this.statusMainForm.TabIndex = 2;
            this.statusMainForm.Text = "statusMainForm";
            // 
            // toolStripStatusLableNote
            // 
            this.toolStripStatusLableNote.AutoSize = false;
            this.toolStripStatusLableNote.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
                        | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
            this.toolStripStatusLableNote.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenInner;
            this.toolStripStatusLableNote.Name = "toolStripStatusLableNote";
            this.toolStripStatusLableNote.Size = new System.Drawing.Size(128, 17);
            this.toolStripStatusLableNote.Text = "Ready";
            // 
            // statusSelectionLock
            // 
            this.statusSelectionLock.BorderStyle = System.Windows.Forms.Border3DStyle.Bump;
            this.statusSelectionLock.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.statusSelectionLock.Image = global::NexusEditor.Properties.Resources.unlock;
            this.statusSelectionLock.ImageTransparentColor = System.Drawing.Color.White;
            this.statusSelectionLock.Name = "statusSelectionLock";
            this.statusSelectionLock.Size = new System.Drawing.Size(16, 17);
            // 
            // splitMainForm
            // 
            this.splitMainForm.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitMainForm.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitMainForm.IsSplitterFixed = true;
            this.splitMainForm.Location = new System.Drawing.Point(0, 55);
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
            this.splitMainForm.Size = new System.Drawing.Size(1016, 664);
            this.splitMainForm.SplitterDistance = 792;
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
            this.splitContainerViewH.Size = new System.Drawing.Size(792, 664);
            this.splitContainerViewH.SplitterDistance = 303;
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
            this.splitContainerViewUp.Size = new System.Drawing.Size(792, 303);
            this.splitContainerViewUp.SplitterDistance = 324;
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
            this.splitContainerViewDown.Size = new System.Drawing.Size(792, 359);
            this.splitContainerViewDown.SplitterDistance = 324;
            this.splitContainerViewDown.SplitterWidth = 2;
            this.splitContainerViewDown.TabIndex = 0;
            // 
            // tabControlRight
            // 
            this.tabControlRight.Controls.Add(this.tabPageActor);
            this.tabControlRight.Controls.Add(this.tabPageTerrain);
            this.tabControlRight.Controls.Add(this.tabPageRes);
            this.tabControlRight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlRight.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tabControlRight.ImageList = this.imageListTab;
            this.tabControlRight.Location = new System.Drawing.Point(0, 0);
            this.tabControlRight.Multiline = true;
            this.tabControlRight.Name = "tabControlRight";
            this.tabControlRight.SelectedIndex = 0;
            this.tabControlRight.ShowToolTips = true;
            this.tabControlRight.Size = new System.Drawing.Size(222, 664);
            this.tabControlRight.SizeMode = System.Windows.Forms.TabSizeMode.FillToRight;
            this.tabControlRight.TabIndex = 0;
            // 
            // tabPageActor
            // 
            this.tabPageActor.Controls.Add(this.groupActorProp);
            this.tabPageActor.Controls.Add(this.groupActorComp);
            this.tabPageActor.ImageKey = "user_suit.gif";
            this.tabPageActor.Location = new System.Drawing.Point(4, 23);
            this.tabPageActor.Name = "tabPageActor";
            this.tabPageActor.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageActor.Size = new System.Drawing.Size(214, 637);
            this.tabPageActor.TabIndex = 3;
            this.tabPageActor.Text = "Actor";
            this.tabPageActor.UseVisualStyleBackColor = true;
            // 
            // groupActorProp
            // 
            this.groupActorProp.Controls.Add(this.propertyGridActor);
            this.groupActorProp.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupActorProp.Location = new System.Drawing.Point(3, 199);
            this.groupActorProp.Name = "groupActorProp";
            this.groupActorProp.Size = new System.Drawing.Size(208, 381);
            this.groupActorProp.TabIndex = 6;
            this.groupActorProp.TabStop = false;
            this.groupActorProp.Text = "Properties";
            // 
            // propertyGridActor
            // 
            this.propertyGridActor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridActor.Location = new System.Drawing.Point(3, 16);
            this.propertyGridActor.Name = "propertyGridActor";
            this.propertyGridActor.Size = new System.Drawing.Size(202, 362);
            this.propertyGridActor.TabIndex = 4;
            // 
            // groupActorComp
            // 
            this.groupActorComp.Controls.Add(this.treeViewActor);
            this.groupActorComp.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupActorComp.Location = new System.Drawing.Point(3, 3);
            this.groupActorComp.Name = "groupActorComp";
            this.groupActorComp.Size = new System.Drawing.Size(208, 196);
            this.groupActorComp.TabIndex = 5;
            this.groupActorComp.TabStop = false;
            this.groupActorComp.Text = "Components";
            // 
            // treeViewActor
            // 
            this.treeViewActor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewActor.Location = new System.Drawing.Point(3, 16);
            this.treeViewActor.Name = "treeViewActor";
            this.treeViewActor.Size = new System.Drawing.Size(202, 177);
            this.treeViewActor.TabIndex = 5;
            // 
            // tabPageTerrain
            // 
            this.tabPageTerrain.Controls.Add(this.groupTrnUtil);
            this.tabPageTerrain.Controls.Add(this.groupTrnLayers);
            this.tabPageTerrain.Controls.Add(this.groupBrush);
            this.tabPageTerrain.ImageKey = "world.gif";
            this.tabPageTerrain.Location = new System.Drawing.Point(4, 23);
            this.tabPageTerrain.Name = "tabPageTerrain";
            this.tabPageTerrain.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTerrain.Size = new System.Drawing.Size(214, 637);
            this.tabPageTerrain.TabIndex = 2;
            this.tabPageTerrain.Text = "Terrain";
            this.tabPageTerrain.UseVisualStyleBackColor = true;
            // 
            // groupTrnUtil
            // 
            this.groupTrnUtil.Controls.Add(this.btnTrnSplatting);
            this.groupTrnUtil.Controls.Add(this.btnTrnBasicMtl);
            this.groupTrnUtil.Controls.Add(this.btnTrnTools);
            this.groupTrnUtil.Controls.Add(this.buttonTrnNoise);
            this.groupTrnUtil.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupTrnUtil.Location = new System.Drawing.Point(3, 535);
            this.groupTrnUtil.Name = "groupTrnUtil";
            this.groupTrnUtil.Size = new System.Drawing.Size(208, 100);
            this.groupTrnUtil.TabIndex = 5;
            this.groupTrnUtil.TabStop = false;
            this.groupTrnUtil.Text = "Util";
            // 
            // btnTrnSplatting
            // 
            this.btnTrnSplatting.Location = new System.Drawing.Point(104, 53);
            this.btnTrnSplatting.Name = "btnTrnSplatting";
            this.btnTrnSplatting.Size = new System.Drawing.Size(92, 20);
            this.btnTrnSplatting.TabIndex = 8;
            this.btnTrnSplatting.Text = "Splatting";
            this.btnTrnSplatting.UseVisualStyleBackColor = true;
            // 
            // btnTrnBasicMtl
            // 
            this.btnTrnBasicMtl.Location = new System.Drawing.Point(6, 53);
            this.btnTrnBasicMtl.Name = "btnTrnBasicMtl";
            this.btnTrnBasicMtl.Size = new System.Drawing.Size(92, 20);
            this.btnTrnBasicMtl.TabIndex = 7;
            this.btnTrnBasicMtl.Text = "Basic Material";
            this.btnTrnBasicMtl.UseVisualStyleBackColor = true;
            // 
            // btnTrnTools
            // 
            this.btnTrnTools.Location = new System.Drawing.Point(104, 27);
            this.btnTrnTools.Name = "btnTrnTools";
            this.btnTrnTools.Size = new System.Drawing.Size(92, 20);
            this.btnTrnTools.TabIndex = 6;
            this.btnTrnTools.Text = "Tools";
            this.btnTrnTools.UseVisualStyleBackColor = true;
            // 
            // buttonTrnNoise
            // 
            this.buttonTrnNoise.Location = new System.Drawing.Point(6, 27);
            this.buttonTrnNoise.Name = "buttonTrnNoise";
            this.buttonTrnNoise.Size = new System.Drawing.Size(92, 20);
            this.buttonTrnNoise.TabIndex = 5;
            this.buttonTrnNoise.Text = "Noise";
            this.buttonTrnNoise.UseVisualStyleBackColor = true;
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
            this.groupTrnLayers.Size = new System.Drawing.Size(208, 325);
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
            // imageListTrnTarget
            // 
            this.imageListTrnTarget.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListTrnTarget.ImageStream")));
            this.imageListTrnTarget.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListTrnTarget.Images.SetKeyName(0, "Heightmap");
            this.imageListTrnTarget.Images.SetKeyName(1, "NoTexture");
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
            this.groupBrush.Controls.Add(this.trackBarTrnStrength);
            this.groupBrush.Controls.Add(this.trackBarTrnOutter);
            this.groupBrush.Controls.Add(label7);
            this.groupBrush.Controls.Add(label6);
            this.groupBrush.Controls.Add(this.trackBarTrnInner);
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
            this.groupBrush.Size = new System.Drawing.Size(208, 207);
            this.groupBrush.TabIndex = 3;
            this.groupBrush.TabStop = false;
            this.groupBrush.Text = "Brush";
            // 
            // trackBarTrnStrength
            // 
            this.trackBarTrnStrength.AutoSize = false;
            this.trackBarTrnStrength.Location = new System.Drawing.Point(42, 169);
            this.trackBarTrnStrength.Maximum = 100;
            this.trackBarTrnStrength.Name = "trackBarTrnStrength";
            this.trackBarTrnStrength.Size = new System.Drawing.Size(152, 20);
            this.trackBarTrnStrength.TabIndex = 35;
            this.trackBarTrnStrength.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarTrnStrength.Value = 1;
            // 
            // trackBarTrnOutter
            // 
            this.trackBarTrnOutter.AutoSize = false;
            this.trackBarTrnOutter.Location = new System.Drawing.Point(42, 143);
            this.trackBarTrnOutter.Maximum = 100;
            this.trackBarTrnOutter.Name = "trackBarTrnOutter";
            this.trackBarTrnOutter.Size = new System.Drawing.Size(152, 20);
            this.trackBarTrnOutter.TabIndex = 34;
            this.trackBarTrnOutter.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarTrnOutter.Value = 8;
            // 
            // trackBarTrnInner
            // 
            this.trackBarTrnInner.AutoSize = false;
            this.trackBarTrnInner.Location = new System.Drawing.Point(42, 117);
            this.trackBarTrnInner.Maximum = 100;
            this.trackBarTrnInner.Name = "trackBarTrnInner";
            this.trackBarTrnInner.Size = new System.Drawing.Size(152, 20);
            this.trackBarTrnInner.TabIndex = 31;
            this.trackBarTrnInner.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarTrnInner.Value = 4;
            // 
            // textBoxTrnFreq
            // 
            this.textBoxTrnFreq.Location = new System.Drawing.Point(87, 91);
            this.textBoxTrnFreq.Name = "textBoxTrnFreq";
            this.textBoxTrnFreq.Size = new System.Drawing.Size(99, 20);
            this.textBoxTrnFreq.TabIndex = 29;
            this.textBoxTrnFreq.Text = "8";
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
            // comboBoxTrnShape
            // 
            this.comboBoxTrnShape.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTrnShape.FormattingEnabled = true;
            this.comboBoxTrnShape.Location = new System.Drawing.Point(88, 39);
            this.comboBoxTrnShape.Name = "comboBoxTrnShape";
            this.comboBoxTrnShape.Size = new System.Drawing.Size(99, 22);
            this.comboBoxTrnShape.TabIndex = 25;
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
            // tabPageRes
            // 
            this.tabPageRes.Controls.Add(this.collapsibleGroupBox2);
            this.tabPageRes.Controls.Add(this.collapsibleGroupBox1);
            this.tabPageRes.ImageKey = "photo.gif";
            this.tabPageRes.Location = new System.Drawing.Point(4, 23);
            this.tabPageRes.Name = "tabPageRes";
            this.tabPageRes.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageRes.Size = new System.Drawing.Size(214, 637);
            this.tabPageRes.TabIndex = 4;
            this.tabPageRes.Text = "Resource";
            this.tabPageRes.UseVisualStyleBackColor = true;
            // 
            // collapsibleGroupBox2
            // 
            this.collapsibleGroupBox2.Controls.Add(this.pictureBoxResource);
            this.collapsibleGroupBox2.Dock = System.Windows.Forms.DockStyle.Top;
            this.collapsibleGroupBox2.Location = new System.Drawing.Point(3, 369);
            this.collapsibleGroupBox2.Name = "collapsibleGroupBox2";
            this.collapsibleGroupBox2.Size = new System.Drawing.Size(208, 262);
            this.collapsibleGroupBox2.TabIndex = 1;
            this.collapsibleGroupBox2.TabStop = false;
            this.collapsibleGroupBox2.Text = "Preview";
            // 
            // pictureBoxResource
            // 
            this.pictureBoxResource.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxResource.Location = new System.Drawing.Point(3, 16);
            this.pictureBoxResource.Name = "pictureBoxResource";
            this.pictureBoxResource.Size = new System.Drawing.Size(202, 243);
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
            this.collapsibleGroupBox1.Size = new System.Drawing.Size(208, 366);
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
            this.listViewResource.Size = new System.Drawing.Size(202, 347);
            this.listViewResource.SmallImageList = this.imageListResource;
            this.listViewResource.TabIndex = 0;
            this.listViewResource.TileSize = new System.Drawing.Size(168, 49);
            this.listViewResource.UseCompatibleStateImageBehavior = false;
            this.listViewResource.View = System.Windows.Forms.View.Tile;
            // 
            // imageListResource
            // 
            this.imageListResource.ColorDepth = System.Windows.Forms.ColorDepth.Depth24Bit;
            this.imageListResource.ImageSize = new System.Drawing.Size(48, 48);
            this.imageListResource.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // imageListTab
            // 
            this.imageListTab.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListTab.ImageStream")));
            this.imageListTab.TransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(185)))), ((int)(((byte)(185)))), ((int)(((byte)(185)))));
            this.imageListTab.Images.SetKeyName(0, "world.gif");
            this.imageListTab.Images.SetKeyName(1, "user_suit.gif");
            this.imageListTab.Images.SetKeyName(2, "photo.gif");
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
            this.toolBtnTranslate,
            this.toolBtnRotate,
            this.toolBtnScale,
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
            this.toolBtnShowLog});
            this.toolMainForm.Location = new System.Drawing.Point(0, 24);
            this.toolMainForm.Name = "toolMainForm";
            this.toolMainForm.Size = new System.Drawing.Size(1016, 31);
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
            // 
            // toolBtnRedo
            // 
            this.toolBtnRedo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBtnRedo.Image = global::NexusEditor.Properties.Resources.Redo;
            this.toolBtnRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnRedo.Name = "toolBtnRedo";
            this.toolBtnRedo.Size = new System.Drawing.Size(28, 28);
            this.toolBtnRedo.Text = "Redo";
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
            // toolSpaceList
            // 
            this.toolSpaceList.AutoSize = false;
            this.toolSpaceList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.toolSpaceList.Items.AddRange(new object[] {
            "World",
            "Local"});
            this.toolSpaceList.Name = "toolSpaceList";
            this.toolSpaceList.Size = new System.Drawing.Size(80, 21);
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
            this.viewToolStripMenuItem1.Size = new System.Drawing.Size(107, 22);
            this.viewToolStripMenuItem1.Text = "View";
            // 
            // screenToolStripMenuItem
            // 
            this.screenToolStripMenuItem.Name = "screenToolStripMenuItem";
            this.screenToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.screenToolStripMenuItem.Text = "Screen";
            // 
            // worldToolStripMenuItem
            // 
            this.worldToolStripMenuItem.Name = "worldToolStripMenuItem";
            this.worldToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.worldToolStripMenuItem.Text = "World";
            // 
            // localToolStripMenuItem
            // 
            this.localToolStripMenuItem.Name = "localToolStripMenuItem";
            this.localToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
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
            this.toolBtnShowLog.Text = "toolStripButton1";
            this.toolBtnShowLog.Click += new System.EventHandler(this.toolBtnShowLog_Click);
            // 
            // EditorMainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(185)))), ((int)(((byte)(185)))), ((int)(((byte)(185)))));
            this.ClientSize = new System.Drawing.Size(1016, 741);
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
            this.groupActorProp.ResumeLayout(false);
            this.groupActorComp.ResumeLayout(false);
            this.tabPageTerrain.ResumeLayout(false);
            this.groupTrnUtil.ResumeLayout(false);
            this.groupTrnLayers.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBrush.ResumeLayout(false);
            this.groupBrush.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTrnStrength)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTrnOutter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTrnInner)).EndInit();
            this.tabPageRes.ResumeLayout(false);
            this.collapsibleGroupBox2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxResource)).EndInit();
            this.collapsibleGroupBox1.ResumeLayout(false);
            this.toolMainForm.ResumeLayout(false);
            this.toolMainForm.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuMainForm;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusMainForm;
        private System.Windows.Forms.SplitContainer splitMainForm;
        private System.Windows.Forms.TabControl tabControlRight;
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
        private System.Windows.Forms.TabPage tabPageTerrain;
        private System.Windows.Forms.TabPage tabPageActor;
        private System.Windows.Forms.TabPage tabPageRes;
        private NControl.CollapsibleGroupBox collapsibleGroupBox1;
        private NControl.CollapsibleGroupBox collapsibleGroupBox2;
        private NControl.CollapsibleGroupBox groupActorProp;
        private System.Windows.Forms.PropertyGrid propertyGridActor;
        private NControl.CollapsibleGroupBox groupActorComp;
        private System.Windows.Forms.TreeView treeViewActor;
        private NControl.CollapsibleGroupBox groupBrush;
        private System.Windows.Forms.ComboBox comboBoxTrnLerp;
        private System.Windows.Forms.ComboBox comboBoxTrnShape;
        private System.Windows.Forms.ComboBox comboBoxTrnOp;
        private NControl.CollapsibleGroupBox groupTrnLayers;
        private System.Windows.Forms.ListView listViewTrnTarget;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.ComboBox comboBoxTrnTarget;
        private System.Windows.Forms.TrackBar trackBarTrnStrength;
        private System.Windows.Forms.TrackBar trackBarTrnOutter;
        private System.Windows.Forms.TrackBar trackBarTrnInner;
        private System.Windows.Forms.TextBox textBoxTrnFreq;
        private System.Windows.Forms.Button btnTrnAddLayer;
        private System.Windows.Forms.Button btnTrnLayerDown;
        private System.Windows.Forms.Button btnTrnRemoveLayer;
        private System.Windows.Forms.Button btnTrnLayerUp;
        private NControl.CollapsibleGroupBox groupTrnUtil;
        private System.Windows.Forms.Button btnTrnSplatting;
        private System.Windows.Forms.Button btnTrnBasicMtl;
        private System.Windows.Forms.Button btnTrnTools;
        private System.Windows.Forms.Button buttonTrnNoise;
        private System.Windows.Forms.ToolStripSplitButton toolBtnCoord;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem screenToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem worldToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem localToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolLevelLayers;
        private System.Windows.Forms.ImageList imageListTrnTarget;
        private System.Windows.Forms.ListView listViewResource;
        private System.Windows.Forms.ToolStripComboBox toolSpaceList;
        private System.Windows.Forms.ToolStripStatusLabel statusSelectionLock;
        private System.Windows.Forms.ToolStripButton toolBtnShowLog;
        private System.Windows.Forms.ToolStripMenuItem showLogToolStripMenuItem;
        private System.Windows.Forms.ImageList imageListResource;
        private System.Windows.Forms.PictureBox pictureBoxResource;

    }
}

