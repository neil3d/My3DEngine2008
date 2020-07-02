/**
 *	Nexus Engine
 *
 *	Author: Neil
 *	Date:	Aug, 2008
*/

#ifndef _NOCTREE_SCENE_H_
#define _NOCTREE_SCENE_H_
#include "nscene_manager.h"

namespace nexus
{
	class ncamera;
	/**
	 *	八叉树的场景管理
	*/
	class noctree_scene :
		public nscene_manager
	{
	public:
		// type definitions
		typedef noctree<nprimitive_component,nprimitive_component::octree_semantics>  prim_octree_type;	

		noctree_scene(void);
		virtual ~noctree_scene(void);
	
		virtual void clear_all();
		virtual void render_visible(const nviewport& view);
		virtual void draw( nrender_primitive_draw_interface* PDI );

	protected:
		virtual void add_primitive(nprimitive_component* prim);
		virtual void remove_primitive(nprimitive_component* prim);
		virtual void move_primitive(nprimitive_component* prim);

		virtual void add_light(nlight_component* lgt);
		virtual void remove_light(nlight_component* lgt);
		virtual void move_light(nlight_component* lgt);

	private:
		void render_octreenode( prim_octree_type::octreenode_ptr pNode, const nviewport& view);

	private:
		bool								m_bshow_prim_octree;
		prim_octree_type*					m_prim_octrant;
		std::set<nlight_component*>			m_light_set;

		nfrustum							m_frustum;

		nDECLARE_CLASS(noctree_scene)
	};
}//namespace nexus
#endif //_NOCTREE_SCENE_H_