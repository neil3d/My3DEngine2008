namespace NexusEditor
{
    partial class TimelineStreamList
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
			m_Canvas.Dispose();
			m_FontFormat.Dispose();
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.m_Scrollbar = new System.Windows.Forms.VScrollBar();
            this.SuspendLayout();
            // 
            // m_Scrollbar
            // 
            this.m_Scrollbar.Location = new System.Drawing.Point(0, 0);
            this.m_Scrollbar.Name = "m_Scrollbar";
            this.m_Scrollbar.Size = new System.Drawing.Size(17, 80);
            this.m_Scrollbar.TabIndex = 0;
            this.m_Scrollbar.ValueChanged += new System.EventHandler(this.m_Scrollbar_ValueChanged);
            this.Controls.Add(m_Scrollbar);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.VScrollBar m_Scrollbar;
    }
}
