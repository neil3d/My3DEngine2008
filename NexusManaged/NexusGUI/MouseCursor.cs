using System;
using NexusEngine;

namespace Nexus.GUI
{
    /// <summary>
    /// 提供软件光标管理
    /// </summary>
    public class MouseCursor
    {
        #region Fields
        protected Vector2 position;
        protected bool isVisible;

        #endregion Fields

        #region Singleton Implementation

        private static MouseCursor instance;

        public MouseCursor()
        {
            if (instance == null)
            {
                instance = this;

                isVisible = true;

                NLogger.Instance.WriteString(LogType.Info,"MouseCursor singleton created.");
            }
        }

        public static MouseCursor Instance
        {
            get { return instance; }
        }

        #endregion Singleton Implementation

        #region Porperties


        public bool IsVisible
        {
            get { return isVisible; }
            set { isVisible = value; }
        }

        public Vector2 Position
        {
            get { return position; }
            set
            {
                position = value;
            }
        }
        #endregion Porperties

        #region Public Methods
        public void OffsetPosition(Vector2 offest)
        {
            position += offest;
        }

        public void ConstrainPosition(Rect constraints)
        {
            if (position.x >= constraints.right)
            {
                position.x = constraints.right - 1;
            }

            if (position.y >= constraints.bottom)
            {
                position.y = constraints.bottom - 1;
            }

            if (position.y < constraints.top)
            {
                position.y = constraints.top;
            }

            if (position.x < constraints.left)
            {
                position.x = constraints.left;
            }
        }

        #endregion Public Methods
    }
}
