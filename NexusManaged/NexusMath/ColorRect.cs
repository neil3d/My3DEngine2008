using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Nexus.MathLib
{
    /// <summary>
    ///		Class that holds details of colors for the four corners of a rectangle.
    /// </summary>
    public class ColorRect
    {
        #region Fields

        private Color topLeft, topRight, bottomLeft, bottomRight;

        #endregion Fields

        #region Porperties
        public Color TopLeft
        {
            get { return this.topLeft; }
            set { this.topLeft = value; }
        }

        public Color TopRight
        {
            get { return this.topRight; }
            set { this.topRight = value; }
        }

        public Color BottomLeft
        {
            get { return this.bottomLeft; }
            set { this.bottomLeft = value; }
        }

        public Color BottomRight
        {
            get { return this.bottomRight; }
            set { this.bottomRight = value; }
        }
        #endregion

        #region Constructor
        /// <summary>
        ///		Default constructor.
        /// </summary>
        public ColorRect()
        {
            topLeft = new Color(1, 1, 1, 1);
            topRight = new Color(1, 1, 1, 1);
            bottomLeft = new Color(1, 1, 1, 1);
            bottomRight = new Color(1, 1, 1, 1);
        }

        /// <summary>
        ///		Constructor.
        /// </summary>
        /// <param name="color">Color to use for the four corners.</param>
        public ColorRect(Color color)
        {
            this.topLeft = color;
            this.topRight = color;
            this.bottomLeft = color;
            this.bottomRight = color;
        }

        /// <summary>
        ///		Constructor.
        /// </summary>
        /// <param name="topLeft"></param>
        /// <param name="topRight"></param>
        /// <param name="bottomLeft"></param>
        /// <param name="bottomRight"></param>
        public ColorRect(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight)
        {
            this.topLeft = topLeft;
            this.topRight = topRight;
            this.bottomLeft = bottomLeft;
            this.bottomRight = bottomRight;
        }
        #endregion

        #region Public Methods
        /// <summary>
        ///		Sets the alpha value for all colors in the color rect.
        /// </summary>
        /// <param name="alpha"></param>
        public void SetAlpha(float alpha)
        {
            topLeft.A = alpha;
            topRight.A = alpha;
            bottomLeft.A = alpha;
            bottomRight.A = alpha;
        }

        /// <summary>
        ///		Returns a string representation of this ColorRect in the form
        ///		"tl:[color] tr:[color] bl:[color] br:[color]".
        /// </summary>
        /// <returns>A string representation of this ColorRect.</returns>
        public override string ToString()
        {
            return string.Format("tl:{0} tr:{1} bl:{2} br:{3}", topLeft, topRight, bottomLeft, bottomRight);
        }

        /// <summary>
        ///		Parses a string representation of the ColorRect and returns the corresponding
        ///		ColorRect object.
        /// </summary>
        /// <param name="data">A string representation of a ColorRect.</param>
        /// <returns>A ColorRect corresponding matching the string representation.</returns>
        public static ColorRect Parse(string data)
        {
            string[] parameters = data.Split(new char[] { ' ', ':' });
            ColorRect colors = new ColorRect();

            for (int i = 0; i < parameters.Length; i++)
            {
                if (parameters[i].CompareTo("tl") == 0)
                {
                    colors.topLeft = Color.Parse(parameters[++i]);
                }
                else if (parameters[i].CompareTo("tr") == 0)
                {
                    colors.topRight = Color.Parse(parameters[++i]);
                }
                else if (parameters[i].CompareTo("bl") == 0)
                {
                    colors.bottomLeft = Color.Parse(parameters[++i]);
                }
                else if (parameters[i].CompareTo("br") == 0)
                {
                    colors.bottomRight = Color.Parse(parameters[++i]);
                }
            }

            return colors;
        }
        #endregion
    }
}
