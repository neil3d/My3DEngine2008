#ifndef _NEXUS_PLAIN_SCENE_H_
#define _NEXUS_PLAIN_SCENE_H_
#include "nscene_manager.h"

namespace nexus
{
	/**
	 *	只使用视锥剪裁的场景管理
	*/
	class nplain_scene :
		public nscene_manager
	{
	public:
		nplain_scene(void);
		virtual ~nplain_scene(void);

		virtual void render_visible(const view_info& view);

		virtual void clear_all()
		{
			m_prim_set.clear();
		}
	protected:
		virtual void add_primitive(nprimitive_component* prim);
		virtual void remove_primitive(nprimitive_component* prim);
		virtual void move_primitive(nprimitive_component* prim);

		virtual void add_light(nlight_component* lgt);
		virtual void remove_light(nlight_component* lgt);
		virtual void move_light(nlight_component* lgt);

	private:
		std::set<nprimitive_component*>	m_prim_set;
		std::set<nlight_component*>	m_lgt_set;	

		nDECLARE_CLASS(nplain_scene)
	};
}//namespace nexus
#endif //_NEXUS_PLAIN_SCENE_H_