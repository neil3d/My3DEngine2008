namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;

    /// <summary>
    /// 屏幕震动
    /// </summary>
    public class CameraScreenShakeModifier : GameCameraModifier
    {
        #region Constructors

        public CameraScreenShakeModifier()
        {
        }

        #endregion Constructors

        #region Methods

        public override void UpdateCamera(NCharacterController viewTarget, NexusEngine.NCamera camera, float deltaTime)
        {
            base.UpdateCamera(viewTarget, camera, deltaTime);
        }

        #endregion Methods
    }
}