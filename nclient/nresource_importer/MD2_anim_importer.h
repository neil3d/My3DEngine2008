#pragma once
#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "../nengine/renderer/render_define.h"


namespace nexus
{
	class MD2_anim_importer :
		public nanim_mesh_importer
	{
	public:
		MD2_anim_importer(void);
		virtual ~MD2_anim_importer(void);
	};
}//nexus