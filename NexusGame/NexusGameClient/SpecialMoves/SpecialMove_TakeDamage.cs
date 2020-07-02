using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NexusGameClient
{
    public class SpecialMove_TakeDamage : SpecialMoveBase
    {
        private float countTime;

        #region Constructor
        public SpecialMove_TakeDamage(GameCharacterController owner)
            : base(owner)
        {
        }
        #endregion

        protected override bool InternalCanDoSpecialMove(SpecialMoveBase oldSM)
        {
            if (this.OwnerCC != null
                && this.OwnerCC.IsDoingSepcialMove(typeof(SpecialMove_Dead)))
            {
                return false;
            }

            return base.InternalCanDoSpecialMove(oldSM);
        }

        public override void SpecailMoveStarted(bool forced, SpecialMoveBase prevMove)
        {
            base.SpecailMoveStarted(forced, prevMove);

            // 播放攻击动画
            this.OwnerCC.Avatar.SetAnim("bek");
            this.OwnerCC.Avatar.PlayAnim(false, 1.0f, 0.0f);
        }

        public override void AnimEndNotify(string sequenceName, float PlayedTime, float ExcessTime)
        {
            base.AnimEndNotify(sequenceName, PlayedTime, ExcessTime);

            this.OwnerCC.DoSpecialMove(null, false);
        }

        public override void TickSpecialMove(float deltaTime)
        {
            base.TickSpecialMove(deltaTime);

            countTime += deltaTime;
            if (countTime >= 0.57f)
            {
                this.OwnerCC.DoSpecialMove(null, false);
            }
        }
    }
}
