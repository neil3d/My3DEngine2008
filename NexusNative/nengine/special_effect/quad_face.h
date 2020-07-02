#ifndef _NEXUS_QUAD_FACE_H_
#define _NEXUS_QUAD_FACE_H_
#include "ncore.h"
#include "neffect_vertex_data.h"

namespace nexus
{

	enum ESFX_FaceType
	{
		ESFX_NormalFace,		// 普通面片
		ESFX_SphereBillboard,	// 球形公告板面
		ESFX_CylinderBillboard,	// 筒形公告板面
	};


	struct quad_face
	{
		vector3		pos;		// 位置
		vector3		rot;		// 对于公告板，只有x分量有用
		vector3		revolution;	// 公转
		vector2		half_size;	// 当前"半径"
		color4f		color;		//当前色彩
		
		vector2		uv_start;	//纹理坐标
		vector2		uv_end;

		quad_face(void)
		{
			memset(this, 0, sizeof(*this));
		}

		static void build_mesh_vertices(neffect_vertex_data& vb, const std::vector<quad_face>& faces, const ncamera& cam, const matrix44& world_mat, ESFX_FaceType type,int face_count=-1);
	};

	typedef std::vector<quad_face> st_quad_array;
}//namespace nexus

#endif //_NEXUS_QUAD_FACE_H_