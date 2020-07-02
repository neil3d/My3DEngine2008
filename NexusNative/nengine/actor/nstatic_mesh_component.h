/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NSTATIC_MESH_COMPONENT_H_
#define _NSTATIC_MESH_COMPONENT_H_
#include "nmesh_component.h"
#include "../resource/nresource_static_mesh.h"
#include "../resource/nresource_io.h"
#include "../renderer/nrender_simple_mesh.h"
#include "../renderer/nrender_element.h"


namespace nexus
{
	/**
	 *	静态模型component
	*/
	class nAPI nstatic_mesh_component :
		public nmesh_component
	{
	public:
		typedef shared_ptr<nstatic_mesh_component> ptr;

		nstatic_mesh_component(const nstring& name_str);
		virtual ~nstatic_mesh_component(void);
		
		/**
		 *	是否自动克隆资源对象的材质，使得自己独立拥有材质
		 *	@remarks 考虑遮挡视线的物体变为半透明的情况，则不能改变共享的资源材质，而必须独立拥有材质
		*/
		void enable_material_clone(bool enable);

		//!	绑定资源
		void reset_resource(nresource_static_mesh::ptr mesh_ptr);

		//!	加载资源，并绑定之
		void reset_resource(const resource_location& static_mesh_loc,enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		void draw_simple_mesh(nrender_primitive_draw_interface* PDI, const color4f& color);

		virtual void _update_transform(const object_space& parent_space);
		
		// implement physics interface
		virtual void _init_cmp_phys();
		virtual void _simulate_cmp_phys(){};
		//virtual void _release_cmp_phys(){};

		virtual void _destroy();
		//--
		virtual nmtl_base* get_material(int lod, int mtl_id);

		virtual void serialize(narchive& ar);
		virtual void on_event(nevent_base& evt);

		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);

	protected:
		virtual void on_resource_ready();
	private:
		bool			m_clone_mtl;
		material_map	m_res_mtl_clone;

		nresource_static_mesh::ptr	m_static_mesh;

		nDECLARE_NAMED_CLASS(nstatic_mesh_component)
	};

	class nAPI nstatic_editor_mesh_component :
		public nstatic_mesh_component
	{
	public:
		nstatic_editor_mesh_component(const nstring& name_str);
		virtual ~nstatic_editor_mesh_component(void);
		virtual void _update_transform(const object_space& parent_space)
		{
			matrix44 mat;
			mat_set_translation(mat,parent_space.location);
			m_mat_world = m_space.local_to_world*mat;
		}
		virtual const vector<mesh_element>& get_mesh_element(int lod);
		virtual bool enable_instance() const { return m_enable_instance; }

		vector<mesh_element> m_empty_meshs;
		nDECLARE_NAMED_CLASS(nstatic_editor_mesh_component)
	};
}//namespace nexus
#endif //_NSTATIC_MESH_COMPONENT_H_