using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;
using System.Resources;

namespace Nexus.GUI
{
    /// <summary>
    /// GUI界面系统，提供界面系统所有可供访问的元素
    /// </summary>
    public sealed class GUISystem : IDisposable
    {
        public EventHandler<GUIEventArgs> StringDictionaryChanged;

        #region Fields

        public readonly static string DefaultRootWindowName = "root";

        /// <summary>
        /// Holds the reference to the Renderer object given to us in the constructor.
        /// </summary>
        private NUICanvas renderer;

        private Window rootWindow;
        private Window windowWithMouse;

        private bool guiRedraw;

        private System.Windows.Forms.Keys modifiers;

        /// <summary>
        /// 键和值查询表，需要在初始化时加载
        /// </summary>
        private UIStringDictionary stringDictionary = new UIStringDictionary();
        public readonly string DefauleStringDictFilePath = "/system/ui/NUIStringTable.resx";
        #endregion Fields

        #region Singleton Implementation
        /// <summary>
        /// GUI System singleton
        /// </summary>
        private static GUISystem instance;

        public GUISystem(NUICanvas renderer)
        {
            if (instance == null)
            {
                instance = this;

                // 初始化界面系统
                this.renderer = renderer;

                // 创建单件实例
                new WindowManager();
                new MouseCursor();

                ReloadStringDictionary();

                // 记录日志
                NLogger.Instance.WriteString(LogType.Info, "****** Nexus GUI System initialization. ******");

            }
        }


        ~GUISystem()
        {
            Dispose(false);
        }

        public static GUISystem Instance
        {
            get { return instance; }
        }

        #endregion Singleton Implementation

