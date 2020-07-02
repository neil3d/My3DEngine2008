/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_SHAPE_COMPONENT_H_
#define _NEXUS_SHAPE_COMPONENT_H_
#include "nmesh_component.h"
#include "../material/nmtl_base.h"

namespace nexus
{
	/**
	 *	包装单个的box或者sphere等集合形状的组建
	*/
	class nAPI nshape_component 
		: public nmesh_component
	{
	public:
		typedef shared_ptr<nshape_component> ptr;

		nshape_component(const nstring& name_str);
		virtual ~nshape_component(void);

		void create_box(float width, float height, float depth);
		void create_sphere(float radius, unsigned short num_rings, unsigned short num_sections);
		static nrender_mesh* create_sphere_mesh(float radius, unsigned short num_rings, unsigned short num_sections);
		void	create_sphere_with_mesh(float radius, unsigned short num_rings, unsigned short num_sections,nrender_mesh* mesh);
		virtual void _update_transform(const object_space& parent_space);
		void set_material(nmtl_base::ptr mtl_ptr);
		
		virtual void _destroy();
		virtual nmtl_base* get_material(int lod, int mtl_id);		
		
		virtual void serialize(narchive& ar);

		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);
		void _init(nactor* owner);
	private:
		void create_render_mesh_by_data();
	private:
		nrender_mesh*					m_mesh_ptr;
		nmtl_base::ptr					m_mtl_ptr;			

		//-- 保存创建参数，供序列化用
		int									m_type;	//0 box, 1 sphere
		vector3								m_box_param;
		float									m_sphere_radius;
		unsigned short					m_sphere_rings;
		unsigned short					m_sphere_sections;


		nDECLARE_NAMED_CLASS(nshape_component)
	};
}//namespace nexus
#endif //_NEXUS_SHAPE_COMPONENT_H_