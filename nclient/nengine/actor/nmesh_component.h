/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NMESH_COMPONENT_H
#define _NMESH_COMPONENT_H
#include "nprimitive_component.h"
#include "../material/nmaterial.h"

namespace nexus
{
	/**
	 * mesh组件
	*/
	class nAPI nmesh_component :
		public nprimitive_component
	{
	public:
		nmesh_component(const nstring& name_str);
		virtual ~nmesh_component(void);

		virtual void render(const view_info& view);

		virtual nmaterial_base* get_material(int lod, int mtl_id);
		virtual transparent_flag get_transparent_flag() const;

		/**
		 *	可以指定另外的material，来替代mesh resource自带的材质
		*/
		void replace_material(int lod, int mtl_id, nmaterial_base::ptr mtl_ptr);

		virtual int get_render_lod() const							{	return 0;}
		virtual nrender_mesh* get_render_mesh(int lod)  = 0;		
	private:
		typedef std::map<std::pair<int, int>, nmaterial_base::ptr> material_map;
		material_map		m_replace_mtl;
		transparent_flag	m_trans_flag;

		// a bridge, 避免使用多继承
		struct mesh_proxy : public nrender_proxy
		{
			nmesh_component* m_owner;
			mesh_proxy():m_owner(NULL)	{}
			void init(nmesh_component* owner)
			{
				m_owner = owner;
			}

			virtual const matrix44& get_world_matrix() const	{	return m_owner->get_world_matrix();}
			virtual int get_render_lod() const					{	return m_owner->get_render_lod(); }
			virtual enum EDepthGroup get_depth_group() const		{	return m_owner->get_depth_group();}
			virtual nrender_mesh* get_render_mesh(int lod) const		{	return m_owner->get_render_mesh(lod);}
			virtual nmaterial_base* get_material(int lod, int mtl_id) const	{	return m_owner->get_material(lod, mtl_id);}
			virtual transparent_flag get_transparent_flag() const		{	return m_owner->get_transparent_flag();}
			virtual const box_sphere_bounds& get_bounds() const			{	return m_owner->get_bounds(); }
		};

		mesh_proxy		m_render_proxy;

		nDECLARE_VIRTUAL_CLASS(nmesh_component)
	};
}//namespace nexus
#endif //_NMESH_COMPONENT_H