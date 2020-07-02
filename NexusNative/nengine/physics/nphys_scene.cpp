#include "StdAfx.h"
#include "nphys_pch.h"
#include "nphysics_engine.h"
#include "nphys_material.h"
#include "nphys_scene.h"
#include "nphys_util.h"
#include "nphys_scene_query_report.h"

namespace nexus
{

	nphys_scene::nphys_scene(void) :
		m_owner(NULL),
		m_nx_scene(NULL),
		m_nx_query(NULL),
		m_rigidbody_compartment(NULL),
		m_fluid_compartment(NULL),
		m_cloth_compartment(NULL),
		m_softbody_compartment(NULL)
	{	}

	bool  nphys_scene::_init( nphysics_engine* physics )
	{
		if( physics==NULL || physics->get_nx_physics_sdk()==NULL )
		{
			nLog_Error( _T("Physics engine has not init.") );
		}

		m_owner = physics;

		// todo: 可编辑的 physis scene 参数
		physx_timing  timing;
		NxSceneDesc SceneDesc;
		SceneDesc.maxTimestep = timing.max_time_step;
		SceneDesc.maxIter = timing.max_sub_steps;
		SceneDesc.timeStepMethod = NX_TIMESTEP_FIXED;
#if defined(WIN32)
		SceneDesc.flags = NX_SF_SIMULATE_SEPARATE_THREAD | NX_SF_DISABLE_SCENE_MUTEX;
#else
		SceneDesc.flags = NX_SF_SIMULATE_SEPARATE_THREAD;
#endif
	
		// callback
		//SceneDesc.userContactReport = &nContactReportObject; // See the GNovodexSDK->setActorGroupPairFlags call below for which object will cause notification.
		//SceneDesc.userNotify = &nNotifyObject;
		//SceneDesc.userContactModify = &nContactModifyObject; // See the GNovodexSDK->setActorGroupPairFlags

		if( m_owner->is_hardoware_simulation() )
		{
			SceneDesc.simType = NX_SIMULATION_HW;
			nLog_Info( _T("Primary PhysX scene will be in hardware.\r\n") );
		}
		else
		{
			SceneDesc.simType = NX_SIMULATION_SW;
			nLog_Info( _T("Primary PhysX scene will be in software.\r\n") );
		}
		m_nx_scene = m_owner->get_nx_physics_sdk()->createScene(SceneDesc);

		// Create the default material
		m_default_material = m_nx_scene->getMaterialFromIndex(0); 
		m_default_material->setRestitution(0.5);
		m_default_material->setStaticFriction(0.5);
		m_default_material->setDynamicFriction(0.5);

		// Set up the filtering equation
		m_nx_scene->setFilterOps(NX_FILTEROP_OR, NX_FILTEROP_OR, NX_FILTEROP_AND);
		m_nx_scene->setFilterBool(true);
		NxGroupsMask zeroMask;
		zeroMask.bits0 = zeroMask.bits1 = zeroMask.bits2 = zeroMask.bits3 = 0;
		m_nx_scene->setFilterConstant0(zeroMask);
		m_nx_scene->setFilterConstant1(zeroMask);

		// Create scene query object
		NxSceneQueryDesc sceneQueryDesc;
		//sceneQueryDesc.executeMode = NX_SQE_ASYNCHRONOUS;
		sceneQueryDesc.report = &g_scene_query_report;
		m_nx_query = m_nx_scene->createSceneQuery(sceneQueryDesc);
		nASSERT(m_nx_query!=NULL);

		NxCompartmentDesc CompartmentDesc;
		if( m_owner->is_hardoware_simulation() )
		{
			CompartmentDesc.deviceCode = (NxU32)NX_DC_PPU_AUTO_ASSIGN;
		}
		else
		{
			CompartmentDesc.deviceCode = (NxU32)NX_DC_CPU;
		}
		//@todo: read this values in from an INI file or something.
		CompartmentDesc.gridHashCellSize = 20.0f;
		CompartmentDesc.gridHashTablePower = 8;
		CompartmentDesc.flags = NX_CF_INHERIT_SETTINGS;
		CompartmentDesc.timeScale = 1.0f;

#if defined(WIN32)
		CompartmentDesc.type = NX_SCT_RIGIDBODY;
		// Rigid-body compartments always in software due to it taking 75 megs of EMU memory!
		NxU32 OldAndBusted = CompartmentDesc.deviceCode;
		CompartmentDesc.deviceCode = (NxU32)NX_DC_CPU;
		m_rigidbody_compartment = m_nx_scene->createCompartment(CompartmentDesc);
		CompartmentDesc.deviceCode = OldAndBusted;
		nASSERT(m_rigidbody_compartment);
#endif

		CompartmentDesc.type = NX_SCT_FLUID;
		m_fluid_compartment = m_nx_scene->createCompartment(CompartmentDesc);
		//nASSERT(m_fluid_compartment);

		CompartmentDesc.type = NX_SCT_CLOTH;
		CompartmentDesc.isValid();
		m_cloth_compartment = m_nx_scene->createCompartment(CompartmentDesc);
		//nASSERT(m_cloth_compartment); // No CPU Cloth Compartments yet...

		CompartmentDesc.type = NX_SCT_SOFTBODY;
		m_softbody_compartment = m_nx_scene->createCompartment(CompartmentDesc);
		//nASSERT(m_softbody_compartment); // No CPU Cloth Compartments yet...

		return true;
	}

