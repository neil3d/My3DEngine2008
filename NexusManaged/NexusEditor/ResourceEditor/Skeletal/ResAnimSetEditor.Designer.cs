namespace NexusEditor.ResourceEditor
{
    partial class ResAnimSetEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ResAnimSetEditor));
            System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem("test1");
            System.Windows.Forms.ListViewItem listViewItem4 = new System.Windows.Forms.ListViewItem("test2");
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("root");
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importMeshLODToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importPSAToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.loadSkeletalMeshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveSkeletalMeshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.loadAnimSetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAnimMeshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.toolStripButtonRest = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSaveMesh = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSaveAnim = new System.Windows.Forms.ToolStripButton();
            this.toolStripImportPSK = new System.Windows.Forms.ToolStripButton();
            this.toolStripImportPSA = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolBtnLit = new System.Windows.Forms.ToolStripButton();
            this.toolBtnUnlit = new System.Windows.Forms.ToolStripButton();
            this.toolBtnWireframe = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolBtnZoomExtents = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolBtnXML = new System.Windows.Forms.ToolStripButton();
            this.statusStripMain = new System.Windows.Forms.StatusStrip();
            this.statusSaveLoc = new System.Windows.Forms.ToolStripStatusLabel();
            this.imageListTab = new System.Windows.Forms.ImageList(this.components);
            this.imageListBtn = new System.Windows.Forms.ImageList(this.components);
            this.splitContainerMain = new System.Windows.Forms.SplitContainer();
            this.splitContainerLeft = new System.Windows.Forms.SplitContainer();
            this.splitContainerController = new System.Windows.Forms.SplitContainer();
            this.checkBoxLoop = new System.Windows.Forms.CheckBox();
            this.btnPlay = new System.Windows.Forms.Button();
            this.trackBarTime = new System.Windows.Forms.TrackBar();
            this.tabControlRight = new System.Windows.Forms.TabControl();
            this.tabPageMesh = new System.Windows.Forms.TabPage();
            this.propertyGridMesh = new System.Windows.Forms.PropertyGrid();
            this.tabPageMtl = new System.Windows.Forms.TabPage();
            this.propertyGridMtl = new System.Windows.Forms.PropertyGrid();
            this.comboBoxSection = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBoxLOD = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPageAnim = new System.Windows.Forms.TabPage();
            this.groupSeqInfo = new NexusEditor.NControl.CollapsibleGroupBox();
            this.propertyGridSeq = new System.Windows.Forms.PropertyGrid();
            this.groupSeqList = new NexusEditor.NControl.CollapsibleGroupBox();
            this.listBoxAnim = new System.Windows.Forms.ListView();
            this.btnTrnRemoveSeq = new System.Windows.Forms.Button();
            this.tabPageSkeleton = new System.Windows.Forms.TabPage();
            this.treeViewSkeletal = new System.Windows.Forms.TreeView();
            this.menuStripMain.SuspendLayout();
            this.toolStripMain.SuspendLayout();
            this.statusStripMain.SuspendLayout();
            this.splitContainerMain.Panel1.SuspendLayout();
            this.splitContainerMain.Panel2.SuspendLayout();
            this.splitContainerMain.SuspendLayout();
            this.splitContainerLeft.Panel2.SuspendLayout();
            this.splitContainerLeft.SuspendLayout();
            this.splitContainerController.Panel1.SuspendLayout();
            this.splitContainerController.Panel2.SuspendLayout();
            this.splitContainerController.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTime)).BeginInit();
            this.tabControlRight.SuspendLayout();
            this.tabPageMesh.SuspendLayout();
            this.tabPageMtl.SuspendLayout();
            this.tabPageAnim.SuspendLayout();
            this.groupSeqInfo.SuspendLayout();
            this.groupSeqList.SuspendLayout();
            this.tabPageSkeleton.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStripMain
            // 
            this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.Size = new System.Drawing.Size(798, 25);
            this.menuStripMain.TabIndex = 1;
            this.menuStripMain.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.importMeshLODToolStripMenuItem,
            this.importPSAToolStripMenuItem,
            this.toolStripSeparator4,
            this.loadSkeletalMeshToolStripMenuItem,
            this.saveSkeletalMeshToolStripMenuItem,
            this.toolStripSeparator5,
            this.loadAnimSetToolStripMenuItem,
            this.saveAnimMeshToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(39, 21);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // importMeshLODToolStripMenuItem
            // 
            this.importMeshLODToolStripMenuItem.Name = "importMeshLODToolStripMenuItem";
            this.importMeshLODToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.importMeshLODToolStripMenuItem.Text = "Import PS&K...";
            this.importMeshLODToolStripMenuItem.Click += new System.EventHandler(this.importMeshLODToolStripMenuItem_Click);
            // 
            // importPSAToolStripMenuItem
            // 
            this.importPSAToolStripMenuItem.Name = "importPSAToolStripMenuItem";
            this.importPSAToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.importPSAToolStripMenuItem.Text = "Import PS&A...";
            this.importPSAToolStripMenuItem.Click += new System.EventHandler(this.importPSAToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(192, 6);
            // 
            // loadSkeletalMeshToolStripMenuItem
            // 
            this.loadSkeletalMeshToolStripMenuItem.Name = "loadSkeletalMeshToolStripMenuItem";
            this.loadSkeletalMeshToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.loadSkeletalMeshToolStripMenuItem.Text = "Load Skeletal&Mesh...";
            this.loadSkeletalMeshToolStripMenuItem.Click += new System.EventHandler(this.loadSkeletalMeshToolStripMenuItem_Click);
            // 
            // saveSkeletalMeshToolStripMenuItem
            // 
            this.saveSkeletalMeshToolStripMenuItem.Name = "saveSkeletalMeshToolStripMenuItem";
            this.saveSkeletalMeshToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.saveSkeletalMeshToolStripMenuItem.Text = "Save SkeletalMesh...";
            this.saveSkeletalMeshToolStripMenuItem.Click += new System.EventHandler(this.saveSkeletalMeshToolStripMenuItem_Click);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(192, 6);
            // 
            // loadAnimSetToolStripMenuItem
            // 
            this.loadAnimSetToolStripMenuItem.Name = "loadAnimSetToolStripMenuItem";
            this.loadAnimSetToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.loadAnimSetToolStripMenuItem.Text = "Load AnimSet...";
            this.loadAnimSetToolStripMenuItem.Click += new System.EventHandler(this.loadAnimSetToolStripMenuItem_Click);
            // 
            // saveAnimMeshToolStripMenuItem
            // 
            this.saveAnimMeshToolStripMenuItem.Name = "saveAnimMeshToolStripMenuItem";
            this.saveAnimMeshToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.saveAnimMeshToolStripMenuItem.Text = "Save AnimMesh...";
            this.saveAnimMeshToolStripMenuItem.Click += new System.EventHandler(this.saveAnimMeshToolStripMenuItem_Click);
            // 
            // toolStripMain
            // 
            this.toolStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonRest,
            this.toolStripButtonSaveMesh,
            this.toolStripButtonSaveAnim,
            this.toolStripImportPSK,
            this.toolStripImportPSA,
            this.toolStripSeparator1,
            this.toolBtnLit,
            this.toolBtnUnlit,
            this.toolBtnWireframe,
            this.toolStripSeparator2,
            this.toolBtnZoomExtents,
            this.toolStripSeparator3,
            this.toolBtnXML});
            this.toolStripMain.Location = new System.Drawing.Point(0, 25);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(798, 40);
            this.toolStripMain.TabIndex = 2;
            this.toolStripMain.Text = "Resource Tool Bar";
            // 
            // toolStripButtonRest
            // 
            this.toolStripButtonRest.Image = global::NexusEditor.Properties.Resources.Reset;
            this.toolStripButtonRest.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonRest.Name = "toolStripButtonRest";
            this.toolStripButtonRest.Size = new System.Drawing.Size(68, 37);
            this.toolStripButtonRest.Text = "   Reset   ";
            this.toolStripButtonRest.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonRest.Click += new System.EventHandler(this.toolStripButtonRest_Click);
            // 
            // toolStripButtonSaveMesh
            // 
            this.toolStripButtonSaveMesh.Image = global::NexusEditor.Properties.Resources.Save;
            this.toolStripButtonSaveMesh.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSaveMesh.Name = "toolStripButtonSaveMesh";
            this.toolStripButtonSaveMesh.Size = new System.Drawing.Size(75, 37);
            this.toolStripButtonSaveMesh.Text = "Save Mesh";
            this.toolStripButtonSaveMesh.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonSaveMesh.ToolTipText = "Save Mesh To File";
            this.toolStripButtonSaveMesh.Click += new System.EventHandler(this.toolStripButtonSaveMesh_Click);
            // 
            // toolStripButtonSaveAnim
            // 
            this.toolStripButtonSaveAnim.Image = global::NexusEditor.Properties.Resources.Save;
            this.toolStripButtonSaveAnim.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSaveAnim.Name = "toolStripButtonSaveAnim";
            this.toolStripButtonSaveAnim.Size = new System.Drawing.Size(72, 37);
            this.toolStripButtonSaveAnim.Text = "Save Anim";
            this.toolStripButtonSaveAnim.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonSaveAnim.ToolTipText = "Save Anim Set To File";
            this.toolStripButtonSaveAnim.Click += new System.EventHandler(this.toolStripButtonSaveAnim_Click);
            // 
            // toolStripImportPSK
            // 
            this.toolStripImportPSK.Image = global::NexusEditor.Properties.Resources.connect;
            this.toolStripImportPSK.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripImportPSK.Name = "toolStripImportPSK";
            this.toolStripImportPSK.Size = new System.Drawing.Size(78, 37);
            this.toolStripImportPSK.Text = "Import PSK";
            this.toolStripImportPSK.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripImportPSK.ToolTipText = "Import Skeletal Mesh LOD ";
            this.toolStripImportPSK.Click += new System.EventHandler(this.toolStripImportPSK_Click);
            // 
            // toolStripImportPSA
            // 
            this.toolStripImportPSA.Image = global::NexusEditor.Properties.Resources.controls9;
            this.toolStripImportPSA.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripImportPSA.Name = "toolStripImportPSA";
            this.toolStripImportPSA.Size = new System.Drawing.Size(78, 37);
            this.toolStripImportPSA.Text = "Import PSA";
            this.toolStripImportPSA.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripImportPSA.ToolTipText = "Import Skeletal Mesh Animation";
            this.toolStripImportPSA.Click += new System.EventHandler(this.toolStripImportPSA_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 40);
            // 
            // toolBtnLit
            // 
            this.toolBtnLit.Image = global::NexusEditor.Properties.Resources.Light;
            this.toolBtnLit.ImageTransparentColor = System.Drawing.Color.Silver;
            this.toolBtnLit.Name = "toolBtnLit";
            this.toolBtnLit.Size = new System.Drawing.Size(57, 37);
            this.toolBtnLit.Text = "    Lit    ";
            this.toolBtnLit.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnLit.Click += new System.EventHandler(this.toolBtnLit_Click);
            // 
            // toolBtnUnlit
            // 
            this.toolBtnUnlit.Image = global::NexusEditor.Properties.Resources.UnLight;
            this.toolBtnUnlit.ImageTransparentColor = System.Drawing.Color.Silver;
            this.toolBtnUnlit.Name = "toolBtnUnlit";
            this.toolBtnUnlit.Size = new System.Drawing.Size(54, 37);
            this.toolBtnUnlit.Text = "  Unlit  ";
            this.toolBtnUnlit.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnUnlit.Click += new System.EventHandler(this.toolBtnUnlit_Click);
            // 
            // toolBtnWireframe
            // 
            this.toolBtnWireframe.Image = global::NexusEditor.Properties.Resources.Wireframe;
            this.toolBtnWireframe.ImageTransparentColor = System.Drawing.Color.White;
            this.toolBtnWireframe.Name = "toolBtnWireframe";
            this.toolBtnWireframe.Size = new System.Drawing.Size(73, 37);
            this.toolBtnWireframe.Text = "Wireframe";
            this.toolBtnWireframe.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnWireframe.Click += new System.EventHandler(this.toolBtnWireframe_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 40);
            // 
            // toolBtnZoomExtents
            // 
            this.toolBtnZoomExtents.Image = global::NexusEditor.Properties.Resources.ZoomExtents;
            this.toolBtnZoomExtents.ImageTransparentColor = System.Drawing.Color.White;
            this.toolBtnZoomExtents.Name = "toolBtnZoomExtents";
            this.toolBtnZoomExtents.Size = new System.Drawing.Size(87, 37);
            this.toolBtnZoomExtents.Text = "ZoomExtents";
            this.toolBtnZoomExtents.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnZoomExtents.Click += new System.EventHandler(this.toolBtnZoomExtents_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 40);
            // 
            // toolBtnXML
            // 
            this.toolBtnXML.Checked = true;
            this.toolBtnXML.CheckState = System.Windows.Forms.CheckState.Checked;
            this.toolBtnXML.Image = global::NexusEditor.Properties.Resources.page_white_code;
            this.toolBtnXML.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnXML.Name = "toolBtnXML";
            this.toolBtnXML.Size = new System.Drawing.Size(87, 37);
            this.toolBtnXML.Text = "Save As XML";
            this.toolBtnXML.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnXML.Click += new System.EventHandler(this.toolBtnXML_Click);
            // 
            // statusStripMain
            // 
            this.statusStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusSaveLoc});
            this.statusStripMain.Location = new System.Drawing.Point(0, 557);
            this.statusStripMain.Name = "statusStripMain";
            this.statusStripMain.Size = new System.Drawing.Size(798, 22);
            this.statusStripMain.TabIndex = 3;
            this.statusStripMain.Text = "statusStrip1";
            // 
            // statusSaveLoc
            // 
            this.statusSaveLoc.Image = global::NexusEditor.Properties.Resources.Save;
            this.statusSaveLoc.IsLink = true;
            this.statusSaveLoc.Name = "statusSaveLoc";
            this.statusSaveLoc.Size = new System.Drawing.Size(104, 17);
            this.statusSaveLoc.Text = "Save Location";
            this.statusSaveLoc.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.statusSaveLoc.ToolTipText = "Save Location";
            // 
            // imageListTab
            // 
            this.imageListTab.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListTab.ImageStream")));
            this.imageListTab.TransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(200)))), ((int)(((byte)(200)))));
            this.imageListTab.Images.SetKeyName(0, "Prop");
            this.imageListTab.Images.SetKeyName(1, "Material");
            this.imageListTab.Images.SetKeyName(2, "AnimInfo");
            // 
            // imageListBtn
            // 
            this.imageListBtn.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListBtn.ImageStream")));
            this.imageListBtn.TransparentColor = System.Drawing.Color.Silver;
            this.imageListBtn.Images.SetKeyName(0, "loop");
            this.imageListBtn.Images.SetKeyName(1, "play");
            this.imageListBtn.Images.SetKeyName(2, "stop");
            this.imageListBtn.Images.SetKeyName(3, "no_loop");
            // 
            // splitContainerMain
            // 
            this.splitContainerMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerMain.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainerMain.Location = new System.Drawing.Point(0, 65);
            this.splitContainerMain.Name = "splitContainerMain";
            // 
            // splitContainerMain.Panel1
            // 
            this.splitContainerMain.Panel1.Controls.Add(this.splitContainerLeft);
            // 
            // splitContainerMain.Panel2
            // 
            this.splitContainerMain.Panel2.Controls.Add(this.tabControlRight);
            this.splitContainerMain.Size = new System.Drawing.Size(798, 492);
            this.splitContainerMain.SplitterDistance = 522;
            this.splitContainerMain.TabIndex = 4;
            // 
            // splitContainerLeft
            // 
            this.splitContainerLeft.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerLeft.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainerLeft.IsSplitterFixed = true;
            this.splitContainerLeft.Location = new System.Drawing.Point(0, 0);
            this.splitContainerLeft.Name = "splitContainerLeft";
            this.splitContainerLeft.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerLeft.Panel2
            // 
            this.splitContainerLeft.Panel2.Controls.Add(this.splitContainerController);
            this.splitContainerLeft.Size = new System.Drawing.Size(522, 492);
            this.splitContainerLeft.SplitterDistance = 455;
            this.splitContainerLeft.TabIndex = 0;
            // 
            // splitContainerController
            // 
            this.splitContainerController.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerController.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainerController.Location = new System.Drawing.Point(0, 0);
            this.splitContainerController.Name = "splitContainerController";
            // 
            // splitContainerController.Panel1
            // 
            this.splitContainerController.Panel1.Controls.Add(this.checkBoxLoop);
            this.splitContainerController.Panel1.Controls.Add(this.btnPlay);
            // 
            // splitContainerController.Panel2
            // 
            this.splitContainerController.Panel2.Controls.Add(this.trackBarTime);
            this.splitContainerController.Size = new System.Drawing.Size(522, 33);
            this.splitContainerController.SplitterDistance = 68;
            this.splitContainerController.TabIndex = 0;
            // 
            // checkBoxLoop
            // 
            this.checkBoxLoop.Appearance = System.Windows.Forms.Appearance.Button;
            this.checkBoxLoop.AutoSize = true;
            this.checkBoxLoop.Checked = true;
            this.checkBoxLoop.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLoop.ImageKey = "loop";
            this.checkBoxLoop.ImageList = this.imageListBtn;
            this.checkBoxLoop.Location = new System.Drawing.Point(9, 5);
            this.checkBoxLoop.Name = "checkBoxLoop";
            this.checkBoxLoop.Size = new System.Drawing.Size(22, 22);
            this.checkBoxLoop.TabIndex = 6;
            this.checkBoxLoop.UseVisualStyleBackColor = true;
            this.checkBoxLoop.Click += new System.EventHandler(this.checkBoxLoop_CheckedChanged);
            // 
            // btnPlay
            // 
            this.btnPlay.ImageKey = "stop";
            this.btnPlay.ImageList = this.imageListBtn;
            this.btnPlay.Location = new System.Drawing.Point(37, 5);
            this.btnPlay.Name = "btnPlay";
            this.btnPlay.Size = new System.Drawing.Size(22, 22);
            this.btnPlay.TabIndex = 5;
            this.btnPlay.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnPlay.UseVisualStyleBackColor = true;
            this.btnPlay.Click += new System.EventHandler(this.btnPlay_Click);
            // 
            // trackBarTime
            // 
            this.trackBarTime.AutoSize = false;
            this.trackBarTime.Dock = System.Windows.Forms.DockStyle.Fill;
            this.trackBarTime.LargeChange = 2;
            this.trackBarTime.Location = new System.Drawing.Point(0, 0);
            this.trackBarTime.Maximum = 100;
            this.trackBarTime.Name = "trackBarTime";
            this.trackBarTime.Size = new System.Drawing.Size(450, 33);
            this.trackBarTime.TabIndex = 3;
            this.trackBarTime.Scroll += new System.EventHandler(this.trackBarTime_Scroll);
            // 
            // tabControlRight
            // 
            this.tabControlRight.Controls.Add(this.tabPageMesh);
            this.tabControlRight.Controls.Add(this.tabPageMtl);
            this.tabControlRight.Controls.Add(this.tabPageAnim);
            this.tabControlRight.Controls.Add(this.tabPageSkeleton);
            this.tabControlRight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlRight.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tabControlRight.ImageList = this.imageListTab;
            this.tabControlRight.Location = new System.Drawing.Point(0, 0);
            this.tabControlRight.Name = "tabControlRight";
            this.tabControlRight.SelectedIndex = 0;
            this.tabControlRight.Size = new System.Drawing.Size(272, 492);
            this.tabControlRight.TabIndex = 0;
            // 
            // tabPageMesh
            // 
            this.tabPageMesh.Controls.Add(this.propertyGridMesh);
            this.tabPageMesh.ImageIndex = 0;
            this.tabPageMesh.Location = new System.Drawing.Point(4, 23);
            this.tabPageMesh.Name = "tabPageMesh";
            this.tabPageMesh.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageMesh.Size = new System.Drawing.Size(264, 465);
            this.tabPageMesh.TabIndex = 0;
            this.tabPageMesh.Text = "Mesh";
            this.tabPageMesh.UseVisualStyleBackColor = true;
            // 
            // propertyGridMesh
            // 
            this.propertyGridMesh.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridMesh.Location = new System.Drawing.Point(3, 3);
            this.propertyGridMesh.Name = "propertyGridMesh";
            this.propertyGridMesh.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.propertyGridMesh.Size = new System.Drawing.Size(258, 459);
            this.propertyGridMesh.TabIndex = 4;
            // 
            // tabPageMtl
            // 
            this.tabPageMtl.Controls.Add(this.propertyGridMtl);
            this.tabPageMtl.Controls.Add(this.comboBoxSection);
            this.tabPageMtl.Controls.Add(this.label2);
            this.tabPageMtl.Controls.Add(this.comboBoxLOD);
            this.tabPageMtl.Controls.Add(this.label1);
            this.tabPageMtl.ImageIndex = 1;
            this.tabPageMtl.Location = new System.Drawing.Point(4, 23);
            this.tabPageMtl.Name = "tabPageMtl";
            this.tabPageMtl.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageMtl.Size = new System.Drawing.Size(264, 465);
            this.tabPageMtl.TabIndex = 1;
            this.tabPageMtl.Text = "Material";
            this.tabPageMtl.UseVisualStyleBackColor = true;
            // 
            // propertyGridMtl
            // 
            this.propertyGridMtl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridMtl.Location = new System.Drawing.Point(3, 75);
            this.propertyGridMtl.Name = "propertyGridMtl";
            this.propertyGridMtl.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.propertyGridMtl.Size = new System.Drawing.Size(258, 387);
            this.propertyGridMtl.TabIndex = 11;
            this.propertyGridMtl.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGridMtl_PropertyValueChanged);
            // 
            // comboBoxSection
            // 
            this.comboBoxSection.Dock = System.Windows.Forms.DockStyle.Top;
            this.comboBoxSection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxSection.FormattingEnabled = true;
            this.comboBoxSection.Location = new System.Drawing.Point(3, 53);
            this.comboBoxSection.Name = "comboBoxSection";
            this.comboBoxSection.Size = new System.Drawing.Size(258, 22);
            this.comboBoxSection.TabIndex = 9;
            this.comboBoxSection.SelectedIndexChanged += new System.EventHandler(this.comboBoxSection_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Top;
            this.label2.Location = new System.Drawing.Point(3, 39);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(43, 14);
            this.label2.TabIndex = 6;
            this.label2.Text = "Section";
            // 
            // comboBoxLOD
            // 
            this.comboBoxLOD.Dock = System.Windows.Forms.DockStyle.Top;
            this.comboBoxLOD.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxLOD.FormattingEnabled = true;
            this.comboBoxLOD.Location = new System.Drawing.Point(3, 17);
            this.comboBoxLOD.Name = "comboBoxLOD";
            this.comboBoxLOD.Size = new System.Drawing.Size(258, 22);
            this.comboBoxLOD.TabIndex = 8;
            this.comboBoxLOD.SelectedIndexChanged += new System.EventHandler(this.comboBoxLOD_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Location = new System.Drawing.Point(3, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(28, 14);
            this.label1.TabIndex = 7;
            this.label1.Text = "LOD";
            // 
            // tabPageAnim
            // 
            this.tabPageAnim.Controls.Add(this.groupSeqInfo);
            this.tabPageAnim.Controls.Add(this.groupSeqList);
            this.tabPageAnim.ImageIndex = 2;
            this.tabPageAnim.Location = new System.Drawing.Point(4, 23);
            this.tabPageAnim.Name = "tabPageAnim";
            this.tabPageAnim.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageAnim.Size = new System.Drawing.Size(264, 465);
            this.tabPageAnim.TabIndex = 2;
            this.tabPageAnim.Text = "Animation";
            this.tabPageAnim.UseVisualStyleBackColor = true;
            // 
            // groupSeqInfo
            // 
            this.groupSeqInfo.Controls.Add(this.propertyGridSeq);
            this.groupSeqInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupSeqInfo.Location = new System.Drawing.Point(3, 212);
            this.groupSeqInfo.Name = "groupSeqInfo";
            this.groupSeqInfo.Size = new System.Drawing.Size(258, 250);
            this.groupSeqInfo.TabIndex = 5;
            this.groupSeqInfo.TabStop = false;
            this.groupSeqInfo.Text = "Anim Sequence Info";
            // 
            // propertyGridSeq
            // 
            this.propertyGridSeq.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridSeq.Location = new System.Drawing.Point(3, 16);
            this.propertyGridSeq.Name = "propertyGridSeq";
            this.propertyGridSeq.Size = new System.Drawing.Size(252, 231);
            this.propertyGridSeq.TabIndex = 3;
            // 
            // groupSeqList
            // 
            this.groupSeqList.Controls.Add(this.listBoxAnim);
            this.groupSeqList.Controls.Add(this.btnTrnRemoveSeq);
            this.groupSeqList.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupSeqList.Location = new System.Drawing.Point(3, 3);
            this.groupSeqList.Name = "groupSeqList";
            this.groupSeqList.Size = new System.Drawing.Size(258, 209);
            this.groupSeqList.TabIndex = 4;
            this.groupSeqList.TabStop = false;
            this.groupSeqList.Text = "Anim Sequence List";
            // 
            // listBoxAnim
            // 
            this.listBoxAnim.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem3,
            listViewItem4});
            this.listBoxAnim.Location = new System.Drawing.Point(7, 19);
            this.listBoxAnim.MultiSelect = false;
            this.listBoxAnim.Name = "listBoxAnim";
            this.listBoxAnim.Size = new System.Drawing.Size(205, 184);
            this.listBoxAnim.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.listBoxAnim.TabIndex = 16;
            this.listBoxAnim.UseCompatibleStateImageBehavior = false;
            this.listBoxAnim.View = System.Windows.Forms.View.List;
            this.listBoxAnim.SelectedIndexChanged += new System.EventHandler(this.listBoxAnim_SelectedIndexChanged);
            // 
            // btnTrnRemoveSeq
            // 
            this.btnTrnRemoveSeq.Image = global::NexusEditor.Properties.Resources.Del;
            this.btnTrnRemoveSeq.Location = new System.Drawing.Point(218, 19);
            this.btnTrnRemoveSeq.Name = "btnTrnRemoveSeq";
            this.btnTrnRemoveSeq.Size = new System.Drawing.Size(30, 30);
            this.btnTrnRemoveSeq.TabIndex = 15;
            this.btnTrnRemoveSeq.UseVisualStyleBackColor = false;
            this.btnTrnRemoveSeq.Click += new System.EventHandler(this.btnTrnRemoveSeq_Click);
            // 
            // tabPageSkeleton
            // 
            this.tabPageSkeleton.Controls.Add(this.treeViewSkeletal);
            this.tabPageSkeleton.Location = new System.Drawing.Point(4, 23);
            this.tabPageSkeleton.Name = "tabPageSkeleton";
            this.tabPageSkeleton.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageSkeleton.Size = new System.Drawing.Size(264, 465);
            this.tabPageSkeleton.TabIndex = 3;
            this.tabPageSkeleton.Text = "Skeleton";
            this.tabPageSkeleton.UseVisualStyleBackColor = true;
            // 
            // treeViewSkeletal
            // 
            this.treeViewSkeletal.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewSkeletal.Location = new System.Drawing.Point(3, 3);
            this.treeViewSkeletal.Name = "treeViewSkeletal";
            treeNode2.Name = "Root";
            treeNode2.Text = "root";
            this.treeViewSkeletal.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode2});
            this.treeViewSkeletal.Size = new System.Drawing.Size(258, 459);
            this.treeViewSkeletal.TabIndex = 1;
            // 
            // ResAnimSetEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.ClientSize = new System.Drawing.Size(798, 579);
            this.Controls.Add(this.splitContainerMain);
            this.Controls.Add(this.statusStripMain);
            this.Controls.Add(this.toolStripMain);
            this.Controls.Add(this.menuStripMain);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ResAnimSetEditor";
            this.ShowIcon = false;
            this.Text = "Nexus AnimSet Editor";
            this.menuStripMain.ResumeLayout(false);
            this.menuStripMain.PerformLayout();
            this.toolStripMain.ResumeLayout(false);
            this.toolStripMain.PerformLayout();
            this.statusStripMain.ResumeLayout(false);
            this.statusStripMain.PerformLayout();
            this.splitContainerMain.Panel1.ResumeLayout(false);
            this.splitContainerMain.Panel2.ResumeLayout(false);
            this.splitContainerMain.ResumeLayout(false);
            this.splitContainerLeft.Panel2.ResumeLayout(false);
            this.splitContainerLeft.ResumeLayout(false);
            this.splitContainerController.Panel1.ResumeLayout(false);
            this.splitContainerController.Panel1.PerformLayout();
            this.splitContainerController.Panel2.ResumeLayout(false);
            this.splitContainerController.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTime)).EndInit();
            this.tabControlRight.ResumeLayout(false);
            this.tabPageMesh.ResumeLayout(false);
            this.tabPageMtl.ResumeLayout(false);
            this.tabPageMtl.PerformLayout();
            this.tabPageAnim.ResumeLayout(false);
            this.groupSeqInfo.ResumeLayout(false);
            this.groupSeqList.ResumeLayout(false);
            this.tabPageSkeleton.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStripMain;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.ToolStripButton toolStripButtonRest;
        private System.Windows.Forms.ToolStripButton toolStripButtonSaveMesh;
        private System.Windows.Forms.ToolStripButton toolStripImportPSK;
        private System.Windows.Forms.ToolStripButton toolStripImportPSA;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolBtnLit;
        private System.Windows.Forms.ToolStripButton toolBtnUnlit;
        private System.Windows.Forms.ToolStripButton toolBtnWireframe;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton toolBtnZoomExtents;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton toolBtnXML;
        private System.Windows.Forms.StatusStrip statusStripMain;
        private System.Windows.Forms.ToolStripStatusLabel statusSaveLoc;
        private System.Windows.Forms.ImageList imageListTab;
        private System.Windows.Forms.ImageList imageListBtn;
        private System.Windows.Forms.SplitContainer splitContainerMain;
        private System.Windows.Forms.SplitContainer splitContainerLeft;
        private System.Windows.Forms.TabControl tabControlRight;
        private System.Windows.Forms.TabPage tabPageMesh;
        private System.Windows.Forms.TabPage tabPageMtl;
        private System.Windows.Forms.PropertyGrid propertyGridMtl;
        private System.Windows.Forms.ComboBox comboBoxSection;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox comboBoxLOD;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TabPage tabPageAnim;
        private NexusEditor.NControl.CollapsibleGroupBox groupSeqInfo;
        private System.Windows.Forms.PropertyGrid propertyGridSeq;
        private NexusEditor.NControl.CollapsibleGroupBox groupSeqList;
        private System.Windows.Forms.Button btnTrnRemoveSeq;
        private System.Windows.Forms.ToolStripMenuItem importMeshLODToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importPSAToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButtonSaveAnim;
        private System.Windows.Forms.SplitContainer splitContainerController;
        private System.Windows.Forms.CheckBox checkBoxLoop;
        private System.Windows.Forms.Button btnPlay;
        private System.Windows.Forms.TrackBar trackBarTime;
        private System.Windows.Forms.ListView listBoxAnim;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem loadSkeletalMeshToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveSkeletalMeshToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem loadAnimSetToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAnimMeshToolStripMenuItem;
        private System.Windows.Forms.TabPage tabPageSkeleton;
        private System.Windows.Forms.TreeView treeViewSkeletal;
        private System.Windows.Forms.PropertyGrid propertyGridMesh;
    }
}