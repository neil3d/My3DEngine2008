#pragma once
#include "ncore.h"
#include "../renderer/render_define.h"
#include "../renderer/nrender_element.h"
#include "../renderer/nhit_proxy_hash.h"

namespace nexus
{
	enum EShowFlag
	{
		EShow_InSceneCapture = 0x00000001,
	};

	class nAPI nviewport
	{
	public:
		unsigned int	x,
						y,
						width,
						height;
		float			min_z,
						max_z;
		ncamera			camera;		
		ERenderMode		render_mode;
		bool			enable_dynamic_shadow;
		void*			handle_wnd;
		unsigned long	show_flags;

		float			near_lod,
						far_lod;
		float			lod_bias;
		matrix44		m_viewport_matrix;
		
		nrender_element::ptr			widgets_render;
		nhit_proxy_hash*				hit_hash;
		bool							_enable_hit_hash;
		int	_device_handler_id;

		//-- 内部计算更新的数据
		nfrustum		frustum;
		
	public:
		nviewport(void);
		~nviewport(void);
		void update()
		{
			frustum.extract(camera.get_view(), camera.get_project());
		}

		void create_hit_hash();		
		void destroy();		

		hit_id get_hit_id(int mx, int my) const
		{
			if(hit_hash)
				return hit_hash->get_hit_id(x+mx, y+my);
			else
				return (hit_id)-1;
		}

		// 返回0~1
		float get_lod(const vector3& pos) const
		{
			float distance = vec_distance(pos, camera.get_eye_pos());
			float lod = 1.0f - (distance-near_lod)/(far_lod-near_lod);
			lod += lod_bias;
			lod = max(lod, 0.0f);
			lod = min(lod, 1.0f);
			return lod;
		}

	private:
		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);

	private:
		nviewport(const nviewport&);
		const nviewport& operator = (const nviewport&);
	};

	class nview_info
	{
	public:
		vector3		look_at;
		vector3		eye_pos;
		matrix44	mat_view;
		matrix44	mat_project;
		matrix44	mat_view_project;

		float		time;
		float     znear;
		float     zfar;
		float     fov;
		float		aspect;
		bool		enable_dynamic_shadow;

		struct view_port {
			unsigned long       X;
			unsigned long       Y;            /* Viewport Top left */
			unsigned long       Width;
			unsigned long       Height;       /* Viewport Dimensions */
			float       MinZ;         /* Min/max of clip Volume */
			float       MaxZ;
		};

		view_port m_view_port;
		nfrustum frustum;
	};
}//namespace nexus
