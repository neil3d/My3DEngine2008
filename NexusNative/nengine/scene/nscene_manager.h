/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NSCENE_MANAGER_H_
#define _NSCENE_MANAGER_H_
#include "ncore.h"
#include "scene_event.h"
#include "ngeneric_octree.h"
#include "../renderer/nrender_element.h"

namespace nexus
{
	/**
	 *	场景管理的基类
	 *	@remark 提供空间分割的算法接口
	*/
	class nscene_manager :
		public nevent_target
	{
	public:
		typedef shared_ptr<nscene_manager> ptr;
		typedef noctree<nprimitive_component,nprimitive_component::octree_semantics>  prim_octree_type;	

		nscene_manager(void);
		virtual ~nscene_manager(void);

		virtual void clear_all();

		virtual void on_event(nevent_base& evt);

		// draw debug information
		virtual void draw( nrender_primitive_draw_interface* PDI );
		virtual unsigned int get_prim_count() { return m_prim_count; }
		virtual unsigned int get_light_count() { return m_light_count; }

		virtual void add_primitive(nprimitive_component* prim);
		virtual void remove_primitive(nprimitive_component* prim);
		virtual void move_primitive(nprimitive_component* prim);

		virtual void add_light(nlight_component* lgt);
		virtual void remove_light(nlight_component* lgt);
		virtual void move_light(nlight_component* lgt);
		virtual void get_visible_primitive(const nfrustum& view_frustum,class render_package_base* package);
		virtual void get_visible_light(const nfrustum& view_frustum,vector<nlight_component*>& out_lgts);

		bool													m_bshow_prim_octree;
		prim_octree_type*								m_prim_octrant;
	private:
		std::set<nlight_component*>			m_lgt_set;	
		
		unsigned int										m_prim_count;
		unsigned int										m_light_count;

		nDECLARE_CLASS(nscene_manager)
	};
}//namespace nexus
#endif //_NSCENE_MANAGER_H_