using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Nexus.GameFramework
{
    /// <summary>
    /// 标识是否为场景中可摆放的物件
    /// </summary>
    [AttributeUsage(AttributeTargets.Class)]
    public class PlaceableAttribute :  System.Attribute 
    {
        private bool placeable;

        public PlaceableAttribute(bool placeable)
        {
            this.placeable = placeable;
        }
    }
}
