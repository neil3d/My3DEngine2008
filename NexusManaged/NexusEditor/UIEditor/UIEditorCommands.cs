using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngineExtension;
using Nexus.GUI;
using NexusEngine;

namespace NexusEditor.UIEditor
{
    public class CommandDeleteWindow : ICommandOption
    {
        public Window TargetWindow { get; private set; }
        public Window ParentWindow { get; private set; }

        public CommandDeleteWindow(Window wnd)
        {
            TargetWindow = wnd;
            ParentWindow = TargetWindow.Parent;
        }


        #region ICommandOption 成员

        public bool Execute()
        {
            try
            {
                if (IsValid())
                {
                    ParentWindow.RemoveChild(TargetWindow);
                    return true;
                }
            }
            finally
            {

            }

            return false;
        }

        public bool UnExecute()
        {
            try
            {
                if (IsValid())
                {
                    ParentWindow.AddChild(TargetWindow);
                    return true;
                }
            }
            finally
            {

            }
            return false;
        }

        #endregion

        #region ICommandOption 成员


        public bool IsValid()
        {
            if (ParentWindow == null || TargetWindow == null || TargetWindow.Name.Equals("root"))
                return false;
            return true;
        }

        #endregion
    }

    #region CommandSetWindowDim

    /// <summary>
    /// 水平居中对齐
    /// </summary>
    public class CommandSetWindowDimCenterX : ICommandOption
    {
        public Window TargetWindow { get; private set; }
        public UIDim OldDimX { get; private set; }
        public UIDim OldDimWidth { get; private set; }

        public CommandSetWindowDimCenterX(Window wnd)
        {
            TargetWindow = wnd;
            OldDimX = TargetWindow.X;
            OldDimWidth = TargetWindow.Width;
        }

        #region ICommandOption 成员

        public bool Execute()
        {
            if (IsValid())
            {
                TargetWindow.X = new UIDim(0.5f, -TargetWindow.AbsoluteSize.x * 0.5f);
                TargetWindow.Width = new UIDim(0, TargetWindow.AbsoluteSize.x);
                return true;
            }
            return false;
        }

        public bool UnExecute()
        {
            if (IsValid())
            {
                TargetWindow.X = OldDimX;
                TargetWindow.Width = OldDimWidth;
                return true;
            }
            return false;
        }

        public bool IsValid()
        {
            if (TargetWindow == null || TargetWindow.Name.Equals("root"))
                return false;
            return true;
        }
        #endregion
    }

    public class CommandSetWindowDimCenterY : ICommandOption
    {
        public Window TargetWindow { get; private set; }
        public UIDim OldDimY { get; private set; }
        public UIDim OldDimHeight { get; private set; }

        public CommandSetWindowDimCenterY(Window wnd)
        {
            TargetWindow = wnd;
            OldDimY = TargetWindow.Y;
            OldDimHeight = TargetWindow.Height;
        }

        #region ICommandOption 成员

        public bool Execute()
        {
            if (IsValid())
            {
                TargetWindow.Y = new UIDim(0.5f, -TargetWindow.AbsoluteSize.y * 0.5f);
                TargetWindow.Height = new UIDim(0, TargetWindow.AbsoluteSize.y);
                return true;
            }
            return false;
        }

        public bool UnExecute()
        {
            if (IsValid())
            {
                TargetWindow.Y = OldDimY;
                TargetWindow.Height = OldDimHeight;
                return true;
            }
            return false;
        }

        public bool IsValid()
        {
            if (TargetWindow == null || TargetWindow.Name.Equals("root"))
                return false;
            return true;
        }
        #endregion
    }

    public class CommandSetWindowDimFullX : ICommandOption
    {
        public Window TargetWindow { get; private set; }
        public UIDim OldDimX { get; private set; }
        public UIDim OldDimWidth { get; private set; }

        public CommandSetWindowDimFullX(Window wnd)
        {
            TargetWindow = wnd;
            OldDimX = TargetWindow.X;
            OldDimWidth = TargetWindow.Width;
        }

        #region ICommandOption 成员

        public bool Execute()
        {
            if (IsValid())
            {
                TargetWindow.X = new UIDim(0, 0);
                TargetWindow.Width = new UIDim(1, 0);
                return true;
            }
            return false;
        }

        public bool UnExecute()
        {
            if (IsValid())
            {
                TargetWindow.X = OldDimX;
                TargetWindow.Width = OldDimWidth;
                return true;
            }
            return false;
        }

        public bool IsValid()
        {
            if (TargetWindow == null || TargetWindow.Name.Equals("root"))
                return false;
            return true;
        }
        #endregion
    }

