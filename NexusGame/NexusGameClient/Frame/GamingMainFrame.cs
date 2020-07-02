using Nexus.GUI.Widgets;
namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;
    using Nexus.GUI;

    public class MessageElement
    {
        public string Message { get; private set; }
        public float TimeStamp { get; private set; }
        public float CountTime { get; set; }

        public MessageElement(string message)
        {
            this.Message = message;
            this.TimeStamp = NativeEngineHelper.GetTimeSeconds();
            this.CountTime = 0;
        }

        private MessageElement()
        {
        }
    }


    public sealed class GamingMainFrame : IGameFrame
    {
        #region Fields
        private UIProgressBar myHP;
        private UIProgressBar myMP;
        //private UIProgressBar myEnergy;
        private UIStaticText myMessageTips1;
        private UIStaticText myMessageTips2;
        private UIStaticText myMessageTips3;
        public readonly float UpdateMessageTipsTime = 2.0f;

        public static List<MessageElement> MessageTips = new List<MessageElement>();
        #endregion

        #region IGameFrame 成员

        public void Destroyed()
        {
        }

        public bool LoadFrame()
        {
            GameHelper.LoadUIForm(GUISystem.Instance.RootWindow, "content", "/ui/form/MainForm.nui");

            myHP = GUISystem.Instance.RootWindow.FindChild("MyHP", true) as UIProgressBar;
            myMP = GUISystem.Instance.RootWindow.FindChild("MyMP", true) as UIProgressBar;
            //myEnergy = GUISystem.Instance.RootWindow.FindChild("MyEnergy", true) as UIProgressBar;

            myMessageTips1 = GUISystem.Instance.RootWindow.FindChild("MessageTips1", true) as UIStaticText;
            myMessageTips1.TextRenderComp.Text = String.Empty;
            myMessageTips1.Hide();
            myMessageTips2 = GUISystem.Instance.RootWindow.FindChild("MessageTips2", true) as UIStaticText;
            myMessageTips2.TextRenderComp.Text = String.Empty;
            myMessageTips2.Hide();
            myMessageTips3 = GUISystem.Instance.RootWindow.FindChild("MessageTips3", true) as UIStaticText;
            myMessageTips3.TextRenderComp.Text = String.Empty;
            myMessageTips3.Hide();

            GameEngine.EngineInstance.LocalPlayer.StatInited += new EventHandler<StatEventArgs>(LocalPlayerController_StatInited);
            GameEngine.EngineInstance.LocalPlayer.StatUpdated += new EventHandler<StatEventArgs>(LocalPlayerController_StatUpdated);

            UpdateStatInfo(GameEngine.EngineInstance.LocalPlayer.StatData);

            return true;
        }

        private void LocalPlayerController_StatInited(object sender, StatEventArgs e)
        {
            UpdateStatInfo(e.Data);
        }

        /// <summary>
        /// 更新角色状态条
        /// </summary>
        private void UpdateStatInfo(NCharacterStatData stat)
        {
            UInt32 currentHP = stat.GetUInt32Value((UInt16)UnitFields.Health);
            UInt32 maxHP = stat.GetUInt32Value((UInt16)UnitFields.MaxHealth);
            UInt32 currentMP = stat.GetUInt32Value((UInt16)UnitFields.Mana);
            UInt32 maxMP = stat.GetUInt32Value((UInt16)UnitFields.MaxMana);
            //UInt32 currentEnergy = stat.GetUInt32Value((UInt16)UnitFields.Energy);
            //UInt32 maxEnergy = stat.GetUInt32Value((UInt16)UnitFields.MaxEnergy);

            //this.myHP.Text.Text = currentHP.ToString() + "/" + maxHP.ToString();
            this.myHP.Progress = maxHP != 0 ? (float)currentHP / (float)maxHP : 1;

            //this.myMP.Text.Text = currentMP.ToString() + "/" + maxMP.ToString();
            this.myMP.Progress = maxMP != 0 ? (float)currentMP / (float)maxMP : 1;

            //this.myEnergy.Text.Text = currentEnergy.ToString() + "/" + maxEnergy.ToString();
            //this.myEnergy.Progress = maxEnergy != 0 ? (float)currentEnergy / (float)maxEnergy : 1;
        }

        private void LocalPlayerController_StatUpdated(object sender, StatEventArgs e)
        {
            UpdateStatInfo(e.Data);
        }

        public void NotifyNetConnected(NetMessageEvent e)
        {
        }

        public void NotifyNetMessageReceived(MessageReceivedEvent e)
        {
        }

        public void Update(float deltaTime, NViewport view)
        {
            UpdateMessageTipsStatck(deltaTime);
        }

        private void UpdateMessageTipsStatck(float deltaTime)
        {
            foreach (var item in MessageTips)
            {
                item.CountTime += deltaTime;
                if (item.CountTime > UpdateMessageTipsTime)
                {
                    MessageTips.Remove(item);
                    break;
                }
            }

            if (MessageTips.Count > 0)
            {
                myMessageTips1.TextRenderComp.Text = MessageTips[0].Message;
                myMessageTips1.Show();
            }
            else
            {
                myMessageTips1.Hide();
            }

            if (MessageTips.Count > 1)
            {
                myMessageTips2.TextRenderComp.Text = MessageTips[1].Message;
                myMessageTips2.Show();
            }
            else
            {
                myMessageTips2.Hide();
            }

            if (MessageTips.Count > 2)
            {
                myMessageTips3.TextRenderComp.Text = MessageTips[2].Message;
                myMessageTips3.Show();
            }
            else
            {
                myMessageTips3.Hide();
            }
        }
        #endregion
    }
}
