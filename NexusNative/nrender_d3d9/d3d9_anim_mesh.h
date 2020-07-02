#ifndef _NEXUS_D3D9_ANIM_MESH_INDEXED_H_
#define _NEXUS_D3D9_ANIM_MESH_INDEXED_H_
#include "d3d9_mesh_section.h"
#include "d3d9_vertex_factory_morph.h"

namespace nexus
{
	class d3d9_anim_mesh_indexed :
		public nrender_anim_mesh
	{
	public:
		d3d9_anim_mesh_indexed(void);
		virtual ~d3d9_anim_mesh_indexed(void);

		virtual void create(nanim_mesh* mesh_ptr, const struct anim_mesh_morph& anim);
		
		virtual nvertex_factory* get_vertex_factory()	{	return m_vertex_factory.get();}
		virtual size_t get_num_section()				{	return m_section_array.size();}
		virtual nrender_mesh_section* get_section(size_t section_index);
		virtual bool pre_render();
		virtual void post_render();
		virtual void draw_setup_effect(nshading_effect* effect_ptr);

	private:
		d3d9_vertex_factory_morph::ptr		m_vertex_factory;
		vector<d3d9_mesh_section::ptr>		m_section_array;
	};
}//namespace nexus

#endif //_NEXUS_D3D9_ANIM_MESH_INDEXED_H_