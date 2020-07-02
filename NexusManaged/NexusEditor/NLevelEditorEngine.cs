using System;
using System.Collections.Generic;
using System.Text;
using NexusEngine;
using System.Windows.Forms;

using System.Reflection;
using NexusEngineExtension;
using Nexus.GameFramework;

namespace NexusEditor
{
    class NLevelEditorEngine : NEditorEngine
    {   
        private NResourceLoc m_levelLoc;    // 当前主关卡的存盘路径(文件夹)    

        /// <summary>
        /// 已经加载的程序集
        /// </summary>
        private HashSet<Assembly> assemblyLoaded = new HashSet<Assembly>();

        /// <summary>
        /// 动态创建ActorFactory
        /// </summary>
        private List<ObjectCreator> actorFactoryCreatorList = new List<ObjectCreator>();

        public event EventHandler LevelChanged;
        public event EventHandler CurrentFolderChanged;
        public event EventHandler CurrentResourceChanged;

        public NLevelEditorEngine()
        { 
        }

        public override void InitModules(NEngineConfig engineCfg, NEditorConfig editofCfg)
        {
            base.InitModules(engineCfg, editofCfg);

            // 加载对应的程序集
            AttachAssemblyInfo(Assembly.GetExecutingAssembly());
            AttachAssemblyInfo(Assembly.GetEntryAssembly());

            LoadActorFactoryCreator();
        }

        private void LoadActorFactoryCreator()
        {
            Assembly assembly = Assembly.GetAssembly(typeof(ActorFactory));
            if (assembly != null)
            {
                AttachAssemblyInfo(assembly);
            }
        }

        /// <summary>
        ///	扫描当前目录加载当前目录下的所有程序集
        /// </summary>
        protected void AttachAssemblyInfo(Assembly assembly)
        {
            if (assembly != null && assemblyLoaded.Add(assembly))
            {
                actorFactoryCreatorList.AddRange(DynamicLoader.Find(assembly, typeof(ActorFactory)));
            }
        }

        public override void CreateMainLevel(string levelName)
        {
            base.CreateMainLevel(levelName);            

            m_levelLoc = new NResourceLoc();
            if (LevelChanged != null)
                LevelChanged(this, null);
        }

        public void SaveMainLevel(NResourceLoc loc)
        {
            SaveLevel(this.MainLevelName, loc);
            
            m_levelLoc = loc;
            if (LevelChanged != null)
                LevelChanged(this, null);
        }

		public void SaveMainLevelAs(NResourceLoc loc)
		{
			SaveLevel(this.MainLevelName, loc);
		}

        public void ExportMainLevel(NResourceLoc loc)
        {
            ExportLevel(this.MainLevelName, loc);
        }

        public void LoadMainLevel(NResourceLoc loc)
        {                 
            try
            {
                //-- 必须先销毁当前的, 因为加载进来的关卡可能和他同名
                DestroyLevel(m_mainLevel);
                m_mainLevel.Dispose();

                //-- load
                NLevel lv = LoadLevel(loc);
                //--
                m_mainLevel = lv;             
                m_levelLoc = loc;
                ResetSubEditors();                
                if (LevelChanged != null)
                    LevelChanged(this, null);
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "Level Load FAILED!");                
                // 保持一个空关卡
                CreateMainLevel("defaultLevel");
            }            
        }

        public bool BindSelectedActorToTerrainEd()
        {
            NActor ac = this.ActorEd.SelectedActor;
            if (ac == null)
                return false;

            NTerrainActor trn = ac as NTerrainActor;
            if (trn == null)
                return false;
            this.TerrainEd.BindTerrain(trn);
            
            return true;
        }
        

        static public new NLevelEditorEngine Instance
        {
            get { return NEditorEngine.Instance() as NLevelEditorEngine; }
        }       
        

        public void DrawWidgets(NRenderPrimitiveDrawInterface PDI)
        {
            if (m_command == EditorCommand.TerrainMode)
            {
                TerrainEd.DrawWidgets(PDI);
            }
            else if (m_command == EditorCommand.NavigateMapMode)
            {
                if (NavigateMapEd != null)
                {
                    NavigateMapEd.DrawWidgets(PDI);
                }
            }
            
            ActorEd.DrawWidgets(PDI);
        }        
        

        public NResourceLoc LevelLoc
        {
            get { return m_levelLoc; }
        }

        public new NActorEditor ActorEd
        {
            get { return base.ActorEd as NActorEditor; }
        }

        public NResourceLoc CurrentFolder
        {
            get { return m_curFolder; }
            set 
            { 
                m_curFolder = value;
                if (CurrentFolderChanged != null)
                    CurrentFolderChanged(this, new EventArgs());
            }
        }

        public NResourceLoc CurrentResource
        {
            get { return m_curResrouce; }
            set 
            { 
                m_curResrouce = value;
                if (CurrentResourceChanged != null)
                    CurrentResourceChanged(this, new EventArgs());
            }
        }

        /// <summary>
        /// 显示或者隐藏所有的地形
        /// </summary>
        public bool TerrainsVisible
        {
            get
            {
                bool ret = true;
                if (m_mainLevel != null)
                {
                    NActor actor = m_mainLevel.FirstActor();
                    while (actor != null)
                    {
                        NTerrainActor terrain = actor as NTerrainActor;
                        if (terrain != null)
                        {
                            ret = terrain.Visible;
                            // 只要有一个地形可见，则认为是可见
                            if (ret)
                                return true;
                        }
                        actor = m_mainLevel.NextActor();
                    }
                }
                return ret;
            }
            set
            {
                if(m_mainLevel != null)
                {
                    NActor actor = m_mainLevel.FirstActor();
                    while (actor != null)
                    {
                        NTerrainActor terrain = actor as NTerrainActor;
                        if (terrain != null)
                        {
                            terrain.Visible = value;
                        }
                        actor = m_mainLevel.NextActor();
                    }
                }
            }
        }
        

        public List<ObjectCreator> ActorFactoryCreatorList
        {
            get
            {
                return actorFactoryCreatorList;
            }
        }
    }
}
