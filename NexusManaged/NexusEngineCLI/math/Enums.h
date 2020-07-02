
#pragma once

namespace NexusEngine
{
	// NOTE: The enumerations defined in this file are in alphabetical order. When
	//       adding new enumerations or renaming existing ones, please make sure
	//       the ordering is maintained.
	
	/// <summary>
	/// Describes how one bounding volume contains another.
	/// </summary>
	public enum class ContainmentType : System::Int32
	{
		/// <summary>
		/// The two bounding volumes don't intersect at all.
		/// </summary>
		Disjoint,

		/// <summary>
		/// One bounding volume completely contains another.
		/// </summary>
		Contains,

		/// <summary>
		/// The two bounding volumes overlap.
		/// </summary>
		Intersects
	};
	
	/// <summary>
	/// Describes the result of an intersection with a plane in three dimensions.
	/// </summary>
	public enum class PlaneIntersectionType : System::Int32
	{
		/// <summary>
		/// The object is behind the plane.
		/// </summary>
		Back,

		/// <summary>
		/// The object is in front of the plane.
		/// </summary>
		Front,

		/// <summary>
		/// The object is intersecting the plane.
		/// </summary>
		Intersecting
	};
		
}