/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_EDITOR_CMD_MGR_H_
#define _NEXUS_EDITOR_CMD_MGR_H_
#include "ncore.h"

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

		nDECLARE_VIRTUAL_CLASS(neditor_cmd)
	};

	/**
	 *	编辑器的command管理器
	 *	@remarks 主要用来实现undo/redo
	*/
	class nEDAPI neditor_cmd_mgr
	{
	public:
		neditor_cmd_mgr(void);
		~neditor_cmd_mgr(void);

		void append_cmd(neditor_cmd::ptr cmd_ptr);
		neditor_cmd::ptr get_last_cmd() const;
		void undo() const;
		void redo() const;
		bool empty() const	{	return m_cmd_queue.empty();}	
	protected:
		std::deque<neditor_cmd::ptr>	m_cmd_queue;
	};
}//namespace nexus

#endif //_NEXUS_EDITOR_CMD_MGR_H_