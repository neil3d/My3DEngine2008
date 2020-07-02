namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Windows.Forms;

    using Nexus.GUI;

    using NexusEngine;
    using System.Diagnostics;

    internal sealed class LocalPlayerController : GameCharacterController
    {
        #region Fields

        /// <summary>
        /// 是否按下跳跃键
        /// </summary>
        public bool PressJump { get; set; }

        private Vector3 moveTargetDirect;

        /// <summary>
        /// 移动的目的地
        /// </summary>
        private Vector3 moveTargetLocation;
        private bool reachTarget;

        /// <summary>
        /// 处理玩家输入
        /// </summary>
        public GamePlayerInput PlayerInput { get; private set; }

        /// <summary>
        /// 摄像机控制
        /// </summary>
        public CameraGameplay GameplayCamera { get; private set; }
        public List<GameCameraModifier> CameraModifiers { get; private set; }
        
        /// <summary>
        /// 向服务器发送的移动消息队列，用于做客户端校验
        /// </summary>
        public List<C2S_Move> NetMovementMessages { get; private set; }

        #endregion Fields

        #region Constructors

        public LocalPlayerController()
        {
            PlayerInput = new GamePlayerInput();
            CameraModifiers = new List<GameCameraModifier>();
            NetMovementMessages = new List<C2S_Move>();

            // 注册玩家输入
            PlayerInput.InjectInputEvent(GUISystem.Instance.RootWindow);
            PlayerInput.KeyDown += new KeyEventHandler(PlayerInput_KeyDown);
            PlayerInput.MouseButtonDown += new EventHandler<GUIMouseEventArgs>(PlayerInput_MouseButtonDown);

            GameplayCamera = new CameraGameplay();
        }

        #endregion Constructors

        #region Properties

        public Vector3 MoveTargetLocation
        {
            get { return moveTargetLocation; }
            set { moveTargetLocation = value; }
        }


        #endregion Properties

        #region Methods

        public void Update(float deltaTime, NViewport vp)
        {
            // 更新输入
            PlayerInput.UpdateInput(deltaTime);
            PreProcessInput(PlayerInput, deltaTime);

            GameplayCamera.UpdateCamera(this, vp.Camera, deltaTime);
            foreach (var camera in CameraModifiers)
            {
                camera.UpdateCamera(this, vp.Camera, deltaTime);
            }
            PlayerMove(deltaTime, vp.Camera);

            Update(deltaTime);

            PlayerInput.ResetInput();
            PressJump = false;
        }

        public override void Update(float deltaTime)
        {
            base.Update(deltaTime);

#if DEBUG
            GameViewportControl.debugInfos.Add(string.Format(System.Globalization.CultureInfo.CurrentCulture,
                "RemoteCharacterController CurrentMovementType[{0}] Acceleration[{1}] Velocity[{2}] Location[{3}] Rotation[{4}] \r\n",
                MovementType.ToString(),
                Acceleration.ToString(true),
                Velocity.ToString(true),
                Location.ToString(true),
                TargetActor.Rotation.ToString(true)));
#endif
        }

        /// <summary>
        /// 处理玩家移动
        /// </summary>
        public void PlayerMove(float deltaTime, NCamera camera)
        {
            GameEngine engine = GameEngine.EngineInstance;
            if (TargetActor == null || engine == null)
                return;

            if (engine.CurrentLevel == null)
                return;

            KeyMove(camera);
            MouseMove(camera);
        }

        /// <summary>
        /// 键盘移动
        /// </summary>
        private void KeyMove(NCamera camera)
        {
            this.Walk = PlayerInput.IsKeyPressed(Keys.ShiftKey);
        }

        /// <summary>
        /// 鼠标移动
        /// </summary>
        private void MouseMove(NCamera camera)
        {
            // 取鼠标射线点击Level中的Hit点
            if ((PlayerInput.ClickLocationX != 0 || PlayerInput.ClickLocationY != 0) && PlayerInput.IsKeyPressed(Keys.LButton))
            {
                Ray mouseRay = camera.GetMouseRay(PlayerInput.ClickLocationX, PlayerInput.ClickLocationY);
                NCheckResult hit = new NCheckResult();
                if (GameEngine.EngineInstance.CurrentLevel.LineCheck(out hit, mouseRay.Position, mouseRay.Position + mouseRay.Direction * camera.ZFar, LineCheckType.World))
                {
                    MoveTargetLocation = new Vector3(hit.location.x,
                        GameEngine.EngineInstance.CurrentLevel.GetWorldHeight(MoveTargetLocation.x, MoveTargetLocation.z),
                        hit.location.z );
                    moveTargetDirect = Vector3.Normalize(MoveTargetLocation - this.TargetActor.Location);
                    reachTarget = false;
                }
            }

#if DEBUG
            GameViewportControl.debugInfos.Add(string.Format(System.Globalization.CultureInfo.CurrentCulture,
                "MoveTargetLocation: {0}\r\n", MoveTargetLocation.ToString()));
#endif

            if (!reachTarget)
            {
                // 朝向目标移动
                Vector3 dir = Vector3.Normalize(MoveTargetLocation - this.TargetActor.Location);
                if (ReachedDestination(this.TargetActor.Location, MoveTargetLocation)
                    || Vector3.Dot(moveTargetDirect, dir) < 0)
                {
                    this.Acceleration = Vector3.Zero;
                    reachTarget = true;
                }
                else
                {
                    this.Acceleration = dir;
                }
            }
        }

        internal override void PreProcessInput(GamePlayerInput gamePlayerInput, float gameTime)
        {
            base.PreProcessInput(gamePlayerInput, gameTime);
        }

        void PlayerInput_MouseButtonDown(object sender, GUIMouseEventArgs e)
        {
            if(e.Button == MouseButtons.Left)
            {

            }
            else if(e.Button == MouseButtons.Right)
            {
                // todo: 测试释放技能
                ClientUseSkill(1909162191);
            }
        }

        void PlayerInput_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Space)
            {
                PressJump = true;
                Jump();
            }
            else if (e.KeyCode == Keys.T && e.Modifiers == Keys.Control)
            {
                NCamera camera = Program.game.GameWindow.ViewportControl.Viewport.Camera;
                Ray mouseRay = camera.GetMouseRay(PlayerInput.MouseValueX, PlayerInput.MouseValueY);
                NCheckResult hit = new NCheckResult();
                if (GameEngine.EngineInstance.CurrentLevel.LineCheck(out hit, mouseRay.Position, mouseRay.Position + mouseRay.Direction * camera.ZFar, LineCheckType.World))
                {
                    this.Transport(GameEngine.EngineInstance.GameLevel, hit.location, true);
                }
            }
            else if (e.KeyCode == Keys.D1)
            {
                // todo: 测试释放技能
                ClientUseSkill(1909162191);
            }
            else if(e.KeyCode == Keys.D2)
            {
                ClientUseSkill(4020073455);
            }
            else if(e.KeyCode == Keys.D3)
            {
                ClientUseSkill(1525076748);
            }
            else if(e.KeyCode == Keys.D4)
            {
                ClientUseSkill(3299876908);
            }
        }

        public void ClientUseSkill(UInt32 spellId)
        {
            NCamera camera = Program.game.GameWindow.ViewportControl.Viewport.Camera;
            Ray mouseRay = camera.GetMouseRay(PlayerInput.MouseValueX, PlayerInput.MouseValueY);
            NCheckResult hit = new NCheckResult();
            if (GameEngine.EngineInstance.CurrentLevel.LineCheck(out hit, mouseRay.Position, mouseRay.Position + mouseRay.Direction * camera.ZFar, LineCheckType.World))
            {
                C2S_CastSpell msg;
                msg.MessageId = C2S_CastSpell.Id;
                msg.SpellID = spellId;
                msg.TargetId = 0;
                msg.TargetPos = hit.location;
                msg.TimeStamp = NativeEngineHelper.GetTimeSeconds();
                msg.CastCount = 1;
                GameFrameManager.SendNetMessage<C2S_CastSpell>(msg);
            }
        }

        #endregion Methods

        /// <summary>
        /// 释放技能结果
        /// </summary>
        internal void OnCastSpellResult(S2C_CastSpellResult ret)
        {
            BuildSpellErrorCode(ret.SpellId, (SpellCastResult)ret.Result);
        }

        internal override void OnSpellFailed(S2C_SpellFailed ret)
        {
            base.OnSpellFailed(ret);

            BuildSpellErrorCode(ret.SpellId, (SpellCastResult)ret.ErrorCode);
        }

        private string BuildSpellErrorCode(UInt32 spellID, SpellCastResult errorCode)
        {
            NSpellInfo info = NSpellInfo.FindInfo(spellID);
            string errorMessageTips = string.Empty;
            switch (errorCode)
            {
                case SpellCastResult.Success:
                    break;
                case SpellCastResult.Failed_NoSpell:
                    errorMessageTips = string.Format(GUISystem.Instance.StringDictionary.GetValue("C_NoSpell"), info.Name);
                    break;
                case SpellCastResult.Failed_InvalidSpell:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_InvalidSpell");
                    break;
                case SpellCastResult.Failed_SpellInProgress:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_SpellInProgress");
                    break;
                case SpellCastResult.Failed_CoolDown:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_CoolDown");
                    break;
                case SpellCastResult.Failed_TooClose:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_TooClose");
                    break;
                case SpellCastResult.Failed_OutofRange:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_OutofRange");
                    break;
                case SpellCastResult.Failed_CasterDead:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_CasterDead");
                    break;
                case SpellCastResult.Failed_Interrupted:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_Interrupted");
                    break;
                case SpellCastResult.Failed_NoMana:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_NoMana");
                    break;
                case SpellCastResult.Failed_NoEnergy:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_NoEnergy");
                    break;
                case SpellCastResult.Failed_LineofSight:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_LineofSight");
                    break;
                case SpellCastResult.Failed_InvalidTargets:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_InvalidTargets");
                    break;
                case SpellCastResult.Failed_InCombat:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_InCombat");
                    break;
                case SpellCastResult.Failed_InFear:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_InFear");
                    break;
                case SpellCastResult.Failed_InSilence:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_InSilence");
                    break;
                case SpellCastResult.Failed_InRoot:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_InRoot");
                    break;
                case SpellCastResult.Failed_CantRepeat:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_CantRepeat");
                    break;
                case SpellCastResult.Failed_Cancel:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_Cancel");
                    break;
                case SpellCastResult.Failed_Move:
                    errorMessageTips = GUISystem.Instance.StringDictionary.GetValue("C_Move");
                    break;
            }

            if (!String.IsNullOrEmpty(errorMessageTips))
            {
                GamingMainFrame.MessageTips.Add(new MessageElement(errorMessageTips));
            }

            return errorMessageTips;
        }


        internal void OnNetMovement(S2C_Move ret)
        {
            // 验证时间戳
            int index = NetMovementMessages.FindLastIndex(x => (x.TimeStamp >= ret.TimeStamp));
            if (index > 0)
                NetMovementMessages.RemoveRange(0, index);

            // 如果在某次移动的时候已经发生了较大的误差则放弃其后的所有移动重新模拟服务器移动
            if (NetMovementMessages.Count > 0
                && (NetMovementMessages[0].Position - ret.Position).LengthSquared2D() > 300 * 300)
            {
                this.Transport(GameEngine.EngineInstance.GameLevel, ret.Position, true);

                Debug.WriteLine(string.Format("[Move] S2C_Move Error! Flag=[{0}] ClientPostion={1} ServerPosition={2}", ret.Flag, NetMovementMessages[0].Position, ret.Position));

                this.Acceleration = ret.Acceleration;
                Vector3 newRot = this.Rotation;
                newRot.y = ret.Yaw;
                this.Rotation = newRot;

                this.NetMovementMessages.Clear();
            }

            //Debug.WriteLine("debug Local Player S2C_Move " + "Flag=" + ret.Flag);
        }
    }
}