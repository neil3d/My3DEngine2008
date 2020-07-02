namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;
    using System.Runtime.InteropServices;

    /// <summary>
    /// 上线加载属性数据
    /// StatData 为定长数组的一种封送形式
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_GetInitStat
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetLoading.CS_GetInitStat;
        public UInt16 MessageId;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_GetInitStat
    {
        public UInt16 MessageId;

        //[MarshalAs(UnmanagedType.ByValArray, SizeConst = ((int)NexusEngine.PlayerFields.End))]
        //public UInt32[] StatData;	// 属性, (变化部分)
    }

    /// <summary>
    /// 上线加载技能数据
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_GetInitSpell
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetLoading.CS_GetInitSpell;
        public UInt16 MessageId;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_GetInitSpell
    {
        public UInt16 MessageId;

        public UInt16 Num;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)NexusEngine.SpellConstant.MaxSpellNum)]
        public UInt32[] SpellDatas;
    }

    /// <summary>
    /// 上线加载buff数据
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_GetInitAura
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetLoading.CS_GetInitAura;
        public UInt16 MessageId;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_GetInitAura
    {
        public UInt16 MessageId;

        public UInt16 Num;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)NexusEngine.SpellConstant.MaxAuraNum)]
        public AuraData[] AuraDatas;
    }


}
