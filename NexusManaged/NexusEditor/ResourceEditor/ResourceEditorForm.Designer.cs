namespace NexusEditor.ResourceEditor
{
    partial class ResourceEditorForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ResourceEditorForm));
            this.menuBarResource = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.resourceEditorsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showStaticMeshEditorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showAnimMeshEditorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolBarResource = new System.Windows.Forms.ToolStrip();
            this.toolStripButtonRefresh = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonStaticMeshEd = new System.Windows.Forms.ToolStripButton();
            this.toolBtnAnimMeshEd = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.thumbnialsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.iconsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.detailsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSplitButton1 = new System.Windows.Forms.ToolStripSplitButton();
            this.nameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.toolTextSearch = new System.Windows.Forms.ToolStripTextBox();
            this.statusBarResource = new System.Windows.Forms.StatusStrip();
            this.splitContainerResource = new System.Windows.Forms.SplitContainer();
            this.tabControlResource = new System.Windows.Forms.TabControl();
            this.tabPageFileSys = new System.Windows.Forms.TabPage();
            this.treeViewFileSys = new System.Windows.Forms.TreeView();
            this.imageListTree = new System.Windows.Forms.ImageList(this.components);
            this.tabPageResFilter = new System.Windows.Forms.TabPage();
            this.listFilter = new System.Windows.Forms.CheckedListBox();
            this.imageListTab = new System.Windows.Forms.ImageList(this.components);
            this.listViewResource = new System.Windows.Forms.ListView();
            this.columnFileName = new System.Windows.Forms.ColumnHeader();
            this.columnFileSize = new System.Windows.Forms.ColumnHeader();
            this.columnResType = new System.Windows.Forms.ColumnHeader();
            this.columnDate = new System.Windows.Forms.ColumnHeader();
            this.imageListForList = new System.Windows.Forms.ImageList(this.components);
            this.imageListForListSmall = new System.Windows.Forms.ImageList(this.components);
            this.contextMenuStripTree = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.saveCurrentLevelHereToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuBarResource.SuspendLayout();
            this.toolBarResource.SuspendLayout();
            this.splitContainerResource.Panel1.SuspendLayout();
            this.splitContainerResource.Panel2.SuspendLayout();
            this.splitContainerResource.SuspendLayout();
            this.tabControlResource.SuspendLayout();
            this.tabPageFileSys.SuspendLayout();
            this.tabPageResFilter.SuspendLayout();
            this.contextMenuStripTree.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuBarResource
            // 
            this.menuBarResource.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.resourceEditorsToolStripMenuItem});
            this.menuBarResource.Location = new System.Drawing.Point(0, 0);
            this.menuBarResource.Name = "menuBarResource";
            this.menuBarResource.Size = new System.Drawing.Size(901, 24);
            this.menuBarResource.TabIndex = 0;
            this.menuBarResource.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // resourceEditorsToolStripMenuItem
            // 
            this.resourceEditorsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showStaticMeshEditorToolStripMenuItem,
            this.showAnimMeshEditorToolStripMenuItem});
            this.resourceEditorsToolStripMenuItem.Name = "resourceEditorsToolStripMenuItem";
            this.resourceEditorsToolStripMenuItem.Size = new System.Drawing.Size(100, 20);
            this.resourceEditorsToolStripMenuItem.Text = "Resource Editors";
            // 
            // showStaticMeshEditorToolStripMenuItem
            // 
            this.showStaticMeshEditorToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.Box;
            this.showStaticMeshEditorToolStripMenuItem.Name = "showStaticMeshEditorToolStripMenuItem";
            this.showStaticMeshEditorToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.showStaticMeshEditorToolStripMenuItem.Text = "Show Static Mesh Editor";
            this.showStaticMeshEditorToolStripMenuItem.Click += new System.EventHandler(this.showStaticMeshEditorToolStripMenuItem_Click);
            // 
            // showAnimMeshEditorToolStripMenuItem
            // 
            this.showAnimMeshEditorToolStripMenuItem.Image = global::NexusEditor.Properties.Resources.AnimMesh;
            this.showAnimMeshEditorToolStripMenuItem.Name = "showAnimMeshEditorToolStripMenuItem";
            this.showAnimMeshEditorToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.showAnimMeshEditorToolStripMenuItem.Text = "Show Anim Mesh Editor";
            this.showAnimMeshEditorToolStripMenuItem.Click += new System.EventHandler(this.showAnimMeshEditorToolStripMenuItem_Click);
            // 
            // toolBarResource
            // 
            this.toolBarResource.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonRefresh,
            this.toolStripSeparator1,
            this.toolStripButtonStaticMeshEd,
            this.toolBtnAnimMeshEd,
            this.toolStripSeparator2,
            this.toolStripDropDownButton1,
            this.toolStripSplitButton1,
            this.toolStripSeparator3,
            this.toolStripLabel1,
            this.toolTextSearch});
            this.toolBarResource.Location = new System.Drawing.Point(0, 24);
            this.toolBarResource.Name = "toolBarResource";
            this.toolBarResource.Size = new System.Drawing.Size(901, 36);
            this.toolBarResource.TabIndex = 1;
            this.toolBarResource.Text = "toolStrip1";
            // 
            // toolStripButtonRefresh
            // 
            this.toolStripButtonRefresh.Image = global::NexusEditor.Properties.Resources.Refresh;
            this.toolStripButtonRefresh.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonRefresh.Name = "toolStripButtonRefresh";
            this.toolStripButtonRefresh.Size = new System.Drawing.Size(49, 33);
            this.toolStripButtonRefresh.Text = "Refresh";
            this.toolStripButtonRefresh.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 36);
            // 
            // toolStripButtonStaticMeshEd
            // 
            this.toolStripButtonStaticMeshEd.Image = global::NexusEditor.Properties.Resources.Box;
            this.toolStripButtonStaticMeshEd.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonStaticMeshEd.Name = "toolStripButtonStaticMeshEd";
            this.toolStripButtonStaticMeshEd.Size = new System.Drawing.Size(75, 33);
            this.toolStripButtonStaticMeshEd.Text = "StaticMeshEd";
            this.toolStripButtonStaticMeshEd.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonStaticMeshEd.Click += new System.EventHandler(this.showStaticMeshEditorToolStripMenuItem_Click);
            // 
            // toolBtnAnimMeshEd
            // 
            this.toolBtnAnimMeshEd.Image = global::NexusEditor.Properties.Resources.AnimMesh;
            this.toolBtnAnimMeshEd.ImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(200)))), ((int)(((byte)(200)))));
            this.toolBtnAnimMeshEd.Name = "toolBtnAnimMeshEd";
            this.toolBtnAnimMeshEd.Size = new System.Drawing.Size(71, 33);
            this.toolBtnAnimMeshEd.Text = "AnimMeshEd";
            this.toolBtnAnimMeshEd.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolBtnAnimMeshEd.Click += new System.EventHandler(this.showAnimMeshEditorToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 36);
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.thumbnialsToolStripMenuItem,
            this.iconsToolStripMenuItem,
            this.detailsToolStripMenuItem});
            this.toolStripDropDownButton1.Image = global::NexusEditor.Properties.Resources.ListMode;
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(61, 33);
            this.toolStripDropDownButton1.Text = "List View";
            this.toolStripDropDownButton1.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // thumbnialsToolStripMenuItem
            // 
            this.thumbnialsToolStripMenuItem.Name = "thumbnialsToolStripMenuItem";
            this.thumbnialsToolStripMenuItem.Size = new System.Drawing.Size(127, 22);
            this.thumbnialsToolStripMenuItem.Text = "Thumbnials";
            this.thumbnialsToolStripMenuItem.Click += new System.EventHandler(this.thumbnialsToolStripMenuItem_Click);
            // 
            // iconsToolStripMenuItem
            // 
            this.iconsToolStripMenuItem.Name = "iconsToolStripMenuItem";
            this.iconsToolStripMenuItem.Size = new System.Drawing.Size(127, 22);
            this.iconsToolStripMenuItem.Text = "Icons";
            this.iconsToolStripMenuItem.Click += new System.EventHandler(this.iconsToolStripMenuItem_Click);
            // 
            // detailsToolStripMenuItem
            // 
            this.detailsToolStripMenuItem.Name = "detailsToolStripMenuItem";
            this.detailsToolStripMenuItem.Size = new System.Drawing.Size(127, 22);
            this.detailsToolStripMenuItem.Text = "Details";
            this.detailsToolStripMenuItem.Click += new System.EventHandler(this.detailsToolStripMenuItem_Click);
            // 
            // toolStripSplitButton1
            // 
            this.toolStripSplitButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.nameToolStripMenuItem,
            this.dateToolStripMenuItem});
            this.toolStripSplitButton1.Image = global::NexusEditor.Properties.Resources.Sort;
            this.toolStripSplitButton1.ImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(200)))), ((int)(((byte)(200)))));
            this.toolStripSplitButton1.Name = "toolStripSplitButton1";
            this.toolStripSplitButton1.Size = new System.Drawing.Size(72, 33);
            this.toolStripSplitButton1.Text = "Sort Mode";
            this.toolStripSplitButton1.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // nameToolStripMenuItem
            // 
            this.nameToolStripMenuItem.Name = "nameToolStripMenuItem";
            this.nameToolStripMenuItem.Size = new System.Drawing.Size(101, 22);
            this.nameToolStripMenuItem.Text = "Name";
            // 
            // dateToolStripMenuItem
            // 
            this.dateToolStripMenuItem.Name = "dateToolStripMenuItem";
            this.dateToolStripMenuItem.Size = new System.Drawing.Size(101, 22);
            this.dateToolStripMenuItem.Text = "Date";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 36);
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
            this.toolTextSearch.Size = new System.Drawing.Size(128, 36);
            this.toolTextSearch.TextChanged += new System.EventHandler(this.toolTextSearch_TextChanged);
            // 
            // statusBarResource
            // 
            this.statusBarResource.Location = new System.Drawing.Point(0, 556);
            this.statusBarResource.Name = "statusBarResource";
            this.statusBarResource.Size = new System.Drawing.Size(901, 22);
            this.statusBarResource.TabIndex = 2;
            this.statusBarResource.Text = "statusStrip1";
            // 
            // splitContainerResource
            // 
            this.splitContainerResource.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitContainerResource.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerResource.Location = new System.Drawing.Point(0, 60);
            this.splitContainerResource.Name = "splitContainerResource";
            // 
            // splitContainerResource.Panel1
            // 
            this.splitContainerResource.Panel1.Controls.Add(this.tabControlResource);
            // 
            // splitContainerResource.Panel2
            // 
            this.splitContainerResource.Panel2.Controls.Add(this.listViewResource);
            this.splitContainerResource.Size = new System.Drawing.Size(901, 496);
            this.splitContainerResource.SplitterDistance = 300;
            this.splitContainerResource.TabIndex = 3;
            // 
            // tabControlResource
            // 
            this.tabControlResource.Controls.Add(this.tabPageFileSys);
            this.tabControlResource.Controls.Add(this.tabPageResFilter);
            this.tabControlResource.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlResource.ImageList = this.imageListTab;
            this.tabControlResource.Location = new System.Drawing.Point(0, 0);
            this.tabControlResource.Name = "tabControlResource";
            this.tabControlResource.SelectedIndex = 0;
            this.tabControlResource.Size = new System.Drawing.Size(298, 494);
            this.tabControlResource.TabIndex = 0;
            // 
            // tabPageFileSys
            // 
            this.tabPageFileSys.Controls.Add(this.treeViewFileSys);
            this.tabPageFileSys.ImageIndex = 0;
            this.tabPageFileSys.Location = new System.Drawing.Point(4, 23);
            this.tabPageFileSys.Name = "tabPageFileSys";
            this.tabPageFileSys.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageFileSys.Size = new System.Drawing.Size(290, 467);
            this.tabPageFileSys.TabIndex = 0;
            this.tabPageFileSys.Text = "FileSys";
            this.tabPageFileSys.UseVisualStyleBackColor = true;
            // 
            // treeViewFileSys
            // 
            this.treeViewFileSys.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewFileSys.ImageIndex = 0;
            this.treeViewFileSys.ImageList = this.imageListTree;
            this.treeViewFileSys.Location = new System.Drawing.Point(3, 3);
            this.treeViewFileSys.Name = "treeViewFileSys";
            this.treeViewFileSys.SelectedImageIndex = 0;
            this.treeViewFileSys.Size = new System.Drawing.Size(284, 461);
            this.treeViewFileSys.TabIndex = 0;
            this.treeViewFileSys.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeViewFileSys_AfterSelect);
            this.treeViewFileSys.Click += new System.EventHandler(this.treeViewFileSys_Click);
            // 
            // imageListTree
            // 
            this.imageListTree.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListTree.ImageStream")));
            this.imageListTree.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListTree.Images.SetKeyName(0, "folder.png");
            this.imageListTree.Images.SetKeyName(1, "folder_open.png");
            this.imageListTree.Images.SetKeyName(2, "heart.png");
            this.imageListTree.Images.SetKeyName(3, "package.png");
            // 
            // tabPageResFilter
            // 
            this.tabPageResFilter.Controls.Add(this.listFilter);
            this.tabPageResFilter.ImageIndex = 1;
            this.tabPageResFilter.Location = new System.Drawing.Point(4, 23);
            this.tabPageResFilter.Name = "tabPageResFilter";
            this.tabPageResFilter.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageResFilter.Size = new System.Drawing.Size(290, 467);
            this.tabPageResFilter.TabIndex = 1;
            this.tabPageResFilter.Text = "ResFilter";
            this.tabPageResFilter.UseVisualStyleBackColor = true;
            // 
            // listFilter
            // 
            this.listFilter.CheckOnClick = true;
            this.listFilter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listFilter.FormattingEnabled = true;
            this.listFilter.Items.AddRange(new object[] {
            "All",
            "Static Mesh",
            "Skeletal Mesh",
            "Morph Anim Mesh",
            "Texture"});
            this.listFilter.Location = new System.Drawing.Point(3, 3);
            this.listFilter.Name = "listFilter";
            this.listFilter.Size = new System.Drawing.Size(284, 452);
            this.listFilter.TabIndex = 0;
            this.listFilter.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.listFilter_ItemCheck);
            // 
            // imageListTab
            // 
            this.imageListTab.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListTab.ImageStream")));
            this.imageListTab.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListTab.Images.SetKeyName(0, "computer.gif");
            this.imageListTab.Images.SetKeyName(1, "drink.gif");
            // 
            // listViewResource
            // 
            this.listViewResource.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.listViewResource.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnFileName,
            this.columnFileSize,
            this.columnResType,
            this.columnDate});
            this.listViewResource.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewResource.GridLines = true;
            this.listViewResource.LargeImageList = this.imageListForList;
            this.listViewResource.Location = new System.Drawing.Point(0, 0);
            this.listViewResource.Name = "listViewResource";
            this.listViewResource.Size = new System.Drawing.Size(595, 494);
            this.listViewResource.TabIndex = 0;
            this.listViewResource.UseCompatibleStateImageBehavior = false;
            this.listViewResource.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listViewResource_MouseDoubleClick);
            this.listViewResource.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewResource_ItemSelectionChanged);
            this.listViewResource.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.listViewResource_KeyPress);
            // 
            // columnFileName
            // 
            this.columnFileName.Text = "Resource File";
            this.columnFileName.Width = 240;
            // 
            // columnFileSize
            // 
            this.columnFileSize.Text = "File Size";
            this.columnFileSize.Width = 120;
            // 
            // columnResType
            // 
            this.columnResType.Text = "Resource Type";
            this.columnResType.Width = 120;
            // 
            // columnDate
            // 
            this.columnDate.Text = "File Time";
            this.columnDate.Width = 120;
            // 
            // imageListForList
            // 
            this.imageListForList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListForList.ImageStream")));
            this.imageListForList.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListForList.Images.SetKeyName(0, "default");
            // 
            // imageListForListSmall
            // 
            this.imageListForListSmall.ColorDepth = System.Windows.Forms.ColorDepth.Depth24Bit;
            this.imageListForListSmall.ImageSize = new System.Drawing.Size(16, 16);
            this.imageListForListSmall.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // contextMenuStripTree
            // 
            this.contextMenuStripTree.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveCurrentLevelHereToolStripMenuItem,
            this.loadLevelToolStripMenuItem});
            this.contextMenuStripTree.Name = "contextMenuStripTree";
            this.contextMenuStripTree.Size = new System.Drawing.Size(204, 48);
            // 
            // saveCurrentLevelHereToolStripMenuItem
            // 
            this.saveCurrentLevelHereToolStripMenuItem.Name = "saveCurrentLevelHereToolStripMenuItem";
            this.saveCurrentLevelHereToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
            this.saveCurrentLevelHereToolStripMenuItem.Text = "Save Current Level here...";
            this.saveCurrentLevelHereToolStripMenuItem.Click += new System.EventHandler(this.saveCurrentLevelHereToolStripMenuItem_Click);
            // 
            // loadLevelToolStripMenuItem
            // 
            this.loadLevelToolStripMenuItem.Name = "loadLevelToolStripMenuItem";
            this.loadLevelToolStripMenuItem.Size = new System.Drawing.Size(203, 22);
            this.loadLevelToolStripMenuItem.Text = "Load Level ...";
            this.loadLevelToolStripMenuItem.Click += new System.EventHandler(this.loadLevelToolStripMenuItem_Click);
            // 
            // ResourceEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(901, 578);
            this.Controls.Add(this.splitContainerResource);
            this.Controls.Add(this.statusBarResource);
            this.Controls.Add(this.toolBarResource);
            this.Controls.Add(this.menuBarResource);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuBarResource;
            this.Name = "ResourceEditorForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Nexus Resource Manager";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ResourceEditorForm_FormClosing);
            this.menuBarResource.ResumeLayout(false);
            this.menuBarResource.PerformLayout();
            this.toolBarResource.ResumeLayout(false);
            this.toolBarResource.PerformLayout();
            this.splitContainerResource.Panel1.ResumeLayout(false);
            this.splitContainerResource.Panel2.ResumeLayout(false);
            this.splitContainerResource.ResumeLayout(false);
            this.tabControlResource.ResumeLayout(false);
            this.tabPageFileSys.ResumeLayout(false);
            this.tabPageResFilter.ResumeLayout(false);
            this.contextMenuStripTree.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuBarResource;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolBarResource;
        private System.Windows.Forms.StatusStrip statusBarResource;
        private System.Windows.Forms.SplitContainer splitContainerResource;
        private System.Windows.Forms.TabControl tabControlResource;
        private System.Windows.Forms.TabPage tabPageFileSys;
        private System.Windows.Forms.TabPage tabPageResFilter;
        private System.Windows.Forms.ListView listViewResource;
        private System.Windows.Forms.TreeView treeViewFileSys;
        private System.Windows.Forms.CheckedListBox listFilter;
        private System.Windows.Forms.ToolStripMenuItem resourceEditorsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showStaticMeshEditorToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripTree;
        private System.Windows.Forms.ToolStripMenuItem saveCurrentLevelHereToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadLevelToolStripMenuItem;
        private System.Windows.Forms.ImageList imageListTree;
        private System.Windows.Forms.ImageList imageListForList;
        private System.Windows.Forms.ToolStripButton toolStripButtonRefresh;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolStripButtonStaticMeshEd;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.ToolStripMenuItem thumbnialsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem iconsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem detailsToolStripMenuItem;
        private System.Windows.Forms.ImageList imageListTab;
        private System.Windows.Forms.ToolStripSplitButton toolStripSplitButton1;
        private System.Windows.Forms.ToolStripMenuItem nameToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem dateToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripTextBox toolTextSearch;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripMenuItem showAnimMeshEditorToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolBtnAnimMeshEd;
        private System.Windows.Forms.ImageList imageListForListSmall;
        private System.Windows.Forms.ColumnHeader columnFileName;
        private System.Windows.Forms.ColumnHeader columnFileSize;
        private System.Windows.Forms.ColumnHeader columnResType;
        private System.Windows.Forms.ColumnHeader columnDate;
    }
}