#include "StdAfx.h"
#include <boost/lexical_cast.hpp>
#include <time.h>
#include "../ncore/timer/ntimestamp.h"
#include "nactor.h"
#include "ncore.h"
#include "../scene/scene_event.h"
#include "../framework/nlevel.h"
#include "../physics/nphys_pch.h"
#include "../physics/nphys_util.h"
#include "../Actor/nmesh_component.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nactor, nevent_target)

	nactor::nactor(const nstring& name_str):m_name(name_str),m_layer_name(ACTOR_DEFAULT_LAYER)
	{
		m_owner = NULL;

		m_visible = true;		
		m_static = false;		
		m_serializable = true;
		m_frozen = false;
		m_kinematic=true;

		m_depth_group = EDG_WorldActor;
	}

	nactor::~nactor(void)
	{
	}

	nactor::ptr nactor::clone()
	{
		nstring class_name=this->reflection_get_class()->get_name();
		
		__time64_t tm;
		_time64(&tm);
		nstring name;
		for(size_t i=0;i<m_name.name_str.length();++i)
		{
			nchar ch=m_name.name_str[i];
			if(!std::isdigit(ch))
			{
				name+=ch;
			}
		}
		name+=boost::lexical_cast<nstring>(tm);
		float ts=get_time();
		name+=boost::lexical_cast<nstring>(ts);

		nactor::ptr actor_ptr( nconstruct<nactor>(class_name, name) );
		nASSERT( actor_ptr );

		nstring buffer;
		narchive::ptr ar_writer=narchive::create_memory_xml_writer(buffer);
		this->serialize(*ar_writer.get());
		ar_writer->close();
		narchive::ptr ar_reader=narchive::create_memory_xml_reader(buffer);
		actor_ptr->serialize(*ar_reader.get());
		actor_ptr->_set_name(name);
		ar_reader->close();
		return actor_ptr;
	}

	void nactor::update(float delta_time, const nviewport& view)
	{
		if(m_static)
			return;

		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->update(delta_time, view);
		}
	}

	nactor_component::ptr nactor::create_component(const nstring& component_name, const nstring& class_name)
	{
		nactor_component::ptr new_comp( nconstruct<nactor_component>(class_name, component_name) );
		new_comp->_init(this);

		m_component_list.push_back( new_comp );
		
		return new_comp;
	}

	void nactor::on_event(nevent_base& evt)
	{
		if( evt.m_event_id == NSCENE_EVENT_ID )
		{
			if (m_owner)//have level
			{
				m_owner->on_event(evt);
			}
		}
	}

	void nactor::_init(nlevel* level_ptr)
	{
		m_owner = level_ptr;

		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_init(this);
		}
	}

	void nactor::look_at(const vector3 & at, const vector3 & up)
	{
		vector3 x, y, z;

		// Z vector

		z = vec_normalize(at - m_space.location);
		x = vec_normalize(vec_cross(up,z));
		y = vec_normalize(vec_cross(z,x));

		m_space.local_to_world.set_axis_x(x);
		m_space.local_to_world.set_axis_y(y);
		m_space.local_to_world.set_axis_z(z);
		
		update_cmp_transform();
	}

	void nactor::move(const vector3& pos, const vector3& rot, const vector3& scale)
	{
		m_space.location = pos;
		m_space.rotation = rot;
		m_space.scale = scale;
		m_space.update_matrix();

		update_cmp_transform();
	}

	void nactor::move( const vector3& pos )
	{
		m_space.location = pos;
		m_space.update_matrix();

		update_cmp_transform();
	}

	void nactor::set_location( const vector3& location )
	{
		m_space.location = location;
		m_space.update_matrix();

		update_cmp_transform();
	}

	void nactor::set_rotation( const vector3& rot )
	{
		m_space.rotation = rot;
		m_space.update_matrix();

		update_cmp_transform();
	}

	void nactor::_remove_from_level()
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_remove_from_level();
		}
	}

	void nactor::_destroy()
	{		
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_destroy();
		}

		//-- 置空owner必须放在最后，因为component可能需要这个数据；
		m_owner = NULL;
	}

	void nactor::init_phys()
	{
		for ( st_component_list::const_iterator cmp_it = m_component_list.begin();
			cmp_it != m_component_list.end(); cmp_it++ )
		{
			const nactor_component::ptr actor_cmp = *cmp_it; 

			// If physics is PHYS_RigidBody, we only create a rigid body for the CollisionComponent.
			if( m_physics_state==Phys_RigidBody && m_collision_cmp != actor_cmp )
				continue;

			actor_cmp->_init_cmp_phys();
		}
	}

	void nactor::simulate_phys( float delta_time )
	{
		for ( st_component_list::const_iterator cmp_it = m_component_list.begin();
			cmp_it != m_component_list.end(); cmp_it++ )
		{
			const nactor_component::ptr actor_cmp = *cmp_it; 
			actor_cmp->_simulate_cmp_phys(delta_time);
		}
		if( m_physics_state == Phys_RigidBody )
		{
			sync_actor_to_physics();	
		}
	}

	void nactor::sync_actor_to_physics()
	{
		if( !m_collision_cmp.get() )
			return;

		nprimitive_component::ptr primitive_cmp = boost::shared_static_cast<nprimitive_component>(m_collision_cmp);
		if( !primitive_cmp.get() )
			return;

		NxActor* nActor = primitive_cmp->get_nx_actor();
		if ( nActor == NULL )
		{
			return;
		}

		nASSERT( nActor->getNbShapes() > 0);
		matrix44 cmp_mat  = physx::nx_matrix_conv( nActor->getGlobalPose() );
		
		vector3 NewLocation = cmp_mat.get_origin();

		//// First find the current Actor-to-Component transform
		//matrix44 RelTM = primitive_cmp->get_space().local_to_world * mat_inverse( m_space.local_to_world );

		//// Then apply the inverse of this to the new Component TM to get the new Actor TM.
		//matrix44 NewTM = mat_inverse( RelTM ) * cmp_mat;
		//vector3 NewLocation = NewTM.get_origin();
		////vector3 NewRotation = NewTM.get_rotator();

		//@ 位置逻辑合法性校验 

		m_space.location = NewLocation;
		//m_space.rotation = NewRotation;
		m_space.update_matrix();

		update_cmp_transform();
	}

	void nactor::release_phys()
	{
		for ( st_component_list::const_iterator cmp_it = m_component_list.begin();
			cmp_it != m_component_list.end(); cmp_it++ )
		{
			const nactor_component::ptr actor_cmp = *cmp_it; 
			actor_cmp->_release_cmp_phys();
		}
	}

	void nactor::serialize(narchive& ar)
	{
		nevent_target::serialize(ar);

		serialize_basic_property(ar);		
		nSERIALIZE(ar, m_component_list);		
	}

	void nactor::serialize_basic_property(narchive& ar)
	{
		nSERIALIZE(ar, m_name);
		nSERIALIZE(ar, m_layer_name);		
		nSERIALIZE(ar, m_visible);
		nSERIALIZE(ar, m_static);
		nSERIALIZE(ar, m_space);
		nSERIALIZE(ar, m_frozen);
	}

	void nactor::_level_loaded(nlevel* level_ptr)
	{
		m_owner = level_ptr;

		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_level_loaded(this);
		}
	}

	void nactor::draw_bounds(nrender_primitive_draw_interface* PDI, const color4f& color)
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			nactor_component::ptr cmp = (*iter);
			if( cmp->get_editable() )
				cmp->draw_bounds(PDI, color);
		}
	}

	void nactor::update_cmp_transform()
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->update_transform(m_space);
		}
	}

	void nactor::_on_device_lost(int param)
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			(*iter)->_on_device_lost(param);
		}
	}

	bool nactor::_on_device_reset(int param)
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			bool ret = (*iter)->_on_device_reset(param);
			if( !ret )
				return false;
		}
		return true;
	}

	bool nactor::remove_component( const nname& name_str )
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end(); ++iter)
		{
			if( (*iter)->get_name() == name_str)
			{
				(*iter)->_remove_from_level();
				(*iter)->_destroy();
				iter = m_component_list.erase(iter);
				return true;
			}
		}
		return false;
	}

	bool nactor::remove_all_component()
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end(); ++iter)
		{
			(*iter)->_remove_from_level();
			(*iter)->_destroy();
		}
		m_component_list.clear();
		return true;
	}

	bool nactor::export_info_resource( gameframework::nlevel_info_resource* res )
	{
		(void)res;
		return false;
	}

	bool nactor::has_same_type_component( nactor* actor )
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end(); ++iter)
		{
			nstring name=(*iter)->get_name();
			nactor_component::ptr comp=actor->get_component(name);
			if(comp!=NULL) return true;
		}
		return false;
	}

	void nactor::set_enable_instance( bool is_enable )
	{
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end(); ++iter)
		{
			if((*iter)->reflection_get_class()->is_kind_of(L""))
			{
				nmesh_component::ptr comp=boost::shared_dynamic_cast<nmesh_component>((*iter));
				if(comp!=NULL)
				{
					comp->set_enable_instance(is_enable);
				}
			}			
		}
	}
}//namespace nexus