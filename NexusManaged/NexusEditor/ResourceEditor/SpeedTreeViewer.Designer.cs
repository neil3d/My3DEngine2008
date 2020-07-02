namespace NexusEditor.ResourceEditor
{
    partial class SpeedTreeViewer
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SpeedTreeViewer));
            this.menuMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripTextBoxLOD = new System.Windows.Forms.ToolStripTextBox();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.propertyGridSpt = new System.Windows.Forms.PropertyGrid();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.toolBtnLit = new System.Windows.Forms.ToolStripButton();
            this.toolBtnUnlit = new System.Windows.Forms.ToolStripButton();
            this.toolBtnZoomExtents = new System.Windows.Forms.ToolStripButton();
            this.toolBtnWireframe = new System.Windows.Forms.ToolStripButton();
            this.menuMain.SuspendLayout();
            this.toolStripMain.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuMain
            // 
            this.menuMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuMain.Location = new System.Drawing.Point(0, 0);
            this.menuMain.Name = "menuMain";
            this.menuMain.Size = new System.Drawing.Size(774, 25);
            this.menuMain.TabIndex = 0;
            this.menuMain.Text = "menuMain";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.toolStripMenuItem1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(39, 21);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(105, 22);
            this.loadToolStripMenuItem.Text = "Load";
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(102, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(105, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // toolStripMain
            // 
            this.toolStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolBtnLit,
            this.toolBtnUnlit,
            this.toolBtnWireframe,
            this.toolBtnZoomExtents,
            this.toolStripSeparator1,
            this.toolStripLabel1,
            this.toolStripTextBoxLOD});
            this.toolStripMain.Location = new System.Drawing.Point(0, 25);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(774, 40);
            this.toolStripMain.TabIndex = 1;
            this.toolStripMain.Text = "toolStripMain";
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(61, 37);
            this.toolStripLabel1.Text = "LOD Bias";
            // 
            // toolStripTextBoxLOD
            // 
            this.toolStripTextBoxLOD.Name = "toolStripTextBoxLOD";
            this.toolStripTextBoxLOD.Size = new System.Drawing.Size(100, 40);
            this.toolStripTextBoxLOD.Text = "0.0";
            this.toolStripTextBoxLOD.Leave += new System.EventHandler(this.toolStripTextBoxLOD_Leave);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 40);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 514);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(774, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 65);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.propertyGridSpt);
            this.splitContainer1.Size = new System.Drawing.Size(774, 449);
            this.splitContainer1.SplitterDistance = 580;
            this.splitContainer1.TabIndex = 3;
            // 
            // propertyGridSpt
            // 
            this.propertyGridSpt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridSpt.Location = new System.Drawing.Point(0, 0);
            this.propertyGridSpt.Name = "propertyGridSpt";
            this.propertyGridSpt.Size = new System.Drawing.Size(190, 449);
            this.propertyGridSpt.TabIndex = 0;
            // 
            // timer1
            // 
            this.timer1.Interval = 20;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
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
            // SpeedTreeViewer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(774, 536);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.toolStripMain);
            this.Controls.Add(this.menuMain);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuMain;
            this.Name = "SpeedTreeViewer";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Nexus SpeedTree Viewer";
            this.VisibleChanged += new System.EventHandler(this.SpeedTreeViewer_VisibleChanged);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SpeedTreeViewer_FormClosing);
            this.menuMain.ResumeLayout(false);
            this.menuMain.PerformLayout();
            this.toolStripMain.ResumeLayout(false);
            this.toolStripMain.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuMain;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.PropertyGrid propertyGridSpt;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBoxLOD;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolBtnLit;
        private System.Windows.Forms.ToolStripButton toolBtnUnlit;
        private System.Windows.Forms.ToolStripButton toolBtnZoomExtents;
        private System.Windows.Forms.ToolStripButton toolBtnWireframe;
    }
}