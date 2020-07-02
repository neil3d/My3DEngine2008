using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;

namespace Nexus.GameFramework
{
    /// <summary>
    /// 游戏层使用的Actor基类
    /// </summary>
    public class GameActor
    {
        #region EventHandler
        /// <summary>
        /// 组合NActor对象即将发生变化
        /// </summary>
        public EventHandler<GameEventArgs> TargetActorChanging;
        /// <summary>
        /// 组合NActor对象发生变化
        /// </summary>
        public EventHandler<GameEventArgs> TargetActorChanged;
        /// <summary>
        /// 角色的位置超出世界范围
        /// </summary>
        public EventHandler<GameEventArgs> OutsideWorldBounds;

        /// <summary>
        /// 移动模式发生改变
        /// </summary>
        public EventHandler<GameEventArgs> MovementTypeChanged;

        #endregion

        #region Fields
        /// <summary>
        /// 组合引擎的NActor
        /// </summary>
        protected NActor targetActor;
        /// <summary>
        /// 眼睛的高度，决定摄像机的高度
        /// </summary>
        public float EyeHeight { get; set; }

        /// <summary>
        /// The maximum slope which the character can walk up
        /// </summary>
        public float SlopeLimit { get; set; }
        /// <summary>
        /// 角色的步阶
        /// Defines the maximum height of an obstacle which the character can climb
        /// </summary>
        public float StepHeight { get; set; }

        /// <summary>
        /// The skin width used by the character. 
        /// </summary>
        public float SkinWidth { get; set; }
        /// <summary>
        /// Specifies the 'up' direction. 
        /// </summary>
        public Vector3 upDirection { get; set; }
        /// <summary>
        /// 加速率
        /// </summary>
        public float AccelRate { get; set; }
        /// <summary>
        /// 地面上最大速率
        /// </summary>
        public float MaxGroundSpeed { get; set; }
        /// <summary>
        /// 飞行最大速率
        /// </summary>
        public float MaxFlySpeed { get; set; }
        /// <summary>
        /// 跳跃最大初始速度
        /// </summary>
        public float MaxJumpSpeed { get; set; }
        /// <summary>
        /// 速度
        /// </summary>
        public Vector3 Velocity { get; set; }
        /// <summary>
        /// 加速度
        /// </summary>
        public Vector3 Acceleration { get; set; }

        /// <summary>
        /// 是走还是跑，false:跑，true走
        /// </summary>
        public bool Walking { get; set; }
        /// <summary>
        /// 行走速率的控制，正常跑的速率*WalkingModifier即得到最终行走的最大速率
        /// </summary>
        public float WalkingModifier { get; set; }

        /// <summary>
        /// 移动模式
        /// </summary>
        private MovementType currentMovementType;

        #endregion

        #region Constructor
        public GameActor()
        {
            SlopeLimit =  0.707f;   // ~= cos(45°)
            StepHeight = 40.0f;
            SkinWidth = 0.1f;
            upDirection = Vector3.UnitY;
            EyeHeight = 100.0f;
            AccelRate = 2048.0f;
            MaxGroundSpeed = 550.0f;
            MaxFlySpeed = 1650.0f;
            MaxJumpSpeed = 1650.0f;
            WalkingModifier = 0.4f;
            currentMovementType = MovementType.Walking;
        }

        public GameActor(NActor target)
            : this()
        {
            targetActor = target;
        }
        #endregion

        #region Porperties
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

