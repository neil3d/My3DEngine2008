namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;
    using System.Reflection;
    using System.Text;

    using Nexus.GameFramework;
    using Nexus.GUI;

    using NexusEngine;

    using NexusEngineExtension;

    /// <summary>
    /// NGameEngine引擎Facade类
    /// </summary>
    public sealed class GameEngine : NGameEngine
    {
        #region Fields

        /// <summary>
        /// 主关卡发生变化
        /// </summary>
        public EventHandler<LevelChangeEventArgs> LevelChanged;

        /// <summary>
        /// 主关卡即将发生变化
        /// </summary>
        public EventHandler<LevelChangeEventArgs> LevelChanging;

        /// <summary>
        /// 角色名称生成器，用于创建NActor时为Actor分配唯一的名称
        /// </summary>
        private NameGenerator<NActor> actorNameGenerator = new NameGenerator<NActor>();

        /// <summary>
        /// 已经加载的程序集
        /// </summary>
        private HashSet<Assembly> assemblyLoaded = new HashSet<Assembly>();

        /// <summary>
        /// 当前关卡，逻辑层只允许加载一个关卡
        /// </summary>
        private NLevel currentLevel;

        /// <summary>
        /// 游戏关卡
        /// </summary>
        private NGameLevel gameLevel = new NGameLevel();
        private NUICanvas guiCanvas;

        /// <summary>
        /// 本地控制玩家
        /// </summary>
        private LocalPlayerController localPlayer;

        /// <summary>
        /// 逻辑层维护角色列表
        /// </summary>
        private CharacterControllerCollection remoteCCs = new CharacterControllerCollection();

        #endregion Fields

        #region Constructors

        public GameEngine()
        {
        }

        #endregion Constructors

        #region Properties

        /// <summary>
        /// 单件访问
        /// </summary>
        public static GameEngine EngineInstance
        {
            get { return (GameEngine)NEngine.Instance(); }
        }

        /// <summary>
        /// 主关卡
        /// </summary>
        public NLevel CurrentLevel
        {
            get { return currentLevel; }
            set
            {
                if (currentLevel != value)
                {
                    OnCurrentLevelChanging(new LevelChangeEventArgs(currentLevel));

                    currentLevel = value;

                    gameLevel.Attach(currentLevel);

                    OnCurrentLevelChanged(new LevelChangeEventArgs(currentLevel));
                }
            }
        }

        public NGameLevel GameLevel
        {
            get { return gameLevel; }
        }

        public NUICanvas GuiCanvas
        {
            get { return guiCanvas; }
        }

        internal LocalPlayerController LocalPlayer
        {
            get { return localPlayer; }
        }

        internal CharacterControllerCollection RemoteCC
        {
            get { return remoteCCs; }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// 创建一个缺省的关卡
        /// </summary>
        public NLevel CreateDefaultLevel(string levelName)
        {
            // 先删除之前的关卡
            if (currentLevel != null)
            {
                DestroyLevel(currentLevel);
            }

            //-- create new
            NLevel newLevel = CreateLevel(levelName, "nlevel");
            newLevel.Init();

            currentLevel = newLevel;

            // 创建缺省的本地玩家
            CreateLocalPlayerController();

            return newLevel;
        }

        internal LocalPlayerController CreateLocalPlayerController()
        {
            if (localPlayer != null)
                return localPlayer;

            localPlayer = new LocalPlayerController();
            localPlayer.Init("LocalPlayerController");
            return localPlayer;
        }

        internal RemoteCharacterController CreateRemoteCharacterController(UInt64 characterID)
        {
            RemoteCharacterController rcc = new RemoteCharacterController(characterID);
            rcc.Init("RC_" + characterID.ToString());
            return rcc;
        }

        public override void DestroyLevel(NLevel level)
        {
            if (CurrentLevel == level)
            {
                CurrentLevel = null;
            }
            base.DestroyLevel(level);
        }

        public override void DestroyLevel(string levelName)
        {
            if (CurrentLevel.Name.Equals(levelName))
            {
                CurrentLevel = null;
            }
            base.DestroyLevel(levelName);
        }

        internal RemoteCharacterController FindCharacterController(UInt64 characterID)
        {
            RemoteCharacterController rcc = null;
            if (remoteCCs.TryGetValue(characterID, out rcc))
            {
                return rcc;
            }
            return null;
        }

        public override void InitModules(NEngineConfig cfg)
        {
            base.InitModules(cfg);

            // 初始化GUI系统
            InitGuiSystem();

            // 加载程序集信息
            AttachAssemblyInfo(Assembly.GetExecutingAssembly());
            AttachAssemblyInfo(Assembly.GetEntryAssembly());
        }

        public NLevel LoadLevel(string levelPath)
        {
            return LoadLevel(new NResourceLoc(levelPath));
        }

        public override NLevel LoadLevel(ValueType loc)
        {
            // 先删除之前的关卡
            if (currentLevel != null)
            {
                DestroyLevel(currentLevel);
            }

            CurrentLevel = base.LoadLevel(loc);
            return CurrentLevel;
        }

        public bool RemoveCharacterController(UInt64 characterID)
        {
            return RemoveCharacterController(FindCharacterController(characterID));
        }

        internal bool RemoveCharacterController(RemoteCharacterController controller)
        {
            if (controller == null)
                return false;

            controller.Destory();
            return remoteCCs.Remove(controller.CharacterID);
        }

        /// <summary>
        /// 逻辑层的Tick机会
        /// </summary>
        /// <param name="gameTime"></param>
        public override void Update(float deltaTime, NViewport vp)
        {
            if (localPlayer != null)
            {
                localPlayer.Update(deltaTime, vp);
            }

            // 给所有的GameActor一次逻辑层更新机会
            foreach (var item in remoteCCs)
            {
                item.Value.Update(deltaTime);
            }
            base.Update(deltaTime, vp);
        }


        /// <summary>
        ///	扫描当前目录加载当前目录下的所有程序集
        /// </summary>
        void AttachAssemblyInfo(Assembly assembly)
        {
        }

        /// <summary>
        /// 初始化GUI系统，在InitModules时调用
        /// </summary>
        private void InitGuiSystem()
        {
            NRenderResourceManager renderResMgr = NEngine.Instance().RenderResManager;
            guiCanvas = renderResMgr.NewUICanvas();

            // 创建UI系统
            new GUISystem(guiCanvas);

            //加载程序集，注册内置窗口类型
            Assembly executingAssembly = Assembly.GetExecutingAssembly();
            WindowManager.Instance.LoadAssembly(Path.GetDirectoryName(executingAssembly.Location) + System.IO.Path.DirectorySeparatorChar + @"Nexus.GUI.dll");
            // 加载当前执行文件中窗口类型
            WindowManager.Instance.AttachAssembly(executingAssembly);
            WindowManager.Instance.AttachAssembly(Assembly.GetEntryAssembly());
            // 创建一个缺省的唯一父窗口
            GUISystem.Instance.RootWindow = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.GUISheet", GUISystem.DefaultRootWindowName, null);
        }

        void OnCurrentLevelChanged(LevelChangeEventArgs levelChangedEvent)
        {
            if (currentLevel != null)
            {
                currentLevel.AttachActor(localPlayer.TargetActor);
            }

            if (LevelChanged != null)
            {
                LevelChanged(this, levelChangedEvent);
            }
        }

        void OnCurrentLevelChanging(LevelChangeEventArgs levelChangedEvent)
        {
            if (currentLevel != null)
            {
                currentLevel.DeattachActor(localPlayer.TargetActor);
            }

            if (LevelChanging != null)
            {
                LevelChanging(this, levelChangedEvent);
            }
        }

        #endregion Methods
    }
}