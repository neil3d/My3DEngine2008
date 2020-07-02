namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;
    using System.Diagnostics;

    internal class GameReplicateMoveCallback : IReplicateMoveCallback
    {
        #region Fields

        private WeakReference ownerController;

        #endregion Fields

        #region Constructors

        internal GameReplicateMoveCallback(LocalPlayerController controller)
        {
            ownerController = new WeakReference(controller);
        }

        #endregion Constructors

        #region Properties

        public LocalPlayerController OwnerController
        {
            get
            {
                return ownerController.Target as LocalPlayerController;
            }
        }

        #endregion Properties

        #region Methods

        public void OnReplicateMove()
        {
            LocalPlayerController controller = OwnerController;
            if(controller != null
                && GameFrameManager.Instance.CurrentFrameType == GameFrameType.Gaming)
            {
                // 发送移动消息
                C2S_Move msg;
                msg.messageId = C2S_Move.Id;
                msg.Position = controller.Location;
                msg.Acceleration = controller.Acceleration;
                msg.Yaw = Vector3.CalcYaw(controller.Acceleration);
                msg.Flag = NSavedMove.ControllerToFlags(controller, controller.PressJump ? (Byte)MovementFlag.PressJump : Byte.MinValue);
                msg.TimeStamp = NativeEngineHelper.GetTimeSeconds();
                GameFrameManager.SendNetMessage<C2S_Move>(msg);

                // 保存到消息队列以便验证
                controller.NetMovementMessages.Add(msg);

                //Debug.WriteLine("Send C2S_Move " + " Position:" + msg.Position + " Acceleration:" + msg.Acceleration + " Flag:" + msg.Flag);
            }
        }

        #endregion Methods
    }
}