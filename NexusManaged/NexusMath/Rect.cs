using System;

namespace Nexus.MathLib
{
    /// <summary>
    /// Summary description for Rect.
    /// </summary>
    public struct Rect
    {
        #region Fields

        private float left, right, top, bottom;

        #endregion

        // --------------------------
        #region Constructor

        /// <summary>
        ///     Constructor.
        /// </summary>
        /// <param name="left">Left.</param>
        /// <param name="top">Top.</param>
        /// <param name="right">Right.</param>
        /// <param name="bottom">Bottom.</param>
        public Rect(float left, float top, float right, float bottom)
        {
            this.left = left;
            this.top = top;
            this.right = right;
            this.bottom = bottom;
        }

        #endregion

        #region Properties

        public float Left
        {
            get { return this.left; }
            set { left = value; }
        }

        public float Right
        {
            get { return this.right;  }
            set { right = value; }
        }

        public float Bottom
        {
            get { return bottom; }
            set { bottom = value; }
        }

        public float Top
        {
            get { return top; }
            set { top = value; }
        }

        /// <summary>
        ///		Get/Set the top-left corner of the rectangle.
        /// </summary>
        /// <value>Point that holds the x and y position.</value>
        public Point Position
        {
            get
            {
                return new Point(left, top);
            }
            set
            {
                Size size = this.Size;

                left = value.X;
                top = value.Y;

                this.Size = size;
            }
        }

        /// <summary>
        ///     Width of this rectangle, from the left side.
        /// </summary>
        public float Width
        {
            get
            {
                return right - left;
            }
            set
            {
                // HACK: test
                if (float.IsNaN(value))
                {
                    value = 0;
                }

                right = left + value;
            }
        }

        /// <summary>
        ///     Height of this rectangle, from the top edge.
        /// </summary>
        public float Height
        {
            get
            {
                return bottom - top;
            }
            set
            {
                // HACK: test
                if (float.IsNaN(value))
                {
                    value = 0;
                }

                bottom = top + value;
            }
        }

        /// <summary>
        ///		Get/Set the size of the rect area.
        /// </summary>
        /// <value>The desired size of the rectangle.</value>
        public Size Size
        {
            get
            {
                return new Size(this.Width, this.Height);
            }
            set
            {
                this.Width = value.Width;
                this.Height = value.Height;
            }
        }

        #endregion

        #region Methods

        /// <summary>
        ///		Check the size of this rect if it is bigger than <paramref name="size"/>, resize it so it isn't.
        /// </summary>
        /// <param name="size">Describes the maximum dimensions that this Rect should be limited to.</param>
        public void ConstrainSizeMax(Size size)
        {
            if (this.Width > size.Width)
            {
                this.Width = size.Width;
            }

            if (this.Height > size.Height)
            {
                this.Height = size.Height;
            }
        }

        /// <summary>
        ///		Check the size of this rect if it is bigger than <paramref name="size"/>, resize it so it isn't.
        /// </summary>
        /// <param name="size">Describes the minimum dimensions that this Rect should be limited to.</param>
        public void ConstrainSizeMin(Size size)
        {
            if (this.Width < size.Width)
            {
                this.Width = size.Width;
            }

            if (this.Height < size.Height)
            {
                this.Height = size.Height;
            }
        }

        /// <summary>
        ///		Check the size of this rect if it is bigger than <paramref name="max"/> or
        ///		smaller than <paramref name="min"/>, resize it so it isn't.
        /// </summary>
        /// <param name="max">Describes the maximum dimensions that this Rect should be limited to.</param>
        /// <param name="min">Describes the minimum dimensions that this Rect should be limited to.</param>
        public void ConstrainSize(Size max, Size min)
        {
            Size currentSize = this.Size;

            if (this.Width > max.Width)
            {
                this.Width = max.Width;
            }
            else if (this.Width < min.Width)
            {
                this.Width = min.Width;
            }

            if (this.Height > max.Height)
            {
                this.Height = max.Height;
            }
            else if (this.Height < min.Height)
            {
                this.Height = min.Height;
            }
        }

        /// <summary>
        ///		return a Rect that is the intersection of 'this' Rect with <paramref name="rect"/>.
        /// </summary>
        /// <remarks>
        ///		It can be assumed that if left == right, or top == bottom, or Width == 0, or Height == 0, then
        ///		'this' rect was totally outside <paramref name="rect"/>.
        /// </remarks>
        /// <param name="rect">Rect to test for intersection.</param>
        /// <returns>Instersection rect.</returns>
        public Rect GetIntersection(Rect rect)
        {
            // check for total exclusion
            if ((right > rect.left) &&
                (left < rect.right) &&
                (bottom > rect.top) &&
                (top < rect.bottom))
            {

                Rect temp;

                // fill in temp with the intersection
                temp.left = (left > rect.left) ? left : rect.left;
                temp.right = (right < rect.right) ? right : rect.right;
                temp.top = (top > rect.top) ? top : rect.top;
                temp.bottom = (bottom < rect.bottom) ? bottom : rect.bottom;

                return temp;
            }

            return new Rect(0.0f, 0.0f, 0.0f, 0.0f);
        }

        /// <summary>
        ///     Return true if the given Point falls within this Rect.
        /// </summary>
        /// <param name="point">Point describing the position to test.</param>
        /// <returns>True if the point is within this rectangle, false otherwise..</returns>
        public bool IsPointInRect(Point point)
        {
            return ((point.X >= left) &&
                    (point.X < right) &&
                    (point.Y >= top) &&
                    (point.Y < bottom));
        }

        /// <summary>
        ///     Applies an offset this Rect.
        /// </summary>
        /// <param name="point">Point containing the offsets to be applied to the Rect.</param>
        public void Offset(Point point)
        {
            left += point.X;
            right += point.X;
            top += point.Y;
            bottom += point.Y;
        }

        public void Offset(float offest)
        {
            left += offest;
            right += offest;
            top += offest;
            bottom += offest;
        }

        /// <summary>
        ///		Returns a string representation of this Rect object in the form
        ///		"l:[left] t:[top] r:[right] b:[bottom]
        /// </summary>
        /// <returns>A string representation of this Rect object.</returns>
        public override string ToString()
        {
            return string.Format("l:{0} t:{1} r:{2} b:{3}", left, top, right, bottom);
        }

        /// <summary>
        ///		Parses the string representation of a Rect and returns the corresponding
        ///		Rect object.
        /// </summary>
        /// <param name="data">String representation of a Rect.</param>
        /// <returns>Rect corresponding to the passed in string.</returns>
        public static Rect Parse(string data)
        {
            string[] parameters = data.Split(new char[] { ' ', ':' });
            Rect rect = new Rect();

            for (int i = 0; i < parameters.Length; i++)
            {
                if (0 == parameters[i].CompareTo("l"))
                {
                    rect.left = float.Parse(parameters[++i]);
                }
                else if (0 == parameters[i].CompareTo("t"))
                {
                    rect.top = float.Parse(parameters[++i]);
                }
                else if (0 == parameters[i].CompareTo("r"))
                {
                    rect.right = float.Parse(parameters[++i]);
                }
                else if (0 == parameters[i].CompareTo("b"))
                {
                    rect.bottom = float.Parse(parameters[++i]);
                }
            }
            return rect;
        }

        #endregion
    }
}
