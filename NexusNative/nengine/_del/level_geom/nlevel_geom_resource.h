/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_LEVEL_GEMO_RESOURCE_H_
#define _NEXUS_LEVEL_GEMO_RESOURCE_H_
#include "../framework/nlevel_geom.h"
#include "../resource/nresource_static_mesh.h"
#include "../renderer/nrender_proxy.h"

namespace nexus
{
	/**
	 *	引用一个nresource_static_mesh对象的静态场景对象
	*/
	class nAPI nlevel_geom_static_mesh :
		public nlevel_geom
	{
	public:
		nlevel_geom_static_mesh(const nstring& name_str);
		virtual ~nlevel_geom_static_mesh(void);

		void create(nresource_ptr<nresource_static_mesh> res_ptr);
		virtual void render();

		virtual const box_sphere_bounds& get_bounds() const
		{
			// todo: transform by world matrix
			return m_res_ptr->get_bounds();
		}
	protected:
		nresource_ptr<nresource_static_mesh>	m_res_ptr;
		
		struct resource_proxy : public nrender_proxy
		{
			nlevel_geom_static_mesh* m_owner;

			resource_proxy():m_owner(NULL)	{}
			void init(nlevel_geom_static_mesh* owner)
			{
				m_owner = owner;
			}

			virtual const matrix44& get_world_matrix() const			{	return m_owner->m_mat_world;}
			virtual int get_render_lod() const							{	return 0;}
			virtual enum EDepthGroup get_depth_group()	const		{	return EDG_World;}			
			virtual nrender_mesh* get_render_mesh(int lod) const
			{
				return m_owner->m_res_ptr->get_render_mesh(lod);
			}
			virtual nmaterial* get_material(int lod, int mtl_id) const
			{
				return m_owner->m_res_ptr->get_material(lod, mtl_id);
			}
			virtual transparent_flag get_transparent_flag() const
			{
				return m_owner->m_res_ptr->get_transparent_flag();
			}
			virtual const box_sphere_bounds& get_bounds() const
			{
				return m_owner->get_bounds();
			}

		};

		resource_proxy m_render_proxy;

		nDECLARE_NAMED_CLASS(nlevel_geom_static_mesh)
	};
}//namespace nexus
#endif //_NEXUS_LEVEL_GEMO_RESOURCE_H_