#ifndef _NEXUS_MATH_EXP_H_
#define _NEXUS_MATH_EXP_H_
#include <sstream>
#include "../py_common.h"

inline void export_math()
{
	using namespace boost::python;
	using namespace nexus;

	class_<nrect>("nrect")
		.def( boost::python::init<long, long, long, long>() )
		.def_readwrite("left", &nrect::left)
		.def_readwrite("top", &nrect::top)
		.def_readwrite("right", &nrect::right)
		.def_readwrite("bottom", &nrect::bottom)
		;

	class_<npoint>("npoint")
		.def( boost::python::init<long, long>() )
		.def_readwrite("x", &npoint::x)
		.def_readwrite("y", &npoint::y)
		;

	//-- vector2 -----------------------------------------------
	class_<vector2>("vector2")
		.def( boost::python::init<float, float>() )
		.def_readwrite("x", &vector2::x)
		.def_readwrite("y", &vector2::y)
		.def( self+=self )
		.def( self-=self )
		.def( self*=float() )
		.def( self+self )
		.def( self-self )
		.def( self*float() )
		;

	// 函数重载
	float(*vec2_length)(const vector2& v) = &vec_length;
	def("vec_length", vec2_length);

	float(*vec2_length_sq)(const vector2& v) = &vec_length_sq;
	def("vec_length_sq", vec2_length_sq);

	vector2(*vec2_normalize)(const vector2& v) = &vec_normalize;
	def("vec_normalize", vec2_normalize);

	//-- vector3 -----------------------------------------------
	class_<vector3>("vector3")
		.def( boost::python::init<float, float, float>() )
		.def_readwrite("x", &vector3::x)
		.def_readwrite("y", &vector3::y)
		.def_readwrite("z", &vector3::z)
		.def( self+=self )
		.def( self-=self )
		.def( self*=float() )
		.def( self+self )
		.def( self-self )
		.def( self*float() )
		;

	// 函数重载
	float(*vec3_length)(const vector3& v) = &vec_length;
	def("vec_length", vec3_length);

	float(*vec3_length_sq)(const vector3& v) = &vec_length_sq;
	def("vec_length_sq", vec3_length_sq);

	vector3(*vec3_normalize)(const vector3& v) = &vec_normalize;
	def("vec_normalize", vec3_normalize);

	def("vec_distance", vec_distance);
	def("vec_distance_sq", vec_distance_sq);
	def("vec_dot", vec_dot);
	def("vec_cross", vec_cross);

	//-- vector4 -----------------------------------------------
	class_<vector4>("vector4")
		.def( boost::python::init<float, float, float, float>() )
		.def_readwrite("x", &vector4::x)
		.def_readwrite("y", &vector4::y)
		.def_readwrite("z", &vector4::z)
		.def_readwrite("w", &vector4::w)
		.def( self+=self )
		.def( self-=self )
		;

	//-- matrix33 -----------------------------------------------
	class_<matrix33>("matrix33")
		.def_readwrite("_11", &matrix33::_11)
		.def_readwrite("_12", &matrix33::_12)
		.def_readwrite("_13", &matrix33::_13)

		.def_readwrite("_21", &matrix33::_21)
		.def_readwrite("_22", &matrix33::_22)
		.def_readwrite("_23", &matrix33::_23)

		.def_readwrite("_31", &matrix33::_31)
		.def_readwrite("_32", &matrix33::_32)
		.def_readwrite("_33", &matrix33::_33)
		;

	//-- matrix44 -----------------------------------------------
	class_<matrix44>("matrix44")
		.def_readwrite("_11", &matrix44::_11)
		.def_readwrite("_12", &matrix44::_12)
		.def_readwrite("_13", &matrix44::_13)
		.def_readwrite("_14", &matrix44::_14)

		.def_readwrite("_21", &matrix44::_21)
		.def_readwrite("_22", &matrix44::_22)
		.def_readwrite("_23", &matrix44::_23)
		.def_readwrite("_24", &matrix44::_24)

		.def_readwrite("_31", &matrix44::_31)
		.def_readwrite("_32", &matrix44::_32)
		.def_readwrite("_33", &matrix44::_33)
		.def_readwrite("_34", &matrix44::_34)

		.def_readwrite("_41", &matrix44::_41)
		.def_readwrite("_42", &matrix44::_42)
		.def_readwrite("_43", &matrix44::_43)
		.def_readwrite("_44", &matrix44::_44)

		.def( self+self )
		.def( self-self )
		.def( self*self )
		.def( self*float() )

		.def("get_axis_x", &matrix44::get_axis_x)
		.def("get_axis_y", &matrix44::get_axis_y)
		.def("get_axis_z", &matrix44::get_axis_z)
		.def("get_origin", &matrix44::get_origin)
		;

		EXP_FUNC(mat_set_identity);
		
		EXP_FUNC(mat_set_lookat_LH);
		EXP_FUNC(mat_set_perspective_LH);
		EXP_FUNC(mat_set_translation);
		EXP_FUNC(mat_set_scaling);
		EXP_FUNC(mat_set_rotation_X);
		EXP_FUNC(mat_set_rotation_Y);
		EXP_FUNC(mat_set_rotation_Z);
		EXP_FUNC(mat_set_rotation_yaw_pitch_roll);
		EXP_FUNC(mat_translate);
		EXP_FUNC(mat_scale);
		EXP_FUNC(mat_rotate_X);
		EXP_FUNC(mat_rotate_Y);
		EXP_FUNC(mat_rotate_Z);
		EXP_FUNC(mat_transpose);
		

		void (*mat_set_rotation_axis1)(matrix44& mat, float angle, const vector3& axis) = &mat_set_rotation_axis;
		void (*mat_set_rotation_axis2)(matrix44& mat, const vector3& u, const vector3& v) = &mat_set_rotation_axis;

		def("mat_set_rotation_axis", mat_set_rotation_axis1);
		def("mat_set_rotation_axis", mat_set_rotation_axis2);

		//-- quat -----------------------------------------------
		class_<quat>("quat")
			.def( boost::python::init<float, float, float, float>() )
			.def_readwrite("x", &quat::x)
			.def_readwrite("y", &quat::y)
			.def_readwrite("z", &quat::z)
			.def_readwrite("w", &quat::w)

			.def( self*self )
			;

		EXP_FUNC(quat_set_identity);
		EXP_FUNC(quat_inverse);
		EXP_FUNC(quat_normalize);
		EXP_FUNC(quat_dot);
		EXP_FUNC(quat_slerp);
		EXP_FUNC(quat_set_rotation);
		
		void (*quat_to_matrix1)(const quat& q, matrix33& mat) = &quat_to_matrix;
		void (*quat_to_matrix2)(const quat& q, matrix44& mat) = &quat_to_matrix;

		def("quat_to_matrix", quat_to_matrix1);
		def("quat_to_matrix", quat_to_matrix2);

}


#endif //_NEXUS_MATH_EXP_H_