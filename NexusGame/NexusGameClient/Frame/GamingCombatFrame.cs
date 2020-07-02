namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;
    using NexusEngineExtension;
    using System.Diagnostics;
    using Nexus.GUI.Widgets;
    using Nexus.GUI;

    public class GamingCombatFrame : IGameFrame
    {
        private UIProgressBar prepareProgressBar;
        private UInt32 currentProgressBarSpellID;
        private float currentProgressBarTimeStamp;
        /// <summary>
        /// 用于更新进度条,单位秒s
        /// </summary>
        private float prepareTime;
        private float prepareMaxTime;

        public GamingCombatFrame()
        {
        }

        #region IGameFrame 成员

        public void Destroyed()
        {
            prepareProgressBar = null;
        }

        public bool LoadFrame()
        {
            GameHelper.LoadUIForm(GUISystem.Instance.RootWindow, "content", "/ui/form/SpellPrepareForm.nui");

            prepareProgressBar = GUISystem.Instance.RootWindow.FindChild("PBSpellPrep", true) as UIProgressBar;
            prepareProgressBar.Hide();
            
            return true;
        }

        public void NotifyNetConnected(NetMessageEvent e)
        {
        }

        public void NotifyNetMessageReceived(MessageReceivedEvent e)
        {
            if (e.messageId == (UInt16)NetSpell.SC_AddAura)
            {
                OnRecvAddAura(e);
            }
            else if (e.messageId == (UInt16)NetSpell.SC_RemoveAura)
            {
                OnRecvRemoveAura(e);
            }
            else if (e.messageId == (UInt16)NetSpell.SC_UpdateAura)
            {
                OnRecvUpdateAura(e);
            }
            else if (e.messageId == (UInt16)NetSpell.SC_CastSpellResult)
            {
                S2C_CastSpellResult ret = MarshalConversion.PtrToStruct<S2C_CastSpellResult>(e.MessagePtr);
                GameEngine.EngineInstance.LocalPlayer.OnCastSpellResult(ret);
            }
            else if (e.messageId == (UInt16)NetSpell.SC_SpellPrepare)
            {
                // 技能准备(吟唱, 无吟唱不会受到该消息)开始或更新
                OnRecvSpellPrepare(e);
            }
            else if (e.messageId == (UInt16)NetSpell.SC_SpellCasting)
            {
                // 技能释放(瞬发, 引导或引导更新)
                OnRecvSpellCasting(e);
            }
            else if (e.messageId == (UInt16)NetSpell.SC_SpellFailed)
            {
                // 技能准备或释放过程中打断等
                OnRecvSpellFailed(e);
            }
            else if (e.messageId == (UInt16)NetSpell.SC_SpellHit)
            {
                // 技能命中信息(需要广播)
                OnRecvSpellHit(e);
            }
        }

        public void Update(float deltaTime, NViewport view)
        {
            // 更新进度条进度
            if (prepareProgressBar.Visible && prepareMaxTime > 0.0f)
            {
                prepareTime -= deltaTime;
                prepareProgressBar.Progress = prepareTime / prepareMaxTime;
            }
        }


        private void OnRecvSpellHit(MessageReceivedEvent e)
        {
            // 发送给承受伤害或治疗的角色处理
            S2C_SpellHit ret = MarshalConversion.PtrToStruct<S2C_SpellHit>(e.MessagePtr);
            if (ret.TargetId == GameEngine.EngineInstance.LocalPlayer.CharacterID)
            {
                GameEngine.EngineInstance.LocalPlayer.OnSpellHit(ret);

                GameHud.Instance.DamageListener.PushHitElement(GameEngine.EngineInstance.LocalPlayer, ret);
            }
            else
            {
                RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.TargetId);
                if (rcc != null)
                {
                    rcc.OnSpellHit(ret);

                    GameHud.Instance.DamageListener.PushHitElement(rcc, ret);
                }
                else
                {
                    NLogger.Instance.WriteString(LogType.Warning, string.Format("S2C_SpellHit can not find controller UintID=[{0}]", ret.TargetId));
                }
            }
        }

        /// <summary>
        /// 技能准备或释放过程中打断等
        /// </summary>
        private void OnRecvSpellFailed(MessageReceivedEvent e)
        {
            S2C_SpellFailed ret = MarshalConversion.PtrToStruct<S2C_SpellFailed>(e.MessagePtr);
            if (ret.CasterId == GameEngine.EngineInstance.LocalPlayer.CharacterID)
            {
                GameEngine.EngineInstance.LocalPlayer.OnSpellFailed(ret);

                // 隐藏进度条
                if (currentProgressBarSpellID == ret.SpellId)
                {
                    prepareProgressBar.Hide();
                }
            }
            else
            {
                RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.CasterId);
                if (rcc != null)
                {
                    rcc.OnSpellFailed(ret);
                }
                else
                {
                    NLogger.Instance.WriteString(LogType.Warning, string.Format("S2C_SpellFailed can not find controller UintID=[{0}]", ret.CasterId));
                }
            }
        }

        /// <summary>
        /// 技能释放(瞬发, 引导或引导更新)
        /// </summary>
        private void OnRecvSpellCasting(MessageReceivedEvent e)
        {
            S2C_SpellCasting ret = MarshalConversion.PtrToStruct<S2C_SpellCasting>(e.MessagePtr);
            if (ret.CasterId == GameEngine.EngineInstance.LocalPlayer.CharacterID)
            {
                GameEngine.EngineInstance.LocalPlayer.OnSpellCasting(ret);

                if (currentProgressBarTimeStamp <= ret.TimeStamp)
                {
                    prepareProgressBar.Hide();
                }
            }
            else
            {
                RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.CasterId);
                if (rcc != null)
                {
                    rcc.OnSpellCasting(ret);
                }
                else
                {
                    NLogger.Instance.WriteString(LogType.Warning, string.Format("S2C_SpellCasting can not find controller UintID=[{0}]", ret.CasterId));
                }
            }
        }

        /// <summary>
        /// 技能准备(吟唱, 无吟唱不会受到该消息)开始或更新
        /// </summary>
        private void OnRecvSpellPrepare(MessageReceivedEvent e)
        {
            S2C_SpellPrepare ret = MarshalConversion.PtrToStruct<S2C_SpellPrepare>(e.MessagePtr);
            if (ret.CasterId == GameEngine.EngineInstance.LocalPlayer.CharacterID)
            {
                GameEngine.EngineInstance.LocalPlayer.OnSpellPrepare(ret);

                // 显示吟唱进度条
                if (currentProgressBarTimeStamp <= ret.TimeStamp)
                {
                    if(ret.CurTime == 0)
                    {
                        prepareProgressBar.Hide();
                    }
                    else
                    {
                        NSpellInfo spell = NSpellInfo.FindInfo(ret.SpellId);
                        Debug.Assert(spell != null);

                        prepareTime = ret.CurTime * 0.001f;
                        prepareMaxTime = ret.MaxTime * 0.001f;
                        prepareProgressBar.Progress = prepareTime / prepareMaxTime;
                        prepareProgressBar.Text.Text = spell.Name;
                        prepareProgressBar.Show();
                        currentProgressBarTimeStamp = ret.TimeStamp;
                        currentProgressBarSpellID = ret.SpellId;
                    }
                }
            }
            else
            {
                RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.CasterId);
                if (rcc != null)
                {
                    rcc.OnSpellPrepare(ret);
                }
                else
                {
                    NLogger.Instance.WriteString(LogType.Warning, string.Format("S2C_SpellPrepare can not find controller UintID=[{0}]", ret.CasterId));
                }
            }
        }

        private void OnRecvUpdateAura(MessageReceivedEvent e)
        {
            S2C_UpdateAura ret = MarshalConversion.PtrToStruct<S2C_UpdateAura>(e.MessagePtr);
            AuraInstance aura = null;
            if (ret.UnitId == GameEngine.EngineInstance.LocalPlayer.CharacterID)
            {
                if (GameEngine.EngineInstance.LocalPlayer.Auras.TryGetValue(ret.Slot, out aura))
                {
                    aura.UpdateData(ret.StackCount, ret.RemainTime, ret.MaxDuration);
                    GameEngine.EngineInstance.LocalPlayer.OnAuraDataChanged(new AuraChangeEventArgs(aura));
                }
            }
            else
            {
                RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.UnitId);
                if (rcc != null)
                {
                    if (rcc.Auras.TryGetValue(ret.Slot, out  aura))
                    {
                        aura.UpdateData(ret.StackCount, ret.RemainTime, ret.MaxDuration);
                        rcc.OnAuraDataChanged(new AuraChangeEventArgs(aura));
                    }
                }
                else
                {
                    NLogger.Instance.WriteString(LogType.Warning, string.Format("S2C_UpdateAura can not find controller UintID=[{0}]", ret.UnitId));
                }
            }
        }

        private void OnRecvRemoveAura(MessageReceivedEvent e)
        {
            S2C_RemoveAura ret = MarshalConversion.PtrToStruct<S2C_RemoveAura>(e.MessagePtr);
            if (ret.UnitId == GameEngine.EngineInstance.LocalPlayer.CharacterID)
            {
                GameEngine.EngineInstance.LocalPlayer.Auras.Remove(ret.Slot);
                GameEngine.EngineInstance.LocalPlayer.OnAuraCollectionChanged(new GameEventArgs());
            }
            else
            {
                RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.UnitId);
                if (rcc != null)
                {
                    rcc.Auras.Remove(ret.Slot);
                    rcc.OnAuraCollectionChanged(new GameEventArgs());
                }
                else
                {
                    NLogger.Instance.WriteString(LogType.Warning, string.Format("S2C_RemoveAura can not find controller UintID=[{0}]", ret.UnitId));
                }
            }
        }
        private void OnRecvAddAura(MessageReceivedEvent e)
        {
            S2C_AddAura ret = MarshalConversion.PtrToStruct<S2C_AddAura>(e.MessagePtr);
            if (ret.UnitId == GameEngine.EngineInstance.LocalPlayer.CharacterID)
            {
                GameEngine.EngineInstance.LocalPlayer.Auras[ret.AuraData.Slot] = new AuraInstance(ret.AuraData);
                GameEngine.EngineInstance.LocalPlayer.OnAuraCollectionChanged(new GameEventArgs());
            }
            else
            {
                RemoteCharacterController rcc = GameEngine.EngineInstance.FindCharacterController(ret.UnitId);
                if (rcc != null)
                {
                    rcc.Auras[ret.AuraData.Slot] = new AuraInstance(ret.AuraData);
                    rcc.OnAuraCollectionChanged(new GameEventArgs());
                }
                else
                {
                    NLogger.Instance.WriteString(LogType.Warning, string.Format("SC_AddAura can not find controller UintID=[{0}]", ret.UnitId));
                }
            }
        }

        #endregion
    }
}
