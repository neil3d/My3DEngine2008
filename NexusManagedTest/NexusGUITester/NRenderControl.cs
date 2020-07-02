using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Reflection;
using System.IO;

using NexusEngine;

namespace Nexus.GUI.Tester
{
    /// <summary>
    /// 定义控件用于Nexus Engine Render
    /// </summary>
    public class NRenderControl : System.Windows.Forms.UserControl
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// window handle
        /// </summary>
        private uint m_HWND = 0;
        public uint HWND
        {
            get { return m_HWND; }
        }

        protected NViewport m_viewport;
        protected NGameTimer m_gameTimer;


        public NRenderControl()
        {
            InitializeComponent();

            // 关联event handle
            this.Load += new EventHandler(NRenderControl_Load);
            this.SizeChanged += new EventHandler(NRenderContorl_SizeChanged);
        }

        public void CreateViewport()
        {
            // initialize Viewport
            int w = NEditorEngine.Instance().Config.ClientWidth;
            int h = NEditorEngine.Instance().Config.ClientHeight;
            m_viewport = new NViewport();
            m_viewport.Camera.SetPerspective(MathConst.PI / 4, w, h, 20, 500000);
            m_viewport.HandleWnd = this.Handle;
            m_viewport.RenderMode = ERenderMode.Unlit;
            this.NRenderContorl_SizeChanged(null, null);

            // create render element
            NWidgetsRender widgetsRender = new NWidgetsRender();
            m_viewport.WidgetsRender = widgetsRender;
            widgetsRender.AddRenderControl(this);

            // 创建UI系统
            new GUISystem(widgetsRender.UICanvas);
            // 签署事件
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(NRenderControl_KeyDown);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(NRenderControl_KeyPress);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(NRenderControl_KeyUp);
            this.MouseClick += new System.Windows.Forms.MouseEventHandler(NRenderControl_MouseClick);
            this.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(NRenderControl_MouseDoubleClick);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(NRenderControl_MouseDown);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(NRenderControl_MouseUp);
            this.MouseWheel += new System.Windows.Forms.MouseEventHandler(NRenderControl_MouseWheel);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(NRenderControl_MouseMove);
            //加载程序集，注册内置窗口类型
            Assembly executingAssembly = Assembly.GetExecutingAssembly();
            WindowManager.Instance.LoadAssembly( Path.GetDirectoryName(executingAssembly.Location) + System.IO.Path.DirectorySeparatorChar + @"Nexus.GUI.dll");
            // 加载当前执行文件中窗口类型
            WindowManager.Instance.AttachAssembly( executingAssembly );
            // 创建一个缺省的唯一父窗口
            GUISystem.Instance.RootWindow = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.GUISheet", "root",null); 
           

            PostInitialize();

            // create game timer
            m_gameTimer = new NGameTimer();
            m_gameTimer.Reset();
        }

        public virtual void PostInitialize()
        {

        }

        #region  Dispose
        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (components != null)
                {
                    components.Dispose();
                }

                if (m_viewport != null)
                {
                    m_viewport.Destroy();
                    m_viewport = null;
                }
            }
            base.Dispose(disposing);
        }
        #endregion

        #region  Initialize Component
        /// <summary>
        /// 设计器支持所需的方法
        /// </summary>
        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // NRenderControl
            // 
            this.Name = "NRenderControl";
            //this.Size = new System.Drawing.Size(800, 600);
            this.ResumeLayout(false);

        }
        #endregion

        #region Event Subscription

        #region  Paint
        protected override void OnPaint(PaintEventArgs pe)
        {
            //base.OnPaint(pe);
            // do self render
            m_gameTimer.Tick();

            NGameEngine gameEngine = Program.g_GameEngine;
            if (gameEngine != null)
            {
                gameEngine.FrameTick(m_gameTimer.DeltaTime, m_viewport);
            }
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            // This overrides the System.Windows.Forms.Control protected method
            // "OnPaintBackground()" so that we don't clear the client area of
            // this form window -- so the viewport doesn't flicker on each frame.
        }

        /// <summary>
        /// Engine render element callback
        /// </summary>
        /// <param name="PDI"></param>
        virtual public void DrawWidgets(NRenderPrimitiveDrawInterface PDI)
        {
            // TODO self draw
        }

        /// <summary>
        /// 使用<see cref="NUICanvas"/>渲染2D界面
        /// </summary>
        /// <param name="Canvas"></param>
        virtual public void DrawWidgets(NUICanvas Canvas)
        {
            GUISystem.Instance.RenderGUI();
        }

        #endregion

        protected virtual void NRenderControl_Load(object sender, EventArgs e)
        {
            m_HWND = (uint)this.Handle.ToInt32();
        }

        protected virtual void NRenderContorl_SizeChanged(object sender, EventArgs e)
        {
            if (m_HWND == 0)
                return;

            m_viewport.Width = (uint)this.ClientSize.Width;
            m_viewport.Height = (uint)this.ClientSize.Height;
            NEngineConfig cfg = NGameEngine.Instance().Config;
            m_viewport.Camera.SetPerspective(MathConst.PI / 4, ClientSize.Width, ClientSize.Height, 20, 500000);
        }

        protected void NRenderControl_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            GUISystem.Instance.InjectKeyDown(e);
        }

        protected void NRenderControl_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
        {
            GUISystem.Instance.InjectKeyPress(e);
        }
        protected void NRenderControl_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            GUISystem.Instance.InjectKeyUp(e);
        }

        protected void NRenderControl_MouseClick(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseClicked(e);
        }

        protected void NRenderControl_MouseDoubleClick(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseDoubleClicked(e);
        }

        protected void NRenderControl_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseDown(e);
        }

        protected void NRenderControl_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseUp(e);
        }

        protected void NRenderControl_MouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseWheel(e);
        }

        protected void NRenderControl_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseMove(e);
        }
        #endregion

    }
}
