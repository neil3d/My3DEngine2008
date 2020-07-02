namespace NexusEditor.EditorMain
{
    partial class NewTerrainDlg
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
            this.propertyGridTrn = new System.Windows.Forms.PropertyGrid();
            this.buttonOk = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.checkBoxCenter = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // propertyGridTrn
            // 
            this.propertyGridTrn.Dock = System.Windows.Forms.DockStyle.Top;
            this.propertyGridTrn.Location = new System.Drawing.Point(0, 0);
            this.propertyGridTrn.Name = "propertyGridTrn";
            this.propertyGridTrn.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.propertyGridTrn.Size = new System.Drawing.Size(404, 325);
            this.propertyGridTrn.TabIndex = 0;
            // 
            // buttonOk
            // 
            this.buttonOk.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonOk.Location = new System.Drawing.Point(235, 332);
            this.buttonOk.Name = "buttonOk";
            this.buttonOk.Size = new System.Drawing.Size(75, 23);
            this.buttonOk.TabIndex = 1;
            this.buttonOk.Text = "下一步";
            this.buttonOk.UseVisualStyleBackColor = true;
            this.buttonOk.Click += new System.EventHandler(this.buttonOk_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(316, 332);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 2;
            this.buttonCancel.Text = "取消";
            this.buttonCancel.UseVisualStyleBackColor = true;
            // 
            // checkBoxCenter
            // 
            this.checkBoxCenter.AutoSize = true;
            this.checkBoxCenter.Checked = true;
            this.checkBoxCenter.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxCenter.Location = new System.Drawing.Point(13, 332);
            this.checkBoxCenter.Name = "checkBoxCenter";
            this.checkBoxCenter.Size = new System.Drawing.Size(96, 16);
            this.checkBoxCenter.TabIndex = 3;
            this.checkBoxCenter.Text = "位置自动居中";
            this.checkBoxCenter.UseVisualStyleBackColor = true;
            // 
            // NewTerrainDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(404, 362);
            this.Controls.Add(this.checkBoxCenter);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOk);
            this.Controls.Add(this.propertyGridTrn);
            this.Name = "NewTerrainDlg";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "地形基础属性";
            this.Load += new System.EventHandler(this.NewTerrainDlg_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PropertyGrid propertyGridTrn;
        private System.Windows.Forms.Button buttonOk;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.CheckBox checkBoxCenter;
    }
}