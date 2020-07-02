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
    using System.Diagnostics;

    /// <summary>
    /// 选择角色
    /// </summary>
    internal sealed class SelectRoleFrame : IGameFrame
    {
        private UIButton btnEnterGame;

        #region Constructors

        public SelectRoleFrame()
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
            // 发送请求角色列表消息
            C2S_CharEnum msg;
            msg.messageId = C2S_CharEnum.Id;
            GameFrameManager.SendNetMessage<C2S_CharEnum>(msg);

            GameHelper.LoadUIForm(GUISystem.Instance.RootWindow, "content", "/ui/form/SelectRoleForm.nui");

            UIButton btnCreateRole = GUISystem.Instance.RootWindow.FindChild("CreateRole", true) as UIButton;
            btnEnterGame = GUISystem.Instance.RootWindow.FindChild("EnterGame", true) as UIButton;
            UIButton btnExitGame = GUISystem.Instance.RootWindow.FindChild("ExitGame", true) as UIButton;

            btnEnterGame.Disable();
            btnCreateRole.Clicked += new EventHandler<GUIEventArgs>(btnCreateRole_Clicked);
            btnEnterGame.Clicked += new EventHandler<GUIEventArgs>(btnEnterGame_Clicked);
            btnExitGame.Clicked += new EventHandler<GUIEventArgs>(btnExitGame_Clicked);

            return true;
        }

        void btnExitGame_Clicked(object sender, GUIEventArgs e)
        {
            Program.game.Exit();
        }

        void btnEnterGame_Clicked(object sender, GUIEventArgs e)
        {
            // 发送进入游戏消息
            C2S_CharEnterGame enterGameReq;
            enterGameReq.messageId = C2S_CharEnterGame.Id;
            enterGameReq.PlayerId = 125;
            GameFrameManager.SendNetMessage<C2S_CharEnterGame>(enterGameReq);

            // 请求服务器事件
            C2S_CheckTime checkTimeReq;
            checkTimeReq.MessageId = C2S_CheckTime.Id;
            checkTimeReq.ClientTimeStamp = NativeEngineHelper.GetTimeSeconds();
            GameFrameManager.SendNetMessage<C2S_CheckTime>(checkTimeReq);

            btnEnterGame.Disable();
        }

        void btnCreateRole_Clicked(object sender, GUIEventArgs e)
        {
            GameFrameManager.Instance.CurrentFrameType = GameFrameType.CreateRole;
        }

        public void NotifyNetMessageReceived(MessageReceivedEvent e)
        {
            if (e.messageId == (UInt16)NetCharacter.SC_CharError)
            {
                // todo: 显示错误消息
                e.Handled = true;
            }
            else if(e.messageId == (UInt16)NetCharacter.SC_CharEnum)
            {
                S2C_CharEnum ret = MarshalConversion.PtrToStruct<S2C_CharEnum>(e.MessagePtr);

                btnEnterGame.Enable();

                e.Handled = true;
            }
            else if (e.messageId == (UInt16)NetCharacter.SC_CharEnterGame)
            {
                // 向服务器请求获取玩家的初始属性数据
                C2S_GetInitStat initStatReq;
                initStatReq.MessageId = C2S_GetInitStat.Id;
                GameFrameManager.SendNetMessage<C2S_GetInitStat>(initStatReq);

                S2C_CharEnterGame ret = MarshalConversion.PtrToStruct<S2C_CharEnterGame>(e.MessagePtr);
                GameEngine.EngineInstance.LocalPlayer.CharacterID = ret.PlayerId;
                GameEngine.EngineInstance.LocalPlayer.Transport(GameEngine.EngineInstance.GameLevel, ret.Position, true);
                GameEngine.EngineInstance.LocalPlayer.Rotation = new Vector3(0.0f, ret.Yaw, 0.0f);
                
                e.Handled = true;
            }
            else if (e.messageId == (UInt16)NetLoading.SC_GetInitStat)
            {
                GameEngine.EngineInstance.LocalPlayer.StatData.OnRecvInitStat(new IntPtr(e.MessagePtr.ToInt32() + 2));

                // 注意发送和接受顺序
                C2S_GetInitSpell initSpellReq;
                initSpellReq.MessageId = C2S_GetInitSpell.Id;
                GameFrameManager.SendNetMessage<C2S_GetInitSpell>(initSpellReq);
            }
            else if (e.messageId == (UInt16)NetLoading.SC_GetInitSpell)
            {
                // 初始化技能
                OnRecvInitSpell(e);

                C2S_GetInitAura initAuraReq;
                initAuraReq.MessageId = C2S_GetInitAura.Id;
                GameFrameManager.SendNetMessage<C2S_GetInitAura>(initAuraReq);
            }
            else if (e.messageId == (UInt16)NetLoading.SC_GetInitAura)
            {
                // 初始化BUFF
                OnRecvInitAura(e);

                // todo: 发送进入地图消息
                C2S_MapEnter enterMapReq;
                enterMapReq.messageId = C2S_MapEnter.Id;
                GameFrameManager.SendNetMessage<C2S_MapEnter>(enterMapReq);
            }
            else if(e.messageId == (UInt16)NetMap.SC_MapEnter)
            {
                GameFrameManager.Instance.CurrentFrameType = GameFrameType.Gaming;

                e.Handled = true;
            }
        }

        private void OnRecvInitAura(MessageReceivedEvent e)
        {
            S2C_GetInitAura ret = MarshalConversion.PtrToStruct<S2C_GetInitAura>(e.MessagePtr);
            for (int index = 0; index < ret.Num; ++index)
            {
                Debug.Assert(index < ret.AuraDatas.Length);
                GameEngine.EngineInstance.LocalPlayer.Auras[ret.AuraDatas[index].Slot] = new AuraInstance(ret.AuraDatas[index]);
            }
            GameEngine.EngineInstance.LocalPlayer.OnAuraCollectionChanged(new GameEventArgs());
        }

        private void OnRecvInitSpell(MessageReceivedEvent e)
        {
            S2C_GetInitSpell ret = MarshalConversion.PtrToStruct<S2C_GetInitSpell>(e.MessagePtr);
            for (int index = 0; index < ret.Num; ++index)
            {
                Debug.Assert(index < ret.SpellDatas.Length);
                UInt32 SpellId = ret.SpellDatas[index];
                GameEngine.EngineInstance.LocalPlayer.Spells[SpellId] = new SpellInstance(SpellId);
            }
            GameEngine.EngineInstance.LocalPlayer.OnSpellCollectionChanged(new GameEventArgs());
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