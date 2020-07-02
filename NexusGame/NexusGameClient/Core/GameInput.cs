namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Reflection;
    using System.Text;
    using System.Windows.Forms;

    using Nexus.GameFramework;
    using Nexus.GUI;

    using NexusEngine;

    using NexusEngineExtension;

    /// <summary>
    /// 处理输入
    /// </summary>
    public class GameInput
    {
        #region Fields

        /// <summary>
        /// 宿主窗口
        /// </summary>
        private WeakReference hostWindow;

        /// <summary>
        /// 受输入影响的变量，会在每帧结束时清零
        /// </summary>
        private PropertyInfo[] inputProperties;

        /// <summary>
        /// 记录键盘状态，同时也记录了鼠标按键的状态
        /// </summary>
        bool[] keyState = new bool[256];
        private int mouseValueDX;
        private int mouseValueDY;
        private int mouseValueWheel;

        /// <summary>
        /// 记录鼠标状态
        /// </summary>
        private int mouseValueX;
        private int mouseValueY;

        /// <summary>
        /// 
        /// </summary>
        private System.Drawing.Point preCaptureMousePosition;

        #endregion Fields

        #region Constructors

        public GameInput()
        {
            this.inputProperties = GetType().GetProperties(
                 BindingFlags.Public | BindingFlags.Instance).Where(
                 p => p.GetCustomAttributes(typeof(InputAttribute), true).Length > 0).ToArray();
        }

        #endregion Constructors

        #region Events

        public event EventHandler<GUIEventArgs> CaptureGained;

        public event EventHandler<GUIEventArgs> CaptureLost;

        public event KeyEventHandler KeyDown;

        public event KeyPressEventHandler KeyPress;

        public event KeyEventHandler KeyUp;

        public event EventHandler<GUIMouseEventArgs> MouseButtonDown;

        public event EventHandler<GUIMouseEventArgs> MouseButtonUp;

        public event EventHandler<GUIMouseEventArgs> MouseClicked;

        public event EventHandler<GUIMouseEventArgs> MouseDoubleClicked;

        public event EventHandler<GUIMouseEventArgs> MouseMove;

        public event EventHandler<GUIMouseEventArgs> MouseWheel;

        #endregion Events

        #region Properties

        public Nexus.GUI.Window HostWindow
        {
            get { return (Nexus.GUI.Window)hostWindow.Target; }
        }

        [Input]
        public int MouseValueDX
        {
            get { return mouseValueDX; }
            set { mouseValueDX = value; }
        }

        [Input]
        public int MouseValueDY
        {
            get { return mouseValueDY; }
            set { mouseValueDY = value; }
        }

        [Input]
        public int MouseValueWheel
        {
            get { return mouseValueWheel; }
            set { mouseValueWheel = value; }
        }

        public int MouseValueX
        {
            get { return mouseValueX; }
        }

        public int MouseValueY
        {
            get { return mouseValueY; }
        }

        #endregion Properties

        #region Methods

        //冻结鼠标
        public virtual void FreezeMouse(System.Drawing.Point pt)
        {
            // 处理鼠标隐藏和Capture
            NativeMethods.CaptureMouse(Program.game.GameWindow.ViewportControl.Handle, true);
            while (NativeMethods.ShowCursor(false) >= 0) ;
            preCaptureMousePosition = pt;
        }

        public bool IsKeyPressed(Keys key)
        {
            return keyState[(int)key];
        }

        //解除鼠标冻结
        public virtual void UnfreezeMouse()
        {
            //恢复被隐藏的鼠标
            NativeMethods.CaptureMouse(Program.game.GameWindow.ViewportControl.Handle, false);
            while (NativeMethods.ShowCursor(true) < 0) ;
            NativeMethods.ClientToScreen(Program.game.GameWindow.ViewportControl.Handle, ref preCaptureMousePosition);
            NativeMethods.SetCursorPos(preCaptureMousePosition.X, preCaptureMousePosition.Y);
        }

        internal virtual void InjectInputEvent(Nexus.GUI.Window window)
        {
            hostWindow = new WeakReference(window);

            // 注册输入响应函数
            window.KeyPress += new System.Windows.Forms.KeyPressEventHandler(window_KeyPress);
            window.KeyDown += new System.Windows.Forms.KeyEventHandler(window_KeyDown);
            window.KeyUp += new System.Windows.Forms.KeyEventHandler(window_KeyUp);

            window.MouseButtonDown += new EventHandler<GUIMouseEventArgs>(window_MouseButtonDown);
            window.MouseButtonUp += new EventHandler<GUIMouseEventArgs>(window_MouseButtonUp);
            window.MouseClicked += new EventHandler<GUIMouseEventArgs>(window_MouseClicked);
            window.MouseDoubleClicked += new EventHandler<GUIMouseEventArgs>(window_MouseDoubleClicked);
            window.MouseMove += new EventHandler<GUIMouseEventArgs>(window_MouseMove);
            window.MouseWheel += new EventHandler<GUIMouseEventArgs>(window_MouseWheel);

            window.CaptureLost += new EventHandler<GUIEventArgs>(window_CaptureLost);
            window.CaptureGained += new EventHandler<GUIEventArgs>(window_CaptureGained);
        }

        internal virtual void ResetInput()
        {
            foreach (PropertyInfo property in inputProperties)
            {
                property.SetValue(this, 0, null);
            }
        }

        internal virtual void UpdateInput(float deltaTime)
        {
        }

        protected virtual void OnCaptureGained(GUIEventArgs e)
        {
            if (CaptureGained != null)
            {
                CaptureGained(this, e);
            }
        }

        protected virtual void OnCaptureLost(GUIEventArgs e)
        {
            if (CaptureLost != null)
            {
                CaptureLost(this, e);
            }
        }

        protected virtual void OnKeyDown(System.Windows.Forms.KeyEventArgs e)
        {
            if (KeyDown != null)
            {
                KeyDown(this, e);
            }
        }

        protected virtual void OnKeyPress(System.Windows.Forms.KeyPressEventArgs e)
        {
            if (KeyPress != null)
            {
                KeyPress(this, e);
            }
        }

        protected virtual void OnKeyUp(System.Windows.Forms.KeyEventArgs e)
        {
            if (KeyUp != null)
            {
                KeyUp(this, e);
            }
        }

        protected virtual void OnMouseButtonDown(GUIMouseEventArgs  e)
        {
            if (MouseButtonDown != null)
            {
                MouseButtonDown(this, e);
            }
        }

        protected virtual void OnMouseButtonUp(GUIMouseEventArgs  e)
        {
            if (MouseButtonUp != null)
            {
                MouseButtonUp(this, e);
            }
        }

        protected virtual void OnMouseClicked(GUIMouseEventArgs  e)
        {
            if (MouseClicked != null)
            {
                MouseClicked(this, e);
            }
        }

        protected virtual void OnMouseDoubleClicked(GUIMouseEventArgs  e)
        {
            if (MouseDoubleClicked != null)
            {
                MouseDoubleClicked(this, e);
            }
        }

        protected virtual void OnMouseMove(GUIMouseEventArgs  e)
        {
            if (MouseMove != null)
            {
                MouseMove(this, e);
            }
        }

        protected virtual void OnMouseWheel(GUIMouseEventArgs  e)
        {
            if (MouseWheel != null)
            {
                MouseWheel(this, e);
            }
        }

        private void window_CaptureGained(object sender, GUIEventArgs e)
        {
            OnCaptureGained(e);
        }

        private void window_CaptureLost(object sender, GUIEventArgs e)
        {
            OnCaptureLost(e);
        }

        private void window_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            keyState[e.KeyValue] = true;

            OnKeyDown(e);
        }

        private void window_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
        {
            OnKeyPress(e);
        }

        private void window_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            keyState[e.KeyValue] = false;

            OnKeyUp(e);
        }

        private void window_MouseButtonDown(object sender, GUIMouseEventArgs e)
        {
            NativeMethods.CaptureMouse(Program.game.GameWindow.ViewportControl.Handle, true);
            switch (e.Button)
            {
            case MouseButtons.Left:
                keyState[(int)System.Windows.Forms.Keys.LButton] = true;
                break;
            case MouseButtons.Right:
                keyState[(int)System.Windows.Forms.Keys.RButton] = true;
                break;
            case MouseButtons.Middle:
                keyState[(int)System.Windows.Forms.Keys.MButton] = true;
                break;
            }

            OnMouseButtonDown(e);
        }

        private void window_MouseButtonUp(object sender, GUIMouseEventArgs e)
        {
            // 显示鼠标并设置鼠标位置到按下时的位置
            NativeMethods.CaptureMouse(Program.game.GameWindow.ViewportControl.Handle, false);
            switch (e.Button)
            {
                case MouseButtons.Left:
                    keyState[(int)System.Windows.Forms.Keys.LButton] = false;
                    break;
                case MouseButtons.Right:
                    keyState[(int)System.Windows.Forms.Keys.RButton] = false;
                    break;
                case MouseButtons.Middle:
                    keyState[(int)System.Windows.Forms.Keys.MButton] = false;
                    break;
            }
            OnMouseButtonUp(e);
        }

        private void window_MouseClicked(object sender, GUIMouseEventArgs e)
        {
            OnMouseClicked(e);
        }

        private void window_MouseDoubleClicked(object sender, GUIMouseEventArgs  e)
        {
            OnMouseDoubleClicked(e);
        }

        private void window_MouseMove(object sender, GUIMouseEventArgs  e)
        {
            mouseValueDX = (int)(e.Position.x - mouseValueX);
            mouseValueDY = (int)(e.Position.y - mouseValueY);
            mouseValueX = (int)e.Position.x;
            mouseValueY = (int)e.Position.y;
            OnMouseMove(e);
        }

        private void window_MouseWheel(object sender, GUIMouseEventArgs  e)
        {
            mouseValueWheel = e.Delta;
            OnMouseWheel(e);
        }

        #endregion Methods
    }
}