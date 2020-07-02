namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;

    public class GameReplicateMoveCallback : IReplicateMoveCallback
    {
        #region Fields

        private WeakReference ownerController;

        #endregion Fields

        #region Constructors

        public GameReplicateMoveCallback(NCharacterController controller)
        {
            ownerController = new WeakReference(controller);
        }

        #endregion Constructors

        #region Properties

        public NCharacterController OwnerController
        {
            get
            {
                return (NCharacterController)ownerController.Target;
            }
        }

        #endregion Properties

        #region Methods

        public void OnReplicateMove()
        {
            NCharacterController controller = OwnerController;
            if(controller != null)
            {
                // 发送移动消息
                C2S_Move msg;
                msg.messageId = C2S_Move.Id;
                msg.Position = controller.Location;
                msg.Acceleration = controller.Acceleration;
                msg.Yaw = controller.Rotation.y;
                msg.Flag = NSavedMove.ControllerToFlags(controller);
                msg.TimeStamp = NativeEngineHelper.GetTimeSeconds();
                GameFrameManager.SendNetMessage<C2S_Move>(msg);
            }
        }

        #endregion Methods
    }
}