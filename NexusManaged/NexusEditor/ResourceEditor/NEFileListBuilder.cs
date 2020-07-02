using System;
using System.Windows.Forms;
using NexusEngine;
using System.Drawing;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 查询文件系统中指定的package的指定目录中的所有文件，并添加到ListView中
    /// </summary>
    class NEFileListBuilder : NFileQuery
    {
        private ListView m_listCtrl;
        private ImageList m_imgList;
        private ImageList m_imgListSmall;
        private string m_pkgName;
        private string m_path;
        private ResourceListFilter m_filter;

        public NEFileListBuilder(ListView listCtrl, ImageList imgList, ImageList imgListSmall, string pkgName, string path)
        {
            m_listCtrl = listCtrl;
            m_imgList = imgList;
            m_imgListSmall = imgListSmall;
            m_pkgName = pkgName;
            m_path = path;
        }

        public void SetFilter(ResourceListFilter filter)
        {
            m_filter = filter;
        }

        /// <summary>
        /// 外部调用接口
        /// </summary>
        public void BuildList()
        {
            m_listCtrl.BeginUpdate();
            m_listCtrl.Items.Clear();
            if(m_imgList!=null)
                m_imgList.Images.Clear();

            NFileSystem fileSys = Program.engine.FileSystem;
            fileSys.QueryPackage(m_pkgName, m_path, this);

            m_listCtrl.EndUpdate();
        }

        /// <summary>
        /// 引擎Callback函数
        /// </summary>
        /// <param name="file">文件对象描述</param>
        public override void OnFileEntity(ref NFileEntity file)
        {
            if (file.type == EFileEntityType.Regular)
            {
                bool add = true;
                if (m_filter!=null)                
                    add = m_filter.FileEnabled(file);

                if (add)
                {
                    ListViewItem it = m_listCtrl.Items.Add(file.FileName);
                    string thumbName =
                        ThumbnailManager.Instance.LoadThumbnail(file.PackageName, file.FilePath,
                        m_imgList, m_imgListSmall);

                    uint fileSize = NEngine.Instance().FileSystem.GetFileSize(file.PackageName, file.FilePath);
                    DateTime fileTime = NEngine.Instance().FileSystem.GetFileTime(file.PackageName, file.FilePath);

                    it.ImageKey = thumbName;
                    it.Tag = file;

                    //-- sub item [file size]
                    string strFileSize;
                    if (fileSize > 1024)
                        strFileSize = string.Format("{0}KB", fileSize / 1024);
                    else
                        strFileSize = fileSize.ToString();
                    it.SubItems.Add(strFileSize);

                    //-- sub item [resource type]
                    it.SubItems.Add(GetResourceTypeByFileExt(file.FileExtension));

                    //-- sub item [file time]
                    string strFileTime = string.Format("{0} {1}",
                        fileTime.ToShortDateString(), fileTime.ToShortTimeString());
                    it.SubItems.Add(strFileTime);
                }
            }
        }

        private string GetResourceTypeByFileExt(string ext)
        {
            string ret = "Unknown";
            switch (ext)
            { 
                case "nam":
                    ret = "Anim Mesh";
                    break;
                case "nmdl":
                    ret = "Static mesh";
                    break;
                case "spt":
                    ret = "Speed Tree";
                    break;
                case "skm":
                    ret = "Skeletal Mesh";
                    break;
                case "animset":
                    ret = "Skeletal Anim Set";
                    break;
                case "txa":
                    ret = "Texture Atlas";
                    break;
                case "nui":
                    ret = "GUI File";
                    break;
                case "mtl":
                    ret = "Material";
                    break;
                case "att":
                    ret = "Game Object Attribute";
                    break;
                case "dds":
                case "tga":
                case "jpg":
                case "bmp":
                case "png":
                    ret = "Texture";
                    break;
            }
            return ret;
        }
    }
}
