using System;
using NexusEngine;
using Nexus.GUI.Components;

namespace Nexus.GUI
{

    #region  GUIEventArgs
    /// <summary>
    /// 定义Nexus GUI事件
    /// 参考CEGUI事件传递机制
    /// </summary>
    public class GUIEventArgs : EventArgs
    {
        /// <summary>
        /// 事件是否被处理，被处理则终止传递
        /// </summary>
        public bool Handled { get; set; }
    }
    #endregion

    #region WindowEventArgs
    /// <summary>
    /// EventArgs based class that is used for objects passed to handlers triggered for events
    /// concerning some Window object.
    /// </summary>
    public class WindowEventArgs : GUIEventArgs
    {
        /// <summary>
        /// pointer to a Window object of relevance to the event.
        /// </summary>
        public Window window;

        public WindowEventArgs(Window wnd)
        {
            this.window = wnd;
        }
    };
    #endregion

    #region  HeaderSequenceEventArgs
    /// <summary>
    ///		GuiEventArgs based class that is used for objects passed to input event handlers
    ///		concerning movement of segments within a ListHeader widget.
    /// </summary>
    public class HeaderSequenceEventArgs : GUIEventArgs
    {
        #region Fields
        /// <summary>
        /// The original column index of the segment that has moved.
        /// </summary>
        public int OldIndex { get; set; }

        /// <summary>
        /// The new column index of the segment that has moved.
        /// </summary>
        public int NewIndex { get; set; }

        #endregion

        #region Constructor
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="oldIndex">Index of segment before it was moved.</param>
        /// <param name="newIndex">Index of segment after it has been moved.</param>
        public HeaderSequenceEventArgs(int oldIndex, int newIndex)
        {
            this.OldIndex = oldIndex;
            this.NewIndex = newIndex;
        }
        #endregion
    }
    #endregion

    #region TreeNodeEvent
    public class TreeNodeEventArg : GUIEventArgs
    {
        public UIComp_TreeNodeBase TreeNode;

        public TreeNodeEventArg(UIComp_TreeNodeBase node)
        {
            this.TreeNode = node;
        }
    }
    #endregion

    #region GUIMouseEventArgs
    /// <summary>
    /// EventArgs based class that is used for objects passed to input event handlers
    /// concerning mouse input.
    /// </summary>
    public class GUIMouseEventArgs : GUIEventArgs
    {
        public GUIMouseEventArgs(System.Windows.Forms.MouseButtons button, int clicks, Vector2 position, int delta)
        {
            this.Button = button;
            this.Clicks = clicks;
            this.Position = position;
            this.Delta = delta;
        }

        /// <summary>
        /// 获取曾按下的是哪个鼠标按钮
        /// </summary>
        public System.Windows.Forms.MouseButtons Button { get; private set; }
        /// <summary>
        /// 获取鼠标在产生鼠标事件时的位置
        /// </summary>
        public Vector2 Position { get; private set; }
        /// <summary>
        /// 获取鼠标轮已转动的制动器数的有符号计数
        /// </summary>
        public int Delta { get; private set; }
        /// <summary>
        /// 获取按下并释放鼠标按钮的次数
        /// </summary>
        public int Clicks { get; private set; }
    };

    #endregion

    //#region KeyEventArgs
    ///// <summary>
    ///// EventArgs based class that is used for objects passed to input event handlers
    ///// concerning keyboard input.
    ///// </summary>
    //public class KeyEventArgs : GUIEventArgs
    //{
    //    /// <summary>
    //    /// Character representation of the key pressed.
    //    /// </summary>
    //    public char character;
    //    /// <summary>
    //    /// Enum val representing the key pressed.
    //    /// </summary>
    //    public KeyCodes keyCode;
    //    /// <summary>
    //    /// current state of the system keys and mouse buttons.
    //    /// </summary>
    //    public SystemKey sysKeys;

    //};

    //public delegate void KeyEventHandler(object sender, KeyEventArgs e);

    //#endregion

    #region TimePulseEventArgs
    public class TimePulseEventArgs : GUIEventArgs
    {
        /// <summary>
        /// 时间单位统一为秒
        /// </summary>
        public float elapsedTime;
        public float deltaTime;
    };

    #endregion TimePulseEventArgs


    #region ResourceLoc
    public class ResourceLocationArgs : GUIEventArgs
    {
        public NResourceLoc ResourceLoc;

        public ResourceLocationArgs(NResourceLoc resLoc)
        {
            this.ResourceLoc = resLoc;
        }
    }
    #endregion

    #region ResourceLoc
    public class FontChangeArgs : GUIEventArgs
    {
        public FontDescription FontDesc { get; private set; }
        public NRenderFont RenderFont { get; private set; }

        public FontChangeArgs(NRenderFont font,FontDescription desc)
        {
            this.RenderFont = font;
            this.FontDesc = desc;
        }
    }
    #endregion
}