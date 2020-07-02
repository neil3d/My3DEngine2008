/**
*	Core of Nexus Engine -  Serialize functions for math data types
*
*	Author: Neil
*	Date:	Dec, 2008
*/

#ifndef _NEXUS_MATH_SERIALIZE_H_
#define _NEXUS_MATH_SERIALIZE_H_

#include "vector.h"
#include "quat.h"
#include "matrix.h"
#include "geom_def.h"

#include "../base_define.h"
#include "../base_depend.h"
#include "../object/narchive.h"

namespace nexus
{
	template<>
	inline narchive& nserialize(narchive& ar, vector2& vec, const TCHAR* obj_name)
	{
		nstring class_name(_T("vector2"));
		ar.object_begin(obj_name, class_name);
			nserialize(ar, vec.x, _T("x"));
			nserialize(ar, vec.y, _T("y"));
		ar.object_end();

		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, vector3& vec, const TCHAR* obj_name)
	{
		nstring class_name(_T("vector3"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, vec.x, _T("x"));
		nserialize(ar, vec.y, _T("y"));
		nserialize(ar, vec.z, _T("z"));
		ar.object_end();

		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, vector4& vec, const TCHAR* obj_name)
	{
		nstring class_name(_T("vector4"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, vec.x, _T("x"));
		nserialize(ar, vec.y, _T("y"));
		nserialize(ar, vec.z, _T("z"));
		nserialize(ar, vec.w, _T("w"));
		ar.object_end();

		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, quat& q, const TCHAR* obj_name)
	{
		nstring class_name(_T("quat"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, q.x, _T("x"));
		nserialize(ar, q.y, _T("y"));
		nserialize(ar, q.z, _T("z"));
		nserialize(ar, q.w, _T("w"));
		ar.object_end();

		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, matrix44& m, const TCHAR* obj_name)
	{
		nstring class_name(_T("matrix44"));
		ar.object_begin(obj_name, class_name);
		
		nserialize(ar, m._11, _T("_11"));
		nserialize(ar, m._12, _T("_12"));
		nserialize(ar, m._13, _T("_13"));
		nserialize(ar, m._14, _T("_14"));

		nserialize(ar, m._21, _T("_21"));
		nserialize(ar, m._22, _T("_22"));
		nserialize(ar, m._23, _T("_23"));
		nserialize(ar, m._24, _T("_24"));

		nserialize(ar, m._31, _T("_31"));
		nserialize(ar, m._32, _T("_32"));
		nserialize(ar, m._33, _T("_33"));
		nserialize(ar, m._34, _T("_34"));

		nserialize(ar, m._41, _T("_41"));
		nserialize(ar, m._42, _T("_42"));
		nserialize(ar, m._43, _T("_43"));
		nserialize(ar, m._44, _T("_44"));
		
		ar.object_end();
		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, box_sphere_bounds& b, const TCHAR* obj_name)
	{
		nstring class_name(_T("box_sphere_bounds"));
		ar.object_begin(obj_name, class_name);
		
		nserialize(ar, b.origin, _T("origin"));
		nserialize(ar, b.box_extent, _T("box_extent"));
		nserialize(ar, b.sphere_radius, _T("sphere_radius"));

		ar.object_end();
		return ar;
	}
}//namespace nexus

#endif //_NEXUS_MATH_SERIALIZE_H_