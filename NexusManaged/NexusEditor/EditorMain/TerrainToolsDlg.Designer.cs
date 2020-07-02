namespace NexusEditor.EditorMain
{
    partial class TerrainToolsDlg
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
            this.groupBoxHmapTools = new System.Windows.Forms.GroupBox();
            this.btnHMapExport = new System.Windows.Forms.Button();
            this.btnImportHmap = new System.Windows.Forms.Button();
            this.btnBrowseHmapFile = new System.Windows.Forms.Button();
            this.textBoxHMapFile = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.buttonClose = new System.Windows.Forms.Button();
            this.groupBoxHmapTools.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxHmapTools
            // 
            this.groupBoxHmapTools.Controls.Add(this.btnHMapExport);
            this.groupBoxHmapTools.Controls.Add(this.btnImportHmap);
            this.groupBoxHmapTools.Controls.Add(this.btnBrowseHmapFile);
            this.groupBoxHmapTools.Controls.Add(this.textBoxHMapFile);
            this.groupBoxHmapTools.Controls.Add(this.label8);
            this.groupBoxHmapTools.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBoxHmapTools.Location = new System.Drawing.Point(0, 0);
            this.groupBoxHmapTools.Name = "groupBoxHmapTools";
            this.groupBoxHmapTools.Size = new System.Drawing.Size(366, 79);
            this.groupBoxHmapTools.TabIndex = 1;
            this.groupBoxHmapTools.TabStop = false;
            this.groupBoxHmapTools.Text = "Heightmap";
            // 
            // btnHMapExport
            // 
            this.btnHMapExport.Enabled = false;
            this.btnHMapExport.Location = new System.Drawing.Point(184, 44);
            this.btnHMapExport.Name = "btnHMapExport";
            this.btnHMapExport.Size = new System.Drawing.Size(75, 23);
            this.btnHMapExport.TabIndex = 3;
            this.btnHMapExport.Text = "Export";
            this.btnHMapExport.UseVisualStyleBackColor = true;
            this.btnHMapExport.Click += new System.EventHandler(this.btnHMapExport_Click);
            // 
            // btnImportHmap
            // 
            this.btnImportHmap.Location = new System.Drawing.Point(103, 44);
            this.btnImportHmap.Name = "btnImportHmap";
            this.btnImportHmap.Size = new System.Drawing.Size(75, 23);
            this.btnImportHmap.TabIndex = 3;
            this.btnImportHmap.Text = "Import";
            this.btnImportHmap.UseVisualStyleBackColor = true;
            this.btnImportHmap.Click += new System.EventHandler(this.btnImportHmap_Click);
            // 
            // btnBrowseHmapFile
            // 
            this.btnBrowseHmapFile.Location = new System.Drawing.Point(321, 17);
            this.btnBrowseHmapFile.Name = "btnBrowseHmapFile";
            this.btnBrowseHmapFile.Size = new System.Drawing.Size(35, 23);
            this.btnBrowseHmapFile.TabIndex = 2;
            this.btnBrowseHmapFile.Text = "...";
            this.btnBrowseHmapFile.UseVisualStyleBackColor = true;
            this.btnBrowseHmapFile.Click += new System.EventHandler(this.btnBrowseHmapFile_Click);
            // 
            // textBoxHMapFile
            // 
            this.textBoxHMapFile.Location = new System.Drawing.Point(68, 18);
            this.textBoxHMapFile.Name = "textBoxHMapFile";
            this.textBoxHMapFile.Size = new System.Drawing.Size(247, 21);
            this.textBoxHMapFile.TabIndex = 1;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(3, 22);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(59, 12);
            this.label8.TabIndex = 0;
            this.label8.Text = "File Name";
            // 
            // buttonClose
            // 
            this.buttonClose.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonClose.Location = new System.Drawing.Point(146, 229);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(75, 23);
            this.buttonClose.TabIndex = 14;
            this.buttonClose.Text = "Close";
            this.buttonClose.UseVisualStyleBackColor = true;
            // 
            // TerrainToolsDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(366, 257);
            this.Controls.Add(this.buttonClose);
            this.Controls.Add(this.groupBoxHmapTools);
            this.Name = "TerrainToolsDlg";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Terrain Tools";
            this.groupBoxHmapTools.ResumeLayout(false);
            this.groupBoxHmapTools.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxHmapTools;
        private System.Windows.Forms.Button btnHMapExport;
        private System.Windows.Forms.Button btnImportHmap;
        private System.Windows.Forms.Button btnBrowseHmapFile;
        private System.Windows.Forms.TextBox textBoxHMapFile;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button buttonClose;

    }
}