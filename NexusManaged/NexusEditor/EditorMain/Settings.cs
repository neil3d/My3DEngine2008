using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Win32;
using NexusEngine;
using System.Windows.Forms;

namespace NexusEditor.EditorMain
{
	class Settings
	{
		//获取指定范畴指定名称的设定项的值，如果该项目不存在，则范围指定的默认值.
		public static object ReadSetting(string section, string name,object default_value)
		{
			RegistryKey rsg = null;
			string path="Software\\RayColor\\NexusEditor\\"+section;
			rsg = Registry.CurrentUser.OpenSubKey(path);
			if (rsg == null)
			{
				return default_value;
			}
			object rst= rsg.GetValue(name);
			if (rst == null) return default_value;
			return rst;
		}
		//将制定名称的设定项储存到指定范畴内
		public static void WriteSetting(string section, string name, object value)
		{
			RegistryKey rsg = null;
			string path = "Software\\RayColor\\NexusEditor\\" + section;
			rsg = Registry.CurrentUser.OpenSubKey(path,true);
			if (rsg == null)
			{
				rsg = Registry.CurrentUser.CreateSubKey(path);
			}
			if (rsg != null)
			{
				rsg.SetValue(name, value);
			}
		}
	}
}
