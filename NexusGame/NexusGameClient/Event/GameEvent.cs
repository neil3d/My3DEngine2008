namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;

    /// <summary>
    /// NActor发生变化事件
    /// </summary>
    public class ActorChangeEventArgs : GameEventArgs
    {
        #region Constructors

        public ActorChangeEventArgs(NActor newActor)
        {
            this.NewLevel = newActor;
        }

        #endregion Constructors

        #region Properties

        public NActor NewLevel
        {
            get; set;
        }

        #endregion Properties
    }

    /// <summary>
    /// 游戏事件基类
    /// </summary>
    public class GameEventArgs : EventArgs
    {
        #region Properties

        public bool Handled
        {
            get; set;
        }

        #endregion Properties
    }

    /// <summary>
    /// NLevel发生变化事件
    /// </summary>
    public class LevelChangeEventArgs : GameEventArgs
    {
        #region Constructors

        public LevelChangeEventArgs(NLevel newLevel)
        {
            this.NewLevel = newLevel;
        }

        #endregion Constructors

        #region Properties

        public NLevel NewLevel
        {
            get; set;
        }

        #endregion Properties
    }


    public class AuraChangeEventArgs : GameEventArgs
    {
        #region Constructors

        public AuraChangeEventArgs(AuraInstance aura)
        {
            this.Aura = aura;
        }

        #endregion Constructors

        #region Properties

        public AuraInstance Aura
        {
            get; set;
        }

        #endregion Properties
    }

    public class StatEventArgs : GameEventArgs
    {
        public StatEventArgs(NCharacterStatData stat)
        {
            this.Data = stat;
        }

        public NCharacterStatData Data { get; set; } 
    }

}