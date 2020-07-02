namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Drawing;
    using System.Globalization;
    using System.Linq;
    using System.Text;
    using System.Windows.Forms;

    using Nexus.GameFramework;
    using Nexus.GUI;

    using NexusEngine;

    /// <summary>
    /// Viewport渲染控件
    /// </summary>
    public class GameViewportControl : UserControl
    {
        #region Fields

        /// <summary>
        /// 显示当前调试信息
        /// </summary>
        public static HashSet<string> debugInfos = new HashSet<string>();

        /// <summary>
        /// 是否显示Debug信息
        /// </summary>
        private bool drawDebugInfo;

        /// <summary>
        /// Game Viewport
        /// </summary>
        private GameViewport gameViewport;

        #endregion Fields

        #region Constructors

        public GameViewportControl()
        {
            InitializeComponent();
            drawDebugInfo = true;
        }

        #endregion Constructors

        #region Properties

        public bool DrawDebugInfo
        {
            get { return drawDebugInfo; }
            set { drawDebugInfo = value; }
        }

        public GameViewport Viewport
        {
            get { return gameViewport; }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// 创建Viewport
        /// 在调用完GameEngine.InitModules之后调用
        /// </summary>
        public void CreateViewport()
        {
            //-- create default level
            GameEngine.EngineInstance.CreateDefaultLevel("defaultLevel");

            // create viewport
            gameViewport = new GameViewport();
            gameViewport.HandleWnd = this.Handle;
            gameViewport.RenderMode = ERenderMode.Unlit;
            this.GameViewportControl_SizeChanged(null, null);

            // Create render element
            GameWidgetsRender widgetsRender = new GameWidgetsRender();
            gameViewport.WidgetsRender = widgetsRender;
            widgetsRender.AddRenderControl(this);

            // inject input
            InjectInputEvent();

        }

        /// <summary>
        /// Engine render element callback
        /// </summary>
        /// <param name="PDI"></param>
        public virtual void DrawWidgets(NRenderPrimitiveDrawInterface pdi)
        {
            // self draw
            if(drawDebugInfo)
            {
                int yOffest = 5;
                foreach (string info in debugInfos)
                {
                    pdi.DrawDebugString(5, yOffest, info, new Color4ub(230, 180, 20, 255));

                    yOffest += 16;
                }

                //pdi.DrawDebugString(5, yOffest,
                //    String.Format("Camera: FOV:{0} Yaw:{1} Pitch:{2} Distance:{3} LookAt:{4}",
                //    MathHelper.ToDegrees(GameplayCamera.FOV).ToString(".00", CultureInfo.CurrentCulture),
                //    MathHelper.ToDegrees(GameplayCamera.Yaw).ToString(".00", CultureInfo.CurrentCulture),
                //    MathHelper.ToDegrees(GameplayCamera.Pitch).ToString(".00", CultureInfo.CurrentCulture),
                //    GameplayCamera.Distance.ToString(".00", CultureInfo.CurrentCulture),
                //    GameplayCamera.LookAt.ToString()),
                //    new Color4ub(230, 180, 20, 255));
            }
            debugInfos.Clear();
        }

        /// <summary>
        /// 使用<see cref="NUICanvas"/>渲染2D界面
        /// </summary>
        /// <param name="Canvas"></param>
        public virtual void DrawWidgets(NUICanvas canvas)
        {
            GUISystem.Instance.RenderGUI();
        }


        /// <summary>
        /// Performs one complete frame for the game.
        /// </summary>
        public void Tick(GameTime gameTime)
        {
            if (gameViewport != null)
            {
                float deltaTime = Math.Max(gameTime.ElapsedGameTime, gameTime.ElapsedRealTime);

                if (drawDebugInfo)
                {
                    debugInfos.Add( String.Format(CultureInfo.CurrentCulture,
                        "FPS:{0} deltaTime:{1}s \r\n",
                        gameTime.FramesPerSecond.ToString(".00", CultureInfo.CurrentCulture),
                        deltaTime.ToString(".00000", CultureInfo.CurrentCulture)));
                }
            }
        }

        /// <summary>
        /// 窗口大小发生变化
        /// </summary>
        protected virtual void GameViewportControl_SizeChanged(object sender, EventArgs e)
        {
            NEngineConfig eng = NEngine.Instance().Config;
            gameViewport.Width = (uint)Math.Min(eng.ClientWidth, this.ClientSize.Width);
            gameViewport.Height = (uint)Math.Min(eng.ClientHeight, this.ClientSize.Height);
            gameViewport.X = (uint)(eng.ClientWidth - gameViewport.Width) / 2;
            gameViewport.Y = (uint)(eng.ClientHeight - gameViewport.Height) / 2;
            gameViewport.Camera.SetPerspective(gameViewport.Camera.FOV,
                Math.Min(this.ClientSize.Width, eng.ClientWidth),
                Math.Min(this.ClientSize.Height, eng.ClientHeight),
                10, 50000);
            gameViewport.Update();

            Window rootWnd = GUISystem.Instance == null ? null : GUISystem.Instance.RootWindow;
            if (rootWnd != null)
            {
                rootWnd.AbsolutePosition = new Vector2(gameViewport.X, gameViewport.Y);
                rootWnd.AbsoluteSize = new Vector2(gameViewport.Width, gameViewport.Height);
            }
        }

        protected override bool IsInputKey(Keys keyData)
        {
            switch (keyData)
            {
                case Keys.Up:
                case Keys.Down:
                case Keys.Right:
                case Keys.Left:
                case Keys.Up | Keys.Shift:
                case Keys.Down | Keys.Shift:
                case Keys.Right | Keys.Shift:
                case Keys.Left | Keys.Shift:
                    return true;
            }
            return base.IsInputKey(keyData);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);
            // do self render
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            // This overrides the System.Windows.Forms.Control protected method
            // "OnPaintBackground()" so that we don't clear the client area of
            // this form window -- so the viewport doesn't flicker on each frame.
        }

        void GameViewportControl_KeyDown(object sender, KeyEventArgs e)
        {
            if (GUISystem.Instance.InjectKeyDown(e))
                return;

            if(e.KeyCode == Keys.F1)
            {
                gameViewport.RenderMode = ERenderMode.Wireframe;
            }
            else if (e.KeyCode == Keys.F2)
            {
                gameViewport.RenderMode = ERenderMode.Unlit;
            }
            else if(e.KeyCode == Keys.F3)
            {
                gameViewport.RenderMode = ERenderMode.Lit;
                gameViewport.EnableDynamicShadow = true;
            }
            else if (e.KeyCode == Keys.F4)
            {
                gameViewport.RenderMode = ERenderMode.Lit;
                gameViewport.EnableDynamicShadow = false;
            }
            else if (e.KeyCode == Keys.F9)
            {
                PrintViewport();
            }
        }

        //public void ScreenCapture(string fileName)
        //{
        //    using (BackgroundWorker worker = new BackgroundWorker())
        //    {
        //        Thread.Sleep(0);
        //        this.Refresh();
        //        worker.DoWork += delegate(object sender, DoWorkEventArgs e)
        //        {
        //            BackgroundWorker wkr = sender as BackgroundWorker;
        //            Rectangle bounds = new Rectangle(Location, Size);
        //            Thread.Sleep(300);
        //            Bitmap bitmap = new Bitmap(bounds.Width, bounds.Height);
        //            using (Graphics g = Graphics.FromImage(bitmap))
        //            {
        //                g.CopyFromScreen(Location, Point.Empty, bounds.Size);
        //            }
        //            e.Result = bitmap;
        //        };
        //        worker.RunWorkerCompleted += delegate(object sender, RunWorkerCompletedEventArgs e)
        //        {
        //            if (e.Result != null && e.Result is Bitmap)
        //            {
        //                Bitmap bitmap = (Bitmap)e.Result;
        //                bitmap.Save(fileName, ImageFormat.Jpeg);
        //            }
        //        };
        //        worker.RunWorkerAsync();
        //    }
        //}

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (((Keys)(int)msg.WParam) == Keys.PrintScreen)
            {
                PrintViewport();
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        protected override bool ProcessKeyEventArgs(ref Message msg)
        {
            if (((Keys)(int)msg.WParam) == Keys.PrintScreen)
            {
                PrintViewport();
            }
            return base.ProcessKeyEventArgs(ref msg);
        }


        /// <summary>
        /// 截屏功能
        /// </summary>
        public static void PrintViewport()
        {
            // 截屏功能
            GameEngine.EngineInstance.ViewportShot(string.Format(System.Globalization.CultureInfo.CurrentCulture,
                @"{0}/screenshots/Screen_{1}.jpg",
                GameEngine.EngineInstance.Config.FileSystemRoot, DateTime.Now.ToFileTime()));
        }

        void GameViewportControl_KeyPress(object sender, KeyPressEventArgs e)
        {
            GUISystem.Instance.InjectKeyPress(e);
        }

        void GameViewportControl_KeyUp(object sender, KeyEventArgs e)
        {
            GUISystem.Instance.InjectKeyUp(e);
        }

        void GameViewportControl_MouseClick(object sender, MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseClicked(e);
        }

        void GameViewportControl_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseDoubleClicked(e);
        }

        void GameViewportControl_MouseDown(object sender, MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseDown(e);
        }

        void GameViewportControl_MouseMove(object sender, MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseMove(e);
        }

        void GameViewportControl_MouseUp(object sender, MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseUp(e);
        }

        void GameViewportControl_MouseWheel(object sender, MouseEventArgs e)
        {
            GUISystem.Instance.InjectMouseWheel(e);
        }

        private void InitializeComponent()
        {
            this.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ResizeRedraw = true;
            this.SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint, true);
        }

        private void InjectInputEvent()
        {
            this.SizeChanged += new EventHandler(GameViewportControl_SizeChanged);

            this.KeyPress += new KeyPressEventHandler(GameViewportControl_KeyPress);
            this.KeyDown += new KeyEventHandler(GameViewportControl_KeyDown);
            this.KeyUp += new KeyEventHandler(GameViewportControl_KeyUp);

            this.MouseClick += new MouseEventHandler(GameViewportControl_MouseClick);
            this.MouseDoubleClick += new MouseEventHandler(GameViewportControl_MouseDoubleClick);
            this.MouseDown += new MouseEventHandler(GameViewportControl_MouseDown);
            this.MouseUp += new MouseEventHandler(GameViewportControl_MouseUp);
            this.MouseWheel += new MouseEventHandler(GameViewportControl_MouseWheel);
            this.MouseMove += new MouseEventHandler(GameViewportControl_MouseMove);
        }

        #endregion Methods
    }
}