#ifndef _NEXUS_OBJECT_SPACE_H_
#define _NEXUS_OBJECT_SPACE_H_
#include "vector.h"
#include "matrix.h"

namespace nexus
{
	struct object_space
	{
		vector3		location;
		vector3		rotation;
		vector3		scale;

		matrix44	local_to_world;

		object_space(void):location(0,0,0),
			rotation(),
			scale(1,1,1)
		{
			mat_set_identity(local_to_world);
		}

		inline object_space(
			const vector3 & irotate,
			const vector3 & iscale,
			const vector3 & itranslate ) :
			rotation( irotate ),
			scale( iscale ),
			location( itranslate )
		{
		}

		void update_matrix()
		{
			matrix44 scl, rot, loc;
			mat_set_scaling(scl, scale.x, scale.y, scale.z);
			mat_set_rotation_yaw_pitch_roll(rot, rotation.y, rotation.x, rotation.z);
			mat_set_translation(loc, location);
			
			local_to_world = (scl*rot)*loc;
		}
	};
}//namespace nexus
#endif