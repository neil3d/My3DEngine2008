#ifndef _NEXUS_D3D9_PDI_H_
#define _NEXUS_D3D9_PDI_H_
#include "d3d_ptr.h"
#include "../nengine/framework/view_info.h"

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

		void init(nview_info* view_ptr);
		void destroy()
		{
			m_line_effect.reset();
			m_font.reset();
			m_color_vert_decl.reset();
		}

		virtual void set_transform(const matrix44& matrix);
		virtual void begin_line_batch();
		virtual void draw_line(const vector3& start, const vector3& end, const color4f& color);
		virtual void draw_wired_box(const AABBox& box, const color4f& color);
		virtual void draw_wired_sphere(const vector3& pos,float radius, const color4f& color);
		virtual void draw_wired_cone(float bottom_radius, float length, const matrix44& transform, const color4f& color);
		virtual void end_line_batch();

		virtual void draw_line_strip(const std::vector<vector3>& pos_array, const color4f& color);

		virtual void draw_simple_mesh(const nrender_simple_mesh* mesh, const color4f& color);
		virtual void draw_debug_string(size_t x, size_t y, const nstring& text, const color4ub& color);

		virtual void draw_mesh_component(const nmesh_component* comp, bool hit_proxy, bool disable_ztest);

	private:
		void on_device_lost(int param);
		bool on_device_reset(int param);

	private:
		void draw_line_shared(D3DPRIMITIVETYPE prim_type);

	private:
		nview_info*		m_view_info;
		std::vector<color_vertex>	m_line_vb;
		bool						m_in_line_batch;
		d3d_ptr<ID3DXEffect>		m_line_effect;
		d3d_ptr<ID3DXFont>			m_font;
		
		d3d_ptr<IDirect3DVertexDeclaration9>	m_color_vert_decl;

		//-----------------------------------------------------------------------------------
		std::vector<vector3>		m_sphere_vertex[3];
		matrix44					m_matrix;
	};
}//namespace nexus

#endif //_NEXUS_D3D9_PDI_H_