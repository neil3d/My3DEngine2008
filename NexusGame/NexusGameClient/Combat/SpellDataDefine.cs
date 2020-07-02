namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Runtime.InteropServices;

    using NexusEngine;

    /// <summary>
    /// native struct nspell_cool_dowm
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct SpellCollDown
    {
        public UInt32 SpellID;			// 技能模板id
        public Int32 Category;			// cd公共类型
        public Int64 CDTime;			// 到期时间time_t
    }

    // buff实例数据(size = 10)
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct AuraData
    {
        public UInt32 InfoID;			// buff模板id
        public Byte Slot;			    // buff槽位, guid
        public Byte StackCount;		    // 当前叠加次数
        public UInt32 RemainTime;		// 剩余时间(单位ms)
        public UInt32 MaxDuration;	    // 总时间(单位ms)
    };
}
