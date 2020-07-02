namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Windows.Forms;

    using Nexus.GameFramework;

    using NexusEngine;
    using Nexus.GUI;

    /// <summary>
    /// 处理玩家输入
    /// </summary>
    public class GamePlayerInput : GameInput
    {
        #region Fields

        /// <summary>
        /// 鼠标左键点击位置
        /// </summary>
        private int clickLocationX;
        private int clickLocationY;

        /// <summary>
        /// 鼠标按下
        /// </summary>
        private bool dragMouse;
        private Vector2 dragStartLocation;

        #endregion Fields

        #region Constructors

        public GamePlayerInput()
        {
        }

        #endregion Constructors

        #region Properties

        [Input]
        public int ClickLocationX
        {
            get { return clickLocationX; }
            set { clickLocationX = value; }
        }

        [Input]
        public int ClickLocationY
        {
            get { return clickLocationY; }
            set { clickLocationY = value; }
        }

        #endregion Properties

        #region Methods

        internal override void UpdateInput(float gameTime)
        {
            base.UpdateInput(gameTime);
        }

        protected override void OnCaptureLost(Nexus.GUI.GUIEventArgs e)
        {
            base.OnCaptureLost(e);
            this.dragMouse = false;
        }

        protected override void OnMouseButtonDown(GUIMouseEventArgs  e)
        {
            base.OnMouseButtonDown(e);

            this.HostWindow.CaptureInput();
            this.ClickLocationX = (int)e.Position.x;
            this.ClickLocationY = (int)e.Position.y;
            this.dragMouse = true;
            this.dragStartLocation = e.Position;
        }

        protected override void OnMouseButtonUp(GUIMouseEventArgs  e)
        {
            base.OnMouseButtonUp(e);
            this.HostWindow.ReleaseInput();
            this.dragMouse = false;
        }

        protected override void OnMouseMove(GUIMouseEventArgs  e)
        {
            base.OnMouseMove(e);

            if (dragMouse)
            {
                this.dragStartLocation = e.Position;
            }
        }

        #endregion Methods
    }
}