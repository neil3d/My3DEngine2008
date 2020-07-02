#ifndef _NEXUS_D3D9_VERTEX_FACTORY_DYNAMIC_H_
#define _NEXUS_D3D9_VERTEX_FACTORY_DYNAMIC_H_
#include "d3d9_vertex_factory.h"

namespace nexus
{
	class d3d9_vertex_factory_dynamic :
		public d3d9_vertex_factory
	{
	public:
		typedef shared_ptr<d3d9_vertex_factory_dynamic> ptr;

		d3d9_vertex_factory_dynamic(void);
		virtual ~d3d9_vertex_factory_dynamic(void);

		void create(const nmesh_vertex_data* mesh_vb);
		virtual void draw_set_pos_only();

		void prepare_frame_data(nmesh_vertex_data* mesh_vb);
		void release_frame_data();
	};
}//namespace nexus

#endif //_NEXUS_D3D9_VERTEX_FACTORY_DYNAMIC_H_