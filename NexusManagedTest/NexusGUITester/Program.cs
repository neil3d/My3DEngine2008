using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Drawing;
using System.IO;
using System.Text;

using NexusEngine;
using Nexus.GUI;

namespace Nexus.GUI.Tester
{
    static class Program
    {
        // 声明全局变量
        public static NGameEngine g_GameEngine;
        public static GUITesterMainForm form = null;

        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            try
            {
                g_GameEngine = new NGameEngine();
                g_GameEngine.InitCore();

                form = GUITesterMainForm.App;

                //-- create engine config
                NEngineConfig engineCfg = new NEngineConfig();
                engineCfg.RenderWndHandle = form.RenderPanelHandle;
                engineCfg.ClientWidth = Screen.PrimaryScreen.Bounds.Width;
                engineCfg.ClientHeight = Screen.PrimaryScreen.Bounds.Height;
                engineCfg.ColorBits = 32;
                engineCfg.FullScreen = false;
                engineCfg.EnableHDR = false;
                engineCfg.RenderClass = "nrenderer_d3d9";
                engineCfg.FileSystemClass = "nstd_file_system";
                engineCfg.EngineDataPkg = "engine_data";
                // 设置File System根目录为包含EngineDataPkg的目录
                //engineCfg.FileSystemRoot = "/work/nexus_engine";
                DirectoryInfo DirInfo = new DirectoryInfo(".");
                DirectoryInfo DirIter = DirInfo.Parent;
                while (DirIter != null && engineCfg.FileSystemRoot == null)
                {
                    foreach (DirectoryInfo SubDir in DirIter.GetDirectories())
                    {
                        if (SubDir.Name == engineCfg.EngineDataPkg)
                        {
                            // 如果该目录的子目录中包含EngineDataPkg，则取该目录为FileSystemRoot
                            engineCfg.FileSystemRoot = DirIter.FullName;
                            break;
                        }
                    }
                    DirIter = DirIter.Parent;
                }
                if (engineCfg.FileSystemRoot == null)
                {
                    // 如果没有找到则设置缺省的目录
                    engineCfg.FileSystemRoot = DirInfo.Parent.FullName;
                }

                //--
                g_GameEngine.InitModules(engineCfg);

                //-- create Viewport
                form.CreateViewport();
              
                Application.Run(form);
            }
            catch (System.Exception e)
            {
                MessageBox.Show( e.Message, e.GetType().ToString(),MessageBoxButtons.OK, MessageBoxIcon.Warning );
            }


            //-- close
            g_GameEngine.Close();
            g_GameEngine.Dispose();
        }
    }
}
