#include "StdAfx.h"
#include "quad_face.h"

namespace nexus
{
	void quad_face::build_mesh_vertices(neffect_vertex_data& vb, const st_quad_array& faces, const ncamera& cam, const matrix44& world_mat, ESFX_FaceType type,int face_count)
	{
		nASSERT( vb.get_num_vert() == faces.size()*6);
		vertex_stream* pos_stream = vb.get_stream(0);
		vertex_stream* diffuse_stream = vb.get_stream(1);
		vertex_stream* uv_stream = vb.get_stream(2);

		vector3* pos_ptr = (vector3*)(pos_stream->get_data());
		color4f* diffuse_ptr = (color4f*)(diffuse_stream->get_data());
		vector2* uv_ptr = (vector2*)(uv_stream->get_data());

		size_t num_faces = (face_count<0?faces.size():face_count);
		vector3 quad_pos[4];
		vector2 quad_uv[4];

		matrix44 mat_trans;
		matrix44 mat_rot;
		matrix44 mat_srot;

		switch (type)
		{		
		case ESFX_SphereBillboard:
		case ESFX_CylinderBillboard:
			{
				matrix44 mdl_view = world_mat*cam.get_view();

				vector3 view_right	= vec_normalize( vector3(mdl_view._11,mdl_view._21,mdl_view._31) );
				vector3 view_up		= vec_normalize( vector3(mdl_view._12,mdl_view._22,mdl_view._32) );
				vector3 view_forward= vec_normalize( vector3(mdl_view._13,mdl_view._23,mdl_view._33) );

				
				for (size_t i=0; i<num_faces; i++)
				{
					const quad_face& face = faces[i];

					mat_set_translation(mat_trans, face.pos);
					mat_set_rotation_yaw_pitch_roll(mat_rot, face.revolution.y, face.revolution.x, face.revolution.z);

					//此处测试world_mat对面片的影响，原文：mat_trans = mat_trans*mat_rot；
					mat_trans = mat_trans*mat_rot*world_mat;
					
					//-- 计算quad的轴
					vector3 xaxis = view_right;
					vector3 yaxis =	vector3(0, 1, 0);
					if(type == ESFX_SphereBillboard)
						yaxis = view_up;

					vector3 zaxis=vec_cross(xaxis,yaxis);
					mat_set_rotation_axis(mat_srot,face.rot.z,zaxis);

					xaxis *= face.half_size.x;
					yaxis *= face.half_size.y;

					//-- 计算顶点位置和uv等
					vector3 face_pos = face.pos*mat_trans;
					quad_pos[0] = -xaxis + yaxis;
					quad_pos[1] = -xaxis - yaxis;
					quad_pos[2] = xaxis - yaxis;					
					quad_pos[3] = xaxis + yaxis;
					for(int j=0;j<4;++j)
					{
						quad_pos[j]=quad_pos[j]*mat_srot;	//计算自转
						quad_pos[j]+=face_pos;
					}

					quad_uv[0] = vector2(face.uv_start.x,	face.uv_start.y);
					quad_uv[1] = vector2(face.uv_start.x,	face.uv_end.y);
					quad_uv[2] = vector2(face.uv_end.x,		face.uv_end.y);
					quad_uv[3] = vector2(face.uv_end.x,		face.uv_start.y);

					// 拷贝到vertex buffer，创建两个三角形
					*pos_ptr = quad_pos[0];	pos_ptr++;
					*pos_ptr = quad_pos[1];	pos_ptr++;
					*pos_ptr = quad_pos[3];	pos_ptr++;

					*pos_ptr = quad_pos[3];	pos_ptr++;
					*pos_ptr = quad_pos[1];	pos_ptr++;
					*pos_ptr = quad_pos[2];	pos_ptr++;

					for (int j=0; j<6; j++)
						diffuse_ptr[j] = face.color;
					diffuse_ptr += 6;

					*uv_ptr = quad_uv[0];	uv_ptr++;
					*uv_ptr = quad_uv[1];	uv_ptr++;
					*uv_ptr = quad_uv[3];	uv_ptr++;

					*uv_ptr = quad_uv[3];	uv_ptr++;
					*uv_ptr = quad_uv[1];	uv_ptr++;
					*uv_ptr = quad_uv[2];	uv_ptr++;

				}// end of for				
			}
			break;
		case ESFX_NormalFace:			
			{
				for (size_t i=0; i<num_faces; i++)
				{
					const quad_face& face = faces[i];
					mat_set_rotation_yaw_pitch_roll(mat_srot, face.rot.y, face.rot.x, face.rot.z);
					mat_set_translation(mat_trans, face.pos);
					mat_set_rotation_yaw_pitch_roll(mat_rot, face.revolution.y, face.revolution.x, face.revolution.z);

					//此处测试world_mat对面片的影响，原文：mat_trans =mat_srot*mat_trans*mat_rot;
					mat_trans =mat_srot*mat_trans*mat_rot*world_mat;

					// 假定原始Quad是在XY面,计算出经过自转+平移的Quad
					quad_pos[0] = vector3(-face.half_size.x,	face.half_size.y,	0)*mat_trans;
					quad_pos[1] = vector3(-face.half_size.x,	-face.half_size.y,	0)*mat_trans;
					quad_pos[2] = vector3(face.half_size.x,		-face.half_size.y,	0)*mat_trans;
					quad_pos[3] = vector3(face.half_size.x,		face.half_size.y,	0)*mat_trans;

					// 计算uv
					quad_uv[0] = vector2(face.uv_start.x,	face.uv_start.y);
					quad_uv[1] = vector2(face.uv_start.x,	face.uv_end.y);
					quad_uv[2] = vector2(face.uv_end.x,		face.uv_end.y);
					quad_uv[3] = vector2(face.uv_end.x,		face.uv_start.y);

					// 拷贝到vertex buffer，创建两个三角形
					*pos_ptr = quad_pos[0];	pos_ptr++;
					*pos_ptr = quad_pos[1];	pos_ptr++;
					*pos_ptr = quad_pos[3];	pos_ptr++;

					*pos_ptr = quad_pos[3];	pos_ptr++;
					*pos_ptr = quad_pos[1];	pos_ptr++;
					*pos_ptr = quad_pos[2];	pos_ptr++;

					for (int j=0; j<6; j++)
						diffuse_ptr[j] = face.color;
					diffuse_ptr += 6;

					*uv_ptr = quad_uv[0];	uv_ptr++;
					*uv_ptr = quad_uv[1];	uv_ptr++;
					*uv_ptr = quad_uv[3];	uv_ptr++;

					*uv_ptr = quad_uv[3];	uv_ptr++;
					*uv_ptr = quad_uv[1];	uv_ptr++;
					*uv_ptr = quad_uv[2];	uv_ptr++;
				}
			}
			break;
		default:
			nASSERT(0 && _T("unknown face type"));
		}

	}
}