        #region IDisposable
        /// <summary>
        // The bulk of the clean-up code is implemented in Dispose(bool)
        /// </summary>
        private void Dispose(bool disposing)
        {
            if (disposing)
            {
                // free managed resources
            }
            // free native resources if there are any.
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion

        #region  Porperties

        public UIStringDictionary StringDictionary
        {
            get { return stringDictionary; }
        }

        public System.Windows.Forms.Keys Modifiers
        {
            get { return modifiers; }
        }

        public Window RootWindow
        {
            get { return rootWindow; }
            set { rootWindow = value; }
        }

        public NUICanvas Renderer
        {
            get { return renderer; }
        }

        public static System.Windows.Forms.MouseButtons ClickButton
        {
            get
            {
                if (System.Windows.Forms.SystemInformation.MouseButtonsSwapped)
                    return System.Windows.Forms.MouseButtons.Right;
                else
                    return System.Windows.Forms.MouseButtons.Left;
            }
        }
        #endregion Porperties

        #region Public Methods

        public void RenderGUI()
        {
            // 渲染GUI窗口
            if (guiRedraw)
            {
                if (rootWindow != null)
                {
                    rootWindow.Render();
                }

                // TODO: render to texture
                //guiRedraw = false;
            }

            // 渲染其他元素
        }

        public void SignalRedraw()
        {
            guiRedraw = true;
        }

        public Vector2 ScreenToClient(Vector2 point)
        {
            point -= this.RootWindow.AbsolutePosition;
            return point;
        }

        public Vector2 ClientToScreen(Vector2 point)
        {
            point += this.RootWindow.AbsolutePosition;
            return point;
        }

        #region Input Injection
        public bool InjectTimePulse(float elapsedTime, float deltaTime)
        {
            if (rootWindow != null)
            {
                TimePulseEventArgs e = new TimePulseEventArgs();
                e.deltaTime = deltaTime;
                e.elapsedTime = elapsedTime;
                rootWindow.OnTimePulse(e);
            }
            return true;
        }

        public bool InjectKeyDown(System.Windows.Forms.KeyEventArgs e)
        {
            // update system keys
            modifiers = e.Modifiers;

            if (rootWindow != null)
            {
                // 从当前Active窗口开始往父窗口传递，直到被处理或者root window
                Window destWindow = rootWindow.ActiveChild;
                while (destWindow != null && !e.Handled)
                {
                    destWindow.OnKeyDown(e);
                    destWindow = destWindow.Parent;
                }
                return e.Handled;
            }
            return false;
        }

        public bool InjectKeyUp(System.Windows.Forms.KeyEventArgs e)
        {
            // update system keys
            modifiers = e.Modifiers;

            if (rootWindow != null)
            {
                Window destWindow = rootWindow.ActiveChild;

                while (destWindow != null && !e.Handled)
                {
                    destWindow.OnKeyUp(e);
                    destWindow = destWindow.Parent;
                }

                return e.Handled;
            }
            return false;
        }

        public bool InjectKeyPress(System.Windows.Forms.KeyPressEventArgs e)
        {
            if (rootWindow != null)
            {
                Window destWindow = rootWindow.ActiveChild;

                while (destWindow != null && !e.Handled)
                {
                    destWindow.OnKeyPress(e);
                    destWindow = destWindow.Parent;
                }

                return e.Handled;
            }
            return false;
        }

        public void InjectMouseDown(System.Windows.Forms.MouseEventArgs e)
        {
            GUIMouseEventArgs guiMouseEvent = new GUIMouseEventArgs(e.Button, e.Clicks, ClientToScreen(new Vector2(e.X, e.Y)), e.Delta);
            Window destWindow = GetTargetWindow(guiMouseEvent.Position);
            Window eventWindow = destWindow;
            while (eventWindow != null
                && !eventWindow.OnMouseButtonDown(guiMouseEvent))
            {
                eventWindow = eventWindow.Parent;
            }
        }

        /// <summary>
        ///		Injects a mouse up event into the system.
        /// </summary>
        /// <param name="button">Mouse button being pressed.</param>
        /// <returns>Returns true if the event was Handled, and false if it is not.</returns>
        public void InjectMouseUp(System.Windows.Forms.MouseEventArgs e)
        {
            GUIMouseEventArgs guiMouseEvent = new GUIMouseEventArgs(e.Button, e.Clicks, ClientToScreen(new Vector2(e.X, e.Y)), e.Delta);
            Window destWindow = GetTargetWindow(guiMouseEvent.Position);

            Window eventWindow = destWindow;

            while (eventWindow != null
                && !eventWindow.OnMouseButtonUp(guiMouseEvent))
            {
                eventWindow = eventWindow.Parent;
            }
        }

        public void InjectMouseClicked(System.Windows.Forms.MouseEventArgs e)
        {
            GUIMouseEventArgs guiMouseEvent = new GUIMouseEventArgs(e.Button, e.Clicks, ClientToScreen(new Vector2(e.X, e.Y)), e.Delta);
            Window destWindow = GetTargetWindow(guiMouseEvent.Position);

            Window eventWindow = destWindow;

            while (eventWindow != null
                && !eventWindow.OnMouseClicked(guiMouseEvent))
            {
                eventWindow = eventWindow.Parent;
            }
        }

        public void InjectMouseDoubleClicked(System.Windows.Forms.MouseEventArgs e)
        {
            GUIMouseEventArgs guiMouseEvent = new GUIMouseEventArgs(e.Button, e.Clicks, ClientToScreen(new Vector2(e.X, e.Y)), e.Delta);
            Window destWindow = GetTargetWindow(guiMouseEvent.Position);

            Window eventWindow = destWindow;

            while (eventWindow != null
                && !eventWindow.OnMouseDoubleClicked(guiMouseEvent))
            {
                eventWindow = eventWindow.Parent;
            }
        }


        /// <summary>
        ///		Method that injects a mouse movement event into the system.
        /// </summary>
        /// <param name="deltaX">Amount the mouse moved on the x axis.</param>
        /// <param name="deltaY">Amount the mouse moved on the y axis.</param>
        /// <returns>Returns true if the event was Handled, and false if it is not.</returns>
        public void InjectMouseMove(System.Windows.Forms.MouseEventArgs e)
        {
            GUIMouseEventArgs guiMouseEvent = new GUIMouseEventArgs(e.Button, e.Clicks, ClientToScreen(new Vector2(e.X, e.Y)), e.Delta);

            MouseCursor mouse = MouseCursor.Instance;
            // move the mouse cursor
            mouse.Position = guiMouseEvent.Position;

            Window destWindow = GetTargetWindow(guiMouseEvent.Position);

            // if there is no GUI sheet, then there is nowhere to send input
            // TODO: keep an eye on passing the same args around
            if (destWindow != null)
            {
                if (destWindow != windowWithMouse)
                {
                    if (windowWithMouse != null)
                    {
                        windowWithMouse.OnMouseLeaves(guiMouseEvent);
                    }

                    windowWithMouse = destWindow;
                    destWindow.OnMouseEnters(guiMouseEvent);
                }

                while (destWindow != null
                    && !destWindow.OnMouseMove(guiMouseEvent))
                {
                    destWindow = destWindow.Parent;
                }
            }
        }

        /// <summary>
        ///		Method that injects a mouse wheel scroll event into the system.
        /// </summary>
        /// <param name="delta">Amount that the mouse wheel was moved.</param>
        /// <returns>Returns true if the event was Handled, and false if it is not.</returns>
        public void InjectMouseWheel(System.Windows.Forms.MouseEventArgs e)
        {
            Vector2 screenPos = ClientToScreen(new Vector2(e.X, e.Y));
            Window destWindow = GetTargetWindow(screenPos);

            if (destWindow != null)
            {
                GUIMouseEventArgs guiMouseEvent = new GUIMouseEventArgs(e.Button, e.Clicks, screenPos, e.Delta);
                destWindow.OnMouseWheel(guiMouseEvent);
            }
        }

        #endregion Input Injection


        /// <summary>
        ///		Given <paramref name="point"/>, return a reference to the <see cref="Window"/> that should receive a 
        ///		mouse input if <paramref name="point"/> is the mouse location.
        /// </summary>
        /// <param name="point">Screen location (in pixels).</param>
        /// <returns>
        ///		Reference to a <see cref="Window"/> object at mouse location <param ref name="point"/> that
        ///		is to receive input.
        /// </returns>
        public Window GetTargetWindow(Vector2 point)
        {
            Window destWindow = null;
            if (rootWindow != null)
            {
                destWindow = Window.CaptureWindow;

                if (destWindow == null)
                {
                    destWindow = rootWindow.GetChildAtPosition(point);

                    if (destWindow == null)
                    {
                        destWindow = rootWindow;
                    }
                }
            }

            return destWindow;
        }

        #region StringDictionary
        public void ReloadStringDictionary()
        {
            // Create a ResXResourceReader for the file *.resx.
            ResXResourceReader rsxr = new ResXResourceReader(NFileSystem.DefaultFileSystemRoot + DefauleStringDictFilePath);
            stringDictionary.CopyFrom(rsxr);
            rsxr.Close();

            GUISystem.Instance.OnStringDictionaryChanged();
        }

        private void OnStringDictionaryChanged()
        {
            if (StringDictionaryChanged != null)
            {
                StringDictionaryChanged(this, new GUIEventArgs());
            }

            if (RootWindow != null)
            {
                RootWindow.NotifyStringDictionaryChanged();
            }
        }

        #endregion

        #endregion Public Methods

    }
}
