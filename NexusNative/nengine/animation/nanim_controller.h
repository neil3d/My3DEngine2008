#ifndef _NEXUS_ANIM_CONTROLLER_H_
#define _NEXUS_ANIM_CONTROLLER_H_
#include "ncore.h"
#include "anim_define.h"

namespace nexus
{
	class nAPI nanim_controller :
		public nobject
	{
	public:
		nanim_controller(void)			{		}
		virtual ~nanim_controller(void)	{		}	

		virtual void tick_anim(float delta_time) = 0;

		// 设置当前播放动画名称
		virtual void set_anim(const nstring& sequence) = 0;

		// 播放指定动画，播放前必须指定动画的名称，参考set_anim
		// param loop : 是否循环播放
		// param rate : 播放速率
		// param start_time : 起始时间
		virtual void play_anim(bool loop, float rate, float start_time) = 0;

		// 停止播放动画
		virtual void stop_anim() = 0;

		// 设置当前anim sequence位置
		virtual void set_position(float new_time, bool fire_notifies) = 0;

		// 获取当前anim sequence剩余时间
		virtual float get_time_left() const = 0;
		// 获取当前anim sequence总长度
		virtual float get_anim_length() const = 0;
		// 获取当前anim sequence总帧数
		virtual size_t get_anim_frame() const = 0;
		// 获取当前anim sequence
		virtual const TCHAR* get_anim_playing() const = 0;

		// advance animation
		virtual void advance_by(float move_deta, float delta_time, bool fire_notifies) = 0; 

		virtual void issue_notifies(float delta_time) = 0;

		virtual void on_anim_end(const nstring& sequence, float played_time) = 0;

		nDECLARE_VIRTUAL_CLASS(nanim_controller)
	};
}//namespace nexus

#endif //_NEXUS_ANIM_CONTROLLER_H_