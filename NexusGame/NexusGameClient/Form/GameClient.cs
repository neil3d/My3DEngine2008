namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using Nexus.GameFramework;
    using Nexus.GUI;

    using NexusEngine;

    using NexusEngineExtension;

    /// <summary>
    /// 游戏客户端主循环控制类
    /// </summary>
    internal sealed class GameClient : Game
    {
        #region Fields

        /// <summary>
        /// 客户端网络连接
        /// </summary>
        private NClientConnection clientNetConnection = new NClientConnection();

        #endregion Fields

        #region Constructors

        public GameClient()
        {
        }

        #endregion Constructors

        #region Properties

        public NClientConnection ClientNetConnection
        {
            get
            {
                return clientNetConnection;
            }
        }

        #endregion Properties

        #region Methods
        /// <summary>
        /// 退出程序时，清理相关资源
        /// </summary>
        /// <param name="e"></param>
        protected override void OnExiting(EventArgs e)
        {
            // 关闭网络连接
            if (clientNetConnection != null)
            {
                clientNetConnection.ClearListeners();
                clientNetConnection.Shutdown();
            }

            base.OnExiting(e);
        }

        protected override void OnFrameEnd(EventArgs e)
        {
            base.OnFrameEnd(e);
        }

        protected override void PostInitGameEngine()
        {
            base.PostInitGameEngine();

            // 初始化
            GameFrameManager.Instance.Initialize();
            GameHud.Instance.Initialize();

            CommandLineArguments arguments = new CommandLineArguments(System.Environment.GetCommandLineArgs());

            // 命令行参数指定创建关卡
            string LevelLoc = arguments["LoadLevel"];
            if (LevelLoc != null)
            {
                gameEngine.LoadLevel(LevelLoc);
            }
            else
            {
                // 加载一个缺省的关卡用于测试
                gameEngine.LoadLevel(@"demo_data:/level");
            }

            if (arguments["Network"] != null)
            {
                GameFrameManager.Instance.CurrentFrameType = GameFrameType.AccountLogin;

                // 初始化网络设置
                ClientNetMessageListener messageListener = new ClientNetMessageListener();
                messageListener.NetConnectAborted += new EventHandler<NetMessageEvent>(messageListener_NetConnectAborted);
                messageListener.NetConnected += new EventHandler<NetMessageEvent>(messageListener_NetConnected);
                messageListener.NetConnectError += new EventHandler<NetMessageEvent>(messageListener_NetConnectError);
                messageListener.NetConnectTimeout += new EventHandler<NetMessageEvent>(messageListener_NetConnectTimeout);
                messageListener.NetDisconnected += new EventHandler<NetMessageEvent>(messageListener_NetDisconnected);
                messageListener.NetMessageReceived += new EventHandler<MessageReceivedEvent>(messageListener_NetMessageReceived);
                clientNetConnection.AddListener(messageListener);
                clientNetConnection.Startup(20, true);

                // 建立网络连接
                string ipAddress = (arguments["IP"] == null) ? "127.0.0.1" : arguments["IP"];
                UInt16 port = (arguments["Port"] == null) ? UInt16.Parse("6000") : UInt16.Parse(arguments["Port"]);

                clientNetConnection.Connect(ipAddress, port);

                // 创建SavedMove
                gameEngine.LocalPlayer.SavedMove = new GameSavedMove(new GameReplicateMoveCallback(gameEngine.LocalPlayer));
            }
            else
            {
                GameFrameManager.Instance.CurrentFrameType = GameFrameType.Gaming;
            }

            Vector3 startPosition = Vector3.Zero;
            string startPositionText = arguments["StartPosition"];
            if (startPositionText != null)
            {
                startPosition = Vector3.Parse(startPositionText);
            }
            gameEngine.LocalPlayer.Transport(gameEngine.GameLevel, startPosition, true);
        }

        /// <summary>
        /// 重载Update接口
        /// </summary>
        protected override void Update(Nexus.GameFramework.GameTime gameTime)
        {
            float deltaTime = Math.Max(gameTime.ElapsedGameTime, gameTime.ElapsedRealTime);

            if (clientNetConnection != null
                && clientNetConnection.SessionStatus != ClientSessionStatus.None
                && clientNetConnection.SessionStatus != ClientSessionStatus.Disconnect)
            {
                clientNetConnection.Update(deltaTime);
            }

            // 注意顺序，ClientNetConnection Update在前
            base.Update(gameTime);

            GameFrameManager.Instance.Update(deltaTime, GameWindow.ViewportControl.Viewport);
            GameHud.Instance.Update(deltaTime, GameWindow.ViewportControl.Viewport);
        }

        void messageListener_NetConnectAborted(object sender, NetMessageEvent e)
        {
            GameFrameManager.Instance.NotifyNetConnectAborted(e);
        }

        void messageListener_NetConnected(object sender, NetMessageEvent e)
        {
            GameFrameManager.Instance.NotifyNetConnected(e);
        }

        void messageListener_NetConnectError(object sender, NetMessageEvent e)
        {
            GameFrameManager.Instance.NotifyNetConnectError(e);
        }

        void messageListener_NetConnectTimeout(object sender, NetMessageEvent e)
        {
            GameFrameManager.Instance.NotifyNetConnectTimeout(e);
        }

        void messageListener_NetDisconnected(object sender, NetMessageEvent e)
        {
            GameFrameManager.Instance.NotifyNetDisconnected(e);
        }

        void messageListener_NetMessageReceived(object sender, MessageReceivedEvent e)
        {
            GameFrameManager.Instance.NotifyNetMessageReceived(e);
        }

        #endregion Methods
    }
}