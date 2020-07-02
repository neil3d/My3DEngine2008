namespace NexusEditor.ResourceEditor
{
	partial class DistributionEditor
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
			System.Windows.Forms.Label label1;
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.groupBoxObject = new System.Windows.Forms.GroupBox();
			this.propertyGridDist = new System.Windows.Forms.PropertyGrid();
			this.btnResetPropertyType = new System.Windows.Forms.Button();
			this.comboBoxType = new System.Windows.Forms.ComboBox();
			this.m_Timeline = new NexusEditor.Timeline();
			label1 = new System.Windows.Forms.Label();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.groupBoxObject.SuspendLayout();
			this.SuspendLayout();
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.splitContainer1.IsSplitterFixed = true;
			this.splitContainer1.Location = new System.Drawing.Point(0, 0);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.groupBoxObject);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.m_Timeline);
			this.splitContainer1.Size = new System.Drawing.Size(1051, 582);
			this.splitContainer1.SplitterDistance = 300;
			this.splitContainer1.TabIndex = 0;
			// 
			// groupBoxObject
			// 
			this.groupBoxObject.Controls.Add(this.propertyGridDist);
			this.groupBoxObject.Controls.Add(this.btnResetPropertyType);
			this.groupBoxObject.Controls.Add(this.comboBoxType);
			this.groupBoxObject.Controls.Add(label1);
			this.groupBoxObject.Dock = System.Windows.Forms.DockStyle.Fill;
			this.groupBoxObject.Location = new System.Drawing.Point(0, 0);
			this.groupBoxObject.Name = "groupBoxObject";
			this.groupBoxObject.Size = new System.Drawing.Size(300, 582);
			this.groupBoxObject.TabIndex = 2;
			this.groupBoxObject.TabStop = false;
			this.groupBoxObject.Text = "Distribution Object";
			// 
			// propertyGridDist
			// 
			this.propertyGridDist.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)));
			this.propertyGridDist.HelpVisible = false;
			this.propertyGridDist.Location = new System.Drawing.Point(9, 49);
			this.propertyGridDist.Name = "propertyGridDist";
			this.propertyGridDist.Size = new System.Drawing.Size(285, 521);
			this.propertyGridDist.TabIndex = 3;
			this.propertyGridDist.ToolbarVisible = false;
			// 
			// btnResetPropertyType
			// 
			this.btnResetPropertyType.Location = new System.Drawing.Point(245, 19);
			this.btnResetPropertyType.Name = "btnResetPropertyType";
			this.btnResetPropertyType.Size = new System.Drawing.Size(49, 23);
			this.btnResetPropertyType.TabIndex = 2;
			this.btnResetPropertyType.Text = "reset";
			this.btnResetPropertyType.UseVisualStyleBackColor = true;
			this.btnResetPropertyType.Click += new System.EventHandler(this.btnResetPropertyType_Click);
			// 
			// comboBoxType
			// 
			this.comboBoxType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxType.FormattingEnabled = true;
			this.comboBoxType.Location = new System.Drawing.Point(42, 20);
			this.comboBoxType.Name = "comboBoxType";
			this.comboBoxType.Size = new System.Drawing.Size(190, 20);
			this.comboBoxType.TabIndex = 1;
			// 
			// label1
			// 
			label1.AutoSize = true;
			label1.Location = new System.Drawing.Point(7, 24);
			label1.Name = "label1";
			label1.Size = new System.Drawing.Size(29, 12);
			label1.TabIndex = 0;
			label1.Text = "Type";
			// 
			// m_Timeline
			// 
			this.m_Timeline.AutoScroll = true;
			this.m_Timeline.AutoSize = true;
			this.m_Timeline.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
			this.m_Timeline.Dock = System.Windows.Forms.DockStyle.Fill;
			this.m_Timeline.Location = new System.Drawing.Point(0, 0);
			this.m_Timeline.Name = "m_Timeline";
			this.m_Timeline.Size = new System.Drawing.Size(747, 582);
			this.m_Timeline.TabIndex = 1;
			this.m_Timeline.TooltipDelegate = null;
			// 
			// DistributionEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1051, 582);
			this.Controls.Add(this.splitContainer1);
			this.Name = "DistributionEditor";
			this.Text = "曲线编辑器";
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.Panel2.PerformLayout();
			this.splitContainer1.ResumeLayout(false);
			this.groupBoxObject.ResumeLayout(false);
			this.groupBoxObject.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer splitContainer1;
		private Timeline m_Timeline;
		private System.Windows.Forms.GroupBox groupBoxObject;
		private System.Windows.Forms.PropertyGrid propertyGridDist;
		private System.Windows.Forms.Button btnResetPropertyType;
		private System.Windows.Forms.ComboBox comboBoxType;


	}
}