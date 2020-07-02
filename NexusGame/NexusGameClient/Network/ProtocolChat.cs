namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Text;

    /// <summary>
    /// 向服务器发送聊天信息，注意按照Type用不同的结构体发送
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
    public struct C2S_ChatMessage_1
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetChat.CS_ChatMessage;
        public UInt16 MessageId;

        public Byte Type;
        public Byte MessageSize;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = (int)(NexusEngine.ChatConstant.P_MAX_MSG_LEN + 1))]
        public string Message;
    }
    /// <summary>
    /// Type = ECT_Channel 时使用
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
    public struct C2S_ChatMessage_2
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetChat.CS_ChatMessage;
        public UInt16 MessageId;

        public Byte Type;
        public Byte MessageSize;
        public UInt32 ChannelId;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = (int)(NexusEngine.ChatConstant.P_MAX_MSG_LEN + 1))]
        public string Message;
    }
    /// <summary>
    /// Type = ECT_Whisper 时使用
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
    public struct C2S_ChatMessage_3
    {
        public const UInt16 Id = (UInt16)NexusEngine.NetChat.CS_ChatMessage;
        public UInt16 MessageId;

        public Byte Type;
        public Byte MessageSize;
        public UInt64 ToPlayerId;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = (int)(NexusEngine.ChatConstant.P_MAX_MSG_LEN + 1))]
        public string Message;
    }

    /// <summary>
    /// tagS2C_ChatMessage
    /// 注意解析该消息的时候需要分为多步完成
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
    public struct S2C_ChatMessage
    {
        public UInt16 MessageId;            // [FieldOffset(0)]

        public UInt64 PlayerId;             // [FieldOffset(2)]
        public Byte Type;                   // [FieldOffset(10)]
        public Byte Flag;                   // [FieldOffset(11)]
        public Byte MsgSize;                // [FieldOffset(12)]

        // [FieldOffset(13)]
        // Union Type
    }


    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct ChatMessage_1
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = (int)(NexusEngine.ChatConstant.P_MAX_MSG_LEN+1))]
        public string Message;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct ChatMessage_2
    {
        public UInt32 ChannelId;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = (int)(NexusEngine.ChatConstant.P_MAX_MSG_LEN + 1))]
        public string Message;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct ChatMessage_3
    {
        public UInt64 ToPlayerId;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = (int)(NexusEngine.ChatConstant.P_MAX_MSG_LEN + 1))]
        public string Message;
    }

}