#ifndef _NEXUS_D3D9_VERTEX_FACTORY_STATIC_H_
#define _NEXUS_D3D9_VERTEX_FACTORY_STATIC_H_
#include "d3d9_vertex_factory.h"

namespace nexus
{
	class d3d9_vertex_factory_common :
		public d3d9_vertex_factory
	{
	public:
		typedef shared_ptr<d3d9_vertex_factory_common> ptr;

		d3d9_vertex_factory_common(void);
		virtual ~d3d9_vertex_factory_common(void);

		virtual void draw_set_pos_only();		

		void create(const nmesh_vertex_data* mesh_vb, D3DPOOL pool);

		void update(const nmesh_vertex_data* mesh_vb);

	private:
		bool	m_support_pos_only;
	};
}//namespace nexus

#endif //_NEXUS_D3D9_VERTEX_FACTORY_STATIC_H_