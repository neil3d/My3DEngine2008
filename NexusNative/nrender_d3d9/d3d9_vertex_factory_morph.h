#ifndef _NEXUS_D3D9_VERTEX_FACTORY_MORPH_H_
#define _NEXUS_D3D9_VERTEX_FACTORY_MORPH_H_
#include "d3d9_vertex_factory.h"

namespace nexus
{
	class d3d9_vertex_factory_morph :
		public d3d9_vertex_factory
	{
	public:
		typedef shared_ptr<d3d9_vertex_factory_morph> ptr;

		d3d9_vertex_factory_morph(void);
		virtual ~d3d9_vertex_factory_morph(void);

		void create(const nmesh_vertex_data* mesh_vb);

		virtual void destroy();
		virtual void draw_set_pos_only();

		void prepare_frame_data(const struct anim_mesh_morph& anim);
		void release_frame_data();
		
	private:
		//@return pos vertex buffer
		d3d_vb_ptr create_vertex_for_frame(nmesh_vertex_data::ptr mesh_frame_vb);
	private:
		d3d_vb_ptr	m_vb_pos_a;
		d3d_vb_ptr	m_vb_pos_b;
		UINT		m_pos_stream_stride;
	};
}//namespace nexus

#endif //_NEXUS_D3D9_VERTEX_FACTORY_MORPH_H_