using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Nexus.MathLib
{
    /// <summary>
    ///		A standard sphere, used mostly for bounds checking.
    /// </summary>
    /// <remarks>
    ///		A sphere in math texts is normally represented by the function
    ///		x^2 + y^2 + z^2 = r^2 (for sphere's centered on the origin). We store spheres
    ///		simply as a center point and a radius.
    /// </remarks>
    public sealed class Sphere
    {
        #region Protected member variables

        private float radius;
        private Vector3 center;

        #endregion

        #region Constructors

        /// <summary>
        ///		Creates a unit sphere centered at the origin.
        /// </summary>
        public Sphere()
        {
            radius = 1.0f;
            center = Vector3.Zero;
        }

        /// <summary>
        /// Creates an arbitrary spehere.
        /// </summary>
        /// <param name="center">Center point of the sphere.</param>
        /// <param name="radius">Radius of the sphere.</param>
        public Sphere(Vector3 center, float radius)
        {
            this.center = center;
            this.radius = radius;
        }

        #endregion

        #region Properties

        /// <summary>
        ///		Gets/Sets the center of the sphere.
        /// </summary>
        public Vector3 Center
        {
            get
            {
                return center;
            }
            set
            {
                center = value;
            }
        }

        /// <summary>
        ///		Gets/Sets the radius of the sphere.
        /// </summary>
        public float Radius
        {
            get
            {
                return radius;
            }
            set
            {
                radius = value;
            }
        }

        #endregion

        #region Intersection methods

        /// <summary>
        ///		Tests for intersection between this sphere and another sphere.
        /// </summary>
        /// <param name="sphere">Other sphere.</param>
        /// <returns>True if the spheres intersect, false otherwise.</returns>
        public bool Intersects(Sphere sphere)
        {
            return ((sphere.center - center).Length <= (sphere.radius + radius));
        }

        /// <summary>
        ///		Returns whether or not this sphere interects a box.
        /// </summary>
        /// <param name="box"></param>
        /// <returns>True if the box intersects, false otherwise.</returns>
        public bool Intersects(AABBox box)
        {
            return MathUtil.Intersects(this, box);
        }

        /// <summary>
        ///		Returns whether or not this sphere interects a plane.
        /// </summary>
        /// <param name="plane"></param>
        /// <returns>True if the plane intersects, false otherwise.</returns>
        public bool Intersects(Plane plane)
        {
            return MathUtil.Intersects(this, plane);
        }

        /// <summary>
        ///		Returns whether or not this sphere interects a Vector3.
        /// </summary>
        /// <param name="vector"></param>
        /// <returns>True if the vector intersects, false otherwise.</returns>
        public bool Intersects(Vector3 vector)
        {
            return (vector - center).Length <= radius;
        }

        #endregion Intersection methods
    }
}