    public class CommandSetWindowDimFullY : ICommandOption
    {
        public Window TargetWindows { get; private set; }
        public UIDim OldDimY { get; private set; }
        public UIDim OldDimHeight { get; private set; }

        public CommandSetWindowDimFullY(Window wnd)
        {
            TargetWindows = wnd;
            OldDimY = TargetWindows.Y;
            OldDimHeight = TargetWindows.Height;
        }

        #region ICommandOption 成员

        public bool Execute()
        {
            if (IsValid())
            {
                TargetWindows.Y = new UIDim(0, 0);
                TargetWindows.Height = new UIDim(1, 0);
                return true;
            }
            return false;
        }

        public bool UnExecute()
        {
            if (IsValid())
            {
                TargetWindows.Y = OldDimY;
                TargetWindows.Height = OldDimHeight;
                return true;
            }
            return false;
        }

        public bool IsValid()
        {
            if (TargetWindows == null || TargetWindows.Name.Equals("root"))
                return false;
            return true;
        }
        #endregion
    }

    public class CommandSetWindowDimY : ICommandOption
    {
        public Window TargetWindows { get; private set; }
        public UIDim OldDim { get; private set; }
        public UIDim TargetDim { get; private set; }

        public CommandSetWindowDimY(Window wnd, UIDim newY)
        {
            TargetWindows = wnd;
            OldDim = TargetWindows.Y;
            TargetDim = newY;
        }


        #region ICommandOption

        public bool Execute()
        {
            if (IsValid())
            {
                TargetWindows.Y = TargetDim;
                return true;
            }
            return false;
        }

        public bool UnExecute()
        {
            if (IsValid())
            {
                TargetWindows.Y = OldDim;
                return true;
            }
            return false;
        }

        public bool IsValid()
        {
            if (TargetWindows == null || TargetWindows.Name.Equals("root"))
                return false;
            return true;
        }
        #endregion
    }

    public class CommandSetWindowDimX : ICommandOption
    {
        public Window TargetWindows { get; private set; }
        public UIDim OldDim { get; private set; }
        public UIDim TargetDim { get; private set; }

        public CommandSetWindowDimX(Window wnd, UIDim newX)
        {
            TargetWindows = wnd;
            OldDim = TargetWindows.X;
            TargetDim = newX;
        }


        #region ICommandOption

        public bool Execute()
        {
            if (IsValid())
            {
                TargetWindows.X = TargetDim;
                return true;
            }
            return false;
        }

        public bool UnExecute()
        {
            if (IsValid())
            {
                TargetWindows.X = OldDim;
                return true;
            }
            return false;
        }

        public bool IsValid()
        {
            if (TargetWindows == null || TargetWindows.Name.Equals("root"))
                return false;
            return true;
        }
        #endregion
    }
    #endregion

    #region CommandSetWindowPosition
    public class CommandSetWindowPosition : ICommandOption
    {
        public Window TargetWindows { get; private set; }
        public Vector2 OldPosition { get; private set; }
        public Vector2 TargetPosition { get; private set; }

        public CommandSetWindowPosition(Window wnd, Vector2 newPosition)
        {
            TargetWindows = wnd;
            OldPosition = wnd.AbsolutePosition;
            TargetPosition = newPosition;
        }


        #region ICommandOption ³ÉÔ±

        public bool Execute()
        {
            if (IsValid())
            {
                TargetWindows.AbsolutePosition = TargetPosition;
                return true;
            }
            return false;
        }

        public bool UnExecute()
        {
            if (IsValid())
            {
                TargetWindows.AbsolutePosition = OldPosition;
                return true;
            }
            return false;
        }
        public bool IsValid()
        {
            if (TargetWindows == null || TargetWindows.Name.Equals("root"))
                return false;
            return true;
        }
        #endregion
    }
    #endregion


    #region CommandSetWindowSize
    public class CommandSetWindowSize : ICommandOption
    {
        public Window TargetWindows { get; private set; }
        public Vector2 OldSize { get; private set; }
        public Vector2 TargetSize { get; private set; }

        public CommandSetWindowSize(Window wnd, Vector2 newSize)
        {
            TargetWindows = wnd;
            OldSize = wnd.AbsoluteSize;
            TargetSize = newSize;
        }

        #region ICommandOption

        public bool Execute()
        {
            if (IsValid())
            {
                TargetWindows.AbsoluteSize = TargetSize;
                return true;
            }
            return false;
        }

        public bool UnExecute()
        {
            if (IsValid())
            {
                TargetWindows.AbsoluteSize = OldSize;
                return true;
            }
            return false;
        }
        public bool IsValid()
        {
            if (TargetWindows == null || TargetWindows.Name.Equals("root"))
                return false;
            return true;
        }
        #endregion
    }
    #endregion


}