	void nphys_scene::destroy()
	{
		if(m_nx_scene && m_owner->get_nx_physics_sdk() )
		{
			if ( m_nx_query )
			{
				m_nx_scene->releaseSceneQuery(*m_nx_query);
				m_nx_query = NULL;
			}

			m_owner->get_nx_physics_sdk()->releaseScene(*m_nx_scene);
			m_nx_scene = NULL;
		}
	}

	void nphys_scene::set_gravity( const vector3& gravity )
	{
		if( m_nx_scene )
		{
			NxVec3 nxGravity = physx::nx_vector_conv(gravity);
			m_nx_scene->setGravity(nxGravity);
		}
	}

	void nphys_scene::get_gravity(vector3& gravity) const 
	{
		if( m_nx_scene )
		{
			NxVec3 nx_g;
			m_nx_scene->getGravity(nx_g);
			gravity = physx::nx_vector_conv( nx_g );
		}
	}




	void nphys_scene::_simulate_phys_scene(float delta_time)
	{
		if( m_nx_scene )
		{
			// Start collision and dynamics for delta time since the last frame
			m_nx_scene->simulate(delta_time);
			m_nx_scene->flushStream();

			begin_raycast();
		}
	}

	void nphys_scene::_get_phys_scene_result()
	{
		if( m_nx_scene )
		{
			//while (!m_nx_scene->fetchResults(NX_ALL_FINISHED, false));
			m_nx_scene->fetchResults(NX_ALL_FINISHED, true);

			finish_raycast();
		}
	}

	unsigned int nphys_scene::create_physics_material( nphys_material* /*material*/ )
	{
		unsigned int res = 0;
		return res;
	}

	unsigned int nphys_scene::find_physics_material( nname& /*material*/ )
	{
		unsigned int res = 0;
		return res;
	}

	//-- scene query interface
	bool nphys_scene::closest_raycast( nphys_async_raycast_result& ret, const vector3& start, const vector3& end, bool dynamic_objects )
	{
		vector3 delta = end - start;
		float distance = vec_length(delta);
		if( distance <= NX_MIN_F32 )
		{
			return true;
		}
		delta /= distance;
		return closest_raycast( ret,start, delta, distance, dynamic_objects);
	}

