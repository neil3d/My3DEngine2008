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
using Nexus.GUI;

namespace NexusEditor.UIEditor
{
    /// <summary>
    /// 定义控件用于Nexus Engine Render
    /// </summary>
    public class NGUIRenderControl : NEViewport
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        protected UIEditor m_owner;
        protected NLevel levelPreview;

        /// <summary>
        /// 最近一次鼠标输入消息的屏幕坐标
        /// </summary>
        public Vector2 LastMouseDownPosition { get; set; }
        public Vector2 LastMouseUpPosition { get; set; }
        public Vector2 LastMouseClickPosition { get; set; }
        public Vector2 LastMousePosition { get; set; }
        public Vector2 DeltaMouseMove { get; set; }

        #region Property

        #endregion Property


        #region delegate
        public UIECreateWindowEventHander WindowCreated;
        #endregion delegate

        public NGUIRenderControl(UIEditor owner)
            : base()
        {
            InitializeComponent();

            NEditorEngine eng = NexusEditor.Program.engine;
            string levelName = "UIPreviewLevel";
            levelPreview = eng.CreateLevel(levelName, "nlevel");
            levelPreview.Init();
            m_focusLevel = levelName;
            SetCameraMode(EditorCameraMode.Top);

            m_owner = owner;

            this.AllowDrop = true;

            // 关联event handle
            this.DragDrop += new DragEventHandler(NGUIRenderControl_DragDrop);
            this.DragEnter += new DragEventHandler(NGUIRenderControl_DragEnter);
            this.DragOver += new DragEventHandler(NGUIRenderControl_DragOver);
        }

        #region  Initialize
        public void CreateViewport()
        {
            // create render element
            NGUIWidgetsRender widgetsRender = new NGUIWidgetsRender();
            m_view.WidgetsRender = widgetsRender;
            widgetsRender.AddRenderControl(this);

            // 创建UI系统
            try
            {
                new GUISystem(widgetsRender.UICanvas);
                //加载程序集，注册内置窗口类型
                Assembly executingAssembly = Assembly.GetExecutingAssembly();
                WindowManager.Instance.LoadAssembly(Path.GetDirectoryName(executingAssembly.Location) + System.IO.Path.DirectorySeparatorChar + @"Nexus.GUI.dll");
                // 加载当前执行文件中窗口类型
                WindowManager.Instance.AttachAssembly(executingAssembly);
                // 创建一个缺省的唯一父窗口
                GUISystem.Instance.RootWindow = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.GUISheet", "root", null);

                // 初始化Root大小
                RefreshRootWindowSize();
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "GUI System create failed!");
            }

