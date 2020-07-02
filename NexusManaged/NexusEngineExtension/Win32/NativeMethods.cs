using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Security;
using System.Windows.Forms;

namespace NexusEngineExtension
{
    public static class NativeMethods
    {
        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        public static extern bool SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool PeekMessage(out NativeMessage message, IntPtr hwnd, uint messageFilterMin, uint messageFilterMax, uint flags);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool GetClientRect(IntPtr hWnd, out NativeRectangle lpRect);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool GetWindowRect(IntPtr hWnd, out NativeRectangle lpRect);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool GetWindowPlacement(IntPtr hWnd, ref WINDOWPLACEMENT lpwndpl);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool SetWindowPlacement(IntPtr hWnd, ref WINDOWPLACEMENT lpwndpl);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern uint SetWindowLong(IntPtr hWnd, int nIndex, uint dwNewLong);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern uint GetWindowLong(IntPtr hWnd, int nIndex);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        static extern bool UpdateWindow(IntPtr hWnd);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool IsIconic(IntPtr hWnd);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool IsZoomed(IntPtr hWnd);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        /// <summary>
        /// <para>The DestroyWindow function destroys the specified window. The function sends WM_DESTROY and WM_NCDESTROY messages to the window to deactivate it and remove the keyboard focus from it. The function also destroys the window's menu, flushes the thread message queue, destroys timers, removes clipboard ownership, and breaks the clipboard viewer chain (if the window is at the top of the viewer chain).</para>
        /// <para>If the specified window is a parent or owner window, DestroyWindow automatically destroys the associated child or owned windows when it destroys the parent or owner window. The function first destroys child or owned windows, and then it destroys the parent or owner window.</para>
        /// <para>DestroyWindow also destroys modeless dialog boxes created by the CreateDialog function.</para>
        /// </summary>
        /// <param name="hwnd">Handle to the window to be destroyed.</param>
        /// <returns>If the function succeeds, the return value is nonzero. If the function fails, the return value is zero. To get extended error information, call GetLastError.</returns>
        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool DestroyWindow(IntPtr hwnd);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool AdjustWindowRect(ref NativeRectangle lpRect, uint dwStyle, [MarshalAs(UnmanagedType.Bool)]bool bMenu);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        public static extern uint SetThreadExecutionState(uint esFlags);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, uint uFlags);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
        public static extern IntPtr MonitorFromWindow(IntPtr hwnd, uint dwFlags);

        public static Rectangle GetClientRectangle(IntPtr handle)
        {
            NativeRectangle rect;
            if (!GetClientRect(handle, out rect))
                return Rectangle.Empty;

            return Rectangle.FromLTRB(rect.Left, rect.Top, rect.Right, rect.Bottom);
        }

        public static Rectangle GetWindowRectangle(IntPtr handle)
        {
            NativeRectangle rect;
            if (!GetWindowRect(handle, out rect))
                return Rectangle.Empty;

            return Rectangle.FromLTRB(rect.Left, rect.Top, rect.Right, rect.Bottom);
        }

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern IntPtr GetCapture();

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern IntPtr SetCapture(IntPtr hWnd);

        public static void CaptureMouse(IntPtr hWnd, bool bCapture)
        {
            IntPtr oldCaptureWnd = NativeMethods.GetCapture();
            bool isMouseCaptured = (oldCaptureWnd == hWnd);
            if (bCapture && !isMouseCaptured)
            {
                NativeMethods.SetCapture(hWnd);
            }
            else if (!bCapture && isMouseCaptured)
            {
                NativeMethods.ReleaseCapture();
            }
        }

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern bool ReleaseCapture();

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr SetActiveWindow(IntPtr hWnd);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern bool GetClipCursor(out NativeRectangle lpRect);

        /// <summary>
        /// Alternative Managed API: System.Windows.Forms.Cursor.Clip
        /// </summary>
        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        public static extern bool ClipCursor(ref NativeRectangle lpRect);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        public static extern bool ClipCursor(IntPtr rect);   

        /// <summary>
        ///  Specifies whether the internal display counter is to be incremented or decremented.
        ///  If bShow is TRUE, the display count is incremented by one. 
        ///  If bShow is FALSE, the display count is decremented by one.
        /// </summary>
        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern int ShowCursor(bool bShow);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern bool GetCursorInfo(out CURSORINFO pci);

        public static bool IsCursorVisible( )
        {
	        CURSORINFO CursorInfo;
	        bool bIsVisible = GetCursorInfo( out CursorInfo );
            bIsVisible = bIsVisible && (CursorInfo.flags & WindowConstants.CURSOR_SHOWING)!=0 && CursorInfo.hCursor != IntPtr.Zero;
	        return bIsVisible;
        }

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern bool SetCursorPos(int X, int Y);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool GetCursorPos(out Point lpPoint);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern IntPtr LoadCursor(IntPtr hInstance, int lpCursorName);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern IntPtr LoadCursorFromFile(string lpFileName);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern bool SetSystemCursor(IntPtr hcur, uint id);

        /// <summary>
        /// 设置系统光标
        /// </summary>
        /// <param name="lpCurFileName">"*.cur"</param>
        /// <returns></returns>
        public static bool SetSystemCursorFromFile(string lpCurFileName)
        {
            IntPtr cursor = LoadCursorFromFile(lpCurFileName);
            return SetSystemCursor(cursor, WindowConstants.OCR_NORMAL);
        }

        /// <summary>
        /// 设置Form窗口的光标
        /// </summary>
        public static void SetFormCursorFromFile(Form form, string lpCurFileName)
        {
            IntPtr cursor = LoadCursorFromFile(lpCurFileName);
            form.Cursor = new Cursor(cursor);
        }

        /// <summary>
        /// 从图片文件中加载光标
        /// </summary>
        /// <param name="form"></param>
        /// <param name="lpImageFileName"></param>
        public static void SetFormCursorFromImage(Form form, string lpImageFileName)
        {
            Bitmap hh = (Bitmap)System.Drawing.Bitmap.FromFile(lpImageFileName);
            Graphics.FromImage(hh);
            IntPtr ptr = hh.GetHicon();
            form.Cursor = new Cursor(ptr);
        }

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern int GetSystemMetrics(SystemMetric smIndex);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern IntPtr GetFocus();

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern bool ScreenToClient(IntPtr hWnd, ref Point lpPoint);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern bool ClientToScreen(IntPtr hWnd, ref Point lpPoint);

        [SuppressUnmanagedCodeSecurityAttribute]
        [DllImport("user32.dll")]
        public static extern bool InvalidateRect(IntPtr hWnd, IntPtr lpRect, bool bErase);

        [DllImport("Kernel32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool IsWow64Process(IntPtr hProcess, [MarshalAs(UnmanagedType.Bool)]ref bool Wow64Process);

        static bool Is64BitOS
        {
            get
            {
                bool bRet = false;

                if (IntPtr.Size == 8)
                {
                    bRet = true;
                }
                else
                {
                    IsWow64Process(System.Diagnostics.Process.GetCurrentProcess().Handle, ref bRet);
                }

                return bRet;
            }
        }
    }
}
