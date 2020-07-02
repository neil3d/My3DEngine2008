using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;

namespace Nexus.GameFramework
{
    public class GameLocalPlayer : GamePlayer
    {
        #region Fields
        /// <summary>
        /// 移动的目的地
        /// </summary>
        private Vector3 moveTargetLocation;
        private Vector3 moveTargetDirect;
        private bool reachTarget;
        #endregion

        #region Properties
        public Vector3 MoveTargetLocation
        {
            get { return moveTargetLocation; }
            set { moveTargetLocation = value; }
        }
        #endregion

        #region Constructor
        public GameLocalPlayer()
        { }
        #endregion

        #region Methods

        #region TickSimulated
        public override void TickSimulated(GameEngine engine, float deltaTime)
        {
            base.TickSimulated(engine, deltaTime);
        }
        #endregion

        /// <summary>
        /// 处理玩家移动
        /// </summary>
        public void PlayerMove(float deltaTime, GameEngine engine, GamePlayerInput playerInput, NCamera camera)
        {
            if (TargetActor == null || engine == null)
                return;

            if (engine.CurrentLevel == null)
                return;

            KeyMove(playerInput);
            MouseMove(playerInput, camera, engine);

#if DEBUG
            GameViewportControl.debugInfos.Add(string.Format(System.Globalization.CultureInfo.CurrentCulture,
                "CurrentMovementType: {0} Acceleration: {1} Velocity: {2} \r\n",
                CurrentMovementType.ToString(),
                Acceleration.ToString(), 
                Velocity.ToString()));
#endif
        }

        /// <summary>
        /// 键盘移动
        /// </summary>
        private void KeyMove(GamePlayerInput playerInput)
        {
            Vector3 accel = new Vector3();
            accel.x = playerInput.IsKeyPressed(System.Windows.Forms.Keys.A) ? -1 : (playerInput.IsKeyPressed(System.Windows.Forms.Keys.D) ? 1 : 0);
            accel.y = playerInput.IsKeyPressed(System.Windows.Forms.Keys.Q) ? -1 : (playerInput.IsKeyPressed(System.Windows.Forms.Keys.E) ? 1 : 0);
            accel.z = playerInput.IsKeyPressed(System.Windows.Forms.Keys.S) ? -1 : (playerInput.IsKeyPressed(System.Windows.Forms.Keys.W) ? 1 : 0);
            this.Acceleration = accel;

            if(playerInput.IsKeyPressed(System.Windows.Forms.Keys.Space))
            {
                Jump();
            }
        }

        /// <summary>
        /// 鼠标移动
        /// </summary>
        private void MouseMove(GamePlayerInput playerInput, NCamera camera, GameEngine engine)
        {
            // 取鼠标射线点击Level中的Hit点
            if (playerInput.ClickLocationX != 0 || playerInput.ClickLocationY != 0)
            {
                Ray mouseRay = camera.GetMouseRay(playerInput.ClickLocationX, playerInput.ClickLocationY);
                NCheckResult hit = new NCheckResult();
                if (engine.CurrentLevel.LineCheck(out hit, mouseRay.Position, mouseRay.Position + mouseRay.Direction * camera.ZFar, LineCheckType.World))
                {
                    MoveTargetLocation = hit.location;
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
                    || Vector3.Dot(moveTargetDirect, dir) < 0 )
                {
                    this.Acceleration = Vector3.Zero;
                    //this.Velocity = Vector3.Zero;
                    reachTarget = true;
                }
                else
                {
                    this.Acceleration = dir;
                }
            }
        }
        #endregion
    }
}
