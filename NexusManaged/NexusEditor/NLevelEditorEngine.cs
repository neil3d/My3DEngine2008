using System;
using System.Collections.Generic;
using System.Text;
using NexusEngine;
using System.Windows.Forms;

namespace NexusEditor
{
    class NLevelEditorEngine : NEditorEngine
    {   
        private NResourceLoc m_levelLoc;    // 当前主关卡的存盘路径(文件夹)        

        public event EventHandler LevelChanged;
        public event EventHandler CurrentFolderChanged;
        public event EventHandler CurrentResourceChanged;

        public NLevelEditorEngine()
        { 
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
                TerrainEd.DrawWidgets(PDI);
            
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
        
    }
}
