using System;

namespace Nexus.GameFramework
{
    /// <summary>
    /// Summary description for GameException.
    /// </summary>
    public class GameException : ApplicationException
    {
        public GameException(string message, params object[] args)
            : base(string.Format(message, args))
        {
        }
        public GameException(string message, Exception innerException, params object[] args)
            : base(string.Format(message, args), innerException)
        {
        }
    }

    /// <summary>
    /// 主关卡为空
    /// </summary>
    public class LevelNullException : GameException
    {
        public LevelNullException(string message, params object[] args)
            : base(string.Format(message, args))
        {
        }
        public LevelNullException(string message, Exception innerException, params object[] args)
            : base(string.Format(message, args), innerException)
        {
        }
    }
}
