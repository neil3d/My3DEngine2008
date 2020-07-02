using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;
using System.Windows.Forms;

namespace NexusEditor.UIEditor
{
    public class UIFileQurery : NFileQuery, IDisposable
    {
        private ListView listView;

        public UIFileQurery(ListView listControl, string pkgName, string fileName)
        {
            this.listView = listControl;

            ReBuildList(pkgName, fileName);
        }

        private void ReBuildList(string pkgName, string fileName)
        {
            listView.BeginUpdate();
            listView.Items.Clear();

            NFileSystem fileSys = Program.engine.FileSystem;
            fileSys.QueryPackage(pkgName, fileName, this);

            listView.EndUpdate();
        }

        /// <summary>
        /// 引擎Callback函数
        /// </summary>
        /// <param name="file">文件对象描述</param>
        public override void OnFileEntity(ref NFileEntity file)
        {
            if (file.type != EFileEntityType.Regular)
            {
                return;
            }

            if(file.FileName.Substring(file.FileName.Length - 4).Equals(".nui"))
            {
                ListViewItem it = listView.Items.Add(file.FilePath);
                it.Tag = file;
            }
        }

        #region IDisposable 成员
        public void Dispose()
        {
        }
        #endregion
    }
}
