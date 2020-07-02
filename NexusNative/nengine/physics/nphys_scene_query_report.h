#pragma once
#ifndef _NEXUS_PHYS_SCENE_QUERY_REPORT_H_
#define _NEXUS_PHYS_SCENE_QUERY_REPORT_H_

namespace nexus
{

	// result from a async collision check.
	struct nphys_async_raycast_result
	{
		/** indicates that there is an outstanding async line check that will be filling in this structure. */
		bool	bcheck_started;

		/** indicates that the async line check has finished, and bhit now contains the result. */
		bool	bcheck_completed;

		/** indicates result of line check. if bhit is true, then the line hit some part of the level. */
		bool	bhit;

		/** physx raycast result */
		NxRaycastHit nx_hit;

		nphys_async_raycast_result()
		{
			bcheck_started = false;
			bcheck_completed = false;
			bhit = false;
		}

		~nphys_async_raycast_result()
		{
			// check that this structure is not waiting for a line check to complete
			nASSERT(!(bcheck_started && !bcheck_completed));
		}

		/** utility to indicate this result structure is ready to be passed in to a ray cast query. */
		bool is_ready()
		{
			if(!bcheck_started || bcheck_completed)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		const vector3 get_hit_location() const 
		{
			if( bhit )
			{
				return physx::nx_vector_conv( nx_hit.worldImpact );
			}
			return vector3::zero;
		}

		const vector3 get_hit_normal() const 
		{
			if( bhit )
			{
				return physx::nx_vector_conv( nx_hit.worldNormal );
			}
			return vector3::zero;
		}

		const float get_hit_distance()
		{
			if( bhit )
			{
				return nx_hit.distance * physx::nx_to_engine_scale;
			}
			return -1.f;
		}
	};

	class nphys_scene_query_report : public NxSceneQueryReport
	{
	public:
		virtual	NxQueryReportResult	onBooleanQuery(void* /*userData*/, bool /*result*/)
		{
			nLog_Error( _T("onBooleanQuery - not supported"));
			return NX_SQR_CONTINUE;
		};
		virtual	NxQueryReportResult	onShapeQuery(void* /*userData*/, NxU32 /*nbHits*/, NxShape** /*hits*/)
		{ 
			nLog_Error( _T("onShapeQuery - not supported"));
			return NX_SQR_CONTINUE;
		};
		virtual	NxQueryReportResult	onSweepQuery(void* /*userData*/, NxU32 /*nbHits*/, NxSweepQueryHit* /*hits*/)
		{ 
			nLog_Error( _T("onSweepQuery - not supported"));
			return NX_SQR_CONTINUE; 
		};

		virtual	NxQueryReportResult	onRaycastQuery(void* userData, NxU32 nbHits, const NxRaycastHit* hits)
		{
			if( userData == NULL )return NX_SQR_ABORT_QUERY;

			nphys_async_raycast_result* res = static_cast<nphys_async_raycast_result*>(userData);
			if ( res )
			{
				if ( nbHits > 0 ) 
				{
					res->bhit = true;
					res->bcheck_completed = true;	
					res->nx_hit = hits[0];
				}
				else
				{
					res->bhit = false;
					res->bcheck_completed = true;	
					res->nx_hit.shape = NULL;
				}
			}
			return NX_SQR_ABORT_QUERY;
		}
	};

	/** SceneQuery async line check report object. */
	static nphys_scene_query_report			g_scene_query_report;

	class nphys_raycast_report : public NxUserRaycastReport
	{
	public:
		virtual bool onHit(const NxRaycastHit& /*hit*/)
		{
			return true;
		}
	private:
	};

} // end of namespace nexus


#endif //--_NEXUS_PHYS_SCENE_QUERY_REPORT_H_