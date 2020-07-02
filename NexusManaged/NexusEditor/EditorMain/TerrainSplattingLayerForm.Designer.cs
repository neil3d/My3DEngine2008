namespace NexusEditor.EditorMain
{
    partial class TerrainSplattingLayerForm
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
            this.propertyGridLayer = new System.Windows.Forms.PropertyGrid();
            this.buttonApplyLayerProperty = new System.Windows.Forms.Button();
            this.buttonClose = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // propertyGridLayer
            // 
            this.propertyGridLayer.Dock = System.Windows.Forms.DockStyle.Top;
            this.propertyGridLayer.Location = new System.Drawing.Point(0, 0);
            this.propertyGridLayer.Name = "propertyGridLayer";
            this.propertyGridLayer.Size = new System.Drawing.Size(382, 295);
            this.propertyGridLayer.TabIndex = 3;
            // 
            // buttonApplyLayerProperty
            // 
            this.buttonApplyLayerProperty.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonApplyLayerProperty.Location = new System.Drawing.Point(106, 301);
            this.buttonApplyLayerProperty.Name = "buttonApplyLayerProperty";
            this.buttonApplyLayerProperty.Size = new System.Drawing.Size(75, 23);
            this.buttonApplyLayerProperty.TabIndex = 4;
            this.buttonApplyLayerProperty.Text = "确定";
            this.buttonApplyLayerProperty.UseVisualStyleBackColor = true;
            this.buttonApplyLayerProperty.Click += new System.EventHandler(this.buttonApplyLayerProperty_Click);
            // 
            // buttonClose
            // 
            this.buttonClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonClose.Location = new System.Drawing.Point(202, 301);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(75, 23);
            this.buttonClose.TabIndex = 14;
            this.buttonClose.Text = "取消";
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
            // 
            // TerrainSplattingLayerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 336);
            this.ControlBox = false;
            this.Controls.Add(this.buttonClose);
            this.Controls.Add(this.buttonApplyLayerProperty);
            this.Controls.Add(this.propertyGridLayer);
            this.Name = "TerrainSplattingLayerForm";
            this.Text = "Terrain Splatting Layer";
            this.Load += new System.EventHandler(this.TerrainSplattingLayerForm_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PropertyGrid propertyGridLayer;
        private System.Windows.Forms.Button buttonApplyLayerProperty;
        private System.Windows.Forms.Button buttonClose;
    }
}