namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;

    public sealed class GameSavedMove : NSavedMove
    {
        #region Constructors

        public GameSavedMove(IReplicateMoveCallback callback)
            : base(callback)
        {
        }

        #endregion Constructors
    }
}