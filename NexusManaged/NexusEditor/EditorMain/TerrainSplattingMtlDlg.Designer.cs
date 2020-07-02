namespace NexusEditor.EditorMain
{
    partial class TerrainSplattingMtlDlg
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
            System.Windows.Forms.Label label10;
            System.Windows.Forms.Label label1;
            this.btnCreateSplat = new System.Windows.Forms.Button();
            this.textBoxAlphaH = new System.Windows.Forms.TextBox();
            this.textBoxAlphaW = new System.Windows.Forms.TextBox();
            label10 = new System.Windows.Forms.Label();
            label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label10
            // 
            label10.AutoSize = true;
            label10.Location = new System.Drawing.Point(3, 16);
            label10.Name = "label10";
            label10.Size = new System.Drawing.Size(71, 12);
            label10.TabIndex = 8;
            label10.Text = "Alpha Width";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(3, 53);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(77, 12);
            label1.TabIndex = 14;
            label1.Text = "Alpha Height";
            // 
            // btnCreateSplat
            // 
            this.btnCreateSplat.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnCreateSplat.Location = new System.Drawing.Point(66, 130);
            this.btnCreateSplat.Name = "btnCreateSplat";
            this.btnCreateSplat.Size = new System.Drawing.Size(75, 23);
            this.btnCreateSplat.TabIndex = 11;
            this.btnCreateSplat.Text = "Create";
            this.btnCreateSplat.UseVisualStyleBackColor = true;
            this.btnCreateSplat.Click += new System.EventHandler(this.btnCreateSplat_Click);
            // 
            // textBoxAlphaH
            // 
            this.textBoxAlphaH.Location = new System.Drawing.Point(80, 50);
            this.textBoxAlphaH.Name = "textBoxAlphaH";
            this.textBoxAlphaH.Size = new System.Drawing.Size(100, 21);
            this.textBoxAlphaH.TabIndex = 10;
            this.textBoxAlphaH.Text = "-1";
            // 
            // textBoxAlphaW
            // 
            this.textBoxAlphaW.Location = new System.Drawing.Point(80, 12);
            this.textBoxAlphaW.Name = "textBoxAlphaW";
            this.textBoxAlphaW.Size = new System.Drawing.Size(100, 21);
            this.textBoxAlphaW.TabIndex = 9;
            this.textBoxAlphaW.Text = "-1";
            // 
            // TerrainSplattingMtlDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(206, 159);
            this.Controls.Add(label1);
            this.Controls.Add(this.btnCreateSplat);
            this.Controls.Add(this.textBoxAlphaH);
            this.Controls.Add(this.textBoxAlphaW);
            this.Controls.Add(label10);
            this.Name = "TerrainSplattingMtlDlg";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Terrain Texture Splatting";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnCreateSplat;
        private System.Windows.Forms.TextBox textBoxAlphaH;
        private System.Windows.Forms.TextBox textBoxAlphaW;
    }
}