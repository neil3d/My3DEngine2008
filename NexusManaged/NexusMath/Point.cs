using System;

namespace Nexus.MathLib
{
    public struct Point
    {

        #region Fields
        private float x, y;
        #endregion

        #region Constructor
        public Point(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
        #endregion

        #region Public Properties
        public float X
        {
            get { return this.x; }
            set { this.x = value; }
        }

        public float Y
        {
            get { return this.y; }
            set { this.y = value; }
        }

        #endregion

        #region  Methods
        /// <summary>
        ///		Add 2 Point structs.
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        public static Point operator +(Point left, Point right)
        {
            return new Point(left.x + right.x, left.y + right.y);
        }

        /// <summary>
        ///		Subtract one Point from another.
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        public static Point operator -(Point left, Point right)
        {
            return new Point(left.x - right.x, left.y - right.y);
        }

        /// <summary>
        ///		Check if 2 Point structs are equal.
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        public static bool operator ==(Point left, Point right)
        {
            return (left.x == right.x) && (left.y == right.y);
        }

        /// <summary>
        ///		Check if 2 Point structs are not equal.
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        public static bool operator !=(Point left, Point right)
        {
            return (left.x != right.x) || (left.y != right.y);
        }

        /// <summary>
        ///		Provides a unique hash code based on the member variables of this
        ///		class.  This should be done because the equality operators (==, !=)
        ///		have been overriden by this class.
        ///		<p/>
        ///		The standard implementation is a simple XOR operation between all local
        ///		member variables.
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode()
        {
            return this.x.GetHashCode() ^ this.y.GetHashCode();
        }

        /// <summary>
        ///		Compares this Vector to another object.  This should be done because the 
        ///		equality operators (==, !=) have been overriden by this class.
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public override bool Equals(object obj)
        {
            if (obj is Point)
                return (this == (Point)obj);
            else
                return false;
        }

        /// <summary>
        /// Returns a string representation of the Point.
        /// </summary>
        /// <returns>The string representation of the point.</returns>
        public override string ToString()
        {
            return string.Format("x:{0} y:{1}", x, y);
        }

        /// <summary>
        /// Parses a string representation of a Point, and returns the Point.
        /// </summary>
        /// <param name="data">String representation of a point.</param>
        /// <returns>Point</returns>
        public static Point Parse(string data)
        {
            string[] parameters = data.Split(new char[] { ' ', ':', ',' });
            Point point = new Point();

            for (int i = 0; i < parameters.Length; i++)
            {
                if (0 == parameters[i].CompareTo("x"))
                {
                    point.x = float.Parse(parameters[++i]);
                }
                else if (0 == parameters[i].CompareTo("y"))
                {
                    point.y = float.Parse(parameters[++i]);
                }
            }
            return point;
        }
        #endregion
    }
}
