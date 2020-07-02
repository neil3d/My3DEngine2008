namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;
    using Nexus.GUI;

    public static class GameHelper
    {
        public static WindowCollection LoadUIForm(Window parent, string pkgName, string fileName)
        {
            WindowCollection ret = new WindowCollection(); 
            Window window = WindowManager.Instance.Deserialize(pkgName, fileName);
            if (window != null)
            {
                if (window.Name.Equals(GUISystem.DefaultRootWindowName, StringComparison.OrdinalIgnoreCase))
                {
                    // 如果序列化的窗口时根节点，那么自处理它的子节点，约定根节点的名字为root
                    foreach (var item in window.Children)
                    {
                        item.PostSerial(parent);
                    }

                    ret = window.Children;
                }
                else
                {
                    window.PostSerial(parent);
                    ret.Add(window);
                }
            }
            return ret;
        }
    }
}
