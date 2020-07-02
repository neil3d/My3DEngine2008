using System;

namespace Nexus.MathLib
{
    /// <summary>
    /// 	Representation of a ray in space, ie a line with an origin and direction.
    /// </summary>
    public class Ray
    {
        #region Fields

        internal Vector3 origin;
        internal Vector3 direction;

        #endregion

        #region Constructors

        /// <summary>
        ///    Default constructor.
        /// </summary>
        public Ray()
        {
            origin = Vector3.Zero;
            direction = Vector3.UnitZ;
        }

        /// <summary>
        ///    Constructor.
        /// </summary>
        /// <param name="origin">Starting point of the ray.</param>
        /// <param name="direction">Direction the ray is pointing.</param>
        public Ray(Vector3 origin, Vector3 direction)
        {
            this.origin = origin;
            this.direction = direction;
        }

        #endregion

        #region Intersection Methods

        /// <summary>
        ///    Tests whether this ray intersects the given box.
        /// </summary>
        /// <param name="box"></param>
        /// <returns>
        ///		Struct containing info on whether there was a hit, and the distance from the 
        ///		origin of this ray where the intersect happened.
        ///	</returns>
        public IntersectResult Intersects(AABBox box)
        {
            return MathUtil.Intersects(this, box);
        }

        /// <summary>
        ///		Tests whether this ray intersects the given plane. 
        /// </summary>
        /// <param name="plane"></param>
        /// <returns>
        ///		Struct containing info on whether there was a hit, and the distance from the 
        ///		origin of this ray where the intersect happened.
        ///	</returns>
        public IntersectResult Intersects(Plane plane)
        {
            return MathUtil.Intersects(this, plane);
        }

        /// <summary>
        ///		Tests whether this ray intersects the given sphere. 
        /// </summary>
        /// <param name="sphere"></param>
        /// <returns>
        ///		Struct containing info on whether there was a hit, and the distance from the 
        ///		origin of this ray where the intersect happened.
        ///	</returns>
        public IntersectResult Intersects(Sphere sphere)
        {
            return MathUtil.Intersects(this, sphere);
        }

        #endregion Intersection Methods

        #region Operator Overloads

        /// <summary>
        ///    Gets the position of a point t units along the ray.
        /// </summary>
        /// <param name="ray"></param>
        /// <param name="t"></param>
        /// <returns></returns>
        public static Vector3 operator *(Ray ray, float t)
        {
            return ray.origin + (ray.direction * t);
        }

        #endregion Operator Overloads

        #region Properties

        /// <summary>
        ///    Gets/Sets the origin of the ray.
        /// </summary>
        public Vector3 Origin
        {
            get
            {
                return origin;
            }
            set
            {
                origin = value;
            }
        }

        /// <summary>
        ///    Gets/Sets the direction this ray is pointing.
        /// </summary>
        /// <remarks>
        ///    A ray has no length, so the direction goes to infinity.
        /// </remarks>
        public Vector3 Direction
        {
            get
            {
                return direction;
            }
            set
            {
                direction = value;
            }
        }

        #endregion
    }
}
