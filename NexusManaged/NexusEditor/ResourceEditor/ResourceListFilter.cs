using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 资源过滤器, 在创建ListView时, 只添加某些后缀的文件
    /// </summary>
    public class ResourceListFilter
    {
        private bool m_allPass;
        private HashSet<string> m_fileExtSet;

        public ResourceListFilter()
        {
            m_fileExtSet = new HashSet<string>();
            m_allPass = false;
        }

        public bool EnableAll
        {
            get { return m_allPass;}
            set { m_allPass = value; }
        }

        public void EnableFileExt(string ext)
        {
            m_fileExtSet.Add(ext);
        }

        
        public void DisableFileExt(string ext)
        {
            m_fileExtSet.Remove(ext);
        }

        public bool FileEnabled(NFileEntity file)
        {
            if (file.type != EFileEntityType.Regular)
                return false;

            if (m_allPass)
                return true;

            return m_fileExtSet.Contains(file.FileExtension);
        }
    }
}
