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
	class nmaterial_base;

	// 一个对象可以有多个材质，每个材质的透明类型不同
	struct transparent_flag
	{
		unsigned int flag;

		transparent_flag(void):flag(0)	{}

		void clear()
		{
			flag = 0;
		}
		void set(enum ETransparentType t)
		{
			flag = flag | (1<<t);
		}
		bool get(enum ETransparentType t)
		{
			return 0!=(flag &  (1<<t));
		}
	};

	template<>
	inline narchive& nserialize(narchive& ar, transparent_flag& f, const TCHAR* obj_name)
	{
		nserialize(ar, f.flag, obj_name);
		return ar;
	}

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
		virtual nmaterial_base* get_material(int lod, int mtl_id) const	= 0;
		virtual transparent_flag get_transparent_flag() const = 0;
		virtual const box_sphere_bounds& get_bounds() const	= 0;		

		// 默认返回material的transparent type
		virtual enum ETransparentType get_transparent_type(int lod, int mesh_sec) const;
	};

}//_NRENDER_PROXY_H_
#endif //_NRENDER_PROXY_H_