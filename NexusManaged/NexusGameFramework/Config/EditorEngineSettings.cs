using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;
using NexusEngine;

namespace Nexus.GameFramework
{
    /// <summary>
    /// 编辑器配置选项，对应 NEditorConfig
    /// </summary>
    public class EditorEngineSettings : ConfigurationElement
    {
        public EditorEngineSettings() { }

        [ConfigurationProperty("ActorEdClassAssembly", DefaultValue = "NexusEditor")]
        public string ActorEdClassAssembly
        {
            get { return this["ActorEdClassAssembly"].ToString(); }
            set { this["ActorEdClassAssembly"] = value; }
        }

        [ConfigurationProperty("ActorEditorClass", DefaultValue = "NexusEditor.NActorEditor")]
        public string ActorEditorClass
        {
            get { return this["ActorEditorClass"].ToString(); }
            set { this["ActorEditorClass"] = value; }
        }
    }
}
