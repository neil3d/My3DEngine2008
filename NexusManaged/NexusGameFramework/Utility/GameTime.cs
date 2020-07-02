namespace Nexus.GameFramework
{
    /// <summary>
    /// Contains the current timing state of the game.
    /// </summary>
    public class GameTime
    {
		private float framesPerSecond;
        /// <summary>
        /// Gets the current frames-per-second measure.
        /// </summary>
        /// <value>The current frames-per-second measure.</value>
        public float FramesPerSecond
        {
			get { return framesPerSecond; }
			set { framesPerSecond = value; }
        }

		private float elapsedGameTime;
        /// <summary>
        /// Gets the elapsed game time, in seconds.
        /// </summary>
        /// <value>The elapsed game time.</value>
        public float ElapsedGameTime
        {
			get { return elapsedGameTime; }
			set { elapsedGameTime = value; }
        }

		private float elapsedRealTime;
        /// <summary>
        /// Gets the elapsed real time, in seconds.
        /// </summary>
        /// <value>The elapsed real time.</value>
        public float ElapsedRealTime
        {
			get { return elapsedRealTime; }
			set { elapsedRealTime = value; }
        }

		private float totalGameTime;
        /// <summary>
        /// Gets the total game time, in seconds.
        /// </summary>
        /// <value>The total game time.</value>
        public float TotalGameTime
        {
            get { return totalGameTime; }
            set { totalGameTime = value; }
        }

        private float totalRealTime;
        /// <summary>
        /// Gets the total real time, in seconds.
        /// </summary>
        /// <value>The total real time.</value>
        public float TotalRealTime
        {
            get { return totalRealTime; }
            set { totalRealTime = value; }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="GameTime"/> class.
        /// </summary>
        public GameTime()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="GameTime"/> class.
        /// </summary>
        /// <param name="totalRealTime">The total real time.</param>
        /// <param name="elapsedRealTime">The elapsed real time.</param>
        /// <param name="totalGameTime">The total game time.</param>
        /// <param name="elapsedGameTime">The elapsed game time.</param>
        public GameTime(float totalRealTime, float elapsedRealTime, float totalGameTime, float elapsedGameTime)
        {
            TotalRealTime = totalRealTime;
            ElapsedRealTime = elapsedRealTime;
            TotalGameTime = totalGameTime;
            ElapsedGameTime = elapsedGameTime;
        }
    }
}