	bool nphys_scene::closest_raycast( nphys_async_raycast_result& ret, const vector3& origin, const vector3& direction, float distance, bool dynamic_objects )
	{
		if(m_nx_scene)
		{
			nASSERT(m_nx_query);
			NxVec3 nx_dir = physx::nx_vector_copy(direction);
			nx_dir.normalize();
			NxRay nray( physx::nx_vector_conv(origin), nx_dir );

			NxShapesType test_types = (dynamic_objects) ? NX_ALL_SHAPES : NX_STATIC_SHAPES;

			ret.bcheck_started = true;
			ret.bcheck_completed = false;

			NxShape* shape = m_nx_scene->raycastClosestShape(nray, test_types, ret.nx_hit, 0xFFFFFFFF, distance * physx::engine_to_nx_scale );
			
			//m_nx_query->raycastClosestShape(nray, test_types, ret.nx_hit, 0xffffffff, distance * physx::engine_to_nx_scale, 0xffffffff, NULL, NULL, &ret );
		
			//m_nx_query->execute();
			//m_nx_query->finish(true);

			ret.bcheck_completed = true;
			ret.bhit = (shape!=NULL);
			return ret.bhit;
		}

		return false;
	}

	void nphys_scene::async_raycast(const vector3& start, const vector3& end, bool dynamic_objects, nphys_async_raycast_result* result)
	{
		vector3 delta = end - start;
		float distance = vec_length(delta);
		if( distance <= NX_MIN_F32 )
		{
			//
			nLog_Error( _T("TOO cloest line nASSERT..") );
			return;
		}
		delta /= distance;
		async_raycast(start, delta, distance, dynamic_objects, result);
	}

	void nphys_scene::async_raycast(const vector3& origin, const vector3& direction, float distance, bool dynamic_objects, nphys_async_raycast_result* result)
	{
		nASSERT(result);
		if( !result->is_ready() )
		{
			nLog_Error(_T("asyncraycheck - result struct not ready."));
			return;
		}

		if(m_nx_scene)
		{
			nASSERT(m_nx_query);
			result->bcheck_started = true;
			result->bcheck_completed = false;
			NxVec3 nx_dir = physx::nx_vector_conv(direction);
			nx_dir.normalize();
			NxRay nray( physx::nx_vector_conv(origin), nx_dir );

			NxShapesType test_types = (dynamic_objects) ? NX_ALL_SHAPES : NX_STATIC_SHAPES;

			result->bcheck_started = true;
			result->bcheck_completed = false;
			m_nx_query->raycastClosestShape(nray, test_types, result->nx_hit, 0xffffffff, distance * physx::engine_to_nx_scale, 0, 0, 0, result);
		}
	}

	void nphys_scene::begin_raycast()
	{
		if(m_nx_scene)
		{
			nASSERT(m_nx_query);
			// If not finished, force it to finish now.
			if(!m_nx_query->finish(false))
			{
				nLog_Error(_T("Calling nphys_scene::begin_raycast and already running!"));
				m_nx_query->finish(true);
			}

			m_nx_query->execute();
		}
	}

	void nphys_scene::finish_raycast()
	{
		if(m_nx_scene)
		{
			nASSERT(m_nx_query);
			// If not finished, force it to finish now.
			if(!m_nx_query->finish(false))
			{
				m_nx_query->finish(true);
			}
		}
	}

	bool nphys_scene::is_hardware_scene() const 
	{
		return m_nx_scene!=NULL&& m_nx_scene->getSimType()==NX_SIMULATION_HW;	
	}

	bool nphys_scene::check_overlap( const box_sphere_bounds& bounds, bool check_ynamic_objects )
	{
		if( m_nx_scene )
		{
			NxSphere sphere( physx::nx_vector_conv(bounds.origin), bounds.sphere_radius * physx::engine_to_nx_scale );
			NxShapesType test_types = (check_ynamic_objects) ? NX_ALL_SHAPES : NX_STATIC_SHAPES;
			return m_nx_scene->checkOverlapSphere(sphere,test_types);
		}
		return false;
	}
}//namespace nexus