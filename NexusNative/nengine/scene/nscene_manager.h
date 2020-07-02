/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NSCENE_MANAGER_H_
#define _NSCENE_MANAGER_H_
#include "ncore.h"
#include "scene_event.h"
#include "ngeneric_octree.h"
#include "../renderer/nrender_element.h"

namespace nexus
{
	/**
	 *	场景管理的基类
	 *	@remark 提供空间分割的算法接口
	*/
	class nscene_manager :
		public nevent_target
	{
	public:
		typedef shared_ptr<nscene_manager> ptr;

		nscene_manager(void);
		virtual ~nscene_manager(void);

		virtual void clear_all() = 0;
		virtual void render_visible(const nviewport& view) = 0;
		virtual void on_event(nevent_base& evt);

		// draw debug information
		virtual void draw( nrender_primitive_draw_interface* /*PDI*/ ){}

	protected:
		virtual void add_primitive(nprimitive_component* prim) = 0;
		virtual void remove_primitive(nprimitive_component* prim) = 0;
		virtual void move_primitive(nprimitive_component* prim) = 0;

		virtual void add_light(nlight_component* lgt) = 0;
		virtual void remove_light(nlight_component* lgt) = 0;
		virtual void move_light(nlight_component* lgt) = 0;

		nDECLARE_VIRTUAL_CLASS(nscene_manager)
	};
}//namespace nexus
#endif //_NSCENE_MANAGER_H_