namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;
    using System.Diagnostics;

    public sealed class AnimMeshAvatar : ICharacterAvatar
    {
        #region Fields

        private WeakReference animMeshComponentRef;
        private WeakReference characterControllerRef;
        private WeakReference targetActorRef;

        #endregion Fields

        #region Constructors

        public AnimMeshAvatar(GameCharacterController controller)
        {
            characterControllerRef = new WeakReference(controller);
        }

        #endregion Constructors

        #region Properties

        public NAnimMeshComponent AnimMeshComponent
        {
            get
            {
                return (NAnimMeshComponent)animMeshComponentRef.Target;
            }
        }

        public NActor OwnerActor
        {
            get
            {
                return (NActor)targetActorRef.Target;
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

            NAnimMeshComponent animMesh = (NAnimMeshComponent)ownerActor.CreateComponent("LPC_Anim_Mesh", "nanim_mesh_component");
            animMeshComponentRef = new WeakReference(animMesh);

            try
            {
                // 初始化资源
                //animMesh.ResetResource(new NResourceLoc("content", @"/NPC/NPC_bandit_001/NPC_bandit_001.nam"));
                animMesh.ResetResource(new NResourceLoc("content", @"/NPC/NPC_grpmon_002/atck.nam"));
            }
            catch (System.Exception)
            {
                //Logger.Instance.LogEvent("Load anim mesh faild!" + e.ToString());
            }
        }

        /// <summary>
        /// tick
        /// </summary>
        public void Update(float deltaTime)
        {
            if (AnimMeshComponent == null || OwnerCharacterController == null)
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
                && shouldAnimPlay != AnimMeshComponent.AnimPlaying)
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
            if (AnimMeshComponent != null)
            {
                AnimMeshComponent.SetAnim(AnimName);
            }
        }

        public void PlayAnim(bool loop, float rate, float startTime)
        {
            if (AnimMeshComponent != null)
            {
                AnimMeshComponent.PlayAnim(loop, rate, startTime);
            }
        }

        #endregion
    }
}