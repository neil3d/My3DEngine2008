namespace NexusEditor.ResourceEditor
{
    partial class StaticMeshEdOptionsDlg
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
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageDefault = new System.Windows.Forms.TabPage();
            this.propertyGridImportDefault = new System.Windows.Forms.PropertyGrid();
            this.tabControl1.SuspendLayout();
            this.tabPageDefault.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPageDefault);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(393, 444);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPageDefault
            // 
            this.tabPageDefault.Controls.Add(this.propertyGridImportDefault);
            this.tabPageDefault.Location = new System.Drawing.Point(4, 21);
            this.tabPageDefault.Name = "tabPageDefault";
            this.tabPageDefault.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageDefault.Size = new System.Drawing.Size(385, 419);
            this.tabPageDefault.TabIndex = 0;
            this.tabPageDefault.Text = "Import Default";
            this.tabPageDefault.UseVisualStyleBackColor = true;
            // 
            // propertyGridImportDefault
            // 
            this.propertyGridImportDefault.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridImportDefault.Location = new System.Drawing.Point(3, 3);
            this.propertyGridImportDefault.Name = "propertyGridImportDefault";
            this.propertyGridImportDefault.Size = new System.Drawing.Size(379, 413);
            this.propertyGridImportDefault.TabIndex = 0;
            // 
            // StaticMeshEdOptionsDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(393, 444);
            this.Controls.Add(this.tabControl1);
            this.Name = "StaticMeshEdOptionsDlg";
            this.Text = "Nexus StaticMeshEd Options";
            this.Load += new System.EventHandler(this.StaticMeshEdOptionsDlg_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPageDefault.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPageDefault;
        private System.Windows.Forms.PropertyGrid propertyGridImportDefault;
    }
}