namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;


    /// <summary>
    /// Special Move一种客户端状态机控制，主要处理输入，移动，动作等行为控制和切换
    /// </summary>
    public abstract class SpecialMoveBase
    {
        #region Fields

        private WeakReference ownerCCRef;

        /// <summary>
        /// 移动速度播放控制 maxSpeed * speedModifier = 最终速度
        /// </summary>
        public float SpeedModifier { get; set; }
        private float OldSpeedModifier { get; set; }

        /// <summary>
        /// 是否禁止移动
        /// </summary>
        public bool DisableMovement { get; set; }
        /// <summary>
        /// 是否禁止转身
        /// </summary>
        public bool DisableRotation { get; set; }

        /// <summary>
        /// 最近一次判断是否可以执行SM的时间
        /// </summary>
        private bool LastCanDoSpecialMove { get; set; }
        private float LastCanDoSpecialMoveTime { get; set; }

        /// <summary>
        /// 移动到目标点
        /// </summary>
        public bool WantReachPreciseDestination { get; private set; }
        public bool ReachedPreciseDestination { get; private set; }
        private Vector3 preciseDestination;

        /// <summary>
        /// 在执行SM结束后恢复原来的移动
        /// </summary>
        public bool RestoreMovement { get; set; }
        public Vector3 OldAcceleration { get; set; }

        /// <summary>
        /// 是否执行SM结束后恢复前一次SpecialMove
        /// </summary>
        public bool RestorePreSpecialMove { get; set; }
        public SpecialMoveBase PreSpecialMove { get; private set; }
        #endregion

        #region Properties

        /// <summary>
        /// 指定移动到目标点
        /// </summary>
        public Vector3 PreciseDestination
        {
            get { return preciseDestination; }
            set
            {
                preciseDestination = value;
                WantReachPreciseDestination = true;
                ReachedPreciseDestination = false;
            }
        }

        public GameCharacterController OwnerCC 
        {
            get { return ownerCCRef.Target as GameCharacterController; }
        }

        #endregion
        protected SpecialMoveBase()
        {
            SpeedModifier = 1.0f;
        }

        protected SpecialMoveBase(GameCharacterController owner)
        {
            SpeedModifier = 1.0f;
            ownerCCRef = new WeakReference(owner);
        }

        #region Methods

        /// <summary>
        /// Tick
        /// </summary>
        public virtual void TickSpecialMove(float deltaTime)
        {
            if (OwnerCC == null)
                return;

            if (WantReachPreciseDestination && !ReachedPreciseDestination)
            {
                float distanse = (PreciseDestination - OwnerCC.Location).Length2D();
                if (Math.Abs(distanse) > 5.0f)
                {
                    float PushMagnitude = Math.Min((distanse / deltaTime), OwnerCC.MaxGroundSpeed * OwnerCC.SpeedUp);
                    Vector3 direction = PreciseDestination - OwnerCC.Location;
                    direction.Normalize();

                    OwnerCC.Velocity = direction * PushMagnitude;
                    OwnerCC.Acceleration = direction;
                }
                else
                {
                    OwnerCC.Acceleration = Vector3.Zero;
                    ReachedPreciseDestination = true;
                }
            }

            if (WantReachPreciseDestination && ReachedPreciseDestination)
            {
                WantReachPreciseDestination = false;
                OnReachedPrecisePosition();
            }
        }

        /// <summary>
        /// 达到指定位置
        /// </summary>
        protected virtual void OnReachedPrecisePosition()
        {
        }

        /// <summary>
        /// 供外部调用判断是否可以执行
        /// </summary>
        /// <param name="forceCheck"></param>
        /// <returns></returns>
        public bool CanDoSpecialMove(SpecialMoveBase oldSM, bool forceCheck)
        {
            if (OwnerCC != null)
            {
                float currentTimeStamp = NativeEngineHelper.GetTimeSeconds();
                if (forceCheck || currentTimeStamp != LastCanDoSpecialMoveTime)
                {
                    LastCanDoSpecialMove = InternalCanDoSpecialMove(oldSM);
                    LastCanDoSpecialMoveTime = currentTimeStamp;
                }
                return LastCanDoSpecialMove;
            }
            return false;
        }

        /// <summary>
        /// 供子类重载判断是否可以执行当前SpecailMove的接口
        /// </summary>
        /// <returns></returns>
        protected virtual bool InternalCanDoSpecialMove(SpecialMoveBase oldSM)
        {
            return true;
        }

        /// <summary>
        /// SM开始执行时调用
        /// </summary>
        /// <param name="forced">是否强制执行</param>
        /// <param name="prevMove">前一个SM的类型</param>
        public virtual void SpecailMoveStarted(bool forced, SpecialMoveBase prevMove)
        {
            if (OwnerCC == null)
            {
                NLogger.Instance.WriteString(LogType.Warning, "[SM] Invalid owner refrence!");
                return;
            }

            NLogger.Instance.WriteString(LogType.Debug, this.OwnerCC.ToString() + " do SM: " + this.ToString());

            if (RestoreMovement)
            {
                OldAcceleration = OwnerCC.Acceleration;
            }
            if (DisableMovement)
            {
                OwnerCC.Acceleration = Vector3.Zero;
            }
            PreSpecialMove = RestorePreSpecialMove ? prevMove : null;
            OldSpeedModifier = OwnerCC.SpeedUp;
            OwnerCC.SpeedUp = this.SpeedModifier;
        }

        public virtual void SpecialMoveEnded()
        {
            NLogger.Instance.WriteString(LogType.Debug, this.OwnerCC.ToString() + " End SM: " + this.ToString());

            if (RestoreMovement)
            {
                OwnerCC.Acceleration = OldAcceleration;
            }
            if(RestorePreSpecialMove)
            {
                OwnerCC.DoSpecialMove(PreSpecialMove, true);
            }
            OwnerCC.SpeedUp = this.OldSpeedModifier;
        }

        /// <summary>
        /// 允许优先响应或者修改输入参数
        /// </summary>
        public virtual void PreProcessInput(GamePlayerInput Input)
        {
        }

        public virtual void AnimEndNotify(string sequenceName, float PlayedTime, float ExcessTime)
        {

        }

        public virtual void AnimEndNotify(NSkeletalAnimSequence sequence, float PlayedTime, float ExcessTime)
        {

        }


        #endregion


    }
}
