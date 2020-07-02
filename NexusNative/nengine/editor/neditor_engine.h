/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_EDITOR_ENGINE_H_
#define _NEXUS_EDITOR_ENGINE_H_
#include "../framework/nengine.h"
#include "../renderer/nhit_proxy_hash.h"

namespace nexus
{
	/**
	 *	编辑器所使用的engine基类
	 *	@remarks 提供面向编辑器的render/update接口(Python friendly)
	*/
	class nEDAPI neditor_engine : public nengine
	{
	public:
		neditor_engine(void);
		virtual ~neditor_engine(void);

		/** 渲染/更新一个view port窗口 */
		virtual void render_level(const nstring& lv_name, const nviewport& view, bool present);		
		/** 更新指定的level及其内的对象 */
		virtual void update_level(const nstring& lv_name, float delta_time);

		nactor::ptr pick_main_level_actor(const nviewport& view, const nstring& lv_name, int mouse_x, int mouse_y);		
	private:
		void render_level_interal(const nstring& lv_name, const nviewport& view, nhit_proxy_hash* hph, bool present);	

		nDECLARE_CLASS(neditor_engine);
	};
}//namespace nexus
#endif //_NEXUS_EDITOR_ENGINE_H_