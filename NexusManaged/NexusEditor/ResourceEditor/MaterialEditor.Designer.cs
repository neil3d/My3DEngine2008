namespace NexusEditor.ResourceEditor
{
	partial class MaterialEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MaterialEditor));
            this.statusStripMain = new System.Windows.Forms.StatusStrip();
            this.statusSaveLoc = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton4 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSave = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolBtnLit = new System.Windows.Forms.ToolStripButton();
            this.toolBtnUnlit = new System.Windows.Forms.ToolStripButton();
            this.toolBtnWireframe = new System.Windows.Forms.ToolStripButton();
            this.toolBtnZoomExtents = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
            this.realTimeMode = new System.Windows.Forms.ToolStripButton();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.propertyGridMtl = new System.Windows.Forms.PropertyGrid();
            this.btnCreateMtl = new System.Windows.Forms.Button();
            this.timerViewUpdate = new System.Windows.Forms.Timer(this.components);
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStripMain.SuspendLayout();
            this.toolStripMain.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStripMain
            // 
            this.statusStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusSaveLoc});
            this.statusStripMain.Location = new System.Drawing.Point(0, 496);
            this.statusStripMain.Name = "statusStripMain";
            this.statusStripMain.Size = new System.Drawing.Size(840, 22);
            this.statusStripMain.TabIndex = 5;
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
            // toolStripMain
            // 
            this.toolStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1,
            this.toolStripButton2,
            this.toolStripButton4,
            this.toolStripButtonSave,
            this.toolStripSeparator1,
            this.toolBtnLit,
            this.toolBtnUnlit,
            this.toolBtnWireframe,
            this.toolBtnZoomExtents,
            this.toolStripSeparator3,
            this.toolStripButton3,
            this.realTimeMode});
            this.toolStripMain.Location = new System.Drawing.Point(0, 25);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(840, 40);
            this.toolStripMain.TabIndex = 6;
            this.toolStripMain.Text = "toolStrip1";
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(36, 37);
            this.toolStripButton1.Text = "新建";
            this.toolStripButton1.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButton1.Click += new System.EventHandler(this.toolStripButton1_Click);
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton2.Image")));
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(36, 37);
            this.toolStripButton2.Text = "打开";
            this.toolStripButton2.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButton2.ToolTipText = "打开材质文件";
            this.toolStripButton2.Click += new System.EventHandler(this.toolStripButton2_Click);
            // 
            // toolStripButton4
            // 
            this.toolStripButton4.Image = global::NexusEditor.Properties.Resources.Save;
            this.toolStripButton4.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton4.Name = "toolStripButton4";
            this.toolStripButton4.Size = new System.Drawing.Size(36, 37);
            this.toolStripButton4.Text = "保存";
            this.toolStripButton4.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButton4.ToolTipText = "保存到文件";
            this.toolStripButton4.Click += new System.EventHandler(this.toolStripButton4_Click);
            // 
            // toolStripButtonSave
            // 
            this.toolStripButtonSave.Image = global::NexusEditor.Properties.Resources.Save;
            this.toolStripButtonSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSave.Name = "toolStripButtonSave";
            this.toolStripButtonSave.Size = new System.Drawing.Size(48, 37);
            this.toolStripButtonSave.Text = "另存为";
            this.toolStripButtonSave.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonSave.ToolTipText = "另存到文件";
            this.toolStripButtonSave.Click += new System.EventHandler(this.toolStripButtonSave_Click);
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
            // toolStripButton3
            // 
            this.toolStripButton3.Image = global::NexusEditor.Properties.Resources.move;
            this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton3.Name = "toolStripButton3";
            this.toolStripButton3.Size = new System.Drawing.Size(60, 37);
            this.toolStripButton3.Text = "预览模型";
            this.toolStripButton3.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButton3.ToolTipText = "使用选中的静态模型预览材质";
            this.toolStripButton3.Click += new System.EventHandler(this.toolStripButton3_Click);
            // 
            // realTimeMode
            // 
            this.realTimeMode.CheckOnClick = true;
            this.realTimeMode.Image = global::NexusEditor.Properties.Resources.action_go;
            this.realTimeMode.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.realTimeMode.Name = "realTimeMode";
            this.realTimeMode.Size = new System.Drawing.Size(60, 37);
            this.realTimeMode.Text = "实时模式";
            this.realTimeMode.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.realTimeMode.Click += new System.EventHandler(this.realTimeMode_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 65);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.propertyGridMtl);
            this.splitContainer1.Panel2.Controls.Add(this.btnCreateMtl);
            this.splitContainer1.Size = new System.Drawing.Size(840, 431);
            this.splitContainer1.SplitterDistance = 560;
            this.splitContainer1.TabIndex = 7;
            // 
            // propertyGridMtl
            // 
            this.propertyGridMtl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridMtl.Location = new System.Drawing.Point(0, 0);
            this.propertyGridMtl.Name = "propertyGridMtl";
            this.propertyGridMtl.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.propertyGridMtl.Size = new System.Drawing.Size(276, 408);
            this.propertyGridMtl.TabIndex = 11;
            this.propertyGridMtl.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGridMtl_PropertyValueChanged);
            // 
            // btnCreateMtl
            // 
            this.btnCreateMtl.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.btnCreateMtl.Location = new System.Drawing.Point(0, 408);
            this.btnCreateMtl.Margin = new System.Windows.Forms.Padding(16, 3, 16, 3);
            this.btnCreateMtl.Name = "btnCreateMtl";
            this.btnCreateMtl.Size = new System.Drawing.Size(276, 23);
            this.btnCreateMtl.TabIndex = 10;
            this.btnCreateMtl.Text = "Create";
            this.btnCreateMtl.UseVisualStyleBackColor = true;
            this.btnCreateMtl.Click += new System.EventHandler(this.btnCreateMtl_Click);
            // 
            // timerViewUpdate
            // 
            this.timerViewUpdate.Enabled = true;
            this.timerViewUpdate.Interval = 20;
            this.timerViewUpdate.Tick += new System.EventHandler(this.timerViewUpdate_Tick);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(840, 25);
            this.menuStrip1.TabIndex = 8;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(39, 21);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.newToolStripMenuItem.Text = "New";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openToolStripMenuItem.Text = "Open";
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveToolStripMenuItem.Text = "Save";
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveAsToolStripMenuItem.Text = "Save As ...";
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.optionToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(52, 21);
            this.toolsToolStripMenuItem.Text = "Tools";
            // 
            // optionToolStripMenuItem
            // 
            this.optionToolStripMenuItem.Name = "optionToolStripMenuItem";
            this.optionToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.optionToolStripMenuItem.Text = "Option";
            // 
            // MaterialEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(840, 518);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.toolStripMain);
            this.Controls.Add(this.statusStripMain);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MaterialEditor";
            this.Text = "材质编辑器";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MaterialEditor_FormClosing);
            this.statusStripMain.ResumeLayout(false);
            this.statusStripMain.PerformLayout();
            this.toolStripMain.ResumeLayout(false);
            this.toolStripMain.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.StatusStrip statusStripMain;
		private System.Windows.Forms.ToolStripStatusLabel statusSaveLoc;
		private System.Windows.Forms.ToolStrip toolStripMain;
		private System.Windows.Forms.ToolStripButton toolStripButtonSave;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.PropertyGrid propertyGridMtl;
		private System.Windows.Forms.Button btnCreateMtl;
		private System.Windows.Forms.Timer timerViewUpdate;
		private System.Windows.Forms.ToolStripButton toolStripButton1;
		private System.Windows.Forms.ToolStripButton toolStripButton2;
		private System.Windows.Forms.ToolStripButton toolBtnLit;
		private System.Windows.Forms.ToolStripButton toolBtnUnlit;
		private System.Windows.Forms.ToolStripButton toolBtnWireframe;
		private System.Windows.Forms.ToolStripButton toolBtnZoomExtents;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
		private System.Windows.Forms.ToolStripButton toolStripButton3;
		private System.Windows.Forms.ToolStripButton realTimeMode;
		private System.Windows.Forms.ToolStripButton toolStripButton4;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionToolStripMenuItem;

	}
}