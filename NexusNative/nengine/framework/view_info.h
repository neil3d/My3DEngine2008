#ifndef _NEXUS_VIEW_INFO_H_
#define _NEXUS_VIEW_INFO_H_
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

	struct nAPI nviewport
	{
		unsigned int	x,
						y,
						width,
						height;
		float			min_z,
						max_z;
		ncamera			camera;		
		ERenderMode		render_mode;
		void*			handle_wnd;
		unsigned long	show_flags;

		float			near_lod,
						far_lod;
		float			lod_bias;
		
		nrender_element::ptr			widgets_render;
		nhit_proxy_hash*				hit_hash;

		//-- 内部计算更新的数据
		nfrustum		frustum;

		nviewport(void):x(0),y(0),width(800),height(600),min_z(0),max_z(1),
			handle_wnd(NULL),show_flags(0),render_mode(ERM_Lit),
			near_lod(200),far_lod(2000),lod_bias(0)
		{
			hit_hash = NULL;
		}
		void update()
		{
			frustum.extract(camera.get_view(), camera.get_project());
		}

		void create_hit_hash();		
		void destroy()
		{
			widgets_render.reset();
			if(hit_hash)
			{
				hit_hash->release();
				hit_hash = NULL;
			}
		}

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
			lod = std::max(lod, 0.0f);
			lod = std::min(lod, 1.0f);
			return lod;
		}
	};
}//namespace nexus

#endif //_NEXUS_VIEW_INFO_H_