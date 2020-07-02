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
#include "../renderer/nrender_simple_mesh.h"
#include "../renderer/nrender_element.h"


namespace nexus
{
	/**
	 *	¾²Ì¬Ä£ÐÍcomponent
	*/
	class nAPI nstatic_mesh_component :
		public nmesh_component
	{
	public:
		typedef shared_ptr<nstatic_mesh_component> ptr;

		nstatic_mesh_component(const nstring& name_str);
		virtual ~nstatic_mesh_component(void);
		
		void reset_resource(nresource_static_mesh::ptr mesh_ptr);
		void reset_resource(const resource_location& static_mesh_loc);

		void draw_simple_mesh(nrender_primitive_draw_interface* PDI, const color4f& color);

		virtual void _update_transform(const object_space& parent_space);
		virtual void _destroy();
		//--
		virtual nrender_mesh* get_render_mesh(int lod);
		virtual nmaterial_base* get_material(int lod, int mtl_id);

		virtual void serialize(narchive& ar);
	private:
		nresource_static_mesh::ptr	m_static_mesh;

		nDECLARE_NAMED_CLASS(nstatic_mesh_component)
	};
}//namespace nexus
#endif //_NSTATIC_MESH_COMPONENT_H_