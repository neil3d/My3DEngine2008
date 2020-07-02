namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;
    using System.Diagnostics;

    /// <summary>
    /// 单个骨骼模型
    /// </summary>
    public sealed class SkeletalMeshAvatar : ICharacterAvatar
    {
        #region Fields

        private WeakReference skeletalMeshComponentRef;
        private WeakReference characterControllerRef;
        private WeakReference targetActorRef;
        public NSkeletalAnimPlayer AnimPlayer { get; private set; }

        #endregion Fields

        #region Constructors

        public SkeletalMeshAvatar(GameCharacterController controller)
        {
            characterControllerRef = new WeakReference(controller);
        }

        #endregion Constructors

        #region Properties


        public NSkeletalMeshComponent SkeletalMeshComponent
        {
            get
            {
                return skeletalMeshComponentRef.Target as NSkeletalMeshComponent;
            }
        }

        public NActor OwnerActor
        {
            get
            {
                return targetActorRef.Target as NActor;
            }
        }

        public GameCharacterController OwnerCharacterController
        {
            get
            {
                return characterControllerRef.Target as GameCharacterController;
            }
        }

        #endregion Properties

        #region Methods

        /// <summary>
        /// 初始化，创建Avatar所需的对象
        /// </summary>
        public void InitAvatar(string uniqueActorName)
        {
            OwnerCharacterController.TargetActor = GameEngine.EngineInstance.CurrentLevel.CreateActor(uniqueActorName, "nactor");
        }

        /// <summary>
        /// 重置Avatar资源
        /// </summary>
        public void ResetAvatarResource(NActor ownerActor)
        {
            targetActorRef = new WeakReference(ownerActor);

            NSkeletalMeshComponent skeletalMeshMeshCmp = (NSkeletalMeshComponent)ownerActor.CreateComponent("LPC_Skeletal_Mesh", "nskeletal_mesh_component");
            skeletalMeshComponentRef = new WeakReference(skeletalMeshMeshCmp);

            try
            {
                // 初始化资源
                skeletalMeshMeshCmp.ResetResource(new NResourceLoc("content", @"/characters/XiaShi/body.skm"), true);
                
                // 初始化动画
                AnimPlayer = skeletalMeshMeshCmp.ResetSkeletalAnimPlayer();
                AnimPlayer.Init(NResourceManager.Instance.LoadSkeletalAnimSet(
                        new NResourceLoc("content", @"/characters/XiaShi/base_act.animset"),
                        EResourceIOMode.Auto,
                        EResourceIOPriority.Normal
                        ), true);
            }
            catch (System.Exception e)
            {
                NLogger.Instance.WriteString(LogType.Exception, "Load  Skeletal Mesh faild!" + e.ToString());
                Program.ShowException(e, "Load  Skeletal Mesh faild!");
            }
        }

        /// <summary>
        /// tick
        /// </summary>
        public void Update(float deltaTime)
        {
            if (SkeletalMeshComponent == null || OwnerCharacterController == null || AnimPlayer== null)
                return;

            float currentVel = OwnerCharacterController.Velocity.LengthSquared2D();
            string shouldAnimPlay;
            bool loop = true;
            if(OwnerCharacterController.SpecialMove != null
                //&& OwnerCharacterController.SpecialMove.DisableMovement
                )
            {
                // TODO: 动画混合时需要播放基本动画
                shouldAnimPlay = String.Empty;
            }
            else if (OwnerCharacterController.MovementType == MovementTypeCLI.Ground)
            {
                if (currentVel > 250 * 250)
                {
                    shouldAnimPlay = "run";
                }
                else if (currentVel > 1)
                {
                    shouldAnimPlay = "walk";
                }
                else
                {
                    shouldAnimPlay = "std";
                }
            }
            else if (OwnerCharacterController.MovementType == MovementTypeCLI.Jump)
            {
                shouldAnimPlay = "jump";
                loop = false;
            }
            else
            {
                shouldAnimPlay = "std";
            }
            if (!string.IsNullOrEmpty(shouldAnimPlay)
                && shouldAnimPlay != AnimPlayer.CurrentAnimPlaying)
            {
                SetAnim(shouldAnimPlay);
                PlayAnim(loop, 1.0f, 0.0f);

                //Debug.WriteLine(string.Format("PlayAnim : {0}", shouldAnimPlay));
            }
        }

        #endregion Methods

        #region ICharacterAvatar 成员


        public void SetAnim(string AnimName)
        {
            if (AnimPlayer != null)
            {
                AnimPlayer.SetAnim(AnimName);
            }
        }

        public void PlayAnim(bool loop, float rate, float startTime)
        {
            if (AnimPlayer != null)
            {
                AnimPlayer.PlayAnim(loop, rate, startTime);
            }
        }

        #endregion
    }
}