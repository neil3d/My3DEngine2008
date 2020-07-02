namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;
    using System.Runtime.InteropServices;

    /// <summary>
    /// tagS2C_StatUpdate
    /// 玩家自己属性变化
    /// 变长部分需要使用nstat_update_recv_helper特殊处理
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_StatUpdate
    {
        public UInt16 MessageId;
    }

    /// <summary>
    /// tagS2C_RemoteStatUpdate
    /// 远程unit属性变化
    /// 变长部分需要使用nstat_update_recv_helper特殊处理
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_RemoteStatUpdate
    {
        public UInt16 MessageId;
        public UInt64 UnitId;
    }

}
