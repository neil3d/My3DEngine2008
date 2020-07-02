namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;
    using System.Diagnostics;

    /// <summary>
    /// buff 实例
    /// </summary>
    public class AuraInstance
    {
        private AuraData m_instanceData;

        public NSpellAuraInfo AuraInfo { get; private set; }

        public AuraData Data
        {
            get { return m_instanceData; }
            private set
            {
                m_instanceData = value;
            }
        }

        public AuraInstance(AuraData data)
        {
            Data = data;
            AuraInfo = NSpellAuraInfo.FindInfo(data.InfoID);
            Debug.Assert(AuraInfo != null);
        }

        public void UpdateData(Byte stackCount, UInt32 remainTime, UInt32 maxDuration)
        {
            m_instanceData.StackCount = stackCount;
            m_instanceData.RemainTime = remainTime;
            m_instanceData.MaxDuration = maxDuration;
        }
    }

    /// <summary>
    /// buff 容器
    /// </summary>
    public class AuraCollection : Dictionary<Byte, AuraInstance>
    {
    }
}
