namespace NexusEditor.ResourceEditor
{
    partial class ResSFXEditor
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
			System.Windows.Forms.Label label1;
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ResSFXEditor));
			this.menuStripMain = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.newNToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openOToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveSToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMain = new System.Windows.Forms.ToolStrip();
			this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.statusStripMain = new System.Windows.Forms.StatusStrip();
			this.splitContainerH = new System.Windows.Forms.SplitContainer();
			this.splitContainerV = new System.Windows.Forms.SplitContainer();
			this.contextMenuStripCreate = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.groupBoxObject = new System.Windows.Forms.GroupBox();
			this.propertyGridDist = new System.Windows.Forms.PropertyGrid();
			this.btnResetPropertyType = new System.Windows.Forms.Button();
			this.comboBoxType = new System.Windows.Forms.ComboBox();
			this.groupBoxCurve = new System.Windows.Forms.GroupBox();
			this.m_Timeline = new NexusEditor.Timeline();
			this.timerRender = new System.Windows.Forms.Timer(this.components);
			this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
			label1 = new System.Windows.Forms.Label();
			this.menuStripMain.SuspendLayout();
			this.toolStripMain.SuspendLayout();
			this.statusStripMain.SuspendLayout();
			this.splitContainerH.Panel1.SuspendLayout();
			this.splitContainerH.Panel2.SuspendLayout();
			this.splitContainerH.SuspendLayout();
			this.splitContainerV.SuspendLayout();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.groupBoxObject.SuspendLayout();
			this.groupBoxCurve.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			label1.AutoSize = true;
			label1.Location = new System.Drawing.Point(7, 25);
			label1.Name = "label1";
			label1.Size = new System.Drawing.Size(29, 12);
			label1.TabIndex = 0;
			label1.Text = "Type";
			// 
			// menuStripMain
			// 
			this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
			this.menuStripMain.Location = new System.Drawing.Point(0, 0);
			this.menuStripMain.Name = "menuStripMain";
			this.menuStripMain.Size = new System.Drawing.Size(980, 25);
			this.menuStripMain.TabIndex = 0;
			this.menuStripMain.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newNToolStripMenuItem,
            this.openOToolStripMenuItem,
            this.saveSToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(39, 21);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// newNToolStripMenuItem
			// 
			this.newNToolStripMenuItem.Name = "newNToolStripMenuItem";
			this.newNToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
			this.newNToolStripMenuItem.Text = "New(&N)";
			this.newNToolStripMenuItem.Click += new System.EventHandler(this.newNToolStripMenuItem_Click);
			// 
			// openOToolStripMenuItem
			// 
			this.openOToolStripMenuItem.Name = "openOToolStripMenuItem";
			this.openOToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
			this.openOToolStripMenuItem.Text = "Open(&O)";
			this.openOToolStripMenuItem.Click += new System.EventHandler(this.openOToolStripMenuItem_Click);
			// 
			// saveSToolStripMenuItem
			// 
			this.saveSToolStripMenuItem.Name = "saveSToolStripMenuItem";
			this.saveSToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
			this.saveSToolStripMenuItem.Text = "Save(&S)";
			this.saveSToolStripMenuItem.Click += new System.EventHandler(this.saveSToolStripMenuItem_Click);
			// 
			// toolStripMain
			// 
			this.toolStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1,
            this.toolStripSeparator1});
			this.toolStripMain.Location = new System.Drawing.Point(0, 25);
			this.toolStripMain.Name = "toolStripMain";
			this.toolStripMain.Size = new System.Drawing.Size(980, 25);
			this.toolStripMain.TabIndex = 1;
			this.toolStripMain.Text = "toolStrip1";
			// 
			// toolStripButton1
			// 
			this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
			this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButton1.Name = "toolStripButton1";
			this.toolStripButton1.Size = new System.Drawing.Size(23, 22);
			this.toolStripButton1.Text = "toolStripButton1";
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
			// 
			// statusStripMain
			// 
			this.statusStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
			this.statusStripMain.Location = new System.Drawing.Point(0, 672);
			this.statusStripMain.Name = "statusStripMain";
			this.statusStripMain.Size = new System.Drawing.Size(980, 22);
			this.statusStripMain.TabIndex = 2;
			this.statusStripMain.Text = "statusStrip1";
			// 
			// splitContainerH
			// 
			this.splitContainerH.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.splitContainerH.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainerH.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
			this.splitContainerH.Location = new System.Drawing.Point(0, 50);
			this.splitContainerH.Name = "splitContainerH";
			this.splitContainerH.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainerH.Panel1
			// 
			this.splitContainerH.Panel1.Controls.Add(this.splitContainerV);
			this.splitContainerH.Panel1.RightToLeft = System.Windows.Forms.RightToLeft.No;
			// 
			// splitContainerH.Panel2
			// 
			this.splitContainerH.Panel2.Controls.Add(this.splitContainer1);
			this.splitContainerH.Panel2.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.splitContainerH.Panel2.SizeChanged += new System.EventHandler(this.splitContainerH_Panel2_SizeChanged);
			this.splitContainerH.Size = new System.Drawing.Size(980, 622);
			this.splitContainerH.SplitterDistance = 323;
			this.splitContainerH.TabIndex = 3;
			// 
			// splitContainerV
			// 
			this.splitContainerV.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.splitContainerV.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainerV.Location = new System.Drawing.Point(0, 0);
			this.splitContainerV.Name = "splitContainerV";
			this.splitContainerV.Panel1MinSize = 240;
			// 
			// splitContainerV.Panel2
			// 
			this.splitContainerV.Panel2.AutoScroll = true;
			this.splitContainerV.Panel2.ContextMenuStrip = this.contextMenuStripCreate;
			this.splitContainerV.Panel2.SizeChanged += new System.EventHandler(this.splitContainerV_Panel2_SizeChanged);
			this.splitContainerV.Size = new System.Drawing.Size(976, 319);
			this.splitContainerV.SplitterDistance = 438;
			this.splitContainerV.TabIndex = 0;
			// 
			// contextMenuStripCreate
			// 
			this.contextMenuStripCreate.Name = "contextMenuStripCreate";
			this.contextMenuStripCreate.Size = new System.Drawing.Size(61, 4);
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.splitContainer1.IsSplitterFixed = true;
			this.splitContainer1.Location = new System.Drawing.Point(0, 0);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.groupBoxObject);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.groupBoxCurve);
			this.splitContainer1.Size = new System.Drawing.Size(976, 291);
			this.splitContainer1.SplitterDistance = 316;
			this.splitContainer1.TabIndex = 2;
			// 
			// groupBoxObject
			// 
			this.groupBoxObject.Controls.Add(this.propertyGridDist);
			this.groupBoxObject.Controls.Add(this.btnResetPropertyType);
			this.groupBoxObject.Controls.Add(this.comboBoxType);
			this.groupBoxObject.Controls.Add(label1);
			this.groupBoxObject.Dock = System.Windows.Forms.DockStyle.Fill;
			this.groupBoxObject.Location = new System.Drawing.Point(0, 0);
			this.groupBoxObject.Name = "groupBoxObject";
			this.groupBoxObject.Size = new System.Drawing.Size(316, 291);
			this.groupBoxObject.TabIndex = 1;
			this.groupBoxObject.TabStop = false;
			this.groupBoxObject.Text = "Distribution Object";
			// 
			// propertyGridDist
			// 
			this.propertyGridDist.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)));
			this.propertyGridDist.HelpVisible = false;
			this.propertyGridDist.Location = new System.Drawing.Point(9, 50);
			this.propertyGridDist.Name = "propertyGridDist";
			this.propertyGridDist.Size = new System.Drawing.Size(301, 235);
			this.propertyGridDist.TabIndex = 3;
			this.propertyGridDist.ToolbarVisible = false;
			// 
			// btnResetPropertyType
			// 
			this.btnResetPropertyType.Location = new System.Drawing.Point(261, 18);
			this.btnResetPropertyType.Name = "btnResetPropertyType";
			this.btnResetPropertyType.Size = new System.Drawing.Size(49, 23);
			this.btnResetPropertyType.TabIndex = 2;
			this.btnResetPropertyType.Text = "reset";
			this.btnResetPropertyType.UseVisualStyleBackColor = true;
			this.btnResetPropertyType.Click += new System.EventHandler(this.btnResetPropertyType_Click);
			// 
			// comboBoxType
			// 
			this.comboBoxType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxType.FormattingEnabled = true;
			this.comboBoxType.Location = new System.Drawing.Point(42, 21);
			this.comboBoxType.Name = "comboBoxType";
			this.comboBoxType.Size = new System.Drawing.Size(213, 20);
			this.comboBoxType.TabIndex = 1;
			// 
			// groupBoxCurve
			// 
			this.groupBoxCurve.Controls.Add(this.m_Timeline);
			this.groupBoxCurve.Dock = System.Windows.Forms.DockStyle.Fill;
			this.groupBoxCurve.Location = new System.Drawing.Point(0, 0);
			this.groupBoxCurve.Name = "groupBoxCurve";
			this.groupBoxCurve.Size = new System.Drawing.Size(656, 291);
			this.groupBoxCurve.TabIndex = 2;
			this.groupBoxCurve.TabStop = false;
			this.groupBoxCurve.Text = "Curve Editor";
			// 
			// m_Timeline
			// 
			this.m_Timeline.AutoScroll = true;
			this.m_Timeline.AutoSize = true;
			this.m_Timeline.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
			this.m_Timeline.Dock = System.Windows.Forms.DockStyle.Fill;
			this.m_Timeline.Location = new System.Drawing.Point(3, 17);
			this.m_Timeline.Name = "m_Timeline";
			this.m_Timeline.Size = new System.Drawing.Size(650, 271);
			this.m_Timeline.TabIndex = 0;
			// 
			// timerRender
			// 
			this.timerRender.Enabled = true;
			this.timerRender.Interval = 20;
			this.timerRender.Tick += new System.EventHandler(this.timerRender_Tick);
			// 
			// toolStripStatusLabel1
			// 
			this.toolStripStatusLabel1.AutoSize = false;
			this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
			this.toolStripStatusLabel1.Size = new System.Drawing.Size(180, 17);
			// 
			// ResSFXEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(980, 694);
			this.Controls.Add(this.splitContainerH);
			this.Controls.Add(this.statusStripMain);
			this.Controls.Add(this.toolStripMain);
			this.Controls.Add(this.menuStripMain);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuStripMain;
			this.Name = "ResSFXEditor";
			this.Text = "Nexus Special Effect Editor";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ResSFXEditor_FormClosing);
			this.menuStripMain.ResumeLayout(false);
			this.menuStripMain.PerformLayout();
			this.toolStripMain.ResumeLayout(false);
			this.toolStripMain.PerformLayout();
			this.statusStripMain.ResumeLayout(false);
			this.statusStripMain.PerformLayout();
			this.splitContainerH.Panel1.ResumeLayout(false);
			this.splitContainerH.Panel2.ResumeLayout(false);
			this.splitContainerH.ResumeLayout(false);
			this.splitContainerV.ResumeLayout(false);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.ResumeLayout(false);
			this.groupBoxObject.ResumeLayout(false);
			this.groupBoxObject.PerformLayout();
			this.groupBoxCurve.ResumeLayout(false);
			this.groupBoxCurve.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStripMain;
        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.StatusStrip statusStripMain;
        private System.Windows.Forms.SplitContainer splitContainerH;
        private System.Windows.Forms.SplitContainer splitContainerV;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripCreate;
        private System.Windows.Forms.Timer timerRender;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.GroupBox groupBoxObject;
        private System.Windows.Forms.PropertyGrid propertyGridDist;
        private System.Windows.Forms.Button btnResetPropertyType;
        private System.Windows.Forms.ComboBox comboBoxType;
        private System.Windows.Forms.GroupBox groupBoxCurve;
        private Timeline m_Timeline;
        private System.Windows.Forms.ToolStripMenuItem newNToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openOToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveSToolStripMenuItem;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
    }
}