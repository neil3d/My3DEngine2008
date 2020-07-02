namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;

    public class SpecialMove_Dead : SpecialMoveBase
    {
        public readonly string DeadAnimation = "die";

        public SpecialMove_Dead(GameCharacterController owner)
            : base(owner)
        {
            DisableMovement = true;
            DisableRotation = true;
        }

        public override void SpecailMoveStarted(bool forced, SpecialMoveBase prevMove)
        {
            base.SpecailMoveStarted(forced, prevMove);
        
            // 播放死亡动画
            this.OwnerCC.Avatar.SetAnim(this.DeadAnimation);
            this.OwnerCC.Avatar.PlayAnim(false, 1.0f, 0.0f);
        }

        public override void AnimEndNotify(string sequenceName, float PlayedTime, float ExcessTime)
        {
            base.AnimEndNotify(sequenceName, PlayedTime, ExcessTime);
        }
    }
}