            PostInitialize();
        }

        public virtual void PostInitialize()
        {

        }
        #endregion

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
            }
            NEditorEngine eng = NexusEditor.Program.engine;
            eng.DestroyLevel(levelPreview);
            levelPreview.Dispose();
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

        #region Paint Event Subscription

        #region  Paint

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            GUISystem.Instance.InjectTimePulse(m_mainTimer.ElapsedTime, m_mainTimer.DeltaTime);
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            // This overrides the System.Windows.Forms.Control protected method
            // "OnPaintBackground()" so that we don't clear the client area of
            // this form window -- so the viewport doesn't flicker on each frame.
        }

        /// <summary>
        /// 使用<see cref="NUICanvas"/>渲染2D界面
        /// </summary>
        /// <param name="Canvas"></param>
        virtual public void DrawWidgets(NUICanvas Canvas)
        {
            DrawDebugInfo(Canvas);

            // 渲染GUI
            GUISystem.Instance.RenderGUI();

            // 绘制当前选中控件的边框
            foreach (Window wnd in m_owner.CurrentSelectWindows)
            {
                Rect drawRect = wnd.Rect;
                Color4f selectColor = new Color4f(System.Drawing.Color.Red);
                Canvas.DrawLine(drawRect.left, drawRect.top, drawRect.right, drawRect.top, selectColor);
                Canvas.DrawLine(drawRect.left, drawRect.top, drawRect.left, drawRect.bottom, selectColor);
                Canvas.DrawLine(drawRect.right, drawRect.bottom, drawRect.right, drawRect.top, selectColor);
                Canvas.DrawLine(drawRect.right, drawRect.bottom, drawRect.left, drawRect.bottom, selectColor);
            }
        }

        private void DrawDebugInfo(NUICanvas Canvas)
        {
        }

        #endregion

        #region EventHandler

        protected override bool IsInputKey(Keys key)
        {
            switch (key)
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
            return base.IsInputKey(key);
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

            RefreshRootWindowSize();
        }

        private void RefreshRootWindowSize()
        {
            Window rootWnd = GUISystem.Instance == null ? null : GUISystem.Instance.RootWindow;
            if (rootWnd != null)
            {
                rootWnd.AbsolutePosition = new Vector2(m_view.X, m_view.Y);
                rootWnd.AbsoluteSize = new Vector2(this.Width, this.Height);
            }
        }


        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);

            LastMouseDownPosition = GUISystem.Instance.ClientToScreen(new Vector2(e.X, e.Y));
            if (m_owner != null && m_owner.CurrentMode == UIEditor.UIEditorMode.Preview)
            {
                GUISystem.Instance.InjectMouseDown(e);
            }
            else
            {
                m_owner.InjectEditorMouseDown(e);
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);

            LastMouseUpPosition = GUISystem.Instance.ClientToScreen(new Vector2(e.X, e.Y));
            if (m_owner != null && m_owner.CurrentMode == UIEditor.UIEditorMode.Preview)
            {
                GUISystem.Instance.InjectMouseUp(e);
            }
            else
            {
                m_owner.InjectEditorMouseUp(e);
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            Vector2 oldMovePos = LastMousePosition;
            LastMousePosition = GUISystem.Instance.ClientToScreen(new Vector2(e.X, e.Y));
            DeltaMouseMove = LastMousePosition - oldMovePos;
            if (m_owner != null && m_owner.CurrentMode == UIEditor.UIEditorMode.Preview)
            {
                GUISystem.Instance.InjectMouseMove(e);
            }
            else
            {
                m_owner.InjectEditorMouseMove(e);
            }
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);

            if (m_owner != null && m_owner.CurrentMode == UIEditor.UIEditorMode.Preview)
            {
                GUISystem.Instance.InjectMouseWheel(e);
            }
            else
            {
                m_owner.InjectEditorMouseWheel(e);
            }
        }

        protected override void OnMouseClick(MouseEventArgs e)
        {
            base.OnMouseClick(e);

            LastMouseClickPosition = GUISystem.Instance.ClientToScreen(new Vector2(e.X, e.Y));
            if (m_owner != null && m_owner.CurrentMode == UIEditor.UIEditorMode.Preview)
            {
                GUISystem.Instance.InjectMouseClicked(e);
            }
            else
            {
                m_owner.InjectEditorMouseClick(e);
            }
        }

        protected override void OnMouseDoubleClick(MouseEventArgs e)
        {
            base.OnMouseDoubleClick(e);

            if (m_owner != null && m_owner.CurrentMode == UIEditor.UIEditorMode.Preview)
            {
                GUISystem.Instance.InjectMouseDoubleClicked(e);
            }
            else
            {
                m_owner.InjectEditorMouseDoubleClick(e);
            }
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);

            GUISystem.Instance.InjectKeyDown(e);
            m_owner.InjectKeyDown(e);
        }

        protected override void OnKeyUp(KeyEventArgs e)
        {
            base.OnKeyUp(e);

            GUISystem.Instance.InjectKeyUp(e);
            m_owner.InjectKeyUp(e);
        }

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            base.OnKeyPress(e);

            GUISystem.Instance.InjectKeyPress(e);
            m_owner.InjectKeyPress(e);
        }

        protected void NGUIRenderControl_DragDrop(object sender, System.Windows.Forms.DragEventArgs e)
        {
            try
            {
                Point pt = this.PointToClient(new Point(e.X, e.Y));
                Vector2 DropPosition = new Vector2(pt.X, pt.Y);
                GUISystem guiSystem = GUISystem.Instance;
                Vector2 screenPosition = guiSystem.ClientToScreen(new Vector2(pt.X, pt.Y));

                if (e.Data.GetDataPresent(typeof(NFileEntity)))
                {
                    NFileEntity entity = (NFileEntity)e.Data.GetData(typeof(NFileEntity));
                    m_owner.CreateStyleWindow(entity, screenPosition);
                }
                else
                {
                    //获得进行"Drag"操作中拖动的字符串
                    string controlType = e.Data.GetData(typeof(string)) as string;

                    // 在拖放的位置创建窗口
                    if (!WindowManager.Instance.IsTypeRegedited(controlType))
                        return;

                    Window parentWnd = guiSystem.GetTargetWindow(screenPosition);
                    if (parentWnd == null)
                    {
                        parentWnd = guiSystem.RootWindow;
                    }

                    Window wnd = WindowManager.Instance.CreateWindow(controlType, parentWnd);
                    wnd.AbsolutePosition = screenPosition;
                    if (wnd.AbsoluteSize.Length() == 0)
                    {
                        wnd.Width = new UIDim(0, 150);
                        wnd.Height = new UIDim(0, 100);
                    }
                    parentWnd.AddChild(wnd);

                    // 发送响应事件
                    if (WindowCreated != null)
                    {
                        UIECreateWindowEventArg createEvent = new UIECreateWindowEventArg(wnd);
                        WindowCreated(this, createEvent);
                    }
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Drag drop failed!");
            }
        }

        protected void NGUIRenderControl_DragOver(object sender, System.Windows.Forms.DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(string))
                || e.Data.GetDataPresent(typeof(NFileEntity)))
            {
                e.Effect = DragDropEffects.Move;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        protected void NGUIRenderControl_DragEnter(object sender, System.Windows.Forms.DragEventArgs e)
        {
            if (!e.Data.GetDataPresent(typeof(string)))
            {
                return;
            }
        }

        #endregion EventHandler

        #endregion
    }
}
