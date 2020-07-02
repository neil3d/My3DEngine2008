namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Text;

    using NexusEngine;

    /// <summary>
    /// 请求移动
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_Move
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetMovement.CS_Move;
        public UInt16 messageId;
        public Vector3 Position;
        public float Yaw;
        public Vector3 Acceleration;
        public float TimeStamp;
        public Byte Flag;
    }

    /// <summary>
    /// npc进入视野
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_CreatureEnterAoi
    {
        public UInt16 messageId;
        public UInt64 CreatureId;
        public Vector3 Position;
        public float Yaw;
        public Vector3 Acceleration;
        public float TimeStamp;
        public Byte Flag;
    }

    /// <summary>
    /// 游戏中静止的对象进入视野
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_GameObjectEnterAoi
    {
        public UInt16 messageId;
        public UInt64 GameObjectId;
        public Vector3 Position;
        public float Yaw;
    }

    /// <summary>
    /// 同步移动
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_Move
    {
        public UInt16 messageId;
        public UInt64 UnitId;
        public Vector3 Position;
        public float Yaw;
        public Vector3 Acceleration;
        public float TimeStamp;
        public Byte Flag;
    }

    /// <summary>
    /// 返回移动失败消息，只返回给请求移动的客户端
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_MoveError
    {
        public UInt16 messageId;
        public UInt64 ObjectId;
        public UInt16 ErrorCode;
        public Vector3 Position;
        public float Orient;
    }

    /// <summary>
    /// 对象离开视野
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_ObjectLeaveAoi
    {
        public UInt16 messageId;
        public UInt64 ObjectId;
    }

    /// <summary>
    /// 玩家进入视野
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_PlayerEnterAoi
    {
        public UInt16 messageId;
        public UInt64 PlayerId;
        public Vector3 Position;
        public float Yaw;
        public Vector3 Acceleration;
        public float TimeStamp;
        public Byte Flag;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_CheckTime
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetMovement.CS_CheckTime;
        public UInt16 MessageId;
        public float ClientTimeStamp;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_CheckTime
    {
        public UInt16 MessageId;
        public float ClientTimeStamp;
        public float ServerTimeStamp;
    }

}