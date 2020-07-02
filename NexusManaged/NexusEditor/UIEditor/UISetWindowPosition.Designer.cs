namespace NexusEditor.UIEditor
{
    partial class UISetWindowPosition
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
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.lblLeft = new System.Windows.Forms.Label();
            this.textBoxLeft = new System.Windows.Forms.TextBox();
            this.labelTop = new System.Windows.Forms.Label();
            this.textBoxTop = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(21, 59);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 2;
            this.btnOK.Text = "确定";
            this.btnOK.UseVisualStyleBackColor = true;
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(102, 59);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "取消";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // lblLeft
            // 
            this.lblLeft.AutoSize = true;
            this.lblLeft.Location = new System.Drawing.Point(25, 9);
            this.lblLeft.Name = "lblLeft";
            this.lblLeft.Size = new System.Drawing.Size(41, 12);
            this.lblLeft.TabIndex = 4;
            this.lblLeft.Text = "横坐标";
            // 
            // textBoxLeft
            // 
            this.textBoxLeft.Location = new System.Drawing.Point(77, 5);
            this.textBoxLeft.Name = "textBoxLeft";
            this.textBoxLeft.Size = new System.Drawing.Size(100, 21);
            this.textBoxLeft.TabIndex = 0;
            this.textBoxLeft.Text = "11";
            // 
            // labelTop
            // 
            this.labelTop.AutoSize = true;
            this.labelTop.Location = new System.Drawing.Point(25, 36);
            this.labelTop.Name = "labelTop";
            this.labelTop.Size = new System.Drawing.Size(41, 12);
            this.labelTop.TabIndex = 5;
            this.labelTop.Text = "竖坐标";
            // 
            // textBoxTop
            // 
            this.textBoxTop.Location = new System.Drawing.Point(77, 32);
            this.textBoxTop.Name = "textBoxTop";
            this.textBoxTop.Size = new System.Drawing.Size(100, 21);
            this.textBoxTop.TabIndex = 1;
            this.textBoxTop.Text = "11";
            // 
            // UISetWindowPosition
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(193, 93);
            this.Controls.Add(this.textBoxTop);
            this.Controls.Add(this.labelTop);
            this.Controls.Add(this.textBoxLeft);
            this.Controls.Add(this.lblLeft);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "UISetWindowPosition";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "设置所选窗体的屏幕位置";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Label lblLeft;
        private System.Windows.Forms.TextBox textBoxLeft;
        private System.Windows.Forms.Label labelTop;
        private System.Windows.Forms.TextBox textBoxTop;
    }
}