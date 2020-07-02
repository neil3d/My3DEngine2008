using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using NexusEngine;

namespace Nexus.GUI.Tester
{
    public class GUITesterMainForm : Form
    {
        private TesterRenderControl renderControl = new TesterRenderControl();
        // update timer
        private Timer updateTimer = new Timer();


        // 定义单件访问
        private static GUITesterMainForm _this = null;
        public static GUITesterMainForm App
        {
            get
            {
                if( _this == null )
                {
                    _this = new GUITesterMainForm();
                    return _this;
                }
                return _this;
            }
        }

        /// <summary>
        /// 获得一个窗口句柄用来初始化引擎
        /// </summary>
        /// <returns>窗口句柄</returns>
        public IntPtr RenderPanelHandle
        {
            get { return this.renderControl.Handle; }
        }

        public GUITesterMainForm()
        {
            InitializeComponent();

            renderControl.Location = new Point(0, 0);
            renderControl.Dock = DockStyle.Fill;
            renderControl.Visible = true;
        
            //-- 添加事件
            this.Load += new EventHandler(GUITesterMainForm_Load);
            this.Closing += new CancelEventHandler(GUITesterMainForm_Closing);
            this.Controls.Add(renderControl);
        }


        public void CreateViewport()
        {
            renderControl.CreateViewport();
        }

        /// <summary>
        /// When the form closes, close the refresh timer
        /// </summary>
        private void GUITesterMainForm_Closing(object sender, EventArgs e)
        {
            SelfDispose();
        }

        private void GUITesterMainForm_Load(object sender, EventArgs e)
        {
            updateTimer.Interval = 10;
            updateTimer.Tick += new EventHandler(updateTimer_Tick);
            updateTimer.Start();
        }

        /// <summary>
        /// When the timer fires, refresh control
        /// </summary>
        private void updateTimer_Tick(object sender, EventArgs e)
        {
            renderControl.Invalidate();
        }

        protected override void Dispose(bool disposing)
        {
            if( disposing )
            {
                SelfDispose();
            }
            base.Dispose(disposing);
        }

        /// <summary>
        /// 自定义的回收机
        /// </summary>
        protected virtual void SelfDispose()
        {
            if (updateTimer != null)
            {
                // close refresh timer
                updateTimer.Stop();
                updateTimer.Dispose();
                updateTimer = null;
            }
        }

        /// <summary>
        /// 设计器支持习惯
        /// </summary>
        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // GUITesterMainForm
            // 
            this.ClientSize = new System.Drawing.Size(800, 600);
            this.Name = "GUITesterMainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Nexus GUI Tester Framework";
            this.ResumeLayout(false);

        }
    }
}
