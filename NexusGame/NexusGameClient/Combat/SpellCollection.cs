
namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngineExtension;
    using NexusEngine;
    using System.Diagnostics;


    /// <summary>
    /// 技能实例数据
    /// </summary>
    public class SpellInstance
    {
        public NSpellInfo SpellInfo { get; private set; }

        public SpellInstance(NSpellInfo info)
        {
            SpellInfo = info;
        }

        public SpellInstance(UInt32 infoId)
        {
            SpellInfo = NSpellInfo.FindInfo(infoId);
            Debug.Assert(SpellInfo != null);
        }
    }

    // 技能容器
    public class SpellCollection : Dictionary<UInt32, SpellInstance>
    {

    }
     
}
