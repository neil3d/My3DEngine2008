/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_SHAPE_COMPONENT_H_
#define _NEXUS_SHAPE_COMPONENT_H_
#include "nmesh_component.h"
#include "../material/nmaterial.h"

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

		void set_material(nmaterial_base::ptr mtl_ptr);
		
		virtual void _destroy();
		virtual nrender_mesh* get_render_mesh(int lod);
		virtual nmaterial_base* get_material(int lod, int mtl_id);		
		
		virtual void serialize(narchive& ar);
	private:
		render_res_ptr<nrender_mesh>	m_mesh_ptr;
		nmaterial_base::ptr				m_mtl_ptr;			

		//-- 保存创建参数，供序列化用
		int				m_type;	//0 box, 1 sphere
		vector3			m_box_param;
		float			m_sphere_radius;
		unsigned short	m_sphere_rings;
		unsigned short	m_sphere_sections;


		nDECLARE_NAMED_CLASS(nshape_component)
	};
}//namespace nexus
#endif //_NEXUS_SHAPE_COMPONENT_H_