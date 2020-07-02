/**
 *	nexus ngame - nmove_controller
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NMOVE_CONTROLLER_H_
#define _NGAME_NMOVE_CONTROLLER_H_

#include "ncommon.h"
#include "ngameframework.h"

namespace nexus {

	struct ncheck_result;
	class nworld_object;

	class nmove_controller : public gameframework::ncharacter_controller
	{
	public:
		nmove_controller();
		virtual ~nmove_controller();

		void set_controlled(nworld_object* obj_ptr) { m_obj_ptr = obj_ptr; }
		// 是否能飞行
		virtual bool can_fly() const;

		// 在每次调用完move函数之后调用，用于重载该函数以实现网络同步等功能
		virtual void post_move();

		// 计算最大速度限制率，返回[0,1]
		virtual float maxspeed_modifier();

		// 移动方式发生改变的回调
		virtual void notify_ground();
		virtual void notify_jump();
		virtual void notify_fly();
		virtual void notify_custom();
		virtual void notify_hit(ncheck_result* hit);

	private:
		nworld_object*			m_obj_ptr;
	};

} // namespace nexus

#endif // _NGAME_NMOVE_CONTROLLER_H_
