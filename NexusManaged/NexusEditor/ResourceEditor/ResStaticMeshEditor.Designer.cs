namespace NexusEditor.ResourceEditor
{
    partial class ResStaticMeshEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ResStaticMeshEditor));
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.resetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importDefautsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.toolStripButtonRest = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonNew = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSave = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonImport = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolBtnLit = new System.Windows.Forms.ToolStripButton();
            this.toolBtnUnlit = new System.Windows.Forms.ToolStripButton();
            this.toolBtnWireframe = new System.Windows.Forms.ToolStripButton();
            this.toolBtnZoomExtents = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolBtnXML = new System.Windows.Forms.ToolStripButton();
            this.statusStripMain = new System.Windows.Forms.StatusStrip();
            this.statusSaveLoc = new System.Windows.Forms.ToolStripStatusLabel();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tabControlMain = new System.Windows.Forms.TabControl();
            this.tabPageProp = new System.Windows.Forms.TabPage();
            this.propertyGridRes = new System.Windows.Forms.PropertyGrid();
            this.tabPageMtl = new System.Windows.Forms.TabPage();
            this.buttonApplyMtlProperty = new System.Windows.Forms.Button();
            this.propertyGridMtl = new System.Windows.Forms.PropertyGrid();
            this.btnCreateMtl = new System.Windows.Forms.Button();
            this.comboBoxSection = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBoxLOD = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.imageListTab = new System.Windows.Forms.ImageList(this.components);
            this.timerViewUpdate = new System.Windows.Forms.Timer(this.components);
            this.menuStripMain.SuspendLayout();
            this.toolStripMain.SuspendLayout();
            this.statusStripMain.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tabControlMain.SuspendLayout();
            this.tabPageProp.SuspendLayout();
            this.tabPageMtl.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStripMain
            // 
            this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem});
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.Size = new System.Drawing.Size(780, 24);
            this.menuStripMain.TabIndex = 0;
            this.menuStripMain.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.importToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.toolStripMenuItem1,
            this.resetToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // importToolStripMenuItem
            // 
            this.importToolStripMenuItem.Name = "importToolStripMenuItem";
            this.importToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.importToolStripMenuItem.Text = "Import...";
            this.importToolStripMenuItem.Click += new System.EventHandler(this.importToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.saveToolStripMenuItem.Text = "Save ...";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(115, 6);
            // 
            // resetToolStripMenuItem
            // 
            this.resetToolStripMenuItem.Name = "resetToolStripMenuItem";
            this.resetToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.resetToolStripMenuItem.Text = "Reset";
            this.resetToolStripMenuItem.Click += new System.EventHandler(this.resetToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.importDefautsToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.toolsToolStripMenuItem.Text = "Tools";
            // 
            // importDefautsToolStripMenuItem
            // 
            this.importDefautsToolStripMenuItem.Name = "importDefautsToolStripMenuItem";
            this.importDefautsToolStripMenuItem.Size = new System.Drawing.Size(111, 22);
            this.importDefautsToolStripMenuItem.Text = "Options";
            this.importDefautsToolStripMenuItem.ToolTipText = "Some Default Values when Import LOD";
            this.importDefautsToolStripMenuItem.Click += new System.EventHandler(this.importDefautsToolStripMenuItem_Click);
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
            this.toolBtnZoomExtents,
            this.toolStripSeparator2,
            this.toolBtnXML});
            this.toolStripMain.Location = new System.Drawing.Point(0, 24);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(780, 36);
            this.toolStripMain.TabIndex = 1;
            this.toolStripMain.Text = "toolStrip1";
            // 
            // toolStripButtonRest
            // 
            this.toolStripButtonRest.Image = global::NexusEditor.Properties.Resources.Reset;
            this.toolStripButtonRest.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonRest.Name = "toolStripButtonRest";
            this.toolStripButtonRest.Size = new System.Drawing.Size(57, 33);
            this.toolStripButtonRest.Text = "   Reset   ";
            this.toolStripButtonRest.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonRest.Click += new System.EventHandler(this.resetToolStripMenuItem_Click);
            // 
            // toolStripButtonNew
            // 
            this.toolStripButtonNew.Image = global::NexusEditor.Properties.Resources.white_picture;
            this.toolStripButtonNew.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonNew.Name = "toolStripButtonNew";
            this.toolStripButtonNew.Size = new System.Drawing.Size(87, 33);
            this.toolStripButtonNew.Text = "New StaticMesh";
            this.toolStripButtonNew.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonNew.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // toolStripButtonSave
            // 
            this.toolStripButtonSave.Image = global::NexusEditor.Properties.Resources.Save;
            this.toolStripButtonSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSave.Name = "toolStripButtonSave";
            this.toolStripButtonSave.Size = new System.Drawing.Size(67, 33);
            this.toolStripButtonSave.Text = "Save to File";
            this.toolStripButtonSave.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonSave.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // toolStripButtonImport
            // 
            this.toolStripButtonImport.Image = global::NexusEditor.Properties.Resources.connect;
            this.toolStripButtonImport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonImport.Name = "toolStripButtonImport";
            this.toolStripButtonImport.Size = new System.Drawing.Size(66, 33);
            this.toolStripButtonImport.Text = "Import LOD";
            this.toolStripButtonImport.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonImport.Click += new System.EventHandler(this.importToolStripMenuItem_Click);
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
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 36);
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
            // 
            // statusStripMain
            // 
            this.statusStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusSaveLoc});
            this.statusStripMain.Location = new System.Drawing.Point(0, 532);
            this.statusStripMain.Name = "statusStripMain";
            this.statusStripMain.Size = new System.Drawing.Size(780, 22);
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
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 60);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.tabControlMain);
            this.splitContainer1.Size = new System.Drawing.Size(780, 472);
            this.splitContainer1.SplitterDistance = 520;
            this.splitContainer1.TabIndex = 3;
            // 
            // tabControlMain
            // 
            this.tabControlMain.Controls.Add(this.tabPageProp);
            this.tabControlMain.Controls.Add(this.tabPageMtl);
            this.tabControlMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlMain.ImageList = this.imageListTab;
            this.tabControlMain.Location = new System.Drawing.Point(0, 0);
            this.tabControlMain.Name = "tabControlMain";
            this.tabControlMain.SelectedIndex = 0;
            this.tabControlMain.Size = new System.Drawing.Size(256, 472);
            this.tabControlMain.TabIndex = 0;
            // 
            // tabPageProp
            // 
            this.tabPageProp.Controls.Add(this.propertyGridRes);
            this.tabPageProp.ImageKey = "Prop";
            this.tabPageProp.Location = new System.Drawing.Point(4, 23);
            this.tabPageProp.Name = "tabPageProp";
            this.tabPageProp.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageProp.Size = new System.Drawing.Size(248, 445);
            this.tabPageProp.TabIndex = 1;
            this.tabPageProp.Text = "Resource";
            this.tabPageProp.UseVisualStyleBackColor = true;
            // 
            // propertyGridRes
            // 
            this.propertyGridRes.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridRes.Location = new System.Drawing.Point(3, 3);
            this.propertyGridRes.Name = "propertyGridRes";
            this.propertyGridRes.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.propertyGridRes.Size = new System.Drawing.Size(242, 439);
            this.propertyGridRes.TabIndex = 0;
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
            this.tabPageMtl.ImageKey = "Material";
            this.tabPageMtl.Location = new System.Drawing.Point(4, 23);
            this.tabPageMtl.Name = "tabPageMtl";
            this.tabPageMtl.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageMtl.Size = new System.Drawing.Size(248, 445);
            this.tabPageMtl.TabIndex = 2;
            this.tabPageMtl.Text = "Material";
            this.tabPageMtl.UseVisualStyleBackColor = true;
            // 
            // buttonApplyMtlProperty
            // 
            this.buttonApplyMtlProperty.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.buttonApplyMtlProperty.Location = new System.Drawing.Point(3, 396);
            this.buttonApplyMtlProperty.Name = "buttonApplyMtlProperty";
            this.buttonApplyMtlProperty.Size = new System.Drawing.Size(242, 23);
            this.buttonApplyMtlProperty.TabIndex = 5;
            this.buttonApplyMtlProperty.Text = "Apply";
            this.buttonApplyMtlProperty.UseVisualStyleBackColor = true;
            this.buttonApplyMtlProperty.Click += new System.EventHandler(this.buttonApplyMtlProperty_Click);
            // 
            // propertyGridMtl
            // 
            this.propertyGridMtl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridMtl.Location = new System.Drawing.Point(3, 67);
            this.propertyGridMtl.Name = "propertyGridMtl";
            this.propertyGridMtl.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.propertyGridMtl.Size = new System.Drawing.Size(242, 352);
            this.propertyGridMtl.TabIndex = 3;
            this.propertyGridMtl.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGridMtl_PropertyValueChanged);
            // 
            // btnCreateMtl
            // 
            this.btnCreateMtl.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.btnCreateMtl.Location = new System.Drawing.Point(3, 419);
            this.btnCreateMtl.Margin = new System.Windows.Forms.Padding(16, 3, 16, 3);
            this.btnCreateMtl.Name = "btnCreateMtl";
            this.btnCreateMtl.Size = new System.Drawing.Size(242, 23);
            this.btnCreateMtl.TabIndex = 2;
            this.btnCreateMtl.Text = "Create";
            this.btnCreateMtl.UseVisualStyleBackColor = true;
            this.btnCreateMtl.Click += new System.EventHandler(this.btnCreateMtl_Click);
            // 
            // comboBoxSection
            // 
            this.comboBoxSection.Dock = System.Windows.Forms.DockStyle.Top;
            this.comboBoxSection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxSection.FormattingEnabled = true;
            this.comboBoxSection.Location = new System.Drawing.Point(3, 47);
            this.comboBoxSection.Name = "comboBoxSection";
            this.comboBoxSection.Size = new System.Drawing.Size(242, 20);
            this.comboBoxSection.TabIndex = 1;
            this.comboBoxSection.SelectedIndexChanged += new System.EventHandler(this.comboBoxSection_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Top;
            this.label2.Location = new System.Drawing.Point(3, 35);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 12);
            this.label2.TabIndex = 0;
            this.label2.Text = "Section";
            // 
            // comboBoxLOD
            // 
            this.comboBoxLOD.Dock = System.Windows.Forms.DockStyle.Top;
            this.comboBoxLOD.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxLOD.FormattingEnabled = true;
            this.comboBoxLOD.Location = new System.Drawing.Point(3, 15);
            this.comboBoxLOD.Name = "comboBoxLOD";
            this.comboBoxLOD.Size = new System.Drawing.Size(242, 20);
            this.comboBoxLOD.TabIndex = 1;
            this.comboBoxLOD.SelectedIndexChanged += new System.EventHandler(this.comboBoxLOD_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Location = new System.Drawing.Point(3, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(23, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "LOD";
            // 
            // imageListTab
            // 
            this.imageListTab.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListTab.ImageStream")));
            this.imageListTab.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListTab.Images.SetKeyName(0, "Prop");
            this.imageListTab.Images.SetKeyName(1, "Material");
            // 
            // timerViewUpdate
            // 
            this.timerViewUpdate.Enabled = true;
            this.timerViewUpdate.Interval = 20;
            this.timerViewUpdate.Tick += new System.EventHandler(this.timerViewUpdate_Tick);
            // 
            // ResStaticMeshEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(780, 554);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.statusStripMain);
            this.Controls.Add(this.toolStripMain);
            this.Controls.Add(this.menuStripMain);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStripMain;
            this.Name = "ResStaticMeshEditor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Nexus Static Mesh Editor";
            this.Load += new System.EventHandler(this.ResStaticMeshEditor_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ResStaticMeshEditor_FormClosing);
            this.menuStripMain.ResumeLayout(false);
            this.menuStripMain.PerformLayout();
            this.toolStripMain.ResumeLayout(false);
            this.toolStripMain.PerformLayout();
            this.statusStripMain.ResumeLayout(false);
            this.statusStripMain.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.tabControlMain.ResumeLayout(false);
            this.tabPageProp.ResumeLayout(false);
            this.tabPageMtl.ResumeLayout(false);
            this.tabPageMtl.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStripMain;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.StatusStrip statusStripMain;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TabControl tabControlMain;
        private System.Windows.Forms.TabPage tabPageProp;
        private System.Windows.Forms.PropertyGrid propertyGridRes;
        private System.Windows.Forms.ToolStripMenuItem importToolStripMenuItem;
        private System.Windows.Forms.TabPage tabPageMtl;
        private System.Windows.Forms.ComboBox comboBoxLOD;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PropertyGrid propertyGridMtl;
        private System.Windows.Forms.ComboBox comboBoxSection;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem resetToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButtonSave;
        private System.Windows.Forms.ToolStripButton toolStripButtonImport;
        private System.Windows.Forms.ToolStripStatusLabel statusSaveLoc;
        private System.Windows.Forms.ToolStripButton toolStripButtonNew;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importDefautsToolStripMenuItem;
        private System.Windows.Forms.Button buttonApplyMtlProperty;
        private System.Windows.Forms.Button btnCreateMtl;
        private System.Windows.Forms.ToolStripButton toolStripButtonRest;
        private System.Windows.Forms.ImageList imageListTab;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolBtnLit;
        private System.Windows.Forms.ToolStripButton toolBtnUnlit;
        private System.Windows.Forms.ToolStripButton toolBtnWireframe;
        private System.Windows.Forms.ToolStripButton toolBtnZoomExtents;
        private System.Windows.Forms.Timer timerViewUpdate;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton toolBtnXML;
    }
}