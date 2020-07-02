using System;
using System.ComponentModel;
using System.Drawing.Design;
using System.Windows.Forms.Design;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 显示/编辑resource基类的数据
    /// </summary>
    class ResProperty
    {
        protected string m_name;
        protected NResource m_resource;        

        public ResProperty(NResource res)
        { 
            m_resource = res;
            m_name = res.Name;
        }

        [Category("Resource Base")]
        public string Name
        {
            get { return m_name; }
            set { m_name = value; }
            
        }

        [Category("Resource Base")]
        public string Location
        {
            get
            {
                return m_resource.Location.ToString();
            }
        }        
    }
}
