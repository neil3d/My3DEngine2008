namespace NexusEditor.EditorMain
{
    partial class TerrainNoiseDlg
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
            this.buttonGenNoise = new System.Windows.Forms.Button();
            this.propertyGridNoise = new System.Windows.Forms.PropertyGrid();
            this.btnCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonGenNoise
            // 
            this.buttonGenNoise.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonGenNoise.Location = new System.Drawing.Point(114, 407);
            this.buttonGenNoise.Name = "buttonGenNoise";
            this.buttonGenNoise.Size = new System.Drawing.Size(75, 23);
            this.buttonGenNoise.TabIndex = 3;
            this.buttonGenNoise.Text = "Generate";
            this.buttonGenNoise.UseVisualStyleBackColor = true;
            this.buttonGenNoise.Click += new System.EventHandler(this.buttonGenNoise_Click);
            // 
            // propertyGridNoise
            // 
            this.propertyGridNoise.Dock = System.Windows.Forms.DockStyle.Top;
            this.propertyGridNoise.Location = new System.Drawing.Point(0, 0);
            this.propertyGridNoise.Name = "propertyGridNoise";
            this.propertyGridNoise.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.propertyGridNoise.Size = new System.Drawing.Size(421, 401);
            this.propertyGridNoise.TabIndex = 2;
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(232, 407);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // TerrainNoiseDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(421, 440);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.buttonGenNoise);
            this.Controls.Add(this.propertyGridNoise);
            this.Name = "TerrainNoiseDlg";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Terrain Noise Generation";
            this.Load += new System.EventHandler(this.TerrainNoiseDlg_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonGenNoise;
        private System.Windows.Forms.PropertyGrid propertyGridNoise;
        private System.Windows.Forms.Button btnCancel;
    }
}