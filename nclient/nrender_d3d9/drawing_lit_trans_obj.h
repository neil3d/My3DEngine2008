#ifndef _NEXUS_DRAWING_LIT_TRANS_H_
#define _NEXUS_DRAWING_LIT_TRANS_H_
#include "drawing_policy.h"
#include "nrenderer_d3d9.h"

namespace nexus
{
	// 绘制透明物体的光照
	class drawing_lit_trans_obj :
		public drawing_policy
	{
		const light_vector& m_lights;
	public:
		drawing_lit_trans_obj(const light_vector& lights);
		virtual ~drawing_lit_trans_obj(void);

		virtual void draw_mesh(const d3d_view_info* view, const nrender_proxy* obj);
	};
}//namespace nexus

#endif //_NEXUS_DRAWING_LIT_TRANS_H_