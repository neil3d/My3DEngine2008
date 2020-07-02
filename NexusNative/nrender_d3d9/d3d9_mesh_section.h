/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_D3D9_INDEX_BUFFER_H_
#define _NEXUS_D3D9_INDEX_BUFFER_H_
#include "d3d_ptr.h"

namespace nexus
{
	/** material id + d3d9 index buffer
	*/
	class d3d9_mesh_section 
		: public nrender_mesh_section
	{
	public:
		typedef shared_ptr<d3d9_mesh_section> ptr;

		d3d9_mesh_section(void);
		virtual ~d3d9_mesh_section(void);

		void create_none_index(int mtl_id, enum EPrimitiveType primitive_type, size_t primitive_count);
		void create(const nmesh_section* sec_ptr, D3DPOOL pool);
		void create(const index_buffer16* index_data, int mtl_id, enum EPrimitiveType primitive_type, D3DPOOL pool);
		
		void draw_set_index_buffer();	
		void draw_call(size_t num_vert);

		void update_index(const index_buffer16* index_data, size_t valid_count);

	private:
		d3d_index_ptr		m_index_buffer;
		D3DPRIMITIVETYPE	m_d3d_pm_type;
		size_t				m_valid_index_count;		
	};
}//namespace nexus
#endif //_NEXUS_D3D9_INDEX_BUFFER_H_