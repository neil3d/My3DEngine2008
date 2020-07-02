namespace NexusEditor
{
    partial class Timeline
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
            this.m_Canvas = new NexusEditor.TimeGrid();
            this.m_ItemList = new NexusEditor.TimelineStreamList();
            this.SuspendLayout();
            // 
            // m_Canvas
            // 
            this.m_Canvas.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.m_Canvas.ForeColor = System.Drawing.Color.Gray;
            this.m_Canvas.Location = new System.Drawing.Point(175, 3);
            this.m_Canvas.Name = "m_Canvas";
            this.m_Canvas.Size = new System.Drawing.Size(472, 290);
            this.m_Canvas.TabIndex = 1;
            // 
            // m_ItemList
            // 
            this.m_ItemList.BackColor = System.Drawing.Color.Gray;
            this.m_ItemList.Dock = System.Windows.Forms.DockStyle.Left;
            this.m_ItemList.ForeColor = System.Drawing.Color.White;
            this.m_ItemList.ItemHeight = 40;
            this.m_ItemList.Location = new System.Drawing.Point(0, 0);
            this.m_ItemList.Name = "m_ItemList";
            this.m_ItemList.SelectedItem = null;
            this.m_ItemList.ShadowColor = System.Drawing.Color.Black;
            this.m_ItemList.Size = new System.Drawing.Size(169, 296);
            this.m_ItemList.TabIndex = 0;
            this.m_ItemList.Text = "timelineStreamList1";
            this.m_ItemList.Click += new System.EventHandler(this.m_ItemListClick);
            // 
            // Timeline
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_Canvas);
            this.Controls.Add(this.m_ItemList);
            this.Name = "Timeline";
            this.Size = new System.Drawing.Size(647, 296);
            this.ResumeLayout(false);

        }

        #endregion

        private TimelineStreamList m_ItemList;
        private TimeGrid m_Canvas;
    }
}
