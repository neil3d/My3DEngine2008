using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Win32;
using NexusEngine;
using System.Windows.Forms;

namespace NexusEditor.ResourceEditor
{
	//使用注册表实现最近使用功能。
	//最近使用的文件名保存在[HKEY_CURRENT_USER\Software\RayColor\NexusEditor\RecentFiles\]键下
	class RecentFile
	{
		public RecentFile()
		{
			m_MaxItemCount = 10;
			m_FileList = new List<NResourceLoc>();
		}

		public int FileCount
		{
			get
			{
				return m_FileList.Count;
			}
		}

		public NResourceLoc GetFile(int index)
		{
			if(index<0 || index>=m_FileList.Count)
			{
				return new NResourceLoc();
			}
			return m_FileList[index];
		}

		//从注册表中读取，然后加载到菜单
		public bool Load()
		{
			RegistryKey rsg = null;
			rsg = Registry.CurrentUser.OpenSubKey("Software\\RayColor\\NexusEditor\\RecentFiles");
			if(rsg==null)
			{
				return false;
			}
			int count = (int)rsg.GetValue("FileCount");
			for (int i = 0; i <count; ++i)
			{
				string fn = (string)rsg.GetValue(i.ToString());
				NResourceLoc loc=new NResourceLoc(fn);
				if (NLevelEditorEngine.Instance.FileSystem.IsFileExist(loc.PackageName,loc.FileName))
				{
					m_FileList.Add(loc);
				}
			}
			return true;
		}
		//保存入注册表
		public void Save()
		{
			RegistryKey rsg = null;
			rsg = Registry.CurrentUser.OpenSubKey("Software\\RayColor\\NexusEditor\\RecentFiles", true);
			if (rsg==null)
			{
				rsg = Registry.CurrentUser.CreateSubKey("Software\\RayColor\\NexusEditor\\RecentFiles");
			}
			if(rsg!=null)
			{
				foreach (string name in rsg.GetValueNames())
				{
					rsg.DeleteValue(name);
				}
				rsg.SetValue("FileCount", m_FileList.Count);
				for(int i=0;i<m_FileList.Count;++i)
				{
					string fn = m_FileList[i].ToString();
					rsg.SetValue(i.ToString(), fn);
				}
			}
		}
		//添加一个新的文件(需要检测是否有重复，如果有重复，则删除旧的，添加新的)
		public void	Add(NResourceLoc loc)
		{
			if (!loc.IsValid()) return;
			int i = m_FileList.IndexOf(loc);
			if(i!=-1)
			{
				m_FileList.RemoveAt(i);
			}
			if(m_FileList.Count>=m_MaxItemCount)
			{
				m_FileList.RemoveAt(m_FileList.Count - 1);
			}
			m_FileList.Insert(0, loc);
		}
		//移除一个文件
		public void Remove(NResourceLoc loc)
		{
			m_FileList.Remove(loc);
		}
		//最多存储多少条记录，默认为10条,新的自动覆盖旧的
		private int m_MaxItemCount;
		//文件列表
		private List<NResourceLoc> m_FileList;
	}
}
