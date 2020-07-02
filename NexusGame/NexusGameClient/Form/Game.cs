namespace NexusGameClient
{
    using System;
    using System.Collections.ObjectModel;
    using System.ComponentModel;
    using System.Configuration;
    using System.Drawing;
    using System.Globalization;
    using System.IO;
    using System.Threading;
    using System.Windows.Forms;

    using Nexus.GameFramework;

    using NexusEngine;

    using NexusEngineExtension;

    using NexusGameClient.Properties;

    /// <summary>
    /// Presents an easy to use wrapper for making games
    /// </summary>
    public abstract class Game : IDisposable
    {
        #region Fields

        GameClock clock = new GameClock();
        bool forceElapsedTimeToZero;
        GameTime gameTime = new GameTime();
        TimeSpan inactiveSleepTime = TimeSpan.FromMilliseconds(20.0);
        TimeSpan lastFrameElapsedGameTime;
        TimeSpan lastFrameElapsedRealTime;
        long lastUpdateFrame;
        float lastUpdateTime;
        TimeSpan maximumElapsedTime = TimeSpan.FromMilliseconds(500.0);
        TimeSpan targetElapsedTime = TimeSpan.FromTicks(166667);
        TimeSpan totalGameTime;

        #endregion Fields

        #region Constructors

        /// <summary>
        /// Initializes the <see cref="Game"/> class.
        /// </summary>
        static Game()
        {
            // setup the application
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Game"/> class.
        /// </summary>
        protected Game()
        {
            GameWindow = new GameWindow();
            GameWindow.ApplicationActivated += Window_ApplicationActivated;
            GameWindow.ApplicationDeactivated += Window_ApplicationDeactivated;
            GameWindow.Suspend += Window_Suspend;
            GameWindow.Resume += Window_Resume;
            GameWindow.Paint += Window_Paint;
        }

        #endregion Constructors

        #region Events

        /// <summary>
        /// Occurs when the game is activated.
        /// </summary>
        public event EventHandler Activated;

        /// <summary>
        /// Occurs when the game is deactivated.
        /// </summary>
        public event EventHandler Deactivated;

        /// <summary>
        /// Occurs when the game is disposed.
        /// </summary>
        public event EventHandler Disposed;

        /// <summary>
        /// Occurs when the game is exiting.
        /// </summary>
        public event EventHandler Exiting;

        /// <summary>
        /// Occurs when a drawing frame ends.
        /// </summary>
        public event EventHandler FrameEnd;

        /// <summary>
        /// Occurs when a drawing frame is about to start.
        /// </summary>
        public event CancelEventHandler FrameStart;

        #endregion Events

        #region Properties

        public static GameEngine gameEngine
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets the game window.
        /// </summary>
        /// <value>The game window.</value>
        public GameWindow GameWindow
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets or sets the inactive sleep time.
        /// </summary>
        /// <value>The inactive sleep time.</value>
        public TimeSpan InactiveSleepTime
        {
            get { return inactiveSleepTime; }
            set
            {
                // error checking
                if (value < TimeSpan.Zero)
                    throw new ArgumentOutOfRangeException("value", "Inactive sleep time cannot be less than zero.");
                inactiveSleepTime = value;
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether this <see cref="Game"/> is active.
        /// </summary>
        /// <value><c>true</c> if active; otherwise, <c>false</c>.</value>
        public bool IsActive
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets a value indicating whether this <see cref="Game"/> is exiting.
        /// </summary>
        /// <value><c>true</c> if exiting; otherwise, <c>false</c>.</value>
        public bool IsExiting
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets or sets a value indicating whether this instance is running.
        /// </summary>
        /// <value>
        /// <c>true</c> if this instance is running; otherwise, <c>false</c>.
        /// </value>
        public bool IsRunning
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets or sets the target elapsed time.
        /// </summary>
        /// <value>The target elapsed time.</value>
        public TimeSpan TargetElapsedTime
        {
            get { return targetElapsedTime; }
            set
            {
                // error checking
                if (value <= TimeSpan.Zero)
                    throw new ArgumentOutOfRangeException("value", "Target elapsed time must be greater than zero.");
                targetElapsedTime = value;
            }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        public void Dispose()
        {
            if (Disposed != null)
                Disposed(this, EventArgs.Empty);

            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Exits the game.
        /// </summary>
        public void Exit()
        {
            // request the game to terminate
            IsExiting = true;
        }

        /// <summary>
        /// Resets the elapsed time.
        /// </summary>
        public void ResetElapsedTime()
        {
            forceElapsedTimeToZero = true;
        }

        /// <summary>
        /// Runs the game.
        /// </summary>
        public void Run()
        {
            IsRunning = true;

            try
            {
                // 创建log
                NLogger.Instance.AddLogOutput(new NFileLogOutput("NexusGameClient.log"));

                SplashForm.StartSplash(Resources.EditorSplash);

                // 初始化本地化信息
                InitCultureInfo();

                // 初始化Nexus Engine
                InitGameEngine();

                // 引擎初始化完成
                PostInitGameEngine();

                SplashForm.CloseSplash();

                gameTime.ElapsedGameTime = 0;
                gameTime.ElapsedRealTime = 0;
                gameTime.TotalGameTime = (float)totalGameTime.TotalSeconds;
                gameTime.TotalRealTime = (float)clock.CurrentTime.TotalSeconds;

                Update(gameTime);

                Application.Idle += Application_Idle;
                Application.Run(GameWindow);
            }
            catch (System.Exception e)
            {
                Program.ShowException(e, "Create Default Level Exception!");
            }
            finally
            {
                Application.Idle -= Application_Idle;
                gameEngine.Close();
                gameEngine.Dispose();
                NLogger.Instance.Clear();
                IsRunning = false;
                OnExiting(EventArgs.Empty);
            }
        }

        /// <summary>
        /// Performs one complete frame for the game.
        /// </summary>
        public void Tick()
        {
            // if we are exiting, do nothing
            if (IsExiting)
                return;

            //// if we are inactive, sleep for a bit
            //if (!IsActive)
            //    Thread.Sleep((int)InactiveSleepTime.TotalMilliseconds);

            clock.Step();

            gameTime.TotalRealTime = (float)clock.CurrentTime.TotalSeconds;
            gameTime.ElapsedRealTime = (float)clock.ElapsedTime.TotalSeconds;
            lastFrameElapsedRealTime += clock.ElapsedTime;
            TimeSpan elapsedAdjustedTime = clock.ElapsedAdjustedTime;
            if (elapsedAdjustedTime < TimeSpan.Zero)
                elapsedAdjustedTime = TimeSpan.Zero;

            if (forceElapsedTimeToZero)
            {
                gameTime.ElapsedRealTime = 0;
                lastFrameElapsedRealTime = elapsedAdjustedTime = TimeSpan.Zero;
                forceElapsedTimeToZero = false;
            }

            // cap the adjusted time
            if (elapsedAdjustedTime > maximumElapsedTime)
                elapsedAdjustedTime = maximumElapsedTime;

            // make sure we shouldn't be exiting
            if (!IsExiting)
            {
                try
                {
                    gameTime.ElapsedGameTime = 0;
                    lastFrameElapsedGameTime = elapsedAdjustedTime;
                    gameTime.TotalGameTime = (float)totalGameTime.TotalSeconds;

                    Update(gameTime);

                    DrawFrame();
                }
                finally
                {
                    totalGameTime += elapsedAdjustedTime;
                }
            }

            // refresh the FPS counter once per second
            lastUpdateFrame++;
            if ((float)clock.CurrentTime.TotalSeconds - lastUpdateTime > 1.0f)
            {
                gameTime.FramesPerSecond = (float)lastUpdateFrame / (float)(clock.CurrentTime.TotalSeconds - lastUpdateTime);
                lastUpdateTime = (float)clock.CurrentTime.TotalSeconds;
                lastUpdateFrame = 0;
            }
        }

        /// <summary>
        /// Releases unmanaged and - optionally - managed resources
        /// </summary>
        /// <param name="disposeManagedResources"><c>true</c> to release both managed and unmanaged resources; <c>false</c> to release only unmanaged resources.</param>
        protected internal virtual void Dispose(bool disposeManagedResources)
        {
        }

        /// <summary>
        /// Called when a frame is ready to be drawn.
        /// </summary>
        /// <param name="gameTime">The time passed since the last frame.</param>
        protected virtual void Draw(GameTime deltaGameTime)
        {
            if (gameEngine != null)
            {
                gameEngine.Render(GameWindow.ViewportControl.Viewport);
            }
        }

        /// <summary>
        /// Raises the <see cref="E:Activated"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnActivated(EventArgs e)
        {
            if (Activated != null)
                Activated(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:Deactivated"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnDeactivated(EventArgs e)
        {
            if (Deactivated != null)
                Deactivated(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:Exiting"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnExiting(EventArgs e)
        {
            if (Exiting != null)
                Exiting(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:FrameEnd"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnFrameEnd(EventArgs e)
        {
            if (FrameEnd != null)
                FrameEnd(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:FrameStart"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.ComponentModel.CancelEventArgs"/> instance containing the event data.</param>
        protected virtual void OnFrameStart(CancelEventArgs e)
        {
            if (FrameStart != null)
                FrameStart(this, e);
        }

        protected virtual void PostInitGameEngine()
        {
        }

        /// <summary>
        /// Allows the game to perform logic processing.
        /// </summary>
        /// <param name="gameTime">The time passed since the last update.</param>
        protected virtual void Update(GameTime deltaGameTime)
        {
            float deltaTime = Math.Max(deltaGameTime.ElapsedGameTime, deltaGameTime.ElapsedRealTime);

            GameWindow.ViewportControl.Tick(deltaGameTime);

            gameEngine.Update(deltaTime, GameWindow.ViewportControl.Viewport);
        }

        void Application_Idle(object sender, EventArgs e)
        {
            NativeMessage message;
            while (!NativeMethods.PeekMessage(out message, IntPtr.Zero, 0, 0, 0))
            {
                if (IsExiting)
                    GameWindow.Close();
                else
                    Tick();
            }
        }

        void DrawFrame()
        {
            try
            {
                if (!IsExiting && !GameWindow.IsMinimized)
                {
                    CancelEventArgs e = new CancelEventArgs(false);
                    OnFrameStart(e);
                    if (!e.Cancel)
                    {
                        gameTime.TotalRealTime = (float)clock.CurrentTime.TotalSeconds;
                        gameTime.ElapsedRealTime = (float)lastFrameElapsedRealTime.TotalSeconds;
                        gameTime.TotalGameTime = (float)totalGameTime.TotalSeconds;
                        gameTime.ElapsedGameTime = (float)lastFrameElapsedGameTime.TotalSeconds;

                        Draw(gameTime);

                        OnFrameEnd(EventArgs.Empty);
                    }
                }
            }
            finally
            {
                lastFrameElapsedGameTime = TimeSpan.Zero;
                lastFrameElapsedRealTime = TimeSpan.Zero;
            }
        }

        private void InitCultureInfo()
        {
            try
            {
                // 读取本地化配置
                Resources.Culture = new CultureInfo(ConfigurationManager.AppSettings["Localization"]);
            }
            catch (System.Exception e)
            {
                Program.ShowException(e, "Culture Info Config error!");
            }
        }

        private void InitGameEngine()
        {
            // 初始化引擎
            gameEngine = new GameEngine();
            gameEngine.InitCore();

            //-- create engine config
            CustomConfigurationSection configuration = CustomConfigurationSection.Open();
            GameWindow.ClientSize = new Size(configuration.EngineSetting.ClientWidth, configuration.EngineSetting.ClientHeight);
            NEngineConfig engineCfg = new NEngineConfig();
            engineCfg.RenderWndHandle = GameWindow.RenderHandle;
            engineCfg.ClientWidth = Screen.PrimaryScreen.Bounds.Width;
            engineCfg.ClientHeight = Screen.PrimaryScreen.Bounds.Height;
            engineCfg.ColorBits = configuration.EngineSetting.ColorBits;
            engineCfg.FullScreen = configuration.EngineSetting.FullScreen;
            engineCfg.EnableHDR = configuration.EngineSetting.EnableHDR;
            engineCfg.EnableSSAO = configuration.EngineSetting.EnableSSAO;
            engineCfg.RenderClass = configuration.EngineSetting.RenderClass;
            engineCfg.FileSystemClass = configuration.EngineSetting.FileSystemClass;
            engineCfg.EngineDataPkg = configuration.EngineSetting.EngineDataPkg;
            engineCfg.ResourceCacheClass = configuration.EngineSetting.ResourceCacheClass;
            engineCfg.ResourceIOClass = configuration.EngineSetting.ResourceIOClass;
            engineCfg.FileSystemRoot = configuration.EngineSetting.FileSystemRoot;
            if (string.IsNullOrEmpty(engineCfg.FileSystemRoot))
            {
                engineCfg.FileSystemRoot = NFileSystem.DefaultFileSystemRoot;
            }

            configuration.Save();

            //初始化引擎
            gameEngine.InitModules(engineCfg);

            //-- create editor windows
            GameWindow.ViewportControl.CreateViewport();
        }

        void Window_ApplicationActivated(object sender, EventArgs e)
        {
            if (!IsActive)
            {
                IsActive = true;
                OnActivated(EventArgs.Empty);
            }
        }

        void Window_ApplicationDeactivated(object sender, EventArgs e)
        {
            if (IsActive)
            {
                IsActive = false;
                OnDeactivated(EventArgs.Empty);
            }
        }

        void Window_Paint(object sender, PaintEventArgs e)
        {
            DrawFrame();
        }

        void Window_Resume(object sender, EventArgs e)
        {
            clock.Resume();
        }

        void Window_Suspend(object sender, EventArgs e)
        {
            clock.Suspend();
        }

        #endregion Methods
    }
}