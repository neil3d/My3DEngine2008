namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;
    using System.Diagnostics;

    internal class RemoteCharacterController : GameCharacterController
    {
        #region Fields

        #endregion Fields

        #region Constructors

        public RemoteCharacterController(UInt64 uintID)
            : base(uintID)
        {
        }

        #endregion Constructors

        #region Properties

   
        #endregion Properties

        #region Methods

        internal void OnNetMovement(S2C_Move ret)
        {
            // 位置误差大于一定的阀值之后修正
            Vector3 position = this.Location;
            if ((position - ret.Position).LengthSquared2D() > 300 * 300)
            {
                this.Transport(GameEngine.EngineInstance.GameLevel, ret.Position, true);
            }

            this.Acceleration = ret.Acceleration;
            NSavedMove.FlagsToController(ret.Flag, this);
            Vector3 newRot = this.Rotation;
            newRot.y = ret.Yaw;
            this.Rotation = newRot;

            //Debug.WriteLine("debug Remote Controller S2C_Move " + "Flag=" + ret.Flag);
        }

        #endregion Methods
    }
}