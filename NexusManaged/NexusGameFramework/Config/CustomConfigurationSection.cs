using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;

namespace Nexus.GameFramework
{
    /// <summary>
    /// 自定义表示配置文件中的节
    /// </summary>
    public class CustomConfigurationSection : ConfigurationSection
    {
        #region Constructor
        private CustomConfigurationSection() { }
        #endregion

        #region Public Methods

        ///<summary>
        ///Get this configuration set from the application's default config file
        ///</summary>
        public static CustomConfigurationSection Open()
        {
            System.Reflection.Assembly assy = System.Reflection.Assembly.GetEntryAssembly();
            return Open(assy.Location);
        }

        ///<summary>
        /// Get this configuration set from a specific config file
        ///</summary>
        public static CustomConfigurationSection Open(string path)
        {
            if ((object)instance == null)
            {
                Configuration config = ConfigurationManager.OpenExeConfiguration(ParserConfigurationPath(path));
                if (config.Sections["CustomConfigurationSection"] == null)
                {
                    instance = new CustomConfigurationSection();
                    config.Sections.Add("CustomConfigurationSection", instance);
                    config.Save(ConfigurationSaveMode.Modified);
                }
                else
                    instance = (CustomConfigurationSection)config.Sections["CustomConfigurationSection"];
            }
            return instance;
        }

        private static string ParserConfigurationPath(string path)
        {
            if (path.EndsWith(".config", StringComparison.InvariantCultureIgnoreCase))
                return path.Remove(path.Length - 7);
            else
                return path;
        }
        ///<summary>
        ///Create a full copy of the current properties
        ///</summary>
        public CustomConfigurationSection Copy()
        {
            CustomConfigurationSection copy = new CustomConfigurationSection();
            string xml = SerializeSection(this, "CustomConfigurationSection", ConfigurationSaveMode.Full);
            System.Xml.XmlReader rdr = new System.Xml.XmlTextReader(new System.IO.StringReader(xml));
            copy.DeserializeSection(rdr);
            return copy;
        }

        ///<summary>
        ///Save the current property values to the config file
        ///</summary>
        public void Save()
        {
            System.Reflection.Assembly assy = System.Reflection.Assembly.GetEntryAssembly();
            Save(assy.Location);
        }

        ///<summary>
        ///Save the current property values to the config file
        ///</summary>
        public void Save(string path)
        {
            Configuration config = ConfigurationManager.OpenExeConfiguration(ParserConfigurationPath(path));
            CustomConfigurationSection section = (CustomConfigurationSection)config.Sections["CustomConfigurationSection"];

            // copy data
            section.EngineSetting = this.EngineSetting;
            section.EditorEngineSetting = this.EditorEngineSetting;

            //Try with "Modified" to see the difference
            config.Save(ConfigurationSaveMode.Full); 
        }

        #endregion Public Methods

        #region Properties

        public static CustomConfigurationSection Default
        {
            get { return defaultInstance; }
        }

        [ConfigurationProperty("EditorEngineSetting")]
        public EditorEngineSettings EditorEngineSetting
        {
            get { return (EditorEngineSettings)this["EditorEngineSetting"]; }
            set { this["EditorEngineSetting"] = value; }
        }

        [ConfigurationProperty("EngineSettings")]
        public EngineSettings EngineSetting
        {
            get { return (EngineSettings)this["EngineSettings"]; }
            set { this["EngineSettings"] = value; }
        }

        [ConfigurationProperty("GameSettings")]
        public GameSettings GameSetting
        {
            get { return (GameSettings)this["GameSettings"]; }
            set { this["GameSettings"] = value; }
        }

        #endregion Properties

        #region Fields
        private static CustomConfigurationSection instance = null;
        private static readonly CustomConfigurationSection defaultInstance = new CustomConfigurationSection();
        #endregion Fields
    }
}
