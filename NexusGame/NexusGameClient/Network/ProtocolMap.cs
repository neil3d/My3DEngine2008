namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Text;
    using NexusEngine;

    /// <summary>
    /// 进入地图结果
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_MapEnter
    {
        public UInt16 messageId;
        public UInt16 ErrorCode;
    }

    /// <summary>
    /// 请求进入地图
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_MapEnter
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetMap.CS_MapEnter;
        public UInt16 messageId;
    }

    /// <summary>
    /// 地图统一错误返回消息，正确的话返回相应数据结果
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_MapError
    {
        public UInt16 messageId;
        public UInt16 ErrorCode;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_TeleportTo
    {
        public UInt16 messageId;
        public UInt32 MapId;
        public Vector3 Position;
        public float Yaw;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_TeleportAck
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetMap.CS_TeleportAck;
        public UInt16 messageId;
    }

}