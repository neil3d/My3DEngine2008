namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Linq;
    using System.Text;

    using Nexus.GameFramework;

    using NexusEngine;

    using NexusEngineExtension;
    using Nexus.GUI;

    /// <summary>
    /// 游戏主Frame
    /// </summary>
    internal sealed class GamingFrame : IGameFrame
    {
        #region Constructors

        public GamingFrame()
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
            return true;
        }

        public void NotifyNetMessageReceived(MessageReceivedEvent e)
        {
            if(e.messageId == (UInt16)NetMovement.SC_Move)
            {
                OnRecvCharacterMove(e);
            }
            else if(e.messageId == (UInt16)NetMovement.SC_PlayerEnterAoi)
            {
                OnRecvPlayerEnterAoi(e);
            }
            else if(e.messageId == (UInt16)NetMovement.SC_CreatureEnterAoi)
            {
                OnRecvCreatureEnterAoi(e);
            }
            else if (e.messageId == (UInt16)NetMovement.SC_GameObjectEnterAoi)
            {
                // todo: 其他游戏对象进入AOI
            }
            else if(e.messageId ==(UInt16)NetMovement.SC_ObjectLeaveAoi)
            {
                // 游戏对象离开AOI
                S2C_ObjectLeaveAoi ret = MarshalConversion.PtrToStruct<S2C_ObjectLeaveAoi>(e.MessagePtr);
                GameEngine.EngineInstance.RemoveCharacterController(ret.ObjectId);
            }
            else if(e.messageId == (UInt16)NetStat.SC_StatUpdate)
            {
                // 玩家自己属性变化
                GameEngine.EngineInstance.LocalPlayer.StatData.OnRecvStatUpdate(new IntPtr(e.MessagePtr.ToInt32() + 2));
            }
            else if(e.messageId == (UInt16)NetStat.SC_RemoteStatUpdate)
            {
                // 远程unit属性变化
                OnRecvRemoteStatUpdate(e);
            }
        }

        private void OnRecvCreatureEnterAoi(MessageReceivedEvent e)
        {
            Debug.Assert(GameFrameManager.Instance.CurrentFrameType == GameFrameType.Gaming);

            // NPC 进入AOI
            S2C_PlayerEnterAoi ret = MarshalConversion.PtrToStruct<S2C_PlayerEnterAoi>(e.MessagePtr);
            Debug.Assert(ret.PlayerId != GameEngine.EngineInstance.LocalPlayer.CharacterID);

            // 先找到是不是存在
            RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.PlayerId);
            if (rcc == null)
            {
                rcc = GameEngine.EngineInstance.CreateRemoteCharacterController(ret.PlayerId);
                GameEngine.EngineInstance.RemoteCC[ret.PlayerId] = rcc;
            }
            Debug.Assert(rcc != null);

            rcc.Transport(GameEngine.EngineInstance.GameLevel, ret.Position, true);
            rcc.Acceleration = ret.Acceleration;
        }

        private void OnRecvPlayerEnterAoi(MessageReceivedEvent e)
        {
            // 玩家进入Aoi
            Debug.Assert(GameFrameManager.Instance.CurrentFrameType == GameFrameType.Gaming);
            S2C_PlayerEnterAoi ret = MarshalConversion.PtrToStruct<S2C_PlayerEnterAoi>(e.MessagePtr);
            if (ret.PlayerId == GameEngine.EngineInstance.LocalPlayer.CharacterID)
            {
                GameEngine.EngineInstance.LocalPlayer.Transport(GameEngine.EngineInstance.GameLevel, ret.Position, true);
                GameEngine.EngineInstance.LocalPlayer.Acceleration = ret.Acceleration;
            }
            else
            {
                // 先找到是不是存在
                RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.PlayerId);
                if (rcc == null)
                {
                    rcc = GameEngine.EngineInstance.CreateRemoteCharacterController(ret.PlayerId);
                    GameEngine.EngineInstance.RemoteCC[ret.PlayerId] = rcc;
                }
                Debug.Assert(rcc != null);

                rcc.Transport(GameEngine.EngineInstance.GameLevel, ret.Position, true);
                rcc.Acceleration = ret.Acceleration;
            }
        }
        private void OnRecvCharacterMove(MessageReceivedEvent e)
        {
            // 移动消息
            Debug.Assert(GameFrameManager.Instance.CurrentFrameType == GameFrameType.Gaming);
            S2C_Move ret = MarshalConversion.PtrToStruct<S2C_Move>(e.MessagePtr);
            if (ret.UnitId == GameEngine.EngineInstance.LocalPlayer.CharacterID)
            {
                GameEngine.EngineInstance.LocalPlayer.OnNetMovement(ret);
            }
            else
            {
                RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.UnitId);
                if (rcc != null)
                {
                    rcc.OnNetMovement(ret);
                }
                else
                {
                    NLogger.Instance.WriteString(LogType.Debug,
                        string.Format("Can not find character controller when receive S2C_Move ID=[{0}]", ret.UnitId));
                }
            }
        }

        private void OnRecvRemoteStatUpdate(MessageReceivedEvent e)
        {
            S2C_RemoteStatUpdate ret = MarshalConversion.PtrToStruct<S2C_RemoteStatUpdate>(e.MessagePtr);
            RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.UnitId);
            if (rcc != null)
            {
                rcc.StatData.OnRecvRemoteStatUpdate(new IntPtr(e.MessagePtr.ToInt32() + 10));
            }
            else
            {
                NLogger.Instance.WriteString(LogType.Warning, string.Format("SC_RemoteStatUpdate can't find the uint {0}", ret.UnitId));
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
