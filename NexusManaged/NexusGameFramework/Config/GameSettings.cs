using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;

namespace Nexus.GameFramework
{
    /// <summary>
    /// 游戏配置
    /// </summary>
    public class GameSettings : ConfigurationElement
    {
        public GameSettings() { }

        [ConfigurationProperty("DefaultLevelLoad", DefaultValue = "demo_data:/level")]
        public string DefaultLevelLoad
        {
            get { return this["DefaultLevelLoad"].ToString(); }
            set { this["DefaultLevelLoad"] = value; }
        }

        [ConfigurationProperty("CameraMinDistance", DefaultValue = "10")]
        public float CameraMinDistance
        {
            get { return (float)this["CameraMinDistance"]; }
            set { this["CameraMinDistance"] = value; }
        }

        [ConfigurationProperty("CameraMaxDistance", DefaultValue = "1000")]
        public float CameraMaxDistance
        {
            get { return (float)this["CameraMaxDistance"]; }
            set { this["CameraMaxDistance"] = value; }
        }

        [ConfigurationProperty("CameraMinPitch", DefaultValue = "-0.15")]
        public float CameraMinPitch
        {
            get { return (float)this["CameraMinPitch"]; }
            set { this["CameraMinPitch"] = value; }
        }

        [ConfigurationProperty("CameraMaxPitch", DefaultValue = "-3.14")]
        public float CameraMaxPitch
        {
            get { return (float)this["CameraMaxPitch"]; }
            set { this["CameraMaxPitch"] = value; }
        }
    }
}
