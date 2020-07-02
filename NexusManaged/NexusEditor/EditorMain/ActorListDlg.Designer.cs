namespace NexusEditor.EditorMain
{
    partial class ActorListDlg
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
            System.Windows.Forms.PictureBox pictureBox1;
            this.listViewActors = new System.Windows.Forms.ListView();
            this.ActorName = new System.Windows.Forms.ColumnHeader();
            this.btnCancel = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkBoxCase = new System.Windows.Forms.CheckBox();
            this.btnSelect = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // listViewActors
            // 
            this.listViewActors.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.ActorName});
            this.listViewActors.Location = new System.Drawing.Point(12, 39);
            this.listViewActors.Name = "listViewActors";
            this.listViewActors.Size = new System.Drawing.Size(310, 480);
            this.listViewActors.TabIndex = 0;
            this.listViewActors.UseCompatibleStateImageBehavior = false;
            this.listViewActors.View = System.Windows.Forms.View.Details;
            this.listViewActors.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listViewActors_MouseDoubleClick);
            // 
            // ActorName
            // 
            this.ActorName.Text = "Name";
            this.ActorName.Width = 238;
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(423, 525);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // pictureBox1
            // 
            pictureBox1.Image = global::NexusEditor.Properties.Resources.Search;
            pictureBox1.Location = new System.Drawing.Point(12, 9);
            pictureBox1.Name = "pictureBox1";
            pictureBox1.Size = new System.Drawing.Size(24, 24);
            pictureBox1.TabIndex = 2;
            pictureBox1.TabStop = false;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(42, 12);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(280, 21);
            this.textBox1.TabIndex = 3;
            // 
            // groupBox1
            // 
            this.groupBox1.Location = new System.Drawing.Point(332, 39);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(172, 480);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Filter";
            // 
            // checkBoxCase
            // 
            this.checkBoxCase.AutoSize = true;
            this.checkBoxCase.Location = new System.Drawing.Point(332, 17);
            this.checkBoxCase.Name = "checkBoxCase";
            this.checkBoxCase.Size = new System.Drawing.Size(138, 16);
            this.checkBoxCase.TabIndex = 5;
            this.checkBoxCase.Text = "Find Case Sensitive";
            this.checkBoxCase.UseVisualStyleBackColor = true;
            // 
            // btnSelect
            // 
            this.btnSelect.Location = new System.Drawing.Point(342, 525);
            this.btnSelect.Name = "btnSelect";
            this.btnSelect.Size = new System.Drawing.Size(75, 23);
            this.btnSelect.TabIndex = 1;
            this.btnSelect.Text = "Select";
            this.btnSelect.UseVisualStyleBackColor = true;
            this.btnSelect.Click += new System.EventHandler(this.btnSelect_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(66, 527);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(49, 19);
            this.button1.TabIndex = 6;
            this.button1.Text = "All";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(129, 527);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(49, 19);
            this.button2.TabIndex = 6;
            this.button2.Text = "None";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(192, 527);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(49, 19);
            this.button3.TabIndex = 6;
            this.button3.Text = "Invert";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // ActorListDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(512, 560);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.checkBoxCase);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(pictureBox1);
            this.Controls.Add(this.btnSelect);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.listViewActors);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ActorListDlg";
            this.ShowIcon = false;
            this.Text = "Select Objects";
            ((System.ComponentModel.ISupportInitialize)(pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView listViewActors;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.ColumnHeader ActorName;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox checkBoxCase;
        private System.Windows.Forms.Button btnSelect;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
    }
}