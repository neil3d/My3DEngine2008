using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using NexusEngine;

using NexusEditor.ResourceEditor;
using System.Text;

namespace NexusEditor
{
    static class Program
    {
        public static NLevelEditorEngine engine;
        public static EditorMainForm mainForm;
        public static ResourceEditorForm resourceForm;
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            NControl.SplashForm.StartSplash(NexusEditor.Properties.Resources.EditorSplash);

            try
            {
                engine = new NLevelEditorEngine();
                engine.InitCore();

                mainForm = new EditorMainForm();
                mainForm.CreateLogForm();

                //-- create engine config
                Size renderSize = new Size(800, 600);
                mainForm.ClientSize = renderSize;
                NEngineConfig engineCfg = new NEngineConfig();
                engineCfg.RenderWndHandle = mainForm.RenderPanelHandle;
                engineCfg.ClientWidth = Screen.PrimaryScreen.Bounds.Width;
                engineCfg.ClientHeight = Screen.PrimaryScreen.Bounds.Height;
                engineCfg.ColorBits = 32;
                engineCfg.FullScreen = false;
                engineCfg.EnableHDR = false;
                engineCfg.RenderClass = "nrenderer_d3d9";
                engineCfg.FileSystemClass = "nstd_file_system";
                engineCfg.FileSystemRoot = "/work/nexus_engine";
                engineCfg.EngineDataPkg = "engine_data";

                //--
                NEditorConfig editorCfg = new NEditorConfig();
                editorCfg.actorEdClassAssembly = "NexusEditor";
                editorCfg.actorEditorClass = "NexusEditor.NActorEditor";
                
                engine.InitModules(engineCfg, editorCfg);                
            }
            catch (System.Exception e)
            {
                ShowException(e, "Nexus Engine Init FAILED!");
                return;
            }

            //-- create default level            
            engine.CreateMainLevel("defaultLevel");

            //-- create editor windows
            mainForm.CreateEditorForms();
            mainForm.CreateViewport();

            //-- open resrouce
            resourceForm = new ResourceEditorForm();

            NControl.SplashForm.CloseSplash();             
            
            //-- run
            resourceForm.Show();
            mainForm.Show();
            mainForm.BringToFront();
            Application.Run(mainForm);

            //-- close            
            engine.RedirectLog(null);
            resourceForm.Close();
            engine.Close();
            engine.Dispose();
            mainForm.Dispose();
        }

        static public void ShowException(System.Exception e, String info)
        {
            StringBuilder msg = new StringBuilder();
            msg.AppendFormat("{0} : \r\n{1}",
                e.GetType().ToString(), e.Message);
            
            StringBuilder title = new StringBuilder();
            title.AppendFormat("Nexus Editor Error : {0}", info);

            MessageBox.Show(msg.ToString(), title.ToString(), MessageBoxButtons.OK, MessageBoxIcon.Warning);

        }

        static public void ShowError(string info)
        {
            MessageBox.Show(info, "Nexus Editor Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }
    }
}
