#ifndef _NEXUS_PHYS_SCENE_H_
#define _NEXUS_PHYS_SCENE_H_
#include "ncore.h"

namespace nexus
{
	class nphysics_engine;
	class nphys_material;
	struct nphys_async_raycast_result;

	/** Timing parameters used for a PhysX primary scene or compartment */
	struct physx_timing
	{
		bool		b_fixed_time_step;
		float		max_time_step;
		int			max_sub_steps;

		physx_timing() :
			b_fixed_time_step(false),
			max_time_step(0.02f),
			max_sub_steps(5)
		{
		}
	};


	class nAPI nphys_scene
		: public nobject
	{
	public:
		// typedef
		typedef boost::shared_ptr<nphys_scene> ptr;

		nphys_scene(void);

		virtual ~nphys_scene(void)	
		{ destroy();	}

		// scene interface
		virtual bool _init( nphysics_engine* physics );
		virtual void destroy();

		// asynchronous simulate
		virtual void _simulate_phys_scene( float delta_time );
		virtual void _get_phys_scene_result();

		virtual void set_gravity( const vector3& gravity );
		virtual void get_gravity(vector3& gravity) const;
	
		virtual bool is_valid_scene(){ return (m_nx_scene!=NULL); }

		nphysics_engine* get_phys_engine() const 
		{
			return m_owner;
		}
		NxScene* get_nx_scene() const 
		{
			return m_nx_scene;
		}
		NxCompartment* get_rigidbody_compartment()const { return m_rigidbody_compartment;	}
		NxCompartment* get_fluid_compartment()const		{ return m_fluid_compartment;		}
		NxCompartment* get_cloth_compartment()const		{ return m_cloth_compartment;		}
		NxCompartment* get_softbody_compartment()const	{ return m_softbody_compartment;	}
		NxMaterial*	get_default_mat() const
		{
			return m_default_material;
		}
		bool is_hardware_scene() const;

		// physics material interface
		virtual unsigned int create_physics_material( nphys_material* material );
		virtual unsigned int find_physics_material( nname& material );

		// scene query interface
		bool closest_raycast( nphys_async_raycast_result& ret, const vector3& origin, const vector3& direction, float distance, bool dynamic_objects );
		bool closest_raycast( nphys_async_raycast_result& ret, const vector3& start, const vector3& end, bool dynamic_objects );

		// overlap check
		bool check_overlap( const box_sphere_bounds& bounds, bool check_ynamic_objects );

		/** 
		*	begin an async ray cast against static geometry in the level using the physics engine. 
		*	@param origin			starting point of ray.
		*	@param direction		normalized direction of the ray.
		*	@param distance			length of ray
		*	@param dynamic_objects	if true, tests against dynamic objects as well.
		*/
		void async_raycast(const vector3& origin, const vector3& direction, float distance, bool dynamic_objects, nphys_async_raycast_result* result);
		
		/** 
		*	begin an async ray cast against static geometry in the level using the physics engine. 
		*	@param start			start point of line
		*	@param end				end point of line
		*	@param dynamic_objects	if true, tests against dynamic objects as well.
		*/
		void async_raycast(const vector3& start, const vector3& end, bool dynamic_objects, nphys_async_raycast_result* result);

	private:
		/** fires off any batched line checks.*/
		void begin_raycast();

		/** forces all line checks to be processed now (blocking) so no nphys_async_raycast_result are referenced by the system. */
		void finish_raycast();
	
	private:
		nphysics_engine*			m_owner;
		NxScene*					m_nx_scene;
		NxMaterial*					m_default_material;
		NxSceneQuery*				m_nx_query;

		NxCompartment*				m_rigidbody_compartment;
		NxCompartment*				m_fluid_compartment;
		NxCompartment*				m_cloth_compartment;
		NxCompartment*				m_softbody_compartment;

		nDECLARE_CLASS(nphys_scene)
	};
}//namespace nexus
#endif //_NEXUS_PHYS_SCENE_H_