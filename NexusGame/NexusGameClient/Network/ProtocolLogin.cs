namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Text;

    using NexusEngine;

    /// <summary>
    /// 请求登陆验证
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_LoginProof
    {
        public const UInt16 Id = (UInt16)NetLogin.CS_LoginProof;
        public UInt16 messageId;

        public UInt32	CurVersion;			// 当前版本ID
		public UInt32	AccountID;			// 用户名名称crc值
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = (int)(NexusEngine.LoginConst.MaxAccountLen))]
		public string	Password;		    // 密码
		public UInt32	IP;					// 登陆IP
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 8)]
        public string   Mac;				// mac地址
    }

    /// <summary>
    /// 登陆验证结果
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct S2C_LoginProofResult
    {
        public UInt16 messageId;
        public UInt16 ErrorCode;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct C2S_MiBao
    {
        public const UInt16 Id = (UInt16)NetLogin.CS_MiBao;
        public UInt16 messageId;

        public UInt32 MibaoCRC;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
    public struct S2C_MiBao
    {
        public UInt16 messageId;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = (int)(NexusEngine.LoginConst.MaxMibaoLen))]
        public string MibaoCRC;
    }

}