                    OnTargetActorChanged(new ActorChangeEventArgs(targetActor));
                }
            }
        }

        /// <summary>
        /// 移动模式
        /// </summary>
        public MovementType CurrentMovementType
        {
            get { return currentMovementType; }
            set
            {
                if (this.currentMovementType != value)
                {
                    this.currentMovementType = value;

                    OnMovementTypeChanged(new GameEventArgs());

                    // 设置禁止移动需要强制设置速度和加速度为零
                    if (currentMovementType == MovementType.Disable)
                    {
                        Velocity = Vector3.Zero;
                        Acceleration = Vector3.Zero;
                    }
                }
            }
        }

        /// <summary>
        /// 计算当前状态的重力
        /// </summary>
        public Vector3 Gravity
        {
            get
            {
                if (CurrentMovementType == MovementType.Disable
                    || CurrentMovementType == MovementType.Flying)
                {
                    return Vector3.Zero;
                }
                return Vector3.UnitY * (-9.8f);
            }
        }

        #endregion

        #region Methods

        #region Tick
        public virtual void TickSimulated(GameEngine engine, float deltaTime)
        {
            this.PerformMovement(engine, deltaTime);
        }
        #endregion

        #region Movement

        /// <summary>
        /// 跳跃
        /// </summary>
        virtual public void Jump()
        {
            if(CanJump())
            {
                Velocity = new Vector3(Velocity.x, Velocity.y + this.MaxJumpSpeed, Velocity.z);
            }
        }

        /// <summary>
        /// 判断当前状态能否跳跃
        /// </summary>
        virtual public bool CanJump()
        {
            if(currentMovementType == MovementType.Walking)
            {
                return true;
            }
            return false;
        }

        /// <summary>
        /// 是否达到目标点测试
        /// </summary>
        /// <param name="testPosition">需要测试的点</param>
        /// <param name="destination">目标点</param>
        virtual public bool ReachedDestination(Vector3 testPosition, Vector3 destination)
        {
            return ReachThresholdTest(testPosition, destination, 40, 40, 40);
        }

        /// <summary>
        /// 根据阀值计算有没有到达目标点
        /// </summary>
        virtual protected bool ReachThresholdTest(Vector3 testPosition, Vector3 destination, float upThresholdAdjust, float downThresholdAdjust, float thresholdAdjust)
        {
            Vector3 Dir = destination - testPosition;

            if ((CurrentMovementType == MovementType.Flying) || (CurrentMovementType == MovementType.Swimming))
            {
                upThresholdAdjust = 2.0f * upThresholdAdjust;
                downThresholdAdjust = 2.0f * downThresholdAdjust;
                thresholdAdjust = 2.0f * thresholdAdjust;
            }

            float diff = Dir.y;
            Dir.y = 0.0f;
            if (Dir.LengthSquared() > thresholdAdjust * thresholdAdjust)
                return false;

            if ((diff > 0.0f) ? (Math.Abs(diff) > upThresholdAdjust) : (Math.Abs(diff) > downThresholdAdjust))
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// 计算最大速率的缩放参数
        /// </summary>
        /// <returns>[0,1]</returns>
        virtual protected float MaxSpeedModifier()
        {
            float ret = 1.0f;
            if (Walking)
            {
                ret *= WalkingModifier;
            }
            return ret;
        }

        /// <summary>
        /// 计算速度和加速度
        /// </summary>
        virtual protected void CalcVelocity(float deltaTime, Vector3 accDir, float maxSpeed)
        {
            float speedModifier = MaxSpeedModifier();
            float maxAccelModifier = speedModifier * AccelRate;
            maxSpeed = maxSpeed * speedModifier;

            // 强制使用最大加速率
            Acceleration = accDir * maxAccelModifier;

            if (Acceleration.LengthSquared() > maxAccelModifier * maxAccelModifier)
            {
                Acceleration = Vector3.Normalize(Acceleration) * maxAccelModifier;
            }

            if (Acceleration.LengthSquared() == 0 && Velocity.LengthSquared() > 0)
            {
                Vector3 OldVel = Velocity;
                // 加速度为零,速度不为零时，减速停下
                Velocity -= Vector3.Normalize(Velocity) * maxAccelModifier * deltaTime;
                if (Vector3.Dot(OldVel, Velocity) < 0)
                {
                    Velocity = Vector3.Zero;
                }
            }
            else
            {
                // final velocity = initial velocity + acceleration * time
                Velocity += Acceleration * deltaTime;
            }

            if (Velocity.LengthSquared() > maxSpeed * maxSpeed)
            {
                Velocity = Vector3.Normalize(Velocity) * maxSpeed;
            }
        }

        /// <summary>
        /// 执行移动
        /// </summary>
        virtual public void PerformMovement(GameEngine engine, float deltaTime)
        {
            switch (CurrentMovementType)
            {
                case MovementType.Walking:
                    PerformWalking(engine, deltaTime);
                    break;
                case MovementType.Flying:
                    PerformFlying(engine, deltaTime);
                    break;
                case MovementType.Swimming:
                    PerformSwimming(engine, deltaTime);
                    break;
                case MovementType.RigidBody:
                    PerformRigidBody(engine, deltaTime);
                    break;
                case MovementType.Custom:
                    PerformCustom(engine, deltaTime);
                    break;
            }
        }

        /// <summary>
        /// 执行陆地行走
        /// </summary>
        /// <param name="deltaTime">时间</param>
        virtual protected void PerformWalking(GameEngine engine, float deltaTime)
        {
            // 限制速度
            Vector3 currentVel = this.Velocity;
            Vector3 currentAccDir = Vector3.Normalize(this.Acceleration);
            currentVel.y = 0;
            currentAccDir.y = -1.0f;

            CalcVelocity(deltaTime, currentAccDir, this.MaxGroundSpeed);

            Vector3 OldLocation = this.TargetActor.Location;
            Vector3 deltaLoation = deltaTime * Velocity;
            NCheckResult hit = new NCheckResult();
            if (!engine.MoveActor(this, deltaLoation, ref hit))
            {
            }
        }

        /// <summary>
        // 执行飞行移动
        /// </summary>
        virtual protected void PerformFlying(GameEngine engine, float deltaTime)
        {
            // 已经飞出世界范围 
            if (!CheckStillInWorld())
                return;

            Vector3 oldAcceleration = Acceleration;

            CalcVelocity(deltaTime, oldAcceleration, this.MaxFlySpeed);

            Vector3 deltaLoation = deltaTime * Velocity;

            NCheckResult hit = new NCheckResult();
            if (!engine.MoveActor(this, deltaLoation, ref hit))
            {
            }

        }

        virtual protected void PerformSwimming(GameEngine engine, float deltaTime)
        {

        }

        virtual protected void PerformRigidBody(GameEngine engine, float deltaTime)
        {

        }

        virtual protected void PerformCustom(GameEngine engine, float deltaTime)
        {

        }


        virtual protected bool CheckStillInWorld()
        {
            Vector3 testLocation = this.TargetActor.Location;
            if (testLocation.x < -MathConst.HalfWorldMax1 || testLocation.x > MathConst.HalfWorldMax1 ||
                testLocation.y < -MathConst.HalfWorldMax1 || testLocation.y > MathConst.HalfWorldMax1 ||
                testLocation.z < -MathConst.HalfWorldMax1 || testLocation.z > MathConst.HalfWorldMax1)
            {
                OnOutsideWorldBounds(new GameEventArgs());
                CurrentMovementType = MovementType.Disable;

                return false;
            }
            return true;
        }

        #endregion

        #region Events

        /// <summary>
        /// 角色的移动模式发生改变
        /// </summary>
        virtual protected void OnMovementTypeChanged(GameEventArgs gameEvent)
        {
            if (MovementTypeChanged != null)
            {
                MovementTypeChanged(this, gameEvent);
            }
        }

        /// <summary>
        /// 角色超出世界范围
        /// </summary>
        /// <param name="gameEvent"></param>
        virtual protected void OnOutsideWorldBounds(GameEventArgs gameEvent)
        {
            if (OutsideWorldBounds != null)
            {
                OutsideWorldBounds(this, gameEvent);
            }
        }

        /// <summary>
        ///  控制引擎NActor对象发生改变
        /// </summary>
        virtual protected void OnTargetActorChanged(ActorChangeEventArgs gameEvent)
        {
            if (TargetActorChanged != null)
            {
                TargetActorChanged(this, gameEvent);
            }
        }
        virtual protected void OnTargetActorChanging(ActorChangeEventArgs gameEvent)
        {
            if (TargetActorChanging != null)
            {
                TargetActorChanging(this, gameEvent);
            }
        }

        #endregion
        #endregion
    }
}
