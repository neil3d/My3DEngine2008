namespace NexusEditor.ResourceEditor
{
    partial class ResAttributeInfoEditor
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
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem("est");
            System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem(new string[] {
            "Test1",
            "test11",
            "Test111"}, -1);
            System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem("Test2");
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ResAttributeInfoEditor));
            this.toolBarResource = new System.Windows.Forms.ToolStrip();
            this.toolStripButtonSave = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonNew = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonDelete = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonClear = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.toolTextSearch = new System.Windows.Forms.ToolStripTextBox();
            this.splitContainerMain = new System.Windows.Forms.SplitContainer();
            this.listViewInfo = new System.Windows.Forms.ListView();
            this.columnHeaderID = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderName = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderDesc = new System.Windows.Forms.ColumnHeader();
            this.propertyGridInfo = new System.Windows.Forms.PropertyGrid();
            this.toolBarResource.SuspendLayout();
            this.splitContainerMain.Panel1.SuspendLayout();
            this.splitContainerMain.Panel2.SuspendLayout();
            this.splitContainerMain.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolBarResource
            // 
            this.toolBarResource.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonSave,
            this.toolStripSeparator2,
            this.toolStripButtonNew,
            this.toolStripButtonDelete,
            this.toolStripButtonClear,
            this.toolStripSeparator3,
            this.toolStripLabel1,
            this.toolTextSearch});
            this.toolBarResource.Location = new System.Drawing.Point(0, 0);
            this.toolBarResource.Name = "toolBarResource";
            this.toolBarResource.Size = new System.Drawing.Size(657, 40);
            this.toolBarResource.TabIndex = 2;
            this.toolBarResource.Text = "toolStrip1";
            // 
            // toolStripButtonSave
            // 
            this.toolStripButtonSave.BackColor = System.Drawing.SystemColors.Window;
            this.toolStripButtonSave.Image = global::NexusEditor.Properties.Resources.Save;
            this.toolStripButtonSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSave.Name = "toolStripButtonSave";
            this.toolStripButtonSave.Size = new System.Drawing.Size(39, 37);
            this.toolStripButtonSave.Text = "Save";
            this.toolStripButtonSave.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonSave.Click += new System.EventHandler(this.toolStripButtonSave_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 40);
            // 
            // toolStripButtonNew
            // 
            this.toolStripButtonNew.BackColor = System.Drawing.SystemColors.Window;
            this.toolStripButtonNew.Image = global::NexusEditor.Properties.Resources.Add;
            this.toolStripButtonNew.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonNew.Name = "toolStripButtonNew";
            this.toolStripButtonNew.Size = new System.Drawing.Size(38, 37);
            this.toolStripButtonNew.Text = "New";
            this.toolStripButtonNew.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonNew.Click += new System.EventHandler(this.toolStripButtonNew_Click);
            // 
            // toolStripButtonDelete
            // 
            this.toolStripButtonDelete.BackColor = System.Drawing.SystemColors.Window;
            this.toolStripButtonDelete.Image = global::NexusEditor.Properties.Resources.Del;
            this.toolStripButtonDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonDelete.Name = "toolStripButtonDelete";
            this.toolStripButtonDelete.Size = new System.Drawing.Size(49, 37);
            this.toolStripButtonDelete.Text = "Delete";
            this.toolStripButtonDelete.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonDelete.Click += new System.EventHandler(this.toolStripButtonDelete_Click);
            // 
            // toolStripButtonClear
            // 
            this.toolStripButtonClear.BackColor = System.Drawing.SystemColors.Window;
            this.toolStripButtonClear.Image = global::NexusEditor.Properties.Resources.delete_layer;
            this.toolStripButtonClear.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonClear.Name = "toolStripButtonClear";
            this.toolStripButtonClear.Size = new System.Drawing.Size(42, 37);
            this.toolStripButtonClear.Text = "Clear";
            this.toolStripButtonClear.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonClear.Click += new System.EventHandler(this.toolStripButtonClear_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 40);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.AutoSize = false;
            this.toolStripLabel1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripLabel1.Image = global::NexusEditor.Properties.Resources.Search;
            this.toolStripLabel1.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripLabel1.ImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(200)))), ((int)(((byte)(200)))));
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(32, 32);
            this.toolStripLabel1.Text = "Search";
            // 
            // toolTextSearch
            // 
            this.toolTextSearch.Name = "toolTextSearch";
            this.toolTextSearch.Size = new System.Drawing.Size(128, 40);
            this.toolTextSearch.TextChanged += new System.EventHandler(this.toolTextSearch_TextChanged);
            // 
            // splitContainerMain
            // 
            this.splitContainerMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerMain.Location = new System.Drawing.Point(0, 40);
            this.splitContainerMain.Name = "splitContainerMain";
            // 
            // splitContainerMain.Panel1
            // 
            this.splitContainerMain.Panel1.Controls.Add(this.listViewInfo);
            // 
            // splitContainerMain.Panel2
            // 
            this.splitContainerMain.Panel2.Controls.Add(this.propertyGridInfo);
            this.splitContainerMain.Size = new System.Drawing.Size(657, 435);
            this.splitContainerMain.SplitterDistance = 339;
            this.splitContainerMain.TabIndex = 3;
            // 
            // listViewInfo
            // 
            this.listViewInfo.BackColor = System.Drawing.SystemColors.Window;
            this.listViewInfo.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderID,
            this.columnHeaderName,
            this.columnHeaderDesc});
            this.listViewInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewInfo.FullRowSelect = true;
            this.listViewInfo.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1,
            listViewItem2,
            listViewItem3});
            this.listViewInfo.Location = new System.Drawing.Point(0, 0);
            this.listViewInfo.Name = "listViewInfo";
            this.listViewInfo.Size = new System.Drawing.Size(339, 435);
            this.listViewInfo.TabIndex = 2;
            this.listViewInfo.UseCompatibleStateImageBehavior = false;
            this.listViewInfo.View = System.Windows.Forms.View.Details;
            this.listViewInfo.SelectedIndexChanged += new System.EventHandler(this.listViewInfo_SelectedIndexChanged);
            this.listViewInfo.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.listViewInfo_ColumnClick);
            // 
            // columnHeaderID
            // 
            this.columnHeaderID.Text = "ID";
            // 
            // columnHeaderName
            // 
            this.columnHeaderName.Text = "Name";
            // 
            // columnHeaderDesc
            // 
            this.columnHeaderDesc.Text = "Description";
            this.columnHeaderDesc.Width = 208;
            // 
            // propertyGridInfo
            // 
            this.propertyGridInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridInfo.Location = new System.Drawing.Point(0, 0);
            this.propertyGridInfo.Name = "propertyGridInfo";
            this.propertyGridInfo.Size = new System.Drawing.Size(314, 435);
            this.propertyGridInfo.TabIndex = 0;
            // 
            // ResAttributeInfoEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.ClientSize = new System.Drawing.Size(657, 475);
            this.Controls.Add(this.splitContainerMain);
            this.Controls.Add(this.toolBarResource);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ResAttributeInfoEditor";
            this.Text = "ResAttributeInfoEditor";
            this.toolBarResource.ResumeLayout(false);
            this.toolBarResource.PerformLayout();
            this.splitContainerMain.Panel1.ResumeLayout(false);
            this.splitContainerMain.Panel2.ResumeLayout(false);
            this.splitContainerMain.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolBarResource;
        private System.Windows.Forms.SplitContainer splitContainerMain;
        private System.Windows.Forms.PropertyGrid propertyGridInfo;
        private System.Windows.Forms.ToolStripButton toolStripButtonNew;
        private System.Windows.Forms.ToolStripButton toolStripButtonDelete;
        private System.Windows.Forms.ToolStripButton toolStripButtonClear;
        private System.Windows.Forms.ListView listViewInfo;
        private System.Windows.Forms.ColumnHeader columnHeaderID;
        private System.Windows.Forms.ColumnHeader columnHeaderName;
        private System.Windows.Forms.ColumnHeader columnHeaderDesc;
        private System.Windows.Forms.ToolStripButton toolStripButtonSave;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripTextBox toolTextSearch;


    }
}