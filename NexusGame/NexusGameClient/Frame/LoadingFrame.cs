namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using Nexus.GameFramework;
    using NexusEngine;

    /// <summary>
    ///  load地图frame
    /// </summary>
    internal sealed class LoadingFrame : IGameFrame
    {
        #region Constructors

        public LoadingFrame()
        {
        }

        #endregion Constructors

        #region Methods

        public void Destroyed()
        {
        }

        public bool LoadFrame()
        {
            return true;
        }

        public void NotifyNetMessageReceived(MessageReceivedEvent e)
        {
        }

        public void Update(float deltaTime, NViewport view)
        {
        }

        #endregion Methods

        #region IGameFrame 成员


        public void NotifyNetConnected(NetMessageEvent e)
        {
        }

        #endregion
    }
}