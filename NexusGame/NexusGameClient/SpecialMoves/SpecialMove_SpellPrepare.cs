namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;

    public class SpecialMove_SpellPrepare : SpecialMoveBase
    {
        #region Fields

        private S2C_SpellPrepare prepareData;

        #endregion

        #region Properties
        public S2C_SpellPrepare PrepareData
        {
            get { return prepareData; }
            set
            {
                prepareData = value;

                OnSpellPrepareChanged();
            }
        }
        #endregion

        public SpecialMove_SpellPrepare(GameCharacterController owner)
            : base(owner)
        {
        }

        protected override bool InternalCanDoSpecialMove(SpecialMoveBase oldSM)
        {
            if(this.OwnerCC!= null
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
            this.OwnerCC.Avatar.SetAnim("easy");
            this.OwnerCC.Avatar.PlayAnim(true, 1.0f, 0.0f);
        }

        public override void AnimEndNotify(string sequenceName, float PlayedTime, float ExcessTime)
        {
            base.AnimEndNotify(sequenceName, PlayedTime, ExcessTime);
        }

        private void OnSpellPrepareChanged()
        {
           //TODO: 处理参数变化响应
        }
    }
}
