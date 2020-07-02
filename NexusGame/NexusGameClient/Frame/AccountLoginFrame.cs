namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Text;

    using Nexus.GameFramework;

    using NexusEngine;

    using NexusEngineExtension;
    using Nexus.GUI;
    using Nexus.GUI.Widgets;

    /// <summary>
    /// 账号登录
    /// </summary>
    internal sealed class AccountLoginFrame : IGameFrame
    {
        private UIEditBox uiID;
        private UIEditBox uiPassword;
        private UIButton btnOK;

        #region Constructors

        public AccountLoginFrame()
        {
        }

        #endregion Constructors

        #region Methods

        public void Destroyed()
        {
            GUISystem.Instance.RootWindow.RemoveChildren();
        }

        public bool LoadFrame()
        {
            GameHelper.LoadUIForm(GUISystem.Instance.RootWindow, "content", "/ui/form/LoginForm.nui");

            uiID = GUISystem.Instance.RootWindow.FindChild("ID", true) as UIEditBox;
            uiPassword = GUISystem.Instance.RootWindow.FindChild("Password", true) as UIEditBox;
            uiPassword.MaxTextLength = (int)LoginConst.MaxPasswordLen;

            btnOK = GUISystem.Instance.RootWindow.FindChild("LoginOK", true) as UIButton;
            UIButton btnCancel = GUISystem.Instance.RootWindow.FindChild("LoginCancel", true) as UIButton;
            btnOK.Disable();
            btnOK.Clicked += new EventHandler<GUIEventArgs>(btnOK_Clicked);
            btnCancel.Clicked += new EventHandler<GUIEventArgs>(btnCancel_Clicked);
            return true;
        }

        void btnCancel_Clicked(object sender, GUIEventArgs e)
        {
            // 退出游戏
            Program.game.Exit();
        }

        void btnOK_Clicked(object sender, GUIEventArgs e)
        {
            // 发送登录消息
            // Create a point struct.
            C2S_LoginProof loginMsg;
            loginMsg.messageId = C2S_LoginProof.Id;
            loginMsg.CurVersion = 0;
            loginMsg.AccountID = NativeEngineHelper.CheckCRC(uiID.Text.ToString().Trim());
            string password = uiPassword.Text.ToString().Trim();
            loginMsg.Password = password;
            loginMsg.IP = 0;
            loginMsg.Mac = String.Empty;
            GameFrameManager.SendNetMessage<C2S_LoginProof>(loginMsg);
           
            btnOK.Disable();
        }

        public void NotifyNetConnected(NetMessageEvent e)
        {
            btnOK.Enable();
        }

        public void NotifyNetMessageReceived(MessageReceivedEvent e)
        {
            if(e.messageId == (UInt16)NetLogin.SC_LoginProofResult)
            {
                S2C_LoginProofResult ret = MarshalConversion.PtrToStruct<S2C_LoginProofResult>(e.MessagePtr);

                // 判断error code，成功则进入选人界面
                if (ret.ErrorCode == 0)
                {
                    GameFrameManager.Instance.CurrentFrameType = GameFrameType.SelectRole;
                }
                else
                {
                    btnOK.IsEnabled = true;
                }

                e.Handled = true;
            }
        }

        public void Update(float deltaTime, NViewport view)
        {
        }

        #endregion Methods

    }
}