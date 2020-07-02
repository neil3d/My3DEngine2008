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
#include "../scene/ngeneric_octree.h"
#include "../physics/nphys_pch.h"
#include "../physics/nphys_body_instance.h"

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
		virtual void update_transform(const object_space& parent_space);
		virtual void _update_transform(const object_space& parent_space);
		virtual void _remove_from_level();

		virtual const matrix44& get_world_matrix() const;		
		virtual enum EDepthGroup get_depth_group() const		{	return m_owner->get_depth_group();}		
		virtual const box_sphere_bounds& get_bounds() const			{	return m_bounds;}
		virtual const AABBox get_AABBox() const					{	return m_bounds.get_AABBox();}	

		virtual bool accept_dynamic_light() const	{	return m_accept_dynamic_light;	}
		virtual void set_accept_dynamic_light(bool val)	{	m_accept_dynamic_light=val;	}
		virtual bool cast_dynamic_shadow() const {	return m_cast_dynamic_shadow;}
		virtual void set_cast_dynamic_shadow(bool val) {	m_cast_dynamic_shadow=val;}
		
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nactor* owner);

		virtual struct nspt_instance* get_spt_instance()
		{
			return NULL;
		}

		// 有场景管理器调用，给primitive_component一次裁剪机会
		virtual bool _is_cull(const nviewport& view);

		virtual void draw_bounds(nrender_primitive_draw_interface* PDI, const color4f& color);
		
		virtual void render(class render_package_base* rnd);

		const object_space& get_space() const	{	return m_space; }
		void move(const vector3& pos, const vector3& rot, const vector3& scale);

		// 设置裁剪距离
		float get_cull_dist() const { return m_cull_dist; }
		void set_cull_dist(float value) { m_cull_dist = value; }

		// 显示/隐藏
		bool get_visible() const { return m_owner? (m_owner->get_visible()&&m_visibale) : m_visibale; }
		void set_visible(bool value) { m_visibale = value; }

		hit_id get_hit_proxy_id() const
		{
			if(m_owner)
				return m_owner->get_name().name_crc;
			else
				return m_name.name_crc;
		}

	protected:
		bool is_resource_ready_event(const nevent_base& evt, const nname& res_name);
	protected:		
		object_space		m_space;		// local space
		matrix44			m_mat_world;	// local to parent space to world space
		box_sphere_bounds	m_bounds;
		// 裁剪距离
		float				m_cull_dist;
		// 显示/隐藏
		bool				m_visibale;

		bool	m_accept_dynamic_light;
		bool	m_cast_dynamic_shadow;
		
		nphys_body_instance::ptr		m_phys_body_instance;

	public:
		// rigid body interface
		virtual void add_impulse( const vector3& impluse, const vector3& position, bool change_vel=false );
		virtual void add_radial_impulse( const vector3& origin, float radius, float strength,bool change_vel=false );
		virtual void add_force( const vector3& force, const vector3& position );
		virtual void add_radial_force( const vector3& origin, float radius, float strength );
		virtual void set_linear_velocity( const vector3& vel, bool add2current =false );
		virtual void set_angular_velocity( const vector3& ang, bool add2current =false );
		virtual void set_nx_position( const vector3& pos );
		virtual void set_nx_rotation( const vector3& rot );
		virtual void wake();
		virtual bool is_sleep();
		virtual void put_to_sleep();
		NxActor* get_nx_actor() const;

		// collision data define
		EPhysCollisionChannel			m_collision_channel;
		PhysCollisionChannelContainer	m_collision_channel_container;
		bool							m_disable_all_rigidbody;
		
	public:
		/** octree semantic definitions. */
		struct octree_semantics
		{
			static int  MIN_NODE_SIZE;

			static const AABBox get_bounding_box(const nprimitive_component* prim)
			{
				return prim->get_AABBox();
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