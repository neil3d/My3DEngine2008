#ifndef _NEXUS_PHYS_BODY_INSTANCE_H_
#define _NEXUS_PHYS_BODY_INSTANCE_H_

#include "nphys_pch.h"

namespace nexus
{
	// forward declear 
	class nactor;
	class nactor_component;

	/** 
	* for cache PhysX actor user data
	*/
	class nphys_body_instance : public nobject
	{
	public:
		typedef shared_ptr<nphys_body_instance>  ptr;

		nphys_body_instance() : m_nx_actor (NULL),m_owner_actor(NULL),m_owner_cmp(NULL)
		{
		}

		virtual ~nphys_body_instance(){};


		NxActor*					m_nx_actor;
		nactor*						m_owner_actor;
		nactor_component*			m_owner_cmp;

	private:
		nDECLARE_CLASS( nphys_body_instance );

	}; //-- end of class nphys_body_instance

}; //-- end of namespace nexus


#endif //-- _NEXUS_PHYS_BODY_INSTANCE_H_