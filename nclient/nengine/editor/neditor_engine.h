/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_EDITOR_ENGINE_H_
#define _NEXUS_EDITOR_ENGINE_H_
#include "../framework/nengine.h"

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
		virtual void render_level(const nstring& lv_name, const ncamera* cam, int wnd_handle);
		/** 更新一次所有的level及其内的对象 */
		virtual void update_objects(float delta_time);

		nDECLARE_CLASS(neditor_engine);
	};
}//namespace nexus
#endif //_NEXUS_EDITOR_ENGINE_H_