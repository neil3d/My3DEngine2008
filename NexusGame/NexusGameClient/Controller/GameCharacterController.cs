namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;

    public class GameCharacterController : NCharacterController
    {
        #region Event

        /// <summary>
        /// 组合NActor对象发生变化
        /// </summary>
        public EventHandler<GameEventArgs> TargetActorChanged;

        /// <summary>
        /// 组合NActor对象即将发生变化
        /// </summary>
        public EventHandler<GameEventArgs> TargetActorChanging;

        /// <summary>
        /// 技能容器发生变化
        /// </summary>
        public EventHandler<GameEventArgs> SpellCollectionChanged;
        /// <summary>
        /// BUff容器发生变化
        /// </summary>
        public EventHandler<GameEventArgs> AuraCollectionChanged;
        /// <summary>
        /// 单个Buff数据发生变化
        /// </summary>
        public EventHandler<AuraChangeEventArgs> AuraDataChanged;
        /// <summary>
        /// 角色属性发生变化
        /// </summary>
        public EventHandler<StatEventArgs> StatInited;
        public EventHandler<StatEventArgs> StatUpdated;

        /// <summary>
        /// SepcialMove
        /// </summary>
        public EventHandler<SpecialMoveEventArgs> SepcialMoveStarted;
        public EventHandler<SpecialMoveEventArgs> SepcialMoveEnded;

        #endregion

        #region Fields
        /// <summary>
        /// 组合引擎的NActor
        /// </summary>
        private NActor targetActor;

        /// <summary>
        /// 角色外观
        /// </summary>
        private ICharacterAvatar avatar;

        /// <summary>
        /// 技能容器
        /// </summary>
        private SpellCollection spells = new SpellCollection();
        /// <summary>
        /// buff容器  
        /// </summary>
        private AuraCollection auras = new AuraCollection();

        /// <summary>
        /// 当前SM
        /// </summary>
        public SpecialMoveBase SpecialMove { get; private set; }

        #endregion

        protected GameCharacterController()
        {
            this.StatData.StatInited += new EventHandler<EventArgs>(StatData_StatInited);
            this.StatData.StatUpdated += new EventHandler<EventArgs>(StatData_StatUpdated);
        }

        public GameCharacterController(UInt64 uintID)
            : this()
        {
            this.CharacterID = uintID;
        }

        #region Properties
        public SpellCollection Spells
        {
            get { return spells; }
        }

        public AuraCollection Auras
        {
            get { return auras; }
        }

        public ICharacterAvatar Avatar
        {
            get
            {
                return avatar;
            }
        }

        /// <summary>
        /// 组合引擎的NActor
        /// </summary>
        public NActor TargetActor
        {
            get { return targetActor; }
            set
            {
                if (TargetActor != value)
                {
                    OnTargetActorChanging(new ActorChangeEventArgs(targetActor));

                    targetActor = value;

                    avatar.ResetAvatarResource(targetActor);

                    OnTargetActorChanged(new ActorChangeEventArgs(targetActor));
                }
            }
        }
        #endregion

        #region Methods
        public override void Destory()
        {
            NLevel level = GameEngine.EngineInstance.CurrentLevel;
            if (level != null)
            {
                level.DestroyActor(TargetActor.Name);
            }

            base.Destory();
        }

        public override bool Init(string uniqueName)
        {
            bool ret = base.Init(uniqueName);

            this.StatData.Init((UInt16)NexusEngine.PlayerFields.End);

            //avatar = new AnimMeshAvatar(this);
            avatar = new SkeletalMeshAvatar(this);
            avatar.InitAvatar(uniqueName);
            return ret;
        }


        public override void Update(float deltaTime)
        {
            base.Update(deltaTime);

            if (avatar != null)
            {
                avatar.Update(deltaTime);
            }

            if (SpecialMove != null)
            {
                SpecialMove.TickSpecialMove(deltaTime);
            }

            bool disableMovement = (SpecialMove != null && SpecialMove.DisableMovement);
            if (!disableMovement)
            {
                this.Move(GameEngine.EngineInstance.GameLevel, deltaTime);
                // 将controller的空间信息同步给NActor
                TargetActor.Location = this.Location;
            }

            // 更新角色朝向
            bool disableRotation = (SpecialMove != null && SpecialMove.DisableRotation);
            if (!disableRotation)
            {
                UpdateTargetRotation(deltaTime);
            }
        }


        /// <summary>
        ///  控制引擎NActor对象发生改变
        /// </summary>
        protected virtual void OnTargetActorChanged(ActorChangeEventArgs gameEvent)
        {
            if (TargetActorChanged != null)
            {
                TargetActorChanged(this, gameEvent);
            }
        }

        protected virtual void OnTargetActorChanging(ActorChangeEventArgs gameEvent)
        {
            if (TargetActorChanging != null)
            {
                TargetActorChanging(this, gameEvent);
            }
        }

        public void OnSpellCollectionChanged(GameEventArgs e)
        {
            if (SpellCollectionChanged != null)
            {
                SpellCollectionChanged(this, e);
            }
        }

        public void OnAuraCollectionChanged(GameEventArgs e)
        {
            if (AuraCollectionChanged != null)
            {
                AuraCollectionChanged(this, e);
            }
        }

        public void OnAuraDataChanged(AuraChangeEventArgs e)
        {
            if (AuraDataChanged != null)
            {
                AuraDataChanged(this, e);
            }
        }

        void StatData_StatUpdated(object sender, EventArgs e)
        {
            OnStatUpdated(new StatEventArgs(this.StatData));
        }

        protected virtual void OnStatUpdated(StatEventArgs statEventArgs)
        {
            if (StatUpdated != null)
            {
                StatUpdated(this, statEventArgs);
            }

            // 处理死亡标记
            HandleDeadFlag();
        }

        private void HandleDeadFlag()
        {
            Type deadSMType = typeof(SpecialMove_Dead);
            if (StatData.HasFlag((UInt16)UnitFields.Flags, (UInt32)UnitStatus.Dead))
            {
                this.DoSpecialMoveByType(deadSMType, true);
            }
            else
            {
                if (IsDoingSepcialMove(deadSMType))
                {
                    this.DoSpecialMoveByType(null, true);
                }
            }
        }

        void StatData_StatInited(object sender, EventArgs e)
        {
            OnStatInited(new StatEventArgs(this.StatData));
        }

        protected virtual void OnStatInited(StatEventArgs statEventArgs)
        {
            if (StatInited != null)
            {
                StatInited(this, statEventArgs);
            }

            // 处理死亡标记
            HandleDeadFlag();
        }


        /// <summary>
        /// 更新角色朝向
        /// </summary>
        /// <param name="deltaTime"></param>
        private void UpdateTargetRotation(float deltaTime)
        {
            // 计算朝向
            if (this.Velocity.LengthSquared2D() > 0)
            {
                Vector3 direct = this.Velocity;
                this.Rotation = new Vector3(0, Vector3.CalcYaw(direct), 0);
            }
            // 原地转身
            TargetActor.RadianRotation = this.Rotation;
        }

        internal virtual void PreProcessInput(GamePlayerInput gamePlayerInput, float gameTime)
        {
            if (SpecialMove != null)
            {
                SpecialMove.PreProcessInput(gamePlayerInput);
            }
        }

        #region SepcialMove

        public bool DoSpecialMoveByType(Type NewMove, bool forceMove)
        {
            if (SpecialMove != null && NewMove == SpecialMove.GetType())
            {
                // ignore redundant calls to the same move
                NLogger.Instance.WriteString(LogType.Debug, "[SM] ignoring redundant call.");
                return false;
            }

            SpecialMoveBase NewMoveInstance = null;
            if (NewMove != null)
            {
                NewMoveInstance = Activator.CreateInstance(NewMove, new object[] { this }) as SpecialMoveBase;
            }
            return DoSpecialMove(NewMoveInstance, forceMove);
        }

        public bool DoSpecialMove(SpecialMoveBase NewMoveInstance, bool forceMove)
        {
            if (!forceMove && NewMoveInstance != null && !NewMoveInstance.CanDoSpecialMove(SpecialMove, forceMove))
            {
                NLogger.Instance.WriteString(LogType.Debug, string.Format("[SM] cannot do requested special move {0}", NewMoveInstance.ToString()));
                return false;
            }

            SpecialMoveBase preSM = SpecialMove;
            if (preSM != null)
            {
                preSM.SpecialMoveEnded();
                OnSepcialMoveEnded(new SpecialMoveEventArgs(preSM));
            }

            SpecialMove = NewMoveInstance;
            if (SpecialMove != null)
            {
                SpecialMove.SpecailMoveStarted(forceMove, preSM);
                OnSepcialMoveStarted(new SpecialMoveEventArgs(SpecialMove));
            }

            return true;
        }

        public bool IsDoingSepcialMove(Type specialMoveType)
        {
            return this.SpecialMove != null && this.SpecialMove.GetType() == specialMoveType;
        }

        private void OnSepcialMoveStarted(SpecialMoveEventArgs specialMoveEventArgs)
        {
            if (SepcialMoveStarted != null)
            {
                SepcialMoveStarted(this, specialMoveEventArgs);
            }
        }

        private void OnSepcialMoveEnded(SpecialMoveEventArgs specialMoveEventArgs)
        {
            if (SepcialMoveEnded != null)
            {
                SepcialMoveEnded(this, specialMoveEventArgs);
            }
        }

        #endregion SepcialMove

        #region NetMessage

        internal virtual void OnSpellPrepare(S2C_SpellPrepare ret)
        {
            // 更新当前SM状态
            Type sepllSMType = typeof(SpecialMove_SpellPrepare);
            if (!IsDoingSepcialMove(sepllSMType))
            {
                DoSpecialMoveByType(sepllSMType, false);
            }

            SpecialMove_SpellPrepare spellSM = SpecialMove as SpecialMove_SpellPrepare;
            if (spellSM != null)
            {
                spellSM.PrepareData = ret;
            }
        }

        internal virtual void OnSpellCasting(S2C_SpellCasting ret)
        {
            // 更新当前SM状态
            Type sepllSMType = typeof(SpecialMove_SpellCasting);
            if (!IsDoingSepcialMove(sepllSMType))
            {
                DoSpecialMoveByType(sepllSMType, false);
            }

            SpecialMove_SpellCasting spellSM = SpecialMove as SpecialMove_SpellCasting;
            if (spellSM != null)
            {
                spellSM.CastingData = ret;
            }
        }

        protected virtual bool IsDoingSpellSpecialMove()
        {
            return IsDoingSepcialMove(typeof(SpecialMove_SpellPrepare)) 
                || IsDoingSepcialMove(typeof(SpecialMove_SpellCasting));
        }

        /// <summary>
        ///  处理打断
        /// </summary>
        internal virtual void OnSpellFailed(S2C_SpellFailed ret)
        {
            if(ret.ErrorCode != (Byte)SpellCastResult.Success
                && IsDoingSpellSpecialMove())
            {
                DoSpecialMove(null, false);
            }
        }

        /// <summary>
        /// 技能命中信息(需要广播)
        /// </summary>
        internal virtual void OnSpellHit(S2C_SpellHit ret)
        {

        }

        #endregion

        #endregion

    }
}
