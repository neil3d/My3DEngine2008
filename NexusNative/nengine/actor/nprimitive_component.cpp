#include "StdAfx.h"
#include "nprimitive_component.h"
#include "../resource/nresource_manager.h"
#include "../scene/scene_event.h"
#include "../physics/nphys_util.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nprimitive_component, nactor_component)
	
	int nprimitive_component::octree_semantics::MIN_NODE_SIZE = 512;

	nprimitive_component::nprimitive_component(const nstring& name_str):nactor_component(name_str),
		m_accept_dynamic_light(true),m_cast_dynamic_shadow(true),
		m_disable_all_rigidbody(false),
		m_visibale(true),
		m_cull_dist(HALF_WORLD_MAX),
		m_octreenode(NULL)
	{		
		mat_set_identity(m_mat_world);		

		m_bounds.box_extent = vector3(HALF_WORLD_MAX,HALF_WORLD_MAX,HALF_WORLD_MAX);
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = HALF_WORLD_MAX;
	}

	nprimitive_component::~nprimitive_component(void)
	{
	}

	const matrix44& nprimitive_component::get_world_matrix() const			
	{	
		return m_mat_world;
	}		

	void nprimitive_component::_init(nactor* owner)
	{
		nactor_component::_init(owner);
		nscene_event evt(ESA_AddPrimitive, this);
		owner->on_event(evt);
	}

	void nprimitive_component::_remove_from_level()
	{
		nscene_event evt(ESA_RemovePrimitive, this);
		m_owner->on_event(evt);
	}

	void nprimitive_component::update_transform(const object_space& parent_space)
	{
		 _update_transform(parent_space);
		 nscene_event evt(ESA_MovePrimitive, this);
		 m_owner->on_event(evt);
	}

	void nprimitive_component::_update_transform(const object_space& parent_space)
	{
		m_mat_world = m_space.local_to_world*parent_space.local_to_world;
	}

	void nprimitive_component::serialize(narchive& ar)
	{
		nactor_component::serialize(ar);

		nSERIALIZE(ar, m_space);
		nSERIALIZE(ar, m_bounds);
		nSERIALIZE(ar, m_accept_dynamic_light);
		nSERIALIZE(ar, m_cast_dynamic_shadow);
	}

	void nprimitive_component::_level_loaded(nactor* owner)
	{
		nactor_component::_level_loaded(owner);
		this->_update_transform(owner->get_space());
		this->_init(owner);
	}

	void nprimitive_component::draw_bounds(nrender_primitive_draw_interface* PDI, const color4f& color)
	{
		PDI->draw_wired_box(get_AABBox(), color);
	}

	void nprimitive_component::render(class render_package_base* rpb)
	{
		(void)rpb;
	}

	void nprimitive_component::move(const vector3& pos, const vector3& rot, const vector3& scale)
	{
		m_space.location = pos;
		m_space.rotation = rot;
		m_space.scale = scale;
		m_space.update_matrix();

		if (m_owner)
		{
			_update_transform(m_owner->get_space());
		}
		else
		{
			m_mat_world = m_space.local_to_world;
		}
	}

	void nprimitive_component::add_impulse( const vector3& impluse, const vector3& position, bool change_vel/* =false */ )
	{
		NxActor* actor = get_nx_actor();
		if(actor && actor->isDynamic() && !actor->readBodyFlag(NX_BF_KINEMATIC))
		{
			NxVec3 nImpulse = physx::nx_vector_conv(impluse);
			if ( vec_length_sq(position) < NX_MIN_F32 )
			{
				if(change_vel)
				{
					physx::add_force_to_actor(actor,nImpulse,(NX_VELOCITY_CHANGE));
				}
				else
				{
					physx::add_force_to_actor(actor,nImpulse,(NX_IMPULSE));
				}
			}
			else
			{
				NxVec3 nPosition = physx::nx_vector_conv(position);
				if(change_vel)
				{
					actor->addForceAtPos(nImpulse, nPosition, NX_VELOCITY_CHANGE);
				}
				else
				{
					actor->addForceAtPos(nImpulse, nPosition, NX_IMPULSE);
				}
			}

			actor->wakeUp();
		}
	}

	void nprimitive_component::add_radial_impulse( const vector3& origin, float radius, float strength,bool change_vel/* =false */ )
	{
		NxActor* nActor = get_nx_actor();
		if( nActor && nActor->isDynamic() && !nActor->readBodyFlag(NX_BF_KINEMATIC) )
		{
			NxVec3 nCOMPos = nActor->getCMassGlobalPosition(); // center of mass in world space
			NxVec3 nOrigin = physx::nx_vector_conv(origin); // origin of radial impulse, in world space

			NxVec3 nDelta = nCOMPos - nOrigin; // vector from origin to COM
			float Mag = nDelta.magnitude() * physx::nx_to_engine_scale; // Distance from COM to origin, in Unreal scale

			// If COM is outside radius, do nothing.
			if (Mag > radius)
				return;

			nDelta.normalize();

			// Scale by physx::engine_to_nx_scale here, because units are velocity * mass. 
			float ImpulseMag = strength * physx::engine_to_nx_scale;
			ImpulseMag *= (1.0f - (Mag / radius));

			NxVec3 nImpulse = nDelta * ImpulseMag;
			if(change_vel)
			{
				physx::add_force_to_actor(nActor,nImpulse,(NX_VELOCITY_CHANGE));
			}
			else
			{
				physx::add_force_to_actor(nActor,nImpulse,(NX_IMPULSE));
			}
			nActor->wakeUp();
		}
	}

	void nprimitive_component::add_force( const vector3& force, const vector3& position )
	{
		NxActor* actor = get_nx_actor();
		if(actor && actor->isDynamic() && !actor->readBodyFlag(NX_BF_KINEMATIC))
		{
			NxVec3 nForce = physx::nx_vector_conv(force);
			if( vec_length_sq(position) < NX_MIN_F32 )
			{
				physx::add_force_to_actor(actor,nForce);
			}
			else
			{
				actor->addForceAtPos(nForce, physx::nx_vector_conv(position));
			}

			actor->wakeUp();
		}
	}

	void nprimitive_component::add_radial_force( const vector3& origin, float radius, float strength )
	{
		NxActor* actor = get_nx_actor();
		if(actor && actor->isDynamic() && !actor->readBodyFlag(NX_BF_KINEMATIC))
		{
			NxVec3 nCOMPos = actor->getCMassGlobalPosition(); // center of mass in world space
			NxVec3 nOrigin = physx::nx_vector_conv(origin); // origin of force, in world space

			NxVec3 nDelta = nCOMPos - nOrigin; // vector from origin to COM
			float Mag = nDelta.magnitude() * physx::nx_to_engine_scale; // Distance from COM to origin, in Unreal scale
			// If COM is outside radius, do nothing.
			if (Mag > radius)
				return;

			nDelta.normalize();

			// using linear falloff, scale with distance.
			float ForceMag = strength * physx::engine_to_nx_scale;
			ForceMag *= (1.0f - (Mag / radius));

			// Apply force
			NxVec3 nForce = nDelta * ForceMag;
			physx::add_force_to_actor(actor,nForce, NX_FORCE);
		}
	}

	void nprimitive_component::set_linear_velocity( const vector3& vel, bool add2current /* =false */ )
	{
		NxActor* nActor = get_nx_actor();
		if (nActor)
		{
			NxVec3 nNewVel = physx::nx_vector_conv(vel);

			if(add2current)
			{
				NxVec3 nOldVel = nActor->getLinearVelocity();
				nNewVel += nOldVel;
			}

			physx::set_linear_velocity_to_actor(nActor,nNewVel);
		}
	}

	void nprimitive_component::set_angular_velocity( const vector3& ang, bool add2current /* =false */ )
	{
		NxActor* nActor = get_nx_actor();
		if (nActor)
		{
			NxVec3 nNewAngVel = physx::nx_vector_conv(ang);

			if(add2current)
			{
				NxVec3 nOldAngVel = nActor->getAngularVelocity();
				nNewAngVel += nOldAngVel;
			}

			nActor->setAngularVelocity(nNewAngVel);
		}
	}

	void nprimitive_component::set_nx_position( const vector3& pos )
	{
		NxActor* nActor = get_nx_actor();
		if (nActor != NULL)
		{
			nActor->setGlobalPosition(physx::nx_vector_conv(pos));

			// Force a physics update now for the owner, to avoid a 1-frame lag when teleporting before the graphics catches up.
			//if (Owner != NULL && Owner->Physics == PHYS_RigidBody)
			//{
			//	Owner->SyncActorToRBPhysics();
			//}
		}
	}

	void nprimitive_component::set_nx_rotation( const vector3& rot )
	{
		NxActor* nActor = get_nx_actor();
		if (nActor != NULL)
		{
			NxVec3 eulerAngle( nRadToDeg(rot.x),nRadToDeg(rot.y),nRadToDeg(rot.z) );
			// Quaternion
			NxQuat q1, q2, q3;
			q1.fromAngleAxis(eulerAngle.x, NxVec3(1,0,0));
			q2.fromAngleAxis(eulerAngle.y, NxVec3(0,1,0));
			q3.fromAngleAxis(eulerAngle.z, NxVec3(0,0,1));
			nActor->setGlobalOrientationQuat( q3*q2*q1 ); // Use global axes

			// Force a physics update now for the owner, to avoid a 1-frame lag when teleporting before the graphics catches up.
			//if (Owner != NULL && Owner->Physics == PHYS_RigidBody)
			//{
			//	Owner->SyncActorToRBPhysics();
			//}
		}
	}

	void nprimitive_component::wake()
	{
		NxActor* Actor = get_nx_actor();
		if (Actor)
			Actor->wakeUp();
	}

	bool nprimitive_component::is_sleep()
	{
		NxActor* Actor = get_nx_actor();
		if (Actor && !Actor->isSleeping())
			return true;

		return false;	
	}
	
	void nprimitive_component::put_to_sleep()
	{
		NxActor* Actor = get_nx_actor();
		if (Actor)
			Actor->putToSleep();
	}

	NxActor* nprimitive_component::get_nx_actor() const
	{
		if( m_phys_body_instance.get() == NULL )
		{
			return NULL;
		}

		return m_phys_body_instance->m_nx_actor;
	}


	bool nprimitive_component::is_resource_ready_event(const nevent_base& evt, const nname& res_name)
	{
		if( evt.m_event_id == NRESOURCE_EVENT_ID )
		{
			const nresource_event& res_evt = static_cast<const nresource_event&>(evt);
			if(res_evt.type == EResEvt_Ready)
			{
				return res_name == res_evt.res_name;
			}
		}

		return false;
	}

	bool nprimitive_component::_is_cull( const nviewport& view )
	{
		return (vec_distance_sq(m_bounds.origin,view.camera.get_eye_pos()) > m_cull_dist * m_cull_dist );
	}
}//namespace nexus