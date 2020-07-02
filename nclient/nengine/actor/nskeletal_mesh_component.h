#ifndef _NEXUS_SKELETAL_MESH_COMPONENT_H_
#define _NEXUS_SKELETAL_MESH_COMPONENT_H_
#include "nmesh_component.h"
#include "../resource/nresource_skeletal_mesh.h"
#include "../renderer/ncpu_skin_mesh.h"
#include "../renderer/nrender_element.h"
#include "../animation/nskel_anim_controller.h"

namespace nexus
{
	class nAPI nskeletal_mesh_component :
		public nmesh_component
	{
	public:
		typedef shared_ptr<nskeletal_mesh_component> ptr;

		nskeletal_mesh_component(const nstring& name_str);
		virtual ~nskeletal_mesh_component(void);

		void reset_resource(nresource_skeletal_mesh::ptr res_mesh, bool cpu_skin);
		nskel_anim_controller::ptr reset_anim_controller(const nstring& anim_ctrl_class_name);

		void draw_skeleton(nrender_primitive_draw_interface* PDI);

		virtual void update(float delta_time, const view_info& view);
		virtual void render(const view_info& view);

		virtual nrender_mesh* get_render_mesh(int lod);
		virtual nmaterial_base* get_material(int lod, int mtl_id);

	protected:
		nresource_skeletal_mesh::ptr	m_mesh_res;
		nskel_anim_controller::ptr		m_anim_ctrl;
		
		bool							m_cpu_skin;		
		
		render_res_array<nrender_cpu_skin_mesh>	m_cpu_skin_meshes;

		nDECLARE_NAMED_CLASS(nskeletal_mesh_component)
	};
}//namespace nexus

#endif //_NEXUS_SKELETAL_MESH_COMPONENT_H_