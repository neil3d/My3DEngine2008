namespace NexusGameClient
{
    using System;
    using System.ComponentModel;
    using System.Drawing;
    using System.IO;
    using System.Reflection;
    using System.Security.Permissions;
    using System.Windows.Forms;

    using Nexus.GameFramework;

    using NexusEngineExtension;


    /// <summary>
    /// Implements a specialized window for games and rendering.
    /// </summary>
    public class GameWindow : Form
    {
        #region Fields

        const string DefaultTitle = "Nexus Game Client";

        Size cachedSize;
        bool inSizeMove;
        bool maximized;
        bool minimized;
        private Screen screen;
        private GameViewportControl viewportControl;

        #endregion Fields

        #region Constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="GameWindow"/> class.
        /// </summary>
        public GameWindow()
        {
            InitializeComponent();
        }

        #endregion Constructors

        #region Events

        /// <summary>
        /// Occurs when the application is activated.
        /// </summary>
        public event EventHandler ApplicationActivated;

        /// <summary>
        /// Occurs when the application is deactivated.
        /// </summary>
        public event EventHandler ApplicationDeactivated;

        /// <summary>
        /// Occurs when the application is resumed.
        /// </summary>
        public event EventHandler Resume;

        /// <summary>
        /// Occurs when the screen on which the window resides is changed.
        /// </summary>
        public event EventHandler ScreenChanged;

        /// <summary>
        /// Occurs when a screen saver is about to be activated.
        /// </summary>
        public event CancelEventHandler Screensaver;

        /// <summary>
        /// Occurs when the application is suspended.
        /// </summary>
        public event EventHandler Suspend;

        /// <summary>
        /// Occurs when the system is resumed.
        /// </summary>
        public event EventHandler SystemResume;

        /// <summary>
        /// Occurs when the system is suspended.
        /// </summary>
        public event EventHandler SystemSuspend;

        /// <summary>
        /// Occurs when the user resizes the window.
        /// </summary>
        public event EventHandler UserResized;

        #endregion Events

        #region Properties

        /// <summary>
        /// Gets a value indicating whether this instance is maximized.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance is maximized; otherwise, <c>false</c>.
        /// </value>
        public bool IsMaximized
        {
            get { return maximized; }
        }

        /// <summary>
        /// Gets a value indicating whether this instance is minimized.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance is minimized; otherwise, <c>false</c>.
        /// </value>
        public bool IsMinimized
        {
            get { return minimized; }
        }

        /// <summary>
        /// 获得一个窗口句柄用来初始化引擎
        /// </summary>
        /// <returns>窗口句柄</returns>
        public IntPtr RenderHandle
        {
            get { return this.viewportControl.Handle; }
        }

        /// <summary>
        /// Gets the screen on which the window resides.
        /// </summary>
        /// <value>The screen.</value>
        public Screen Screen
        {
            get { return screen; }
            private set { screen = value; }
        }

        public GameViewportControl ViewportControl
        {
            get { return viewportControl; }
        }

        #endregion Properties

        #region Methods

        public Icon GetDefaultIcon()
        {
            return Properties.Resources.MainFormIcon;
        }

        /// <summary>
        /// Raises the <see cref="E:ApplicationActivated"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnApplicationActivated(EventArgs e)
        {
            if (ApplicationActivated != null)
                ApplicationActivated(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:ApplicationDeactivated"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnApplicationDeactivated(EventArgs e)
        {
            if (ApplicationDeactivated != null)
                ApplicationDeactivated(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:System.Windows.Forms.Form.Load"/> event.
        /// </summary>
        /// <param name="e">An <see cref="T:System.EventArgs"/> that contains the event data.</param>
        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            cachedSize = Size;
        }

        /// <summary>
        /// Raises the <see cref="E:System.Windows.Forms.Form.ResizeBegin"/> event.
        /// </summary>
        /// <param name="e">A <see cref="T:System.EventArgs"/> that contains the event data.</param>
        protected override void OnResizeBegin(EventArgs e)
        {
            base.OnResizeBegin(e);

            // suspend any processing until we are done being minimized
            inSizeMove = true;
            cachedSize = Size;
            OnSuspend(EventArgs.Empty);
        }

        /// <summary>
        /// Raises the <see cref="E:System.Windows.Forms.Form.ResizeEnd"/> event.
        /// </summary>
        /// <param name="e">A <see cref="T:System.EventArgs"/> that contains the event data.</param>
        protected override void OnResizeEnd(EventArgs e)
        {
            base.OnResizeEnd(e);

            // check for screen and size changes
            OnUserResized(EventArgs.Empty);
            UpdateScreen();
            inSizeMove = false;

            // resume application processing
            OnResume(EventArgs.Empty);
        }

        /// <summary>
        /// Raises the <see cref="E:Resume"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnResume(EventArgs e)
        {
            if (Resume != null)
                Resume(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:ScreenChanged"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnScreenChanged(EventArgs e)
        {
            if (ScreenChanged != null)
                ScreenChanged(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:Screensaver"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.ComponentModel.CancelEventArgs"/> instance containing the event data.</param>
        protected virtual void OnScreensaver(CancelEventArgs e)
        {
            if (Screensaver != null)
                Screensaver(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:Suspend"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnSuspend(EventArgs e)
        {
            if (Suspend != null)
                Suspend(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:SystemResume"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnSystemResume(EventArgs e)
        {
            if (SystemResume != null)
                SystemResume(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:SystemSuspend"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnSystemSuspend(EventArgs e)
        {
            if (SystemSuspend != null)
                SystemSuspend(this, e);
        }

        /// <summary>
        /// Raises the <see cref="E:UserResized"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        protected virtual void OnUserResized(EventArgs e)
        {
            if (UserResized != null)
                UserResized(this, e);
        }

        /// <summary>
        /// Handles raw window messages.
        /// </summary>
        /// <param name="m">The raw message.</param>
        [SecurityPermission(SecurityAction.LinkDemand, Flags = SecurityPermissionFlag.UnmanagedCode)]
        protected override void WndProc(ref Message m)
        {
            if (m.Msg == WindowConstants.WM_SIZE)
            {
                if (m.WParam == WindowConstants.SIZE_MINIMIZED)
                {
                    minimized = true;
                    maximized = false;

                    OnSuspend(EventArgs.Empty);
                }
                else
                {
                    Rectangle client = NativeMethods.GetClientRectangle(m.HWnd);
                    if (client.Height == 0)
                    {
                        // rapidly clicking the task bar to minimize and restore a window
                        // can cause a WM_SIZE message with SIZE_RESTORED when
                        // the window has actually become minimized due to rapid change
                        // so just ignore this message
                    }
                    else if (m.WParam == WindowConstants.SIZE_MAXIMIZED)
                    {
                        if (minimized)
                            OnResume(EventArgs.Empty);

                        minimized = false;
                        maximized = true;

                        OnUserResized(EventArgs.Empty);
                        UpdateScreen();
                    }
                    else if (m.WParam == WindowConstants.SIZE_RESTORED)
                    {
                        if (minimized)
                            OnResume(EventArgs.Empty);

                        minimized = false;
                        maximized = false;

                        if (!inSizeMove && Size != cachedSize)
                        {
                            OnUserResized(EventArgs.Empty);
                            UpdateScreen();
                            cachedSize = Size;
                        }
                    }
                }
            }
            else if (m.Msg == WindowConstants.WM_ACTIVATEAPP)
            {
                if (m.WParam != IntPtr.Zero)
                    OnApplicationActivated(EventArgs.Empty);
                else
                    OnApplicationDeactivated(EventArgs.Empty);
            }
            else if (m.Msg == WindowConstants.WM_POWERBROADCAST)
            {
                if (m.WParam == WindowConstants.PBT_APMQUERYSUSPEND)
                {
                    OnSystemSuspend(EventArgs.Empty);
                    m.Result = (IntPtr)1;
                    return;
                }
                else if (m.WParam == WindowConstants.PBT_APMRESUMESUSPEND)
                {
                    OnSystemResume(EventArgs.Empty);
                    m.Result = (IntPtr)1;
                    return;
                }
            }
            else if (m.Msg == WindowConstants.WM_SYSCOMMAND)
            {
                long wparam = m.WParam.ToInt64() & 0xFFF0;
                if (wparam == (long)SystemCommands.SC_MONITORPOWER || wparam == (long)SystemCommands.SC_SCREENSAVE)
                {
                    CancelEventArgs e = new CancelEventArgs();
                    OnScreensaver(e);
                    if (e.Cancel)
                    {
                        m.Result = IntPtr.Zero;
                        return;
                    }
                }
                if (wparam == (long)SystemCommands.SC_KEYMENU)
                {
                    //屏蔽alt键的处理，因为默认会将焦点转移到菜单
                    return;
                }
            }

            base.WndProc(ref m);
        }

        static string GetAssemblyTitle(Assembly assembly)
        {
            if (assembly == null)
                return null;

            AssemblyTitleAttribute[] customAttributes = (AssemblyTitleAttribute[])assembly.GetCustomAttributes(typeof(AssemblyTitleAttribute), true);
            if (customAttributes != null && customAttributes.Length > 0)
                return customAttributes[0].Title;

            return null;
        }

        static string GetDefaultTitle()
        {
            string assemblyTitle = GetAssemblyTitle(Assembly.GetEntryAssembly());
            if (!string.IsNullOrEmpty(assemblyTitle))
                return assemblyTitle;

            try
            {
                Uri uri = new Uri(Application.ExecutablePath);
                return Path.GetFileNameWithoutExtension(uri.LocalPath);
            }
            catch (ArgumentNullException)
            {
                // swallow the exception
            }
            catch (UriFormatException)
            {
                // swallow the exception
            }

            return DefaultTitle;
        }

        static Screen ScreenFromHandle(IntPtr windowHandle)
        {
            Rectangle rectangle = NativeMethods.GetWindowRectangle(windowHandle);

            Screen bestScreen = null;
            int mostArea = 0;
            foreach (Screen screen in Screen.AllScreens)
            {
                Rectangle r = Rectangle.Intersect(rectangle, screen.Bounds);
                int area = r.Width * r.Height;

                if (area > mostArea)
                {
                    mostArea = area;
                    bestScreen = screen;
                }
            }

            if (bestScreen == null)
                bestScreen = Screen.PrimaryScreen;
            return bestScreen;
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();

            MinimumSize = new Size(200, 200);
            Icon = GetDefaultIcon();
            Text = GetDefaultTitle();
            Screen = ScreenFromHandle(Handle);

            this.KeyPreview = true;
            this.ResizeRedraw = true;
            this.SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint, true);

            this.viewportControl = new GameViewportControl();
            viewportControl.Visible = true;
            this.Controls.Add(viewportControl);

            this.ResumeLayout(false);
        }

        void UpdateScreen()
        {
            Screen current = Screen.FromHandle(Handle);
            if (Screen == null || Screen.DeviceName != current.DeviceName)
            {
                Screen = current;
                if (Screen != null)
                    OnScreenChanged(EventArgs.Empty);
            }
        }

        #endregion Methods
    }
}