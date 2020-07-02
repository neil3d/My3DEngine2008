/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NPRIMITIVE_COMPONENT_H_
#define _NPRIMITIVE_COMPONENT_H_
#include "../framework/nactor.h"
#include "nactor_component.h"
#include "../renderer/nrender_proxy.h"

#include "../scene/ngeneric_octree.h"

namespace nexus
{
	/**
	 *	各种可见图元的基类
	*/
	class nAPI nprimitive_component :
		public nactor_component
	{
	public:
		typedef shared_ptr<nprimitive_component> ptr;

		nprimitive_component(const nstring& name_str);
		virtual ~nprimitive_component(void);

		virtual void _init(nactor* owner);
		virtual void _on_actor_move();
		virtual void _remove_from_level();

		virtual const matrix44& get_world_matrix() const;		
		virtual enum EDepthGroup get_depth_group() const		{	return EDG_World;}		
		virtual const box_sphere_bounds& get_bounds() const			{	return m_bounds;}
		virtual const AABBox get_AABBox() const					{	return m_bounds.get_AABBox();}		
	
	protected:		
		matrix44			m_mat_local;
		matrix44			m_mat_world;
		box_sphere_bounds	m_bounds;
		
	public:
		/** octree semantic definitions. */
		struct octree_semantics
		{
			enum { MIN_NODE_SIZE = 512 };

			static const AABBox get_bounding_box(const nprimitive_component* prim)
			{
				return prim->get_bounds().get_AABBox();
			}
		};

		typedef noctree_node<nprimitive_component, nprimitive_component::octree_semantics> prim_octreenode_type;
		void set_octreenode( prim_octreenode_type* _node ){ m_octreenode = _node; }
		prim_octreenode_type* get_octreenode() { return m_octreenode; }
	private:
		prim_octreenode_type*		m_octreenode;

	protected:
		nDECLARE_VIRTUAL_CLASS(nprimitive_component)
	};
}//namespace nexus
#endif //_NPRIMITIVE_COMPONENT_H_