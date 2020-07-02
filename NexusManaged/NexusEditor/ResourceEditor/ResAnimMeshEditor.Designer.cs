namespace NexusEditor.ResourceEditor
{
    partial class ResAnimMeshEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ResAnimMeshEditor));
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.toolStripButtonRest = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonNew = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSave = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonImport = new System.Windows.Forms.ToolStripButton();
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
            this.splitContainerMain = new System.Windows.Forms.SplitContainer();
            this.splitContainerLeft = new System.Windows.Forms.SplitContainer();
            this.checkBoxLoop = new System.Windows.Forms.CheckBox();
            this.imageListBtn = new System.Windows.Forms.ImageList(this.components);
            this.trackBarTime = new System.Windows.Forms.TrackBar();
            this.btnPlay = new System.Windows.Forms.Button();
            this.tabControlRight = new System.Windows.Forms.TabControl();
            this.tabPageProperty = new System.Windows.Forms.TabPage();
            this.propertyGridRes = new System.Windows.Forms.PropertyGrid();
            this.tabPageMtl = new System.Windows.Forms.TabPage();
            this.buttonApplyMtlProperty = new System.Windows.Forms.Button();
            this.propertyGridMtl = new System.Windows.Forms.PropertyGrid();
            this.btnCreateMtl = new System.Windows.Forms.Button();
            this.comboBoxSection = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBoxLOD = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPageAnim = new System.Windows.Forms.TabPage();
            this.groupSeqInfo = new NControl.CollapsibleGroupBox();
            this.propertyGridSeq = new System.Windows.Forms.PropertyGrid();
            this.groupSeqList = new NControl.CollapsibleGroupBox();
            this.btnTrnRemoveSeq = new System.Windows.Forms.Button();
            this.listBoxAnim = new System.Windows.Forms.ListBox();
            this.imageListTab = new System.Windows.Forms.ImageList(this.components);
            this.timerTick = new System.Windows.Forms.Timer(this.components);
            this.menuStripMain.SuspendLayout();
            this.toolStripMain.SuspendLayout();
            this.statusStripMain.SuspendLayout();
            this.splitContainerMain.Panel1.SuspendLayout();
            this.splitContainerMain.Panel2.SuspendLayout();
            this.splitContainerMain.SuspendLayout();
            this.splitContainerLeft.Panel2.SuspendLayout();
            this.splitContainerLeft.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTime)).BeginInit();
            this.tabControlRight.SuspendLayout();
            this.tabPageProperty.SuspendLayout();
            this.tabPageMtl.SuspendLayout();
            this.tabPageAnim.SuspendLayout();
            this.groupSeqInfo.SuspendLayout();
            this.groupSeqList.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStripMain
            // 
            this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.Size = new System.Drawing.Size(915, 24);
            this.menuStripMain.TabIndex = 0;
            this.menuStripMain.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // toolStripMain
            // 
            this.toolStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonRest,
            this.toolStripButtonNew,
            this.toolStripButtonSave,
            this.toolStripButtonImport,
            this.toolStripSeparator1,
            this.toolBtnLit,
            this.toolBtnUnlit,
            this.toolBtnWireframe,
            this.toolStripSeparator2,
            this.toolBtnZoomExtents,
            this.toolStripSeparator3,
            this.toolBtnXML});
            this.toolStripMain.Location = new System.Drawing.Point(0, 24);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(915, 36);
            this.toolStripMain.TabIndex = 1;
            this.toolStripMain.Text = "Resource Tool Bar";
            // 
            // toolStripButtonRest
            // 
            this.toolStripButtonRest.Image = global::NexusEditor.Properties.Resources.Reset;
            this.toolStripButtonRest.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonRest.Name = "toolStripButtonRest";
            this.toolStripButtonRest.Size = new System.Drawing.Size(57, 33);
            this.toolStripButtonRest.Text = "   Reset   ";
            this.toolStripButtonRest.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonRest.Click += new System.EventHandler(this.toolStripButtonRest_Click);
            // 
            // toolStripButtonNew
            // 
            this.toolStripButtonNew.Image = global::NexusEditor.Properties.Resources.white_picture;
            this.toolStripButtonNew.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonNew.Name = "toolStripButtonNew";
            this.toolStripButtonNew.Size = new System.Drawing.Size(83, 33);
            this.toolStripButtonNew.Text = "New AnimMesh";
            this.toolStripButtonNew.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonNew.Click += new System.EventHandler(this.toolStripButtonNew_Click);
            // 
            // toolStripButtonSave
            // 
            this.toolStripButtonSave.Image = global::NexusEditor.Properties.Resources.Save;
            this.toolStripButtonSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSave.Name = "toolStripButtonSave";
            this.toolStripButtonSave.Size = new System.Drawing.Size(67, 33);
            this.toolStripButtonSave.Text = "Save to File";
            this.toolStripButtonSave.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonSave.Click += new System.EventHandler(this.toolStripButtonSave_Click);
            // 
            // toolStripButtonImport
            // 
            this.toolStripButtonImport.Image = global::NexusEditor.Properties.Resources.connect;
            this.toolStripButtonImport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonImport.Name = "toolStripButtonImport";
            this.toolStripButtonImport.Size = new System.Drawing.Size(66, 33);
            this.toolStripButtonImport.Text = "Import LOD";
            this.toolStripButtonImport.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonImport.Click += new System.EventHandler(this.toolStripButtonImport_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 36);
            // 
            // toolBtnLit
            // 
            this.toolBtnLit.Image = global::NexusEditor.Properties.Resources.Light;
            this.toolBtnLit.ImageTransparentColor = System.Drawing.Color.Silver;
            this.toolBtnLit.Name = "toolBtnLit";
            this.toolBtnLit.Size = new System.Drawing.Size(46, 33);
            this.toolBtnLit.Text = "    Lit    ";
            this.toolBtnLit.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnLit.Click += new System.EventHandler(this.toolBtnLit_Click);
            // 
            // toolBtnUnlit
            // 
            this.toolBtnUnlit.Image = global::NexusEditor.Properties.Resources.UnLight;
            this.toolBtnUnlit.ImageTransparentColor = System.Drawing.Color.Silver;
            this.toolBtnUnlit.Name = "toolBtnUnlit";
            this.toolBtnUnlit.Size = new System.Drawing.Size(44, 33);
            this.toolBtnUnlit.Text = "  Unlit  ";
            this.toolBtnUnlit.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnUnlit.Click += new System.EventHandler(this.toolBtnUnlit_Click);
            // 
            // toolBtnWireframe
            // 
            this.toolBtnWireframe.Image = global::NexusEditor.Properties.Resources.Wireframe;
            this.toolBtnWireframe.ImageTransparentColor = System.Drawing.Color.White;
            this.toolBtnWireframe.Name = "toolBtnWireframe";
            this.toolBtnWireframe.Size = new System.Drawing.Size(61, 33);
            this.toolBtnWireframe.Text = "Wireframe";
            this.toolBtnWireframe.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnWireframe.Click += new System.EventHandler(this.toolBtnWireframe_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 36);
            // 
            // toolBtnZoomExtents
            // 
            this.toolBtnZoomExtents.Image = global::NexusEditor.Properties.Resources.ZoomExtents;
            this.toolBtnZoomExtents.ImageTransparentColor = System.Drawing.Color.White;
            this.toolBtnZoomExtents.Name = "toolBtnZoomExtents";
            this.toolBtnZoomExtents.Size = new System.Drawing.Size(74, 33);
            this.toolBtnZoomExtents.Text = "ZoomExtents";
            this.toolBtnZoomExtents.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnZoomExtents.Click += new System.EventHandler(this.toolBtnZoomExtents_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 36);
            // 
            // toolBtnXML
            // 
            this.toolBtnXML.Checked = true;
            this.toolBtnXML.CheckState = System.Windows.Forms.CheckState.Checked;
            this.toolBtnXML.Image = global::NexusEditor.Properties.Resources.page_white_code;
            this.toolBtnXML.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBtnXML.Name = "toolBtnXML";
            this.toolBtnXML.Size = new System.Drawing.Size(72, 33);
            this.toolBtnXML.Text = "Save As XML";
            this.toolBtnXML.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnXML.Click += new System.EventHandler(this.toolBtnXML_Click);
            // 
            // statusStripMain
            // 
            this.statusStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusSaveLoc});
            this.statusStripMain.Location = new System.Drawing.Point(0, 616);
            this.statusStripMain.Name = "statusStripMain";
            this.statusStripMain.Size = new System.Drawing.Size(915, 22);
            this.statusStripMain.TabIndex = 2;
            this.statusStripMain.Text = "statusStrip1";
            // 
            // statusSaveLoc
            // 
            this.statusSaveLoc.Image = global::NexusEditor.Properties.Resources.Save;
            this.statusSaveLoc.IsLink = true;
            this.statusSaveLoc.Name = "statusSaveLoc";
            this.statusSaveLoc.Size = new System.Drawing.Size(90, 17);
            this.statusSaveLoc.Text = "Save Location";
            this.statusSaveLoc.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.statusSaveLoc.ToolTipText = "Save Location";
            // 
            // splitContainerMain
            // 
            this.splitContainerMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerMain.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainerMain.Location = new System.Drawing.Point(0, 60);
            this.splitContainerMain.Name = "splitContainerMain";
            // 
            // splitContainerMain.Panel1
            // 
            this.splitContainerMain.Panel1.Controls.Add(this.splitContainerLeft);
            // 
            // splitContainerMain.Panel2
            // 
            this.splitContainerMain.Panel2.Controls.Add(this.tabControlRight);
            this.splitContainerMain.Size = new System.Drawing.Size(915, 556);
            this.splitContainerMain.SplitterDistance = 640;
            this.splitContainerMain.TabIndex = 3;
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
            this.splitContainerLeft.Panel2.Controls.Add(this.checkBoxLoop);
            this.splitContainerLeft.Panel2.Controls.Add(this.trackBarTime);
            this.splitContainerLeft.Panel2.Controls.Add(this.btnPlay);
            this.splitContainerLeft.Panel2.Resize += new System.EventHandler(this.splitContainerLeft_Panel2_Resize);
            this.splitContainerLeft.Size = new System.Drawing.Size(640, 556);
            this.splitContainerLeft.SplitterDistance = 520;
            this.splitContainerLeft.TabIndex = 0;
            // 
            // checkBoxLoop
            // 
            this.checkBoxLoop.Appearance = System.Windows.Forms.Appearance.Button;
            this.checkBoxLoop.AutoSize = true;
            this.checkBoxLoop.Checked = true;
            this.checkBoxLoop.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLoop.ImageKey = "loop";
            this.checkBoxLoop.ImageList = this.imageListBtn;
            this.checkBoxLoop.Location = new System.Drawing.Point(9, 6);
            this.checkBoxLoop.Name = "checkBoxLoop";
            this.checkBoxLoop.Size = new System.Drawing.Size(22, 22);
            this.checkBoxLoop.TabIndex = 2;
            this.checkBoxLoop.UseVisualStyleBackColor = true;
            this.checkBoxLoop.CheckedChanged += new System.EventHandler(this.checkBoxLoop_CheckedChanged);
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
            // trackBarTime
            // 
            this.trackBarTime.AutoSize = false;
            this.trackBarTime.Location = new System.Drawing.Point(65, 3);
            this.trackBarTime.Maximum = 100;
            this.trackBarTime.Name = "trackBarTime";
            this.trackBarTime.Size = new System.Drawing.Size(572, 26);
            this.trackBarTime.TabIndex = 1;
            this.trackBarTime.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // btnPlay
            // 
            this.btnPlay.ImageKey = "play";
            this.btnPlay.ImageList = this.imageListBtn;
            this.btnPlay.Location = new System.Drawing.Point(37, 6);
            this.btnPlay.Name = "btnPlay";
            this.btnPlay.Size = new System.Drawing.Size(22, 22);
            this.btnPlay.TabIndex = 0;
            this.btnPlay.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.btnPlay.UseVisualStyleBackColor = true;
            this.btnPlay.Click += new System.EventHandler(this.btnPlay_Click);
            // 
            // tabControlRight
            // 
            this.tabControlRight.Controls.Add(this.tabPageProperty);
            this.tabControlRight.Controls.Add(this.tabPageMtl);
            this.tabControlRight.Controls.Add(this.tabPageAnim);
            this.tabControlRight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlRight.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tabControlRight.ImageList = this.imageListTab;
            this.tabControlRight.Location = new System.Drawing.Point(0, 0);
            this.tabControlRight.Name = "tabControlRight";
            this.tabControlRight.SelectedIndex = 0;
            this.tabControlRight.Size = new System.Drawing.Size(271, 556);
            this.tabControlRight.TabIndex = 0;
            // 
            // tabPageProperty
            // 
            this.tabPageProperty.Controls.Add(this.propertyGridRes);
            this.tabPageProperty.ImageIndex = 0;
            this.tabPageProperty.Location = new System.Drawing.Point(4, 23);
            this.tabPageProperty.Name = "tabPageProperty";
            this.tabPageProperty.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageProperty.Size = new System.Drawing.Size(263, 529);
            this.tabPageProperty.TabIndex = 0;
            this.tabPageProperty.Text = "Resource";
            this.tabPageProperty.UseVisualStyleBackColor = true;
            // 
            // propertyGridRes
            // 
            this.propertyGridRes.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridRes.Location = new System.Drawing.Point(3, 3);
            this.propertyGridRes.Name = "propertyGridRes";
            this.propertyGridRes.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.propertyGridRes.Size = new System.Drawing.Size(257, 523);
            this.propertyGridRes.TabIndex = 1;
            // 
            // tabPageMtl
            // 
            this.tabPageMtl.Controls.Add(this.buttonApplyMtlProperty);
            this.tabPageMtl.Controls.Add(this.propertyGridMtl);
            this.tabPageMtl.Controls.Add(this.btnCreateMtl);
            this.tabPageMtl.Controls.Add(this.comboBoxSection);
            this.tabPageMtl.Controls.Add(this.label2);
            this.tabPageMtl.Controls.Add(this.comboBoxLOD);
            this.tabPageMtl.Controls.Add(this.label1);
            this.tabPageMtl.ImageIndex = 1;
            this.tabPageMtl.Location = new System.Drawing.Point(4, 23);
            this.tabPageMtl.Name = "tabPageMtl";
            this.tabPageMtl.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageMtl.Size = new System.Drawing.Size(263, 529);
            this.tabPageMtl.TabIndex = 1;
            this.tabPageMtl.Text = "Material";
            this.tabPageMtl.UseVisualStyleBackColor = true;
            // 
            // buttonApplyMtlProperty
            // 
            this.buttonApplyMtlProperty.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.buttonApplyMtlProperty.Location = new System.Drawing.Point(3, 480);
            this.buttonApplyMtlProperty.Name = "buttonApplyMtlProperty";
            this.buttonApplyMtlProperty.Size = new System.Drawing.Size(257, 23);
            this.buttonApplyMtlProperty.TabIndex = 12;
            this.buttonApplyMtlProperty.Text = "Apply";
            this.buttonApplyMtlProperty.UseVisualStyleBackColor = true;
            this.buttonApplyMtlProperty.Click += new System.EventHandler(this.buttonApplyMtlProperty_Click);
            // 
            // propertyGridMtl
            // 
            this.propertyGridMtl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridMtl.Location = new System.Drawing.Point(3, 75);
            this.propertyGridMtl.Name = "propertyGridMtl";
            this.propertyGridMtl.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.propertyGridMtl.Size = new System.Drawing.Size(257, 428);
            this.propertyGridMtl.TabIndex = 11;
            this.propertyGridMtl.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGridMtl_PropertyValueChanged);
            // 
            // btnCreateMtl
            // 
            this.btnCreateMtl.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.btnCreateMtl.Location = new System.Drawing.Point(3, 503);
            this.btnCreateMtl.Margin = new System.Windows.Forms.Padding(16, 3, 16, 3);
            this.btnCreateMtl.Name = "btnCreateMtl";
            this.btnCreateMtl.Size = new System.Drawing.Size(257, 23);
            this.btnCreateMtl.TabIndex = 10;
            this.btnCreateMtl.Text = "Create";
            this.btnCreateMtl.UseVisualStyleBackColor = true;
            this.btnCreateMtl.Click += new System.EventHandler(this.btnCreateMtl_Click);
            // 
            // comboBoxSection
            // 
            this.comboBoxSection.Dock = System.Windows.Forms.DockStyle.Top;
            this.comboBoxSection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxSection.FormattingEnabled = true;
            this.comboBoxSection.Location = new System.Drawing.Point(3, 53);
            this.comboBoxSection.Name = "comboBoxSection";
            this.comboBoxSection.Size = new System.Drawing.Size(257, 22);
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
            this.comboBoxLOD.Size = new System.Drawing.Size(257, 22);
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
            this.tabPageAnim.Size = new System.Drawing.Size(263, 529);
            this.tabPageAnim.TabIndex = 2;
            this.tabPageAnim.Text = "Animation";
            this.tabPageAnim.UseVisualStyleBackColor = true;
            // 
            // groupSeqInfo
            // 
            this.groupSeqInfo.Controls.Add(this.propertyGridSeq);
            this.groupSeqInfo.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupSeqInfo.Location = new System.Drawing.Point(3, 212);
            this.groupSeqInfo.Name = "groupSeqInfo";
            this.groupSeqInfo.Size = new System.Drawing.Size(257, 311);
            this.groupSeqInfo.TabIndex = 5;
            this.groupSeqInfo.TabStop = false;
            this.groupSeqInfo.Text = "Anim Sequence Info";
            // 
            // propertyGridSeq
            // 
            this.propertyGridSeq.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridSeq.Location = new System.Drawing.Point(3, 16);
            this.propertyGridSeq.Name = "propertyGridSeq";
            this.propertyGridSeq.Size = new System.Drawing.Size(251, 292);
            this.propertyGridSeq.TabIndex = 3;
            // 
            // groupSeqList
            // 
            this.groupSeqList.Controls.Add(this.btnTrnRemoveSeq);
            this.groupSeqList.Controls.Add(this.listBoxAnim);
            this.groupSeqList.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupSeqList.Location = new System.Drawing.Point(3, 3);
            this.groupSeqList.Name = "groupSeqList";
            this.groupSeqList.Size = new System.Drawing.Size(257, 209);
            this.groupSeqList.TabIndex = 4;
            this.groupSeqList.TabStop = false;
            this.groupSeqList.Text = "Anim Sequence List";
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
            // listBoxAnim
            // 
            this.listBoxAnim.Dock = System.Windows.Forms.DockStyle.Left;
            this.listBoxAnim.FormattingEnabled = true;
            this.listBoxAnim.ItemHeight = 14;
            this.listBoxAnim.Location = new System.Drawing.Point(3, 16);
            this.listBoxAnim.Name = "listBoxAnim";
            this.listBoxAnim.Size = new System.Drawing.Size(209, 186);
            this.listBoxAnim.TabIndex = 0;
            this.listBoxAnim.SelectedIndexChanged += new System.EventHandler(this.listBoxAnim_SelectedIndexChanged);
            // 
            // imageListTab
            // 
            this.imageListTab.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListTab.ImageStream")));
            this.imageListTab.TransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(200)))), ((int)(((byte)(200)))));
            this.imageListTab.Images.SetKeyName(0, "Prop");
            this.imageListTab.Images.SetKeyName(1, "Material");
            this.imageListTab.Images.SetKeyName(2, "AnimInfo");
            // 
            // timerTick
            // 
            this.timerTick.Interval = 20;
            this.timerTick.Tick += new System.EventHandler(this.timerTick_Tick);
            // 
            // ResAnimMeshEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(915, 638);
            this.Controls.Add(this.splitContainerMain);
            this.Controls.Add(this.statusStripMain);
            this.Controls.Add(this.toolStripMain);
            this.Controls.Add(this.menuStripMain);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStripMain;
            this.Name = "ResAnimMeshEditor";
            this.Text = "Nexus Anim Mesh Editor";
            this.Load += new System.EventHandler(this.ResAnimMeshEditor_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ResAnimMeshEditor_FormClosing);
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
            this.splitContainerLeft.Panel2.PerformLayout();
            this.splitContainerLeft.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTime)).EndInit();
            this.tabControlRight.ResumeLayout(false);
            this.tabPageProperty.ResumeLayout(false);
            this.tabPageMtl.ResumeLayout(false);
            this.tabPageMtl.PerformLayout();
            this.tabPageAnim.ResumeLayout(false);
            this.groupSeqInfo.ResumeLayout(false);
            this.groupSeqList.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStripMain;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.StatusStrip statusStripMain;
        private System.Windows.Forms.SplitContainer splitContainerMain;
        private System.Windows.Forms.TabControl tabControlRight;
        private System.Windows.Forms.TabPage tabPageProperty;
        private System.Windows.Forms.TabPage tabPageMtl;
        private System.Windows.Forms.ToolStripButton toolStripButtonRest;
        private System.Windows.Forms.ToolStripButton toolStripButtonNew;
        private System.Windows.Forms.ToolStripButton toolStripButtonSave;
        private System.Windows.Forms.ToolStripButton toolStripButtonImport;
        private System.Windows.Forms.ToolStripButton toolBtnLit;
        private System.Windows.Forms.ToolStripButton toolBtnUnlit;
        private System.Windows.Forms.ToolStripButton toolBtnWireframe;
        private System.Windows.Forms.ToolStripButton toolBtnZoomExtents;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ImageList imageListTab;
        private System.Windows.Forms.Button buttonApplyMtlProperty;
        private System.Windows.Forms.PropertyGrid propertyGridMtl;
        private System.Windows.Forms.Button btnCreateMtl;
        private System.Windows.Forms.ComboBox comboBoxSection;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox comboBoxLOD;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PropertyGrid propertyGridRes;
        private System.Windows.Forms.SplitContainer splitContainerLeft;
        private System.Windows.Forms.Button btnPlay;
        private System.Windows.Forms.TrackBar trackBarTime;
        private System.Windows.Forms.TabPage tabPageAnim;
        private System.Windows.Forms.ListBox listBoxAnim;
        private System.Windows.Forms.PropertyGrid propertyGridSeq;
        private NControl.CollapsibleGroupBox groupSeqInfo;
        private NControl.CollapsibleGroupBox groupSeqList;
        private System.Windows.Forms.ToolStripStatusLabel statusSaveLoc;
        private System.Windows.Forms.Timer timerTick;
        private System.Windows.Forms.CheckBox checkBoxLoop;
        private System.Windows.Forms.ImageList imageListBtn;
        private System.Windows.Forms.Button btnTrnRemoveSeq;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton toolBtnXML;
    }
}