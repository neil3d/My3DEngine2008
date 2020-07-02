#ifndef _NEXUS_PHYS_MATERIAL_H_
#define _NEXUS_PHYS_MATERIAL_H_
#include "../ncore/ncore.h"
#include "../resource/nresource.h"

namespace nexus
{
	class nAPI nphys_material 
		: public nresource
	{
	public:
		typedef boost::shared_ptr<nphys_material>  ptr;
		
		// surface properties
		float	dynamic_friction; 
		float	static_friction; 
		float	restitution;
		
		// Anisotropic friction support
		float	dynamic_friction_v; 
		float	static_friction_v; 
		vector3 dir_of_anisotropy;
		bool	enable_anisotropic_friction;

		nphys_material( nstring& identifier ) :
			nresource(identifier)
		{
		}

		nphys_material() :
			nresource(_T("default")),
			dynamic_friction(0.7f),
			static_friction(0.7f),
			restitution(0.3f),
			enable_anisotropic_friction(false)
		{
		}

		virtual ~nphys_material()
		{
		}

		// –Ú¡–ªØ
		virtual void serialize(narchive& ar)
		{
			nresource::serialize(ar);

			nSERIALIZE(ar, dynamic_friction);
			nSERIALIZE(ar, static_friction);
			nSERIALIZE(ar, restitution);
			nSERIALIZE(ar, dynamic_friction_v);
			nSERIALIZE(ar, static_friction_v);
			nSERIALIZE(ar, dir_of_anisotropy);
			nSERIALIZE(ar, enable_anisotropic_friction);
		}

	private:

		nDECLARE_CLASS(nphys_material)
	};

} // namespace nexus

#endif // _NEXUS_PHYS_MATERIAL_H_