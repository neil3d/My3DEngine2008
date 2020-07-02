#include "StdAfx.h"
#include "NcharacterFields.h"
#include "ntype_def.h"

namespace NexusEngine
{


	bool NCharacterFieldsHelper::IsPlayer( System::Int64 guid )
	{
		return IS_PLAYER(guid);
	}

	bool NCharacterFieldsHelper::IsCreature( System::Int64 guid )
	{
		return IS_CREATURE(guid);
	}
}
