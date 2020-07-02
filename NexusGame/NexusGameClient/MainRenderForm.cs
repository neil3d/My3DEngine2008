using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using NexusEngine;
using NexusEngineExtension;
using Nexus.GameFramework;

namespace NexusGameClient
{
    public partial class MainRenderForm : Form
    {
        #region Fields
        private GameViewportControl viewportControl;

        #endregion

        #region Constructor
        public MainRenderForm()
        {
            InitializeComponent();
        }

        #endregion

        #region Properties
        /// <summary>
        /// 获得一个窗口句柄用来初始化引擎
        /// </summary>
        /// <returns>窗口句柄</returns>
        public IntPtr RenderHandle
        {
            get { return this.viewportControl.Handle; }
        }
        #endregion

        #region Methods

        private void InitializeComponent()
        {
            this.SuspendLayout();

            this.Name = "MainRenderForm";
            this.viewportControl = new GameViewportControl();
            viewportControl.Visible = true;
            this.Controls.Add(viewportControl);

            this.Load += new EventHandler(MainRenderForm_Load);
            this.Closing += new CancelEventHandler(MainRenderForm_Closing);

            this.ResumeLayout(false);
        }

        void MainRenderForm_Closing(object sender, CancelEventArgs e)
        {
        }

        void MainRenderForm_Load(object sender, EventArgs e)
        {
        }

        internal void CreateViewport(GameEngine gameEngine)
        {
            viewportControl.CreateViewport(gameEngine);
        }
        #endregion
    }
}
