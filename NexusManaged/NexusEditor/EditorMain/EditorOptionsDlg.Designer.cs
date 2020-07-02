namespace NexusEditor.EditorMain
{
    partial class EditorOptionsDlg
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
            this.tabControlMain = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.propertyGridBasic = new System.Windows.Forms.PropertyGrid();
            this.tabPageEngine = new System.Windows.Forms.TabPage();
            this.propertyGridEngine = new System.Windows.Forms.PropertyGrid();
            this.tabControlMain.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPageEngine.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControlMain
            // 
            this.tabControlMain.Controls.Add(this.tabPage1);
            this.tabControlMain.Controls.Add(this.tabPageEngine);
            this.tabControlMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlMain.Location = new System.Drawing.Point(0, 0);
            this.tabControlMain.Name = "tabControlMain";
            this.tabControlMain.SelectedIndex = 0;
            this.tabControlMain.Size = new System.Drawing.Size(383, 454);
            this.tabControlMain.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.propertyGridBasic);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(375, 428);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Basic";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // propertyGridBasic
            // 
            this.propertyGridBasic.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridBasic.Location = new System.Drawing.Point(3, 3);
            this.propertyGridBasic.Name = "propertyGridBasic";
            this.propertyGridBasic.Size = new System.Drawing.Size(369, 422);
            this.propertyGridBasic.TabIndex = 0;
            // 
            // tabPageEngine
            // 
            this.tabPageEngine.Controls.Add(this.propertyGridEngine);
            this.tabPageEngine.Location = new System.Drawing.Point(4, 22);
            this.tabPageEngine.Name = "tabPageEngine";
            this.tabPageEngine.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageEngine.Size = new System.Drawing.Size(375, 428);
            this.tabPageEngine.TabIndex = 1;
            this.tabPageEngine.Text = "Engine";
            this.tabPageEngine.UseVisualStyleBackColor = true;
            // 
            // propertyGridEngine
            // 
            this.propertyGridEngine.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridEngine.Location = new System.Drawing.Point(3, 3);
            this.propertyGridEngine.Name = "propertyGridEngine";
            this.propertyGridEngine.Size = new System.Drawing.Size(369, 422);
            this.propertyGridEngine.TabIndex = 0;
            // 
            // EditorOptionsDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(383, 454);
            this.Controls.Add(this.tabControlMain);
            this.Name = "EditorOptionsDlg";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Nexus Editor Options";
            this.tabControlMain.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPageEngine.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControlMain;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.PropertyGrid propertyGridBasic;
        private System.Windows.Forms.TabPage tabPageEngine;
        private System.Windows.Forms.PropertyGrid propertyGridEngine;
    }
}