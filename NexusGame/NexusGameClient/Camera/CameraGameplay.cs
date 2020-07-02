namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Windows.Forms;

    using NexusEngine;

    /// <summary>
    /// 游戏视角镜头控制
    /// </summary>
    public class CameraGameplay : GameCameraModifier
    {
        #region Fields

        /// <summary>
        /// 摄像机的FOV参数
        /// </summary>
        private float fov;
        private Interpolater m_DistanceInterpolater;

        /// <summary>
        /// 是否处于自由观景模式
        /// </summary>
        private bool m_FreeMode;

        //上一次的ViewTarget
        private Vector3 m_LastTarget;
        private CameraRotater m_PitchRotater;

        /// <summary>
        /// 摄像机镜头推拉的速度
        /// </summary>
        private float m_PullSpeed;

        /// <summary>
        /// 摄像机镜头旋转的速度
        /// </summary>
        private float m_RotateSpeed;

        //摄像机旋转器
        private CameraRotater m_YawRotater;

        #endregion Fields

        #region Constructors

        public CameraGameplay()
        {
            minDistance = 360;
            maxDistance = 1800;
            minPitch = -MathConst.PI * 0.27f;
            maxPitch = -MathConst.PI * 0.05f;

            // ~60°
            fov = MathConst.PI * 0.3333f;
            Distance = 1500;
            Yaw = MathConst.PI * 0.75f;
            LookAt = new Vector3(512, 2000, 512);

            //推拉速度暂定为:键盘4秒完成整个Distance
            m_PullSpeed = (MaxDistance - MinDistance) * 0.3f;

            //旋转速度
            m_RotateSpeed = (float)System.Math.PI * 0.5f;

            m_FreeMode = false;

            m_YawRotater = new CameraRotater();
            m_YawRotater.Speed = m_RotateSpeed;

            m_PitchRotater = new CameraRotater();
            m_PitchRotater.Speed = m_RotateSpeed;

            m_DistanceInterpolater = new Interpolater();
            m_DistanceInterpolater.Speed = m_PullSpeed;
        }

        #endregion Constructors

        #region Properties

        /// <summary>
        /// 摄像机的FOV参数
        /// </summary>
        public float FOV
        {
            get { return fov; }
            set { fov = value; }
        }

        #endregion Properties

        #region Methods

        public override void UpdateCamera(NCharacterController viewTarget, NexusEngine.NCamera camera, float deltaTime)
        {
            GamePlayerInput playerInput = null;
            if (viewTarget is LocalPlayerController)
            {
                LocalPlayerController lcc = viewTarget as LocalPlayerController;
                playerInput = lcc.PlayerInput;
            }

            if (playerInput != null)
            {
                if (playerInput.IsKeyPressed(Keys.ControlKey) && playerInput.IsKeyPressed(Keys.Menu))
                {
                    //自由观景模式
                    if (!m_FreeMode)
                    {
                        m_FreeMode = true;
                        //记录当前摄像机数据，以备将来恢复
                        m_PitchRotater.StartValue = Pitch;
                        m_YawRotater.StartValue = Yaw;
                        System.Drawing.Point pt = new System.Drawing.Point(playerInput.MouseValueX, playerInput.MouseValueY);
                        playerInput.FreezeMouse(pt);
                    }
                    if (playerInput.MouseValueDY != 0)
                    {
                        float pitch = -playerInput.MouseValueDY * m_RotateSpeed;
                        this.MovePitch(pitch * deltaTime);
                    }
                    if (playerInput.MouseValueDX != 0)
                    {
                        float yaw = -playerInput.MouseValueDX * m_RotateSpeed;
                        this.MoveYaw(yaw * deltaTime);
                    }
                }
                else
                {
                    if (m_FreeMode)
                    {
                        m_FreeMode = false;
                        playerInput.UnfreezeMouse();

                        m_PitchRotater.EndValue = Pitch;
                        m_PitchRotater.Start();

                        m_YawRotater.EndValue = Yaw;
                        m_YawRotater.Start();
                    }
                    else
                    {
                        if (!m_PitchRotater.IsEnd)
                        {
                            m_PitchRotater.Update(deltaTime);
                            this.MovePitch(m_PitchRotater.Delta);
                        }
                        if (!m_YawRotater.IsEnd)
                        {
                            m_YawRotater.Update(deltaTime);
                            this.MoveYaw(m_YawRotater.Delta);
                        }
                        if (!m_DistanceInterpolater.IsEnd)
                        {
                            m_DistanceInterpolater.Update(deltaTime);
                            this.MoveDistance(m_DistanceInterpolater.Delta);
                        }
                    }

                    if (playerInput.IsKeyPressed(Keys.Menu))
                    {
                        if (playerInput.MouseValueWheel != 0)
                        {
                            m_DistanceInterpolater.StartValue = Distance;
                            m_DistanceInterpolater.EndValue = Distance + (playerInput.MouseValueWheel < 0 ? -1 : 1) * (MaxDistance - MinDistance) * 0.1f;
                            m_DistanceInterpolater.Start();
                        }
                        else if (playerInput.IsKeyPressed(Keys.Up))
                        {
                            this.MoveDistance(-m_PullSpeed * deltaTime);
                        }
                        else if (playerInput.IsKeyPressed(Keys.Down))
                        {
                            this.MoveDistance(m_PullSpeed * deltaTime);
                        }
                    }
                    else if (playerInput.MouseValueWheel != 0)
                    {
                        //float dyaw = (playerInput.MouseValueWheel > 0 ? m_RotateSpeed : -m_RotateSpeed) * 0.5f;
                        float dyaw = (playerInput.MouseValueWheel > 0 ? 1 : -1) * (float)Math.PI / 9.0f;
                        m_PitchRotater.StartValue = Pitch;
                        m_YawRotater.StartValue = Yaw + dyaw;
                        m_PitchRotater.EndValue = Pitch;
                        m_YawRotater.EndValue = Yaw;
                        m_PitchRotater.Start();
                        m_YawRotater.Start();
                    }
                    else if (playerInput.IsKeyPressed(Keys.Left))
                    {
                        this.MoveYaw(-m_RotateSpeed * deltaTime);
                    }
                    else if (playerInput.IsKeyPressed(Keys.Right))
                    {
                        this.MoveYaw(m_RotateSpeed * deltaTime);
                    }
                }
            }

            camera.FOV = this.fov;

            if (viewTarget != null)
            {
                // 设置好View Target Point
                Vector3 lookAt = viewTarget.Location;
                lookAt.y += viewTarget.EyesHeight;
                m_LastTarget = this.LookAt;
                this.LookAt = lookAt;
            }

            base.UpdateCamera(viewTarget, camera, deltaTime);

            //如果当前摄像机运动到地表以下，则恢复到地表
            Vector3 pos = camera.Position;
            float height = GameEngine.EngineInstance.CurrentLevel.GetWorldHeight(pos.x, pos.z);
            if (pos.y < height + 100.0f)
            {
                float dh = pos.y - height - 100.0f;
                float dis = (this.LookAt - pos).Length();
                float pitch = (float)Math.Atan(dh / dis);
                this.MovePitch(pitch);
                base.UpdateCamera(viewTarget, camera, deltaTime);
            }
        }

        #endregion Methods
    }

    class CameraRotater : Interpolater
    {
        #region Methods

        public override void Start()
        {
            float dv = (StartValue - EndValue) % ((float)Math.PI * 2.0f);
            if (dv > (float)Math.PI)
            {
                dv -= ((float)Math.PI * 2.0f);
            }
            if (dv < (float)-Math.PI)
            {
                dv += ((float)Math.PI * 2.0f);
            }

            m_Time = Math.Abs(dv) / Speed;
            if (m_Time > 0)
            {
                m_Step = dv / m_Time;
            }
            else
            {
                m_Step = 0;
            }
            //if (Speed == 0) Speed = 1;
            //m_Step = (EndValue - StartValue) / Speed;
            //m_Value = StartValue;
        }

        #endregion Methods

        #region Other

        //public override void Update(float esp)
        //{
        //    m_Delta = m_Step * (EndValue - m_Value) * esp;
        //    m_Value += m_Delta;
        //    m_Time -= esp;
        //}
        //public override bool IsEnd
        //{
        //    get
        //    {
        //        return m_Value >= EndValue;
        //    }
        //}

        #endregion Other
    }

    //插值器
    class Interpolater
    {
        #region Fields

        public float EndValue;
        public float Speed;
        public float StartValue;

        protected float m_Delta;
        protected float m_Step;
        protected float m_Time;
        protected float m_Value;

        #endregion Fields

        #region Constructors

        public Interpolater()
        {
            StartValue = 0;
            EndValue = 0;
            Speed = 1.0f;
            m_Time = 1.0f;
        }

        #endregion Constructors

        #region Properties

        public float Delta
        {
            get
            {
                return m_Delta;
            }
        }

        public virtual bool IsEnd
        {
            get
            {
                return m_Time <= 0;
            }
        }

        public float Value
        {
            get
            {
                return m_Value;
            }
        }

        #endregion Properties

        #region Methods

        public virtual void Start()
        {
            float dv = (StartValue - EndValue);
            m_Time = Math.Abs(dv) / Speed;
            if (m_Time > 0)
            {
                m_Step = dv / m_Time;
            }
            else
            {
                m_Step = 0;
            }
            m_Value = StartValue;
        }

        //计算当前value
        public virtual void Update(float esp)
        {
            m_Delta = m_Step * esp;
            m_Value += m_Delta;
            m_Time -= esp;
        }

        #endregion Methods
    }

	//加速插值器
	class AccelerateInterpolater
	{
		#region Fields

		public float EndValue;
		public float Speed;
		public float StartValue;

		protected float m_Delta;
		protected float m_Step;
		protected float m_Time;
		protected float m_Value;

		protected float m_Count;

		#endregion Fields

		#region Constructors

		public AccelerateInterpolater()
		{
			StartValue = 0;
			EndValue = 0;
			Speed = 1.0f;
			m_Time = 1.0f;
		}

		#endregion Constructors

		#region Properties

		public float Delta
		{
			get
			{
				return m_Delta;
			}
		}

		public virtual bool IsEnd
		{
			get
			{
				return m_Time <= 0;
			}
		}

		public float Value
		{
			get
			{
				return m_Value;
			}
		}

		#endregion Properties

		#region Methods

		public virtual void Start()
		{
			float dv = (StartValue - EndValue);
			m_Time = Math.Abs(dv) / Speed;
			if (m_Time > 0)
			{
				m_Step = dv / m_Time;
			}
			else
			{
				m_Step = 0;
			}
			m_Value = StartValue;
			++m_Count;
		}

		//计算当前value
		public virtual void Update(float esp)
		{
			m_Delta = m_Step * esp;
			m_Value += m_Delta;
			m_Time -= esp;
			if(IsEnd && m_Count>0)
			{
				--m_Count;
			}
		}

		#endregion Methods
	}
}