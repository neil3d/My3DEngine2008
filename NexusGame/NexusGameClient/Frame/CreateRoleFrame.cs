using NexusEngine;
namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using Nexus.GameFramework;
    using Nexus.GUI.Widgets;
    using Nexus.GUI;

    /// <summary>
    /// 创建角色
    /// </summary>
    internal sealed class CreateRoleFrame : IGameFrame
    {
        private UIEditBox uiRoleName;
        private UIRadioButton btnBoy;
        private UIRadioButton btnGirl;
        private UIButton btnOK;

        #region Constructors

        public CreateRoleFrame()
        {
        }

        #endregion Constructors

        #region Methods

        public void Destroyed()
        {
        }

        public bool LoadFrame()
        {
            GameHelper.LoadUIForm(GUISystem.Instance.RootWindow, "content", "/ui/form/CreateRoleForm.nui");

            uiRoleName = GUISystem.Instance.RootWindow.FindChild("CreateRoleNameEditBox", true) as UIEditBox;
            btnBoy = GUISystem.Instance.RootWindow.FindChild("CreateBoy", true) as UIRadioButton;
            btnGirl = GUISystem.Instance.RootWindow.FindChild("CreateGirl", true) as UIRadioButton;
            btnOK = GUISystem.Instance.RootWindow.FindChild("LoginOK", true) as UIButton;
            UIButton btnCancel = GUISystem.Instance.RootWindow.FindChild("CreateCancel", true) as UIButton;

            uiRoleName.ClearText();
            uiRoleName.TextChange += new EventHandler<WindowEventArgs>(uiRoleName_TextChange);
            
            btnOK.Clicked += new EventHandler<GUIEventArgs>(btnOK_Clicked);
            btnOK.Disable();

            btnCancel.Clicked += new EventHandler<GUIEventArgs>(btnCancel_Clicked);
          
            return true;
        }

        void btnCancel_Clicked(object sender, GUIEventArgs e)
        {
            // 返回选择角色界面
            GameFrameManager.Instance.CurrentFrameType = GameFrameType.SelectRole;
        }

        void btnOK_Clicked(object sender, GUIEventArgs e)
        {
            // 发送创建角色消息
            C2S_CharCreate msg;
            msg.messageId = C2S_CharCreate.Id;
            GameFrameManager.SendNetMessage<C2S_CharCreate>(msg);
        }

        void uiRoleName_TextChange(object sender, WindowEventArgs e)
        {
            if(uiRoleName.Text.Length > 0)
            {
                btnOK.Enable();
            }
        }

        public void NotifyNetMessageReceived(MessageReceivedEvent e)
        {
            if(e.messageId == (UInt16)NetCharacter.SC_CharCreate)
            {
                GameFrameManager.Instance.CurrentFrameType = GameFrameType.SelectRole;
            }
        }

        public void Update(float deltaTime, NViewport view)
        {
        }

        #endregion Methods

        #region IGameFrame 成员


        public void NotifyNetConnected(NetMessageEvent e)
        {
        }

        #endregion
    }
}