namespace NexusEditor.ResourceEditor
{
    partial class SFXElement
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SFXElement));
            this.panel1 = new System.Windows.Forms.Panel();
            this.labelType = new System.Windows.Forms.Label();
            this.btnRemove = new System.Windows.Forms.Button();
            this.checkBoxHide = new System.Windows.Forms.CheckBox();
            this.imageListIcon = new System.Windows.Forms.ImageList(this.components);
            this.propertyGridEffect = new System.Windows.Forms.PropertyGrid();
            this.Restart = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.Restart);
            this.panel1.Controls.Add(this.labelType);
            this.panel1.Controls.Add(this.btnRemove);
            this.panel1.Controls.Add(this.checkBoxHide);
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(198, 58);
            this.panel1.TabIndex = 0;
            // 
            // labelType
            // 
            this.labelType.AutoSize = true;
            this.labelType.Location = new System.Drawing.Point(5, 6);
            this.labelType.Name = "labelType";
            this.labelType.Size = new System.Drawing.Size(41, 12);
            this.labelType.TabIndex = 3;
            this.labelType.Text = "label1";
            // 
            // btnRemove
            // 
            this.btnRemove.BackColor = System.Drawing.Color.Transparent;
            this.btnRemove.Image = global::NexusEditor.Properties.Resources.action_stop;
            this.btnRemove.Location = new System.Drawing.Point(171, 26);
            this.btnRemove.Name = "btnRemove";
            this.btnRemove.Size = new System.Drawing.Size(22, 22);
            this.btnRemove.TabIndex = 2;
            this.btnRemove.UseVisualStyleBackColor = false;
            this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
            // 
            // checkBoxHide
            // 
            this.checkBoxHide.AutoSize = true;
            this.checkBoxHide.BackColor = System.Drawing.Color.Transparent;
            this.checkBoxHide.Location = new System.Drawing.Point(9, 31);
            this.checkBoxHide.Name = "checkBoxHide";
            this.checkBoxHide.Size = new System.Drawing.Size(15, 14);
            this.checkBoxHide.TabIndex = 1;
            this.checkBoxHide.UseVisualStyleBackColor = false;
            this.checkBoxHide.CheckedChanged += new System.EventHandler(this.checkBoxHide_CheckedChanged);
            // 
            // imageListIcon
            // 
            this.imageListIcon.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListIcon.ImageStream")));
            this.imageListIcon.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListIcon.Images.SetKeyName(0, "const");
            this.imageListIcon.Images.SetKeyName(1, "curve");
            // 
            // propertyGridEffect
            // 
            this.propertyGridEffect.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.propertyGridEffect.Location = new System.Drawing.Point(3, 67);
            this.propertyGridEffect.Name = "propertyGridEffect";
            this.propertyGridEffect.Size = new System.Drawing.Size(197, 413);
            this.propertyGridEffect.TabIndex = 1;
            this.propertyGridEffect.SelectedGridItemChanged += new System.Windows.Forms.SelectedGridItemChangedEventHandler(this.propertyGridEffect_SelectedGridItemChanged);
            this.propertyGridEffect.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGridEffect_PropertyValueChanged);
            // 
            // Restart
            // 
            this.Restart.BackColor = System.Drawing.Color.Transparent;
            this.Restart.Location = new System.Drawing.Point(30, 28);
            this.Restart.Name = "Restart";
            this.Restart.Size = new System.Drawing.Size(20, 19);
            this.Restart.TabIndex = 4;
            this.Restart.Text = ">";
            this.Restart.UseVisualStyleBackColor = false;
            this.Restart.Click += new System.EventHandler(this.Restart_Click);
            // 
            // SFXElement
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(this.propertyGridEffect);
            this.Controls.Add(this.panel1);
            this.Name = "SFXElement";
            this.Size = new System.Drawing.Size(204, 483);
            this.SizeChanged += new System.EventHandler(this.SFXElement_SizeChanged);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.CheckBox checkBoxHide;
        private System.Windows.Forms.Button btnRemove;
        private System.Windows.Forms.ImageList imageListIcon;
        private System.Windows.Forms.PropertyGrid propertyGridEffect;
        private System.Windows.Forms.Label labelType;
        private System.Windows.Forms.Button Restart;
    }
}
