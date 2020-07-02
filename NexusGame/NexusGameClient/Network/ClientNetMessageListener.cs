namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;

    /// <summary>
    /// 定义NetMessageEvent事件
    /// </summary>
    public class MessageReceivedEvent : NetMessageEvent
    {
        #region Constructors

        public MessageReceivedEvent(System.Int16 messageID, System.IntPtr message)
        {
            this.messageId = messageID;
            this.MessagePtr = message;
        }

        #endregion Constructors

        #region Properties

        public System.Int16 messageId
        {
            get;
            private set;
        }

        public System.IntPtr MessagePtr
        {
            get;
            private set;
        }

        #endregion Properties
    }

    public class NetMessageEvent : EventArgs
    {
        #region Properties

        /// <summary>
        /// 事件是否被处理，被处理则终止传递
        /// </summary>
        public bool Handled
        {
            get;
            set;
        }

        #endregion Properties
    }

    internal sealed class ClientNetMessageListener : INetMessageListener
    {
        #region Constructors

        public ClientNetMessageListener()
        {
        }

        #endregion Constructors

        #region Events

        public event EventHandler<NetMessageEvent> NetConnectAborted;

        public event EventHandler<NetMessageEvent> NetConnected;

        public event EventHandler<NetMessageEvent> NetConnectError;

        public event EventHandler<NetMessageEvent> NetConnectTimeout;

        public event EventHandler<NetMessageEvent> NetDisconnected;

        public event EventHandler<MessageReceivedEvent> NetMessageReceived;

        #endregion Events

        #region Methods

        public void OnConnectAborted()
        {
            if (NetConnectAborted != null)
            {
                NetConnectAborted(this, new NetMessageEvent());
            }
        }

        public void OnConnected()
        {
            if (NetConnected != null)
            {
                NetConnected(this, new NetMessageEvent());
            }
        }

        public void OnConnectError()
        {
            if (NetConnectError != null)
            {
                NetConnectError(this, new NetMessageEvent());
            }
        }

        public void OnConnectTimeout()
        {
            if (NetConnectTimeout != null)
            {
                NetConnectTimeout(this, new NetMessageEvent());
            }
        }

        public void OnDisconnected()
        {
            if (NetDisconnected != null)
            {
                NetDisconnected(this, new NetMessageEvent());
            }
        }

        public void OnNetMessage(System.Int16 messageID, IntPtr message)
        {
            if (NetMessageReceived != null)
            {
                NetMessageReceived(this, new MessageReceivedEvent(messageID, message));
            }
        }

        #endregion Methods
    }
}