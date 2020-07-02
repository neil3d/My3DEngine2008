/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NRENDER_RESOURCE_MANAGER_H_
#define _NRENDER_RESOURCE_MANAGER_H_
#include "ncore.h"
#include "../framework/nsubsystem.h"
#include "nrender_resource.h"
#include "nrender_mesh.h"
#include "nrender_static_mesh_indexed.h"
#include "nrender_static_mesh.h"
#include "nrender_anim_mesh.h"
#include "ncpu_skin_mesh.h"
#include "nrender_texture.h"
#include "nrender_target.h"
#include "nrender_dynamic_mesh_indexed.h"
#include "nrender_simple_mesh.h"
#include "nhit_proxy_hash.h"

namespace nexus
{
	/**
	 *	创建、管理render hardware上的资源对象
	 *	@remark 【线程安全】
	*/
	class nAPI nrender_resource_manager :
		public nsubsystem
	{
	public:
		typedef shared_ptr<nrender_resource_manager> ptr;

		nrender_resource_manager(void)	{	}
		virtual ~nrender_resource_manager(void)	{	}

		virtual nrender_static_mesh* alloc_static_mesh() = 0;
		virtual nrender_static_mesh_indexed* alloc_static_mesh_indexed() = 0;
		virtual nrender_anim_mesh* alloc_anim_mesh_indexed() = 0;
		virtual nrender_cpu_skin_mesh* alloc_cup_skin_mesh() = 0;		
		virtual nrender_dynamic_mesh_indexed* alloc_dynamic_mesh_indexed() = 0;
		virtual nrender_simple_mesh* alloc_simple_mesh() = 0;

		virtual nrender_texture2D*	alloc_texture_2d() = 0;
		virtual nrender_cube_map*	alloc_cube_map() = 0;
		virtual nrender_heightmap*	alloc_heightmap() = 0;
		virtual nrender_alphamap*	alloc_alphamap() = 0;
		virtual nrender_target* alloc_render_target() = 0;
		virtual nhit_proxy_hash* alloc_hit_proxy_hash() = 0;

		virtual void free_resource(nrender_resource* res) = 0;

		virtual void set_default_material(nmaterial_base* mtl, nmaterial_base* mtl_two_sided) = 0;
	private:	

		nDECLARE_VIRTUAL_CLASS(nrender_resource_manager);
	};
}//namespace nexus
#endif //_NRENDER_RESOURCE_MANAGER_H_