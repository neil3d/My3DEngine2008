namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Reflection;
    using System.Runtime.InteropServices;
    using System.Text;

    using Nexus.GameFramework;

    using NexusEngine;

    using NexusEngineExtension;

    #region Enumerations

    public enum GameFrameType : int
    {
        None,
        AccountLogin,
        SelectRole,
        CreateRole,
        Gaming,
    }

    #endregion Enumerations

    /// <summary>
    /// Game Frame Manager
    /// 单件，管理game frame(enum GameFrameType)之间的切换
    /// </summary>
    internal sealed class GameFrameManager : Singleton<GameFrameManager>
    {
        #region Fields

        /// <summary>
        /// 当前类型下创建的Frame
        /// </summary>
        private List<IGameFrame> currentFrames = new List<IGameFrame>();

        /// <summary>
        ///  当前frame类型
        /// </summary>
        private GameFrameType currentFrameType;
        private Dictionary<GameFrameType, HashSet<Type>> registeredFrames = new Dictionary<GameFrameType,HashSet<Type>>();

        #endregion Fields

        #region Constructors

        private GameFrameManager()
        {
        }

        #endregion Constructors

        #region Events

        public event EventHandler<GameFrameTransferEvent> FrameTransfered;

        public event EventHandler<GameFrameTransferEvent> FrameTransfering;

        #endregion Events

        #region Properties

        /// <summary>
        /// 当前所处 frame 类型
        /// </summary>
        public GameFrameType CurrentFrameType
        {
            get
            {
                return currentFrameType;
            }
            set
            {
                if(currentFrameType != value)
                {
                    OnFrameTransfering(new GameFrameTransferEvent(currentFrameType));

                    currentFrameType = value;

                    OnFrameTransfered(new GameFrameTransferEvent(currentFrameType));
                }
            }
        }

        #endregion Properties

        #region Methods

        public override bool Initialize(params object[] args)
        {
            // 注册frame
			RegisterFrame(GameFrameType.AccountLogin, typeof(AccountLoginFrame));
			RegisterFrame(GameFrameType.SelectRole, typeof(SelectRoleFrame));
			RegisterFrame(GameFrameType.CreateRole, typeof(CreateRoleFrame));
			RegisterFrame(GameFrameType.Gaming, typeof(LoadingFrame));
			RegisterFrame(GameFrameType.Gaming, typeof(GamingFrame));
			RegisterFrame(GameFrameType.Gaming, typeof(GamingMainFrame));
			RegisterFrame(GameFrameType.Gaming, typeof(GamingCombatFrame));
            RegisterFrame(GameFrameType.Gaming, typeof(MiniMapFrame));

            // 加载属性模板表
            NLevelupInfo.LoadFromFile();
            NSpellInfo.LoadFromFile();
            NSpellAuraInfo.LoadFromFile();
            NSpellEffectInfo.LoadFromFile();

            return base.Initialize(args);
        }

        public void NotifyNetConnectAborted(NetMessageEvent e)
        {
            CurrentFrameType = GameFrameType.AccountLogin;
        }

        public void NotifyNetConnected(NetMessageEvent e)
        {
            // 处理收到的网络消息
            foreach (var frame in currentFrames)
            {
                if (!e.Handled)
                {
                    frame.NotifyNetConnected(e);
                }
            }
        }

        public void NotifyNetConnectError(NetMessageEvent e)
        {
            // todo: 处理重新连接
            //CurrentFrameType = GameFrameType.AccountLogin;
        }

        public void NotifyNetConnectTimeout(NetMessageEvent e)
        {
            // todo: 处理超时重新连接
        }

        public void NotifyNetDisconnected(NetMessageEvent e)
        {
            // todo: 处理重新连接
            //CurrentFrameType = GameFrameType.AccountLogin;
        }

        public void NotifyNetMessageReceived(MessageReceivedEvent e)
        {
            GameFrameType oldType = CurrentFrameType;
            // 处理收到的网络消息
            foreach (var frame in currentFrames)
            {
                if(!e.Handled)
                {
                    frame.NotifyNetMessageReceived(e);
                }

                // 消息处理的过程中有可能会改变currentFrames，从而导致循环被破坏
                if (CurrentFrameType != oldType)
                {
                    break;
                }
            }
        }

        /// <summary>
        /// 注册game frame
        /// </summary>
        /// <param name="enumType"></param>
        /// <param name="frameType"></param>
        public void RegisterFrame(GameFrameType enumType, Type frameType)
        {
            if( !frameType.IsAbstract
                //&& frameType.IsSubclassOf(typeof(IGameFrame))
                )
            {
                if (!registeredFrames.ContainsKey(enumType))
                {
                    registeredFrames.Add(enumType, new HashSet<Type>());
                }

                registeredFrames[enumType].Add(frameType);
            }
        }

        /// <summary>
        /// 帧更新接口
        /// </summary>
        /// <param name="deltaTime"></param>
        public void Update(float deltaTime, NViewport view)
        {
            foreach (var frame in currentFrames)
            {
                frame.Update(deltaTime, view);
            }
        }

        internal static void SendNetMessage<T>(T message)
        {
            int messageSize = Marshal.SizeOf(message);
            // Initialize unmanged memory to hold the struct.
            IntPtr rawDataPtr = Marshal.AllocHGlobal(messageSize);
            try
            {
                // Copy the struct to unmanaged memory.
                Marshal.StructureToPtr(message, rawDataPtr, false);

                // send network message
                unsafe
                {
                    Program.game.ClientNetConnection.SendMessage((void*)rawDataPtr, (UInt32)messageSize);
                }
            }
            finally
            {
                // Free the unmanaged memory.
                Marshal.FreeHGlobal(rawDataPtr);
            }
        }

        private void OnFrameTransfered(GameFrameTransferEvent gameFrameTransferEvent)
        {
            if (FrameTransfered != null)
            {
                FrameTransfered(this, gameFrameTransferEvent);
            }

            // 加载frame的资源
            if (registeredFrames.ContainsKey(gameFrameTransferEvent.frameType))
            {
                foreach (var typeFrame in registeredFrames[gameFrameTransferEvent.frameType])
                {
                    IGameFrame frame = (IGameFrame)Activator.CreateInstance(typeFrame);
                    if(frame != null)
                    {
                        frame.LoadFrame();
                        currentFrames.Add(frame);
                    }
                }
            }
        }

        private void OnFrameTransfering(GameFrameTransferEvent gameFrameTransferEvent)
        {
            if(FrameTransfering != null)
            {
                FrameTransfering(this, gameFrameTransferEvent);
            }

            // 销毁当前frame的资源
            foreach( var frame in currentFrames)
            {
                frame.Destroyed();
            }
            currentFrames.Clear();
        }

        #endregion Methods
    }

    internal sealed class GameFrameTransferEvent : EventArgs
    {
        #region Constructors

        public GameFrameTransferEvent(GameFrameType frame)
        {
            frameType = frame;
        }

        #endregion Constructors

        #region Properties

        public GameFrameType frameType
        {
            private set;
            get;
        }

        #endregion Properties
    }
}