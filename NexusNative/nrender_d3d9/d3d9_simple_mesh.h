#pragma once
#include "d3d_ptr.h"
#include "../nengine/framework/view_info.h"

namespace nexus
{
	class d3d9_simple_mesh
		: public nrender_simple_mesh
	{
	public:
		d3d9_simple_mesh(void);
		virtual ~d3d9_simple_mesh(void);

		virtual void create(const nsimple_mesh* mesh);	

		void draw(const nview_info* view, const color4f& color) const;
		void draw_index_primitive(d3d_effect_ptr effect) const;

	private:
		d3d_vb_ptr		m_vert_buffer;
		d3d_index_ptr	m_index_buffer;
		unsigned int		m_num_tri;
		unsigned int		m_num_vert;
	};
}//namespace nexus