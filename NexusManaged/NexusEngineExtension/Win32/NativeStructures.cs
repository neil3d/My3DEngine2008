using System;
using System.Drawing;
using System.Runtime.InteropServices;

namespace NexusEngineExtension
{
    [StructLayout(LayoutKind.Sequential)]
    public struct NativeRectangle
    {
        #region Variables.
        /// <summary>
        /// Left position of the rectangle.
        /// </summary>
        public int Left;
        /// <summary>
        /// Top position of the rectangle.
        /// </summary>
        public int Top;
        /// <summary>
        /// Right position of the rectangle.
        /// </summary>
        public int Right;
        /// <summary>
        /// Bottom position of the rectangle.
        /// </summary>
        public int Bottom;
        #endregion

        #region Operators.
        /// <summary>
        /// Operator to convert a NativeRectangle to Drawing.Rectangle.
        /// </summary>
        /// <param name="rect">Rectangle to convert.</param>
        /// <returns>A Drawing.Rectangle</returns>
        public static implicit operator Rectangle(NativeRectangle rect)
        {
            return Rectangle.FromLTRB(rect.Left, rect.Top, rect.Right, rect.Bottom);
        }

        /// <summary>
        /// Operator to convert Drawing.Rectangle to a NativeRectangle.
        /// </summary>
        /// <param name="rect">Rectangle to convert.</param>
        /// <returns>NativeRectangle rectangle.</returns>
        public static implicit operator NativeRectangle(Rectangle rect)
        {
            return new NativeRectangle(rect.Left, rect.Top, rect.Right, rect.Bottom);
        }
        #endregion

        #region Constructor.
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="left">Horizontal position.</param>
        /// <param name="top">Vertical position.</param>
        /// <param name="right">Right most side.</param>
        /// <param name="bottom">Bottom most side.</param>
        public NativeRectangle(int left, int top, int right, int bottom)
        {
            Left = left;
            Top = top;
            Right = right;
            Bottom = bottom;
        }
        #endregion

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct NativeMessage
    {
        public IntPtr hWnd;
        public uint msg;
        public IntPtr wParam;
        public IntPtr lParam;
        public uint time;
        public Point p;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct WINDOWPLACEMENT
    {
        public int length;
        public int flags;
        public int showCmd;
        public Point ptMinPosition;
        public Point ptMaxPosition;
        public NativeRectangle rcNormalPosition;

        public static int Length
        {
            get { return Marshal.SizeOf(typeof(WINDOWPLACEMENT)); }
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct CURSORINFO
    {
        public Int32 cbSize;        // Specifies the size, in bytes, of the structure. 
        // The caller must set this to Marshal.SizeOf(typeof(CURSORINFO)).
        public Int32 flags;         // Specifies the cursor state. This parameter can be one of the following values:
        //    0             The cursor is hidden.
        //    CURSOR_SHOWING    The cursor is showing.
        public IntPtr hCursor;          // Handle to the cursor. 
        public Point ptScreenPos;       // A POINT structure that receives the screen coordinates of the cursor. 
    }
}
