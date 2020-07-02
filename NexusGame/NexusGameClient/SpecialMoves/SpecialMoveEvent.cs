using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NexusGameClient
{
    /// <summary>
    /// SpecialMoveEvent
    /// </summary>
    public class SpecialMoveEventArgs : GameEventArgs
    {
        public SpecialMoveBase SM { get; set; }

        #region Constructors
        public SpecialMoveEventArgs(SpecialMoveBase sm)
        {
            this.SM = SM;
        }
        #endregion Constructors

    }
}
