/**
*	Nexus Engine - Animation Mesh data define
*
*	Author: Neil
*	Date:	Dec, 2008
*/

#ifndef _NEXUS_ANIM_MESH_H_
#define _NEXUS_ANIM_MESH_H_
#include "ncore.h"
#include "mesh_index_data.h"
#include "mesh_vertex_data_common.h"
#include "../material/nmaterial.h"
#include "math/geom_def.h"
#include "mesh_lod.h"

namespace nexus
{
	struct nanim_mesh_track
	{
		vector<vector3>				m_pos_keys;
		vector<vector3>				m_normal_keys;
		vector<vector3>				m_tangent_keys;
		vector<vector2>				m_uv_keys;
	};

	template<>
	inline narchive& nserialize(narchive& ar, nanim_mesh_track& obj, const TCHAR* obj_name)
	{
		nstring class_name(_T("nanim_mesh_track"));
		ar.object_begin(obj_name, class_name);
		
		nserialize(ar, obj.m_pos_keys, _T("pos_keys"));
		nserialize(ar, obj.m_normal_keys, _T("normal_keys"));
		nserialize(ar, obj.m_tangent_keys, _T("tangent_keys"));
		nserialize(ar, obj.m_uv_keys, _T("uv_keys"));

		ar.object_end();

		return ar;
	}

	/**
	 *	顶点插值动画序列
	*/
	struct nAPI nanim_mesh_sequence
		: public nobject
	{
		typedef shared_ptr<nanim_mesh_sequence> ptr;

		nstring						m_name;

		//-- size of track vector == number of vertices
		vector<nanim_mesh_track>	m_tracks;
		
		vector<float>				m_frame_time;
		vector<box_sphere_bounds>	m_frame_bounds;

		float get_length() const
		{
			float len = 0;
			for(size_t i=0; i<m_frame_time.size(); i++)
				len += m_frame_time[i];
			return len;
		}

		bool has_pos_keys() const
		{
			return m_tracks.size() > 0
				&& m_tracks[0].m_pos_keys.size() >1;
		}

		bool has_normal_keys() const
		{
			return m_tracks.size() > 0
				&& m_tracks[0].m_normal_keys.size() >1;
		}

		bool has_tangent_keys() const
		{
			return m_tracks.size() > 0
				&& m_tracks[0].m_uv_keys.size() >1;
		}

		bool has_uv_keys() const
		{
			return m_tracks.size() > 0
				&& m_tracks[0].m_tangent_keys.size() >1;
		}

		size_t get_num_vert() const
		{
			return m_tracks.size();
		}

		size_t get_num_frame() const
		{
			return m_frame_time.size();
		}
		float get_frame_time(size_t frm) const
		{
			return m_frame_time[frm];
		}

		virtual void serialize(narchive& ar);
		nDECLARE_CLASS(nanim_mesh_sequence)
	};

	/**
	 *	顶点插值动画mesh
	*/
	struct nAPI nanim_mesh
		: public nobject
	{
		typedef shared_ptr<nanim_mesh> ptr;

		vector<nmesh_section::ptr>	m_secton_array;
		vector<nmaterial_base::ptr>	m_mtl_array;
		
		vector<nanim_mesh_sequence::ptr> m_sequence_array;

		size_t get_face_count() const
		{
			size_t num_indices = 0;
			for(size_t i=0; i<m_secton_array.size(); i++)
				num_indices += m_secton_array[i]->m_index_buffer.get_index_count();

			return num_indices/3;
		}
		
		virtual void serialize(narchive& ar);
		nDECLARE_CLASS(nanim_mesh)
	};
}//namespace nexus
#endif //_NEXUS_ANIM_MESH_H_