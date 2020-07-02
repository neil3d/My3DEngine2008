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
	*	编辑command基类
	*	@remarsk 其派生类对象添加到mgr中，用来实现undo
	*/
	class nEDAPI neditor_cmd : public nobject
	{
	public:
		typedef shared_ptr<neditor_cmd> ptr;

		neditor_cmd(void)	{}
		virtual ~neditor_cmd(void)	{}

		virtual bool execute(void) = 0;
		virtual bool undo(void) = 0;
		virtual bool redo(void) = 0;
		virtual nstring get_name(void) = 0;
		virtual nstring get_desc(void) = 0;

		nDECLARE_VIRTUAL_CLASS(neditor_cmd)
	};

	/**
	 *	编辑器所使用的engine基类
	 *	@remarks 提供面向编辑器的render/update接口(Python friendly)
	*/
	class nEDAPI neditor_engine : public nengine
	{
	public:
		neditor_engine(void);
		virtual ~neditor_engine(void);

		static neditor_engine* instance();

		/** 渲染/更新一个view port窗口 */
		virtual void render_level(const nstring& lv_name, const nviewport& view, bool present);		
		/** 更新指定的level及其内的对象 */
		virtual void update_level(const nstring& lv_name, float delta_time, const nviewport& view);

		nactor::ptr pick_main_level_actor(const nviewport& view, const nstring& lv_name, int mouse_x, int mouse_y);		

		//-- command manager相关接口
		virtual void push_command(neditor_cmd::ptr new_cmd) = 0;
		
	private:
		void render_level_interal(const nstring& lv_name, const nviewport& view, nhit_proxy_hash* hph, bool present);	

		nDECLARE_VIRTUAL_CLASS(neditor_engine);
	};
}//namespace nexus
#endif //_NEXUS_EDITOR_ENGINE_H_