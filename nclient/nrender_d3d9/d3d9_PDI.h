#ifndef _NEXUS_D3D9_PDI_H_
#define _NEXUS_D3D9_PDI_H_
#include "d3d_ptr.h"
#include "d3d_view_info.h"

namespace nexus
{
	class d3d9_PDI
		: public nrender_primitive_draw_interface
	{
		struct color_vertex
		{
			vector3	pos;
			color4f	color;
		};

	public:
		d3d9_PDI(void);
		virtual ~d3d9_PDI(void);

		void init(d3d_view_info* view_ptr);

		virtual void begin_line_batch();
		virtual void draw_line(const vector3& start, const vector3& end, const color4f& color);
		virtual void end_line_batch();

		virtual void draw_line_strip(const std::vector<vector3>& pos_array, const color4f& color);

		virtual void draw_simple_mesh(const nrender_simple_mesh* mesh, const color4f& color);
		virtual void draw_debug_string(size_t x, size_t y, const nstring& text, const color4ub& color);

	private:
		void draw_line_shared(D3DPRIMITIVETYPE prim_type);

	private:
		
		struct d3d_view_info*		m_view_info;
		std::vector<color_vertex>	m_line_vb;
		bool						m_in_line_batch;
		d3d_ptr<ID3DXEffect>		m_line_effect;
		d3d_ptr<ID3DXFont>			m_font;
		
		d3d_ptr<IDirect3DVertexDeclaration9>	m_color_vert_decl;
	};
}//namespace nexus

#endif //_NEXUS_D3D9_PDI_H_