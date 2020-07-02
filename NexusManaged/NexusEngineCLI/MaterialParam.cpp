#include "StdAfx.h"
#include "MaterialParam.h"

namespace NexusEngine
{
	void NMaterialParam::FromNative(const nexus::material_parameter_value& np)
	{
		name = gcnew String(np.name.c_str());
		type = static_cast<NexusEngine::EMaterialParameter>(np.type);
		floatVal = Vector4(np.vec_value.x,np.vec_value.y,np.vec_value.z,np.vec_value.w);
		if( np.tex_value )
			textureLoc.FromNative(np.tex_value->get_location());		
		intVal = np.int_value;
	}
}//namespace NexusEngine