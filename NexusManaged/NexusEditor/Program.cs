using System;
using System.IO;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using NexusEngine;
using NexusEngineExtension;
using Nexus.GameFramework;
using NexusEditor.ResourceEditor;

using System.Text;
using System.Globalization;
using System.Configuration;

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

            // 创建log
            NLogger.Instance.AddLogOutput(new NFileLogOutput("NexusEditor.log"));

            // 读取本地化配置
            NexusEditor.Properties.Resources.Culture = new CultureInfo(ConfigurationManager.AppSettings["Localization"]);

            SplashForm.StartSplash(NexusEditor.Properties.Resources.EditorSplash);

            try
            {
                engine = new NLevelEditorEngine();
                engine.InitCore();

                mainForm = new EditorMainForm();
                mainForm.CreateLogForm();

                //-- create engine config
                CustomConfigurationSection configuration = CustomConfigurationSection.Open();
                mainForm.ClientSize = new Size(configuration.EngineSetting.ClientWidth, configuration.EngineSetting.ClientHeight);
                NEngineConfig engineCfg = new NEngineConfig();
                engineCfg.RenderWndHandle = mainForm.RenderPanelHandle;
                engineCfg.ClientWidth = Screen.PrimaryScreen.Bounds.Width;
                engineCfg.ClientHeight = Screen.PrimaryScreen.Bounds.Height;
                engineCfg.ColorBits = configuration.EngineSetting.ColorBits;
                engineCfg.FullScreen = configuration.EngineSetting.FullScreen;
                engineCfg.EnableHDR = configuration.EngineSetting.EnableHDR;
                engineCfg.EnableSSAO = configuration.EngineSetting.EnableSSAO;
                engineCfg.RenderClass = configuration.EngineSetting.RenderClass;
                engineCfg.FileSystemClass = configuration.EngineSetting.FileSystemClass;
                engineCfg.EngineDataPkg = configuration.EngineSetting.EngineDataPkg;
                engineCfg.ResourceCacheClass = configuration.EngineSetting.ResourceCacheClass;
                engineCfg.ResourceIOClass = configuration.EngineSetting.ResourceIOClass;
                engineCfg.FileSystemRoot = configuration.EngineSetting.FileSystemRoot;
                if (string.IsNullOrEmpty(engineCfg.FileSystemRoot))
                {
                    engineCfg.FileSystemRoot = NFileSystem.DefaultFileSystemRoot;
                }
  
                //--
                NEditorConfig editorCfg = new NEditorConfig();
                editorCfg.actorEdClassAssembly = configuration.EditorEngineSetting.ActorEdClassAssembly;
                editorCfg.actorEditorClass = configuration.EditorEngineSetting.ActorEditorClass;

                configuration.Save();

                engine.InitModules(engineCfg, editorCfg);                
            }
            catch (System.Exception e)
            {
                ShowException(e, "Nexus Engine Init FAILED!");
                engine.Dispose();
                mainForm.Dispose();
                return;
            }

            try
            {
	           //-- create default level            
	            engine.CreateMainLevel("defaultLevel");
	
	            //-- create editor windows
	            mainForm.CreateEditorForms();
	            mainForm.CreateViewport();
	
	            //-- open resrouce
	            resourceForm = new ResourceEditorForm();
	
	            SplashForm.CloseSplash();             
	            
	            //-- run
	            resourceForm.Show();
	            mainForm.Show();
	            mainForm.BringToFront();
	            Application.Run(mainForm);
            }
            catch (System.Exception e)
            {
                ShowException(e, "Nexus Engine Exception!");
            }
            //-- close 
            resourceForm.Close();
            engine.Close();
            engine.Dispose();

            NLogger.Instance.Clear();

            mainForm.Dispose();
        }

        /// <summary>
        /// 对话框显示客户端异常
        /// </summary>
        /// <param name="e"></param>
        /// <param name="info"></param>
        static public void ShowException(System.Exception e, String info)
        {
            StringBuilder title = new StringBuilder();
            title.AppendFormat("Nexus Editor Error : {0}", info);

            string messsge = BuildExceptionString(e);
            NLogger.Instance.WriteString(LogType.Exception, messsge);
            MessageBox.Show(messsge, title.ToString(), MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }

        private static string BuildExceptionString(Exception exception)
        {
            string errMessage = string.Empty;

            errMessage += exception.Message + Environment.NewLine + exception.StackTrace;

            while (exception.InnerException != null)
            {
                errMessage += BuildInnerExceptionString(exception.InnerException);
                exception = exception.InnerException;
            }

            return errMessage;
        }

        private static string BuildInnerExceptionString(Exception innerException)
        {
            string errMessage = string.Empty;

            errMessage += Environment.NewLine + " InnerException ";
            errMessage += Environment.NewLine + innerException.Message + Environment.NewLine + innerException.StackTrace;

            return errMessage;
        }

        static public void ShowError(string info)
        {
            MessageBox.Show(info, "Nexus Editor Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }
    }
}
