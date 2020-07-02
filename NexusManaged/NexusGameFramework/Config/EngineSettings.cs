using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;

namespace Nexus.GameFramework
{
    /// <summary>
    /// 引擎配置选项，对应NEngineConfig
    /// </summary>
    public class EngineSettings : ConfigurationElement
    {
        #region Constructor
        public EngineSettings() { }
        #endregion

        #region Porperties
        /// <summary>
        /// 窗口宽度
        /// </summary>
        [ConfigurationProperty("ClientWidth", DefaultValue = "1024")]
        public Int32 ClientWidth
        {
            get { return (Int32)this["ClientWidth"]; }
            set { this["ClientWidth"] = value; }
        }

        /// <summary>
        /// 窗口高度
        /// </summary>
        [ConfigurationProperty("ClientHeight", DefaultValue = "768")]
        public Int32 ClientHeight
        {
            get { return (Int32)this["ClientHeight"]; }
            set { this["ClientHeight"] = value; }
        }

        /// <summary>
        /// 颜色深度
        /// </summary>
        [ConfigurationProperty("ColorBits", DefaultValue = "32")]
        public Byte ColorBits
        {
            get { return (Byte)this["ColorBits"]; }
            set { this["ColorBits"] = value; }
        }

        /// <summary>
        /// 是否全屏
        /// </summary>
        [ConfigurationProperty("FullScreen", DefaultValue = "false")]
        public bool FullScreen
        {
            get { return (bool)this["FullScreen"]; }
            set { this["FullScreen"] = value; }
        }

        [ConfigurationProperty("EnableHDR", DefaultValue = "false")]
        public bool EnableHDR
        {
            get { return (bool)this["EnableHDR"]; }
            set { this["EnableHDR"] = value; }
        }

        [ConfigurationProperty("EnableSSAO", DefaultValue = "false")]
        public bool EnableSSAO
        {
            get { return (bool)this["EnableSSAO"]; }
            set { this["EnableSSAO"] = value; }
        }

        [ConfigurationProperty("RenderClass", DefaultValue = "nrenderer_d3d9")]
        public string RenderClass
        {
            get { return (string)this["RenderClass"]; }
            set { this["RenderClass"] = value; }
        }


        [ConfigurationProperty("FileSystemClass", DefaultValue = "nstd_file_system")]
        public string FileSystemClass
        {
            get { return (string)this["FileSystemClass"]; }
            set { this["FileSystemClass"] = value; }
        }

        [ConfigurationProperty("EngineDataPkg", DefaultValue = "engine_data")]
        public string EngineDataPkg
        {
            get { return (string)this["EngineDataPkg"]; }
            set { this["EngineDataPkg"] = value; }
        }

        [ConfigurationProperty("FileSystemRoot", DefaultValue = null)]
        public string FileSystemRoot
        {
            get { return (string)this["FileSystemRoot"]; }
            set { this["FileSystemRoot"] = value; }
        }

        [ConfigurationProperty("ResourceCacheClass", DefaultValue = "nresource_cache_basic")]
        public string ResourceCacheClass
        {
            get { return (string)this["ResourceCacheClass"]; }
            set { this["ResourceCacheClass"] = value; }
        }

        [ConfigurationProperty("ResourceIOClass", DefaultValue = "nresource_io_basic")]
        public string ResourceIOClass
        {
            get { return (string)this["ResourceIOClass"]; }
            set { this["ResourceIOClass"] = value; }
        }
        #endregion
    }
}
