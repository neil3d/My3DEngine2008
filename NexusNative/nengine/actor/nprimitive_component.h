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
		virtual void _update_transform(const object_space& parent_space);
		virtual void _remove_from_level();

		virtual const matrix44& get_world_matrix() const;		
		virtual enum EDepthGroup get_depth_group() const		{	return EDG_World;}		
		virtual const box_sphere_bounds& get_bounds() const			{	return m_bounds;}
		virtual const AABBox get_AABBox() const					{	return m_bounds.get_AABBox();}				
	
		bool accept_dynamic_light() const	{	return m_accept_dynamic_light;	}
		void set_accept_dynamic_light(bool val)	{	m_accept_dynamic_light=val;	}
		bool cast_dynamic_shadow() const {	return m_cast_dynamic_shadow;}
		void set_cast_dynamic_shadow(bool val) {	m_cast_dynamic_shadow=val;}
		
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nactor* owner);

		virtual void draw_bounds(nrender_primitive_draw_interface* PDI, const color4f& color);

		virtual const nrender_proxy* get_render_proxy() const
		{
			return NULL;
		}

		const object_space& get_space() const	{	return m_space; }
		void move(const vector3& pos, const vector3& rot, const vector3& scale);
	protected:		
		object_space		m_space;		// local space
		matrix44			m_mat_world;	// local to parent space to world space
		box_sphere_bounds	m_bounds;

		bool	m_accept_dynamic_light;
		bool	m_cast_dynamic_shadow;
		struct primitive_proxy : public nrender_proxy
		{
			nprimitive_component* m_comp;

			primitive_proxy():m_comp(NULL)
			{}
			void init(nprimitive_component* comp)
			{
				m_comp = comp;
			}

			virtual const matrix44& get_world_matrix() const	{	return m_comp->get_world_matrix();}
			virtual enum EDepthGroup get_depth_group() const		{	return m_comp->get_depth_group();}
			virtual const box_sphere_bounds& get_bounds() const		{	return m_comp->get_bounds(); }
			virtual bool accept_dynamic_light() const			{ return m_comp->accept_dynamic_light();	}
			virtual bool cast_dynamic_shadow() const	{	return m_comp->cast_dynamic_shadow();}
			virtual hit_id get_hit_proxy_id() const		
			{
				nactor* owner = m_comp->get_owner();
				if( owner )
					return owner->get_name().name_crc;
				else
					return m_comp->get_name().name_crc;
			}
		};
		
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