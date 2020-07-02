/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NMESH_COMPONENT_H
#define _NMESH_COMPONENT_H
#include "nprimitive_component.h"
#include "../renderer/nrender_mesh.h"
#include "../material/nmtl_base.h"

namespace nexus
{
	struct mesh_element;
	/**
	 * mesh组件
	*/
	class nAPI nmesh_component :
		public nprimitive_component
	{
	public:
		typedef shared_ptr<nmesh_component> ptr;

		nmesh_component(const nstring& name_str);
		virtual ~nmesh_component(void);

		virtual void _destroy();
		virtual void render(class render_package_base* rpb);
	
		virtual nmtl_base* get_material(int lod, int mtl_id);		
		/**
		 *	可以指定另外的material，来替代mesh resource自带的材质
		*/
		void replace_material(int lod, int mtl_id, nmtl_base::ptr mtl_ptr);

		virtual int get_render_lod() const							{	return 0;}
		virtual const vector<mesh_element>& get_mesh_element(int lod)  { return m_mesh_elements; }
		virtual void set_enable_instance(bool is_enable ) { m_enable_instance = is_enable; }
		virtual bool enable_instance() const { return false; }
		virtual void serialize(narchive& ar);

	protected:
		virtual void on_resource_ready() {};

		typedef std::map<std::pair<int, int>, nmtl_base::ptr> material_map;
		bool						m_enable_instance;
		vector<mesh_element> m_mesh_elements;

	private:		
		material_map		m_replace_mtl;
		
		nDECLARE_VIRTUAL_CLASS(nmesh_component)
	};

	struct  mesh_element
	{
		nrender_mesh*	mesh;
		matrix44		matrix;
		nrender_mesh_section* sec;
		nmtl_base* mtl;
		nprimitive_component* comp;
		bool instance;

		mesh_element(
			nprimitive_component* in_comp,
			nrender_mesh*	in_mesh,
			nrender_mesh_section* in_mesh_section,
			nmtl_base* in_mtl,
			bool binstance = false
			):
		comp(in_comp),
			matrix(matrix44::identity),
			mesh(in_mesh),
			sec(in_mesh_section),
			mtl(in_mtl),
			instance(binstance)
		{};

		//static mesh deco object 专用构造函数
		mesh_element(
			const matrix44& _matrix,
			nrender_mesh*	in_mesh,
			nrender_mesh_section* in_mesh_section,
			nmtl_base* in_mtl,
			bool binstance = false
			):
		comp(NULL),
			matrix(_matrix),
			mesh(in_mesh),
			sec(in_mesh_section),
			mtl(in_mtl),
			instance(binstance)
		{};
	};
}//namespace nexus
#endif //_NMESH_COMPONENT_H