using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NexusEditor.UIEditor
{
    public class UIEEventArg : EventArgs
    {
        public bool Handled;

        public UIEEventArg()
        {
        }
    }

    public delegate void UIEEventHandler(object sender,UIEEventArg e);

    /// <summary>
    /// 创建新窗口事件
    /// </summary>
    public class UIECreateWindowEventArg : UIEEventArg
    {
        public Nexus.GUI.Window NewCreatedWindow; 

        public UIECreateWindowEventArg(Nexus.GUI.Window wnd)
        {
            NewCreatedWindow = wnd;
        }
    }
    public delegate void UIECreateWindowEventHander(object sender, UIECreateWindowEventArg e);

}
