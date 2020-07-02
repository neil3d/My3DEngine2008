namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using Nexus.GameFramework;
    using NexusEngine;

    /// <summary>
    ///  game frame
    ///  沿袭功夫game frame的概念
    /// </summary>
    public interface IGameFrame
    {
        #region Methods

        /// <summary>
        /// 销毁game frame
        /// </summary>
        void Destroyed();

        /// <summary>
        /// 加载game frame需要的资源,load场景和UI等
        /// </summary>
        bool LoadFrame();

        void NotifyNetConnected(NetMessageEvent e);
        void NotifyNetMessageReceived(MessageReceivedEvent e);

        void Update(float deltaTime, NViewport view);

        #endregion Methods

    }
}