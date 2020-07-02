namespace NexusEditor
{
    partial class NavigationMapDlg
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
            this.colorNavMap = new System.Windows.Forms.ColorDialog();
            this.propertyGridNavMapDesc = new System.Windows.Forms.PropertyGrid();
            this.SaveBitmap = new System.Windows.Forms.Button();
            this.ApplyDesc = new System.Windows.Forms.Button();
            this.ResetNavMap = new System.Windows.Forms.Button();
            this.AdapterTerrain = new System.Windows.Forms.Button();
            this.Rebuild = new System.Windows.Forms.Button();
            this.rebuildByActors = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // propertyGridNavMapDesc
            // 
            this.propertyGridNavMapDesc.Location = new System.Drawing.Point(2, 1);
            this.propertyGridNavMapDesc.Name = "propertyGridNavMapDesc";
            this.propertyGridNavMapDesc.Size = new System.Drawing.Size(239, 277);
            this.propertyGridNavMapDesc.TabIndex = 0;
            // 
            // SaveBitmap
            // 
            this.SaveBitmap.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.SaveBitmap.Location = new System.Drawing.Point(8, 375);
            this.SaveBitmap.Name = "SaveBitmap";
            this.SaveBitmap.Size = new System.Drawing.Size(96, 32);
            this.SaveBitmap.TabIndex = 1;
            this.SaveBitmap.Text = "Save Bitmap";
            this.SaveBitmap.UseVisualStyleBackColor = true;
            this.SaveBitmap.Click += new System.EventHandler(this.SaveBitmap_Click);
            // 
            // ApplyDesc
            // 
            this.ApplyDesc.Location = new System.Drawing.Point(133, 375);
            this.ApplyDesc.Name = "ApplyDesc";
            this.ApplyDesc.Size = new System.Drawing.Size(96, 32);
            this.ApplyDesc.TabIndex = 1;
            this.ApplyDesc.Text = "Apply";
            this.ApplyDesc.UseVisualStyleBackColor = true;
            this.ApplyDesc.Click += new System.EventHandler(this.ApplyDesc_Click);
            // 
            // ResetNavMap
            // 
            this.ResetNavMap.Location = new System.Drawing.Point(8, 295);
            this.ResetNavMap.Name = "ResetNavMap";
            this.ResetNavMap.Size = new System.Drawing.Size(96, 32);
            this.ResetNavMap.TabIndex = 1;
            this.ResetNavMap.Text = "Reset";
            this.ResetNavMap.UseVisualStyleBackColor = true;
            this.ResetNavMap.Click += new System.EventHandler(this.ResetNavMap_Click);
            // 
            // AdapterTerrain
            // 
            this.AdapterTerrain.Location = new System.Drawing.Point(133, 295);
            this.AdapterTerrain.Name = "AdapterTerrain";
            this.AdapterTerrain.Size = new System.Drawing.Size(96, 32);
            this.AdapterTerrain.TabIndex = 1;
            this.AdapterTerrain.Text = "Adapter Terrain";
            this.AdapterTerrain.UseVisualStyleBackColor = true;
            this.AdapterTerrain.Click += new System.EventHandler(this.AdapterTerrain_Click);
            // 
            // Rebuild
            // 
            this.Rebuild.Location = new System.Drawing.Point(8, 333);
            this.Rebuild.Name = "Rebuild";
            this.Rebuild.Size = new System.Drawing.Size(96, 32);
            this.Rebuild.TabIndex = 1;
            this.Rebuild.Text = "Rebuild By Line Check";
            this.Rebuild.UseVisualStyleBackColor = true;
            this.Rebuild.Click += new System.EventHandler(this.Rebuild_Click);
            // 
            // rebuildByActors
            // 
            this.rebuildByActors.Location = new System.Drawing.Point(133, 333);
            this.rebuildByActors.Name = "rebuildByActors";
            this.rebuildByActors.Size = new System.Drawing.Size(96, 32);
            this.rebuildByActors.TabIndex = 1;
            this.rebuildByActors.Text = "Rebuild By Actors";
            this.rebuildByActors.UseVisualStyleBackColor = true;
            this.rebuildByActors.Click += new System.EventHandler(this.rebuildByActors_Click);
            // 
            // NavigationMapDlg
            // 
            this.AcceptButton = this.ApplyDesc;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(244, 419);
            this.Controls.Add(this.rebuildByActors);
            this.Controls.Add(this.Rebuild);
            this.Controls.Add(this.ApplyDesc);
            this.Controls.Add(this.SaveBitmap);
            this.Controls.Add(this.AdapterTerrain);
            this.Controls.Add(this.ResetNavMap);
            this.Controls.Add(this.propertyGridNavMapDesc);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "NavigationMapDlg";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Navigation Map Description";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ColorDialog colorNavMap;
        private System.Windows.Forms.PropertyGrid propertyGridNavMapDesc;
        private System.Windows.Forms.Button SaveBitmap;
        private System.Windows.Forms.Button ApplyDesc;
        private System.Windows.Forms.Button ResetNavMap;
        private System.Windows.Forms.Button AdapterTerrain;
        private System.Windows.Forms.Button Rebuild;
        private System.Windows.Forms.Button rebuildByActors;
    }
}