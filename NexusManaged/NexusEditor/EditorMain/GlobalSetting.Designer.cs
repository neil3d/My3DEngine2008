namespace NexusEditor.EditorMain
{
	partial class GlobalSettingForm
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
			this.m_PropertyGrid = new System.Windows.Forms.PropertyGrid();
			this.m_List = new System.Windows.Forms.TreeView();
			this.SuspendLayout();
			// 
			// m_PropertyGrid
			// 
			this.m_PropertyGrid.LineColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
			this.m_PropertyGrid.Location = new System.Drawing.Point(200, 0);
			this.m_PropertyGrid.Name = "m_PropertyGrid";
			this.m_PropertyGrid.Size = new System.Drawing.Size(350, 482);
			this.m_PropertyGrid.TabIndex = 1;
			// 
			// m_List
			// 
			this.m_List.Location = new System.Drawing.Point(-1, 0);
			this.m_List.Name = "m_List";
			this.m_List.Size = new System.Drawing.Size(195, 482);
			this.m_List.TabIndex = 2;
			this.m_List.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.m_List_AfterSelect);
			// 
			// GlobalSettingForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(552, 484);
			this.Controls.Add(this.m_List);
			this.Controls.Add(this.m_PropertyGrid);
			this.Name = "GlobalSettingForm";
			this.Text = "全局效果设置";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.GlobalSettingForm_FormClosing);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.PropertyGrid m_PropertyGrid;
		private System.Windows.Forms.TreeView m_List;


	}
}