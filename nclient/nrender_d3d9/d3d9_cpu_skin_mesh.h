#ifndef _NEXUS_D3D9_CPU_SKIN_MESH_H_
#define _NEXUS_D3D9_CPU_SKIN_MESH_H_
#include "d3d9_vertex_factory_dynamic.h"
#include "d3d9_mesh_section.h"

namespace nexus
{
	class d3d9_cpu_skin_mesh
		: public nrender_cpu_skin_mesh
	{
	public:
		d3d9_cpu_skin_mesh(void);
		virtual ~d3d9_cpu_skin_mesh(void);

		virtual void create(nskeletal_mesh::ptr mesh_ptr);

		virtual nshader_modifier* get_shader_mod()	{	return &s_shader_modifier; }
		virtual nvertex_factory* get_vertex_factory();
		virtual size_t get_num_section();		
		virtual nrender_mesh_section* get_section(size_t section_index);

		virtual bool pre_render();
		virtual void post_render();

	protected:
		d3d9_vertex_factory_dynamic::ptr	m_vertex_factory;
		vector<d3d9_mesh_section::ptr>		m_section_array;		

	private:
		struct nshader_modifier_cpu_skin : public nshader_modifier
		{
			shader_define	m_macro;

			nshader_modifier_cpu_skin(void)
				:m_macro("CPU_SKIN", "1")
			{}

			virtual nstring get_name() const	{	return _T("cpu_skin");}
			virtual size_t get_num_macro() const	{	return 1;}
			virtual const shader_define& get_macro(size_t i) const
			{
				return m_macro;
			}
		};
		static nshader_modifier_cpu_skin	s_shader_modifier;
	};
}//namespace nexus

#endif //_NEXUS_D3D9_CPU_SKIN_MESH_H_