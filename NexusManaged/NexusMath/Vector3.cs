using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Nexus.MathLib
{
    /// <summary>
    /// 3维向量
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        #region Fields
        public float x;
        public float y;
        public float z;


        private static readonly Vector3 positiveInfinityVector = new Vector3(float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity);
        public static Vector3 PositiveInfinity { get { return positiveInfinityVector; } }

        private static readonly Vector3 negativeInfinityVector = new Vector3(float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity);
        public static Vector3 NegativeInfinity { get { return negativeInfinityVector; } }

        private static readonly Vector3 invalidVector = new Vector3(float.NaN, float.NaN, float.NaN);
        public static Vector3 Invalid { get { return invalidVector; } }
        private static readonly Vector3 zeroVector = new Vector3(0.0f, 0.0f, 0.0f);
        private static readonly Vector3 unitX = new Vector3(1.0f, 0.0f, 0.0f);
        private static readonly Vector3 unitY = new Vector3(0.0f, 1.0f, 0.0f);
        private static readonly Vector3 unitZ = new Vector3(0.0f, 0.0f, 1.0f);
        private static readonly Vector3 negativeUnitX = new Vector3(-1.0f, 0.0f, 0.0f);
        private static readonly Vector3 negativeUnitY = new Vector3(0.0f, -1.0f, 0.0f);
        private static readonly Vector3 negativeUnitZ = new Vector3(0.0f, 0.0f, -1.0f);
        private static readonly Vector3 unitVector = new Vector3(1.0f, 1.0f, 1.0f);

        #endregion

        #region Constructors
        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        #endregion


        #region Overloaded operators + CLS compliant method equivalents

        public static bool operator ==(Vector3 left, Vector3 right)
        {
            return (left.x == right.x && left.y == right.y && left.z == right.z);
        }

        public static bool operator !=(Vector3 left, Vector3 right)
        {
            return (left.x != right.x || left.y != right.y || left.z != right.z);
        }

        public static Vector3 Multiply(Vector3 left, Vector3 right)
        {
            return left * right;
        }

        public static Vector3 operator *(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
        }

        public static Vector3 Multiply(Vector3 left, float scale)
        {
            return left * scale;
        }

        public static Vector3 operator *(Vector3 left, float scale)
        {
            return new Vector3(left.x * scale, left.y * scale, left.z * scale);
        }

        public static Vector3 Multiply(float scale, Vector3 right)
        {
            return scale * right;
        }

        public static Vector3 operator *(float scale, Vector3 right)
        {
            return new Vector3(scale * right.x, scale * right.y, scale * right.z);
        }

        public static Vector3 Divide(Vector3 left, Vector3 right)
        {
            return left / right;
        }

        public static Vector3 operator /(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x / right.x, left.y / right.y, left.z / right.z);
        }

        public static Vector3 Divide(Vector3 left, float scale)
        {
            return left / scale;
        }

        public static Vector3 operator /(Vector3 left, float scale)
        {
            Debug.Assert(scale != 0.0f, "Cannot divide a Vector3 by zero.");

            float inverse = 1.0f / scale;
            return new Vector3(left.x * inverse, left.y * inverse, left.z * inverse);
        }


        public static Vector3 Add(Vector3 left, Vector3 right)
        {
            return left + right;
        }

        public static Vector3 operator +(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
        }

        public static Vector3 Subtract(Vector3 left, Vector3 right)
        {
            return left - right;
        }

        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
        }

        public static Vector3 Negate(Vector3 left)
        {
            return -left;
        }

        public static Vector3 operator -(Vector3 left)
        {
            return new Vector3(-left.x, -left.y, -left.z);
        }

        public static bool operator >(Vector3 left, Vector3 right)
        {
            return (left.x > right.x && left.y > right.y && left.z > right.z);
        }

        public static bool operator <(Vector3 left, Vector3 right)
        {
            return (left.x < right.x && left.y < right.y && left.z < right.z);
        }

        public static explicit operator Vector4(Vector3 vec)
        {
            return new Vector4(vec.x, vec.y, vec.z, 1.0f);
        }

        public static Vector3 SymmetricRandom()
        {
            return SymmetricRandom(1f, 1f, 1f);
        }

        public static Vector3 SymmetricRandom(Vector3 maxComponentMagnitude)
        {
            return SymmetricRandom(maxComponentMagnitude.x, maxComponentMagnitude.y, maxComponentMagnitude.z);
        }

        public static Vector3 SymmetricRandom(float maxComponentMagnitude)
        {
            return SymmetricRandom(maxComponentMagnitude, maxComponentMagnitude, maxComponentMagnitude);
        }

        public static Vector3 SymmetricRandom(float xMult, float yMult, float zMult)
        {
            return new Vector3(
                (xMult == 0) ? 0 : xMult * MathUtil.SymmetricRandom(),
                (yMult == 0) ? 0 : yMult * MathUtil.SymmetricRandom(),
                (zMult == 0) ? 0 : zMult * MathUtil.SymmetricRandom()
                );
        }

        #endregion

        #region Public methods

        /// <summary>
        /// 自定义索引器
        /// </summary>
        /// <param name="index"> 
        /// access a Vector by index 0 = x, 1 = y, 2 = z 
        /// </param>
        public float this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return x;
                    case 1: return y;
                    case 2: return z;
                    default:
                        throw new ArgumentOutOfRangeException("Indexer boundaries overrun in Vector3, index must be from 0 to 2");
                }
            }
            set
            {
                switch (index)
                {
                    case 0: x = value; break;
                    case 1: y = value; break;
                    case 2: z = value; break;
                    default:
                        throw new ArgumentOutOfRangeException("Indexer boundaries overrun in Vector3, index must be from 0 to 2");
                }
            }
        }

        public object[] ToObjectArray()
        {
            return new object[] { x, y, z };
        }

        public float[] ToArray()
        {
            return new float[] { x, y, z };
        }

        public float Dot(Vector3 vec)
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        public Vector3 Cross(Vector3 vec)
        {
            return new Vector3(
                (this.y * vec.z - this.z * vec.y),
                (this.z * vec.x - this.x * vec.z),
                (this.x * vec.y - this.y * vec.x)
                );
        }

        public Vector3 ToNormalized()
        {
            Vector3 vec = this;
            vec.Normalize();
            return vec;
        }

        /// <summary>
        ///		Normalizes the vector.
        /// </summary>
        /// <remarks>
        ///		This method normalises the vector such that it's
        ///		length / magnitude is 1. The result is called a unit vector.
        ///		<p/>
        ///		This function will not crash for zero-sized vectors, but there
        ///		will be no changes made to their components.
        ///	</remarks>
        ///	<returns>The previous length of the vector.</returns>
        public float Normalize()
        {
            float length = MathUtil.Sqrt(this.x * this.x + this.y * this.y + this.z * this.z);

            // Will also work for zero-sized vectors, but will change nothing
            if (length > float.Epsilon)
            {
                float inverseLength = 1.0f / length;

                this.x *= inverseLength;
                this.y *= inverseLength;
                this.z *= inverseLength;
            }

            return length;
        }

        /// <summary>
        ///		Compares the supplied vector and updates it's x/y/z components of they are higher in value.
        /// </summary>
        /// <param name="compare"></param>
        public void Ceil(Vector3 compare)
        {
            if (compare.x > x)
                x = compare.x;
            if (compare.y > y)
                y = compare.y;
            if (compare.z > z)
                z = compare.z;
        }

        /// <summary>
        ///		Compares the supplied vector and updates it's x/y/z components of they are lower in value.
        /// </summary>
        /// <param name="compare"></param>
        /// <returns></returns>
        public void Floor(Vector3 compare)
        {
            if (compare.x < x)
                x = compare.x;
            if (compare.y < y)
                y = compare.y;
            if (compare.z < z)
                z = compare.z;
        }

        #endregion

        #region Public Properties

        public float Length
        {
            get
            {
                return MathUtil.Sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
            }
        }

        public float LengthSquared
        {
            get
            {
                return (this.x * this.x + this.y * this.y + this.z * this.z);
            }
        }
        #endregion

        #region  Static Constant Properties

        /// <summary>
        ///		Gets a Vector3 with all components set to 0.
        /// </summary>
        public static Vector3 Zero
        {
            get
            {
                return zeroVector;
            }
        }

        /// <summary>
        ///		Gets a Vector3 with all components set to 1.
        /// </summary>
        public static Vector3 UnitScale
        {
            get
            {
                return unitVector;
            }
        }

        /// <summary>
        ///		Gets a Vector3 with the X set to 1, and the others set to 0.
        /// </summary>
        public static Vector3 UnitX
        {
            get
            {
                return unitX;
            }
        }

        /// <summary>
        ///		Gets a Vector3 with the Y set to 1, and the others set to 0.
        /// </summary>
        public static Vector3 UnitY
        {
            get
            {
                return unitY;
            }
        }

        /// <summary>
        ///		Gets a Vector3 with the Z set to 1, and the others set to 0.
        /// </summary>
        public static Vector3 UnitZ
        {
            get
            {
                return unitZ;
            }
        }

        /// <summary>
        ///		Gets a Vector3 with the X set to -1, and the others set to 0.
        /// </summary>
        public static Vector3 NegativeUnitX
        {
            get
            {
                return negativeUnitX;
            }
        }

        /// <summary>
        ///		Gets a Vector3 with the Y set to -1, and the others set to 0.
        /// </summary>
        public static Vector3 NegativeUnitY
        {
            get
            {
                return negativeUnitY;
            }
        }

        /// <summary>
        ///		Gets a Vector3 with the Z set to -1, and the others set to 0.
        /// </summary>
        public static Vector3 NegativeUnitZ
        {
            get
            {
                return negativeUnitZ;
            }
        }

        #endregion

        #region Object Override
        /// <summary>
        /// 重载Object.ToString()
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return string.Format("({0},{1},{2})", this.x, this.y, this.z);
        }

        public static Vector3 Parse(string text)
        {
            string[] vals = text.TrimStart('(', '[', '<').TrimEnd(')', ']', '>').Split(',');
            if (vals.Length != 3)
                throw new FormatException(string.Format("Cannot parse the text '{0}' because it does not have 3 parts separated by commas in the form (x,y,z) with optional parenthesis.", text));
            try
            {
                return new Vector3(
                    float.Parse(vals[0].Trim()),
                    float.Parse(vals[1].Trim()),
                    float.Parse(vals[2].Trim())
                    );
            }
            catch
            {
                throw new FormatException("The parts of the vectors must be decimal numbers");
            }
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
            return x.GetHashCode() ^ y.GetHashCode() ^ z.GetHashCode();
        }

        /// <summary>
        ///		Compares this Vector to another object.  This should be done because the 
        ///		equality operators (==, !=) have been overriden by this class.
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public override bool Equals(object obj)
        {
            return (obj is Vector3) && (this == (Vector3)obj);
        }
        #endregion

    } // end of struct Vector3

} // end of namespace Nexus.MathLib
