using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Nexus.GameFramework
{
    public static class MathHelper
    {
        public static float Lerp(float start, float end, float amount)
        {
            return start + ((end - start) * amount);
        }

        public static float ToRadians(float degrees)
        {
            return degrees / 57.2957795f;
        }

        public static float ToDegrees(float radians)
	    {
            return radians * 57.2957795f;
	    }

        public static T Clamp<T>(T value, T min, T max) where T : IComparable
        {
            if (value.CompareTo(max) >= 0)
                return max;
            if (value.CompareTo(min) <= 0)
                return min;
            return value;
        }
    }
}
