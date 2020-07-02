namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Text;

    using NexusEngine;

    /// <summary>
    /// 请求创建角色
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_CharCreate
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetCharacter.CS_CharCreate;
        public UInt16 messageId;
    }

    /// <summary>
    /// 请求删除角色
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_CharDelete
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetCharacter.CS_CharDelete;
        public UInt16 messageId;
    }

    /// <summary>
    /// 进入游戏
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_CharEnterGame
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetCharacter.CS_CharEnterGame;
        public UInt16 messageId;
        public UInt64 PlayerId;
    }

    /// <summary>
    /// 请求角色列表
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_CharEnum
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetCharacter.CS_CharEnum;
        public UInt16 messageId;
    }

    /// <summary>
    /// 离开游戏
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_CharLeaveGame
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetCharacter.CS_CharLeaveGame;
        public UInt16 messageId;
    }

    /// <summary>
    /// 创建角色结果
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_CharCreate
    {
        public UInt16 messageId;
    }

    /// <summary>
    /// 删除角色结果
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_CharDelete
    {
        public UInt16 messageId;
    }

    /// <summary>
    /// 进入游戏
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_CharEnterGame
    {
        public UInt16 messageId;
        public UInt64 PlayerId;
        public Vector3 Position;
        public float Yaw;
    }

    /// <summary>
    /// 角色列表结果
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_CharEnum
    {
        public UInt16 messageId;
    }

    /// <summary>
    /// 角色管理统一错误返回消息，正确的话返回相应数据结果
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_CharError
    {
        public UInt16 messageId;
        public UInt16 ErrorCode;
    }
}