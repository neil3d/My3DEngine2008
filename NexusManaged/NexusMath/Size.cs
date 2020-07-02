using System;

namespace Nexus.MathLib
{
    /// <summary>
    ///		Struct that holds the size (width and height) of something.
    /// </summary>
    public struct Size
    {
        #region Fields
        private float width;
        private float height;
        #endregion

        #region Porperties
        public float Width
        {
            get { return this.width; }
            set { this.width = value; }
        }

        public float Height
        {
            get { return this.height; }
            set { this.height = value; }
        }
        #endregion

        #region Constructors
        public Size(float width, float height)
        {
            this.width = width;
            this.height = height;
        }
        #endregion


        #region Methods
        /// <summary>
        ///		Returns a string representation of this Size object.
        /// </summary>
        /// <returns>String representation of this object.</returns>
        public override string ToString()
        {
            return string.Format("w:{0} h:{1}", width, height);
        }

        /// <summary>
        ///		Parses the string representation of a Size object, and returns the
        ///		corresponding object.
        /// </summary>
        /// <param name="data">String representation of a Size object.</param>
        /// <returns>Size object corresponding to the passed in string.</returns>
        public static Size Parse(string data)
        {
            string[] parameters = data.Split(new char[] { ' ', ':' });
            Size size = new Size();

            for (int i = 0; i < parameters.Length; i++)
            {
                if (0 == parameters[i].CompareTo("w"))
                {
                    size.width = float.Parse(parameters[++i]);
                }
                else if (0 == parameters[i].CompareTo("h"))
                {
                    size.height = float.Parse(parameters[++i]);
                }
            }
            return size;
        }
        #endregion
    }
} // end of namespace Nexus.MathLib
