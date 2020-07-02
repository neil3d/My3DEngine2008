using System;
using System.Globalization;

namespace Nexus.MathLib
{
    /// <summary>
    ///	颜色，使用4个浮点数的颜色值
    /// </summary>
    public struct Color
    {
        #region Fields

        /// <summary>
        ///		Alpha component [0.0, 1.0];
        /// </summary>
        private float a;
        /// <summary>
        ///		Red component [0.0, 1.0];
        /// </summary>
        private float r;
        /// <summary>
        ///		Green component [0.0, 1.0];
        /// </summary>
        private float g;
        /// <summary>
        ///		Blue component [0.0, 1.0];
        /// </summary>
        private float b;

        #endregion Fields

        #region Constructors

        /// <summary>
        ///		Constructor.
        /// </summary>
        /// <remarks>Alpha defaults to 1.0f.</remarks>
        /// <param name="r">Red component.</param>
        /// <param name="g">Green component.</param>
        /// <param name="b">Blue component.</param>
        public Color(float r, float g, float b)
            : this(1.0f, r, g, b) { }

        /// <summary>
        ///		Constructor.
        /// </summary>
        /// <param name="a">Alpha component.</param>
        /// <param name="r">Red component.</param>
        /// <param name="g">Green component.</param>
        /// <param name="b">Blue component.</param>
        public Color(float a, float r, float g, float b)
        {
            this.a = a;
            this.r = r;
            this.g = g;
            this.b = b;
        }

        /// <summary>
        ///		Constructor for creating a color from a hex (HTML) value.
        /// </summary>
        /// <param name="hexValue">Color value in the format 0xAARRGGBB.</param>
        public Color(uint hexValue)
        {
            this.a = ((hexValue & 0xFF000000) >> 24) / 255.0f;
            this.r = ((hexValue & 0x00FF0000) >> 16) / 255.0f;
            this.g = ((hexValue & 0x0000FF00) >> 8) / 255.0f;
            this.b = (hexValue & 0x000000FF) / 255.0f;
        }

        #endregion Constructors

        #region Porperties
        public float A
        {
            get { return this.a; }
            set { this.a = value; }
        }

        public float R
        {
            get { return this.r; }
            set { this.r = value; }
        }

        public float G
        {
            get { return this.g; }
            set { this.g = value; }
        }

        public float B
        {
            get { return this.b; }
            set { this.b = value; }
        }

        public uint ARGB
        {
            get { return this.ToARGB(); }
            set { ARGBTo(value); }
        }

        #endregion Porperties

        #region Methods

        public void ARGBTo(uint hexValue)
        {
            this.a = ((hexValue & 0xFF000000) >> 24) / 255.0f;
            this.r = ((hexValue & 0x00FF0000) >> 16) / 255.0f;
            this.g = ((hexValue & 0x0000FF00) >> 8) / 255.0f;
            this.b = (hexValue & 0x000000FF) / 255.0f;
        }

        /// <summary>
        ///		Returns this color as a 32-bit 0xAARRGGBB integer.
        /// </summary>
        /// <returns>A 32-bit 0xAARRGGBB representation of this color object.</returns>
        public uint ToARGB()
        {
            uint result = 0;

            result += ((uint)(a * 255.0f)) << 24;
            result += ((uint)(r * 255.0f)) << 16;
            result += ((uint)(g * 255.0f)) << 8;
            result += ((uint)(b * 255.0f));

            return result;
        }

        /// <summary>
        ///		Returns a string representation of the color in 0xAARRGGBB format.
        /// </summary>
        /// <returns>A string 0xAARRGGBB representation of this color object.</returns>
        public override string ToString()
        {
            return string.Format("{0:x}", ToARGB());
        }

        /// <summary>
        ///		Parses the string representation of a color and returns the corresponding
        ///		Color object.
        /// </summary>
        /// <param name="data">String representation of the Color "AARRGGBB".</param>
        /// <returns>A Color object with the correspoding color value.</returns>
        public static Color Parse(string data)
        {
            return new Color(uint.Parse(data, NumberStyles.HexNumber));
        }

        #endregion Methods

        #region Operators

        public override bool Equals(object obj)
        {
            if (!(obj is Color))
                return false;
            return ((Color)obj) == this;
        }

        public override int GetHashCode()
        {
            return a.GetHashCode() ^ r.GetHashCode() ^ g.GetHashCode() ^ b.GetHashCode();
        }



        /// <summary>
        ///		Compare 2 colors.
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        public static bool operator ==(Color left, Color right)
        {
            return MathUtil.FloatEqual(left.a, right.a) &&
                    MathUtil.FloatEqual(left.r, right.r) &&
                    MathUtil.FloatEqual(left.g, right.g) &&
                    MathUtil.FloatEqual(left.b, right.b);
        }

        /// <summary>
        ///		Compare 2 colors.
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        public static bool operator !=(Color left, Color right)
        {
            return !MathUtil.FloatEqual(left.a, right.a) ||
                    !MathUtil.FloatEqual(left.r, right.r) ||
                    !MathUtil.FloatEqual(left.g, right.g) ||
                    !MathUtil.FloatEqual(left.b, right.b);
        }

        #endregion Operators
    }
}
