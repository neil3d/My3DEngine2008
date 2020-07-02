/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NRENDER_PROXY_H_
#define _NRENDER_PROXY_H_
#include "ncore.h"
#include "nrender_mesh.h"
#include "render_define.h"
#include "../animation/nanim_controller.h"

namespace nexus
{
	class nmtl_base;
	/**
	*	primitive component/level geom与渲染系统的接口
	*
	*	@remark 提供渲染系统所需的参数
	*	@see class nprimitive_component, class nlevel_geom_resource
	*/
	struct nAPI nrender_proxy
	{
		virtual const matrix44& get_world_matrix() const			= 0;
		virtual int get_render_lod() const							= 0;
		virtual enum EDepthGroup get_depth_group() const		= 0;		
		virtual nrender_mesh* get_render_mesh(int lod) const		= 0;
		virtual nmtl_base* get_material(int lod, int mtl_id) const	= 0;		
		virtual const box_sphere_bounds& get_bounds() const	= 0;		
		
		virtual bool accept_dynamic_light() const = 0;
		virtual bool cast_dynamic_shadow() const = 0;

		virtual hit_id get_hit_proxy_id() const = 0;

		// 默认返回material的transparent type
		virtual enum ETransparentType get_transparent_type(int mesh_sec) const;
		size_t get_num_mesh_section() const
		{
			nrender_mesh* mesh = get_render_mesh( get_render_lod() );
			if( mesh )
				return mesh->get_num_section();
			else
				return 0;
		}

		virtual void draw_setup_effect(nshading_effect* effect_ptr)	const
		{
			(void)effect_ptr;
		}
	};

}//_NRENDER_PROXY_H_
#endif //_NRENDER_PROXY_H_