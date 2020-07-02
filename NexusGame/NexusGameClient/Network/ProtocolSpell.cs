namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;
    using System.Runtime.InteropServices;

    /// <summary>
    /// 请求释放技能(size = 31)
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_CastSpell
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetSpell.CS_CastSpell;
        public UInt16 MessageId;
        public UInt32 SpellID;      // 请求释放的技能id
        public UInt64 TargetId;     //  释放目标id, 没有为0
        public Vector3 TargetPos;	// 释放目标坐标, 没有不设置
        public float TimeStamp;		// 时间戳
        public Byte CastCount;		// 0 连续释放, > 0 实际次数
    }

    /// <summary>
    /// 释放技能结果(只发送给请求者)(size = 7)
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_CastSpellResult
    {
        public UInt16 MessageId;
        public UInt32 SpellId;			// 释放哪个技能
        public Byte Result;				// 结果
    }

    /// <summary>
    /// 请求取消释放技能(size = 2)
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_CancelCast
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetSpell.CS_CancelCast;
        public UInt16 MessageId;
    }

    /// <summary>
	/// 技能准备(吟唱, 无吟唱不会受到该消息)开始或更新(size = 22)
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_SpellPrepare
    {
        public UInt16 MessageId;
        public UInt64 CasterId;			// 释放者id
        public UInt32 SpellId;			// 释放的技能
        public float TimeStamp;			// 时间戳
        public UInt32 MaxTime;			// 准备最大时间ms
        public UInt32 CurTime;			// 准备剩余时间ms
    }

    /// <summary>
    /// 技能释放(瞬发, 引导或引导更新)(size = 43)
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_SpellCasting
    {
        public UInt16 MessageId;
        public UInt64 CasterId;			// 释放者id
        public UInt32 SpellId;			// 释放的技能
        public float TimeStamp;			// 时间戳
        public UInt32 MaxTime;			// 瞬发时为0, != 0 为通道最大时间, 单位ms
        public UInt32 CurTime;			// 瞬发时为0, != 0 为通道剩余时间, 单位ms
        public UInt64 TargetId;			// 释放目标id, 如果有对多个目标的技能, 需添加新的消息, 如闪电链, 对多个目标引导
        public Vector3 TargetPos;		// 释放目标坐标, 根据技能模板信息, 判断是使用坐标还是target_id
        public Byte CastIndex;			// 连续释放的索引
    }

    /// <summary>
    /// 技能准备或释放过程中打断等(需要广播)(size = 15)
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_SpellFailed
    {
        public UInt16 MessageId;
        public UInt64 CasterId;			// 释放者id
        public UInt32 SpellId;			// 释放的技
        public Byte ErrorCode;				// 错误原因
    }

    /// <summary>
    /// 技能命中信息(需要广播)(size = 15)
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_SpellHit
    {
        public UInt16 MessageId;
        public UInt64 CasterId;			// 造成伤害或治疗的玩家
        public UInt64 TargetId;			// 承受伤害或治疗的玩家
        public UInt32 InfoId;			// 哪个技能或buff
        public float TimeStamp;			// 时间戳
        public Int32 Value;				// HP伤害或治疗数值(<0为伤害, >0为治疗, =0为未命中, 完全吸收, 免疫, 反弹, 躲避)
        public Int32 Absorb;			// 伤害吸收数值
        public Byte Flag;				// 命中, 暴击, 未命中, 吸收, 免疫, 反弹, 躲避, 见ESpellHitResult
        public Byte Type;				// 0=技能, 1=buff, 见ESpellHitType
    }

    /// <summary>
    /// 请求取消buff(size = 3)
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_CancelAura
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetSpell.CS_CancelAura;
        public UInt16 MessageId;
        public Byte Slot;           // buff的唯一标识
    }

    /// <summary>
    /// 添加一个buff实例
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_AddAura
    {
        public UInt16 MessageId;

        public UInt64 UnitId;			// 谁添加
        public AuraData AuraData;  
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_RemoveAura
    {
        public UInt16 MessageId;

        public UInt64 UnitId;			// 谁删除
        public Byte Slot;				// buff的唯一标识
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_UpdateAura
    {
        public UInt16 MessageId;

        public UInt64 UnitId;		// buff拥有者id
        public Byte Slot;			// buff的唯一标识
        public Byte StackCount;		// 当前叠加次数
        public UInt32 RemainTime;	// 剩余时间(单位ms)
        public UInt32 MaxDuration;	// 总时间(单位ms)
    }
}
