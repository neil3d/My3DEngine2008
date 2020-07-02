/**
*	Nexus Engine - Animation Mesh Component
*
*	Author: Neil
*	Date:	Dec, 2008
*/

#ifndef _NEXUS_ANIM_MESH_COMPONENT_H_
#define _NEXUS_ANIM_MESH_COMPONENT_H_
#include "nmesh_component.h"
#include "../resource/nresource_anim_mesh.h"
#include "../animation/nmesh_anim_controller.h"
#include "../renderer/nrender_anim_mesh.h"

namespace nexus
{
	/**
	 *	顶点插值动画组件
	 *	@remark 管理resource，render mesh和anim controller
	*/
	class nAPI nanim_mesh_component :
		public nmesh_component
	{
	public:
		typedef shared_ptr<nanim_mesh_component> ptr;

		nanim_mesh_component(const nstring& name_str);
		virtual ~nanim_mesh_component(void);

		void reset_resource(nresource_anim_mesh::ptr res_ptr);
		void reset_resource(const resource_location& loc);
		void play_anim(const nstring& seq_name, bool loop, float start_time = 0);

		virtual void _destroy();
		virtual void update(float delta_time, const nviewport& view);
		virtual void render(const nviewport& view);
		virtual void _update_transform(const object_space& parent_space);

		virtual nrender_mesh* get_render_mesh(int lod);
		virtual nmaterial_base* get_material(int lod, int mtl_id);

		virtual void serialize(narchive& ar);
	private:
		void create_dynamic_data();
	private:
		nresource_anim_mesh::ptr					m_mesh_res;
		render_res_ptr<nrender_anim_mesh>			m_render_mesh;
		nmesh_anim_controller::ptr					m_anim_ctrl;

		nDECLARE_NAMED_CLASS(nanim_mesh_component)
	};
}//namespace nexus

#endif //_NEXUS_ANIM_MESH_COMPONENT_H_