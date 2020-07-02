using NexusEngine;
namespace NexusGameClient
{
    using System;
    using System.Text;
    using System.Windows.Forms;

    static class Program
    {
        #region Fields

        public static GameClient game;

        #endregion Fields

        #region Methods

        /// <summary>
        /// 对话框显示异常
        /// </summary>
        /// <param name="e"></param>
        /// <param name="info"></param>
        public static void ShowException(System.Exception e, String info)
        {
            StringBuilder title = new StringBuilder();
            title.AppendFormat("Nexus Game Error : {0}", info);
           
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

        [STAThread]
        static void Main()
        {
            Environment.CurrentDirectory = Application.StartupPath;

            using (game = new GameClient())
                game.Run();
        }

        #endregion Methods
    }
}