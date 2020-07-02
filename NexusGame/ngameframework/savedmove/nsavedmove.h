#ifndef _NEXUS_GAMEFRAMEWORK_SAVEDMOVE_H_
#define _NEXUS_GAMEFRAMEWORK_SAVEDMOVE_H_
#include "ncore.h"
#include "../character_controller/nmovement_define.h"
#include "replicate_move_callback.h"

namespace gameframework
{
	class ncharacter_controller;

	/**
	* class nsavedmove
	* @remark	主要用途为判断是否需要同步移动提供一种统一的策略
				记录网络玩家或者NPC最近时间内的移动，
				定义移动消息的同步策略，和数据压缩
				为了更好的节省移动同步消息量
	*/
	class nGameFrameworkAPI nsavedmove
	{
	public:
		typedef uint8	move_flags;
		typedef boost::shared_ptr<nsavedmove> ptr;

		enum move_flag_type
		{
			EMFT_Walk		= 1<<0,
			EMFT_Jump		= 1<<1,
			EMFT_Fly		= 1<<2,
			// 注意PressJump才是真正控制角色跳跃的参数，EMFT_Jump只是标识当前的状态为JUMP
			// 避免因为网络不稳定导致的远程角色多次跳跃问题
			EMFT_PressJump	= 1<<3,

			EMFT_Custom		= 1<<7,
		};

		nsavedmove(replicate_move_callback_ptr replicate);
		virtual ~nsavedmove();

		// init
		virtual void init(ncharacter_controller* controller);
		
		// update
		virtual void update(float delta_time, ncharacter_controller* controller);

		// 压缩/解压缩移动参数并且设置给角色控制器
		// 将部分移动参数压缩，包括m_walk，jump, fly等
		// param: custom_flag 允许外部设置一些自定义的标记，该函数将使用只设置m_walk，jump, fly，其他标志位保留
		static move_flags compressed_controller_flags(ncharacter_controller* cc, move_flags custom_flag);
		static void flags_to_controller(move_flags flags, ncharacter_controller* cc);

		const vector3& get_saved_acceleration() const
		{
			return m_saved_acceleration;
		}

		const vector3& get_saved_velocity() const
		{
			return m_saved_velocity;
		}

		const vector3& get_saved_location()const
		{
			return m_saved_location;
		}

		const vector3& get_saved_rotation() const
		{
			return m_saved_rotation;
		}

		bool get_saved_walk() const
		{
			return m_walk;
		}

		EMovementType get_saved_movement_type() const
		{
			return m_movement_type;
		}


	protected:
		// 发送移动同步
		virtual void replicate_move();

		// 判断是否是重要移动
		virtual bool is_important_movement(ncharacter_controller* controller);

	private:
		void reset_move_parm(ncharacter_controller* controller);

	protected:
		// 最大同步间隔时间(单位: s)，同步间隔大于这个值是必须同步一次
		float m_max_response_time;

		// 记录本次移动的时间戳
		float m_timestamp;
		// 是走/跑，true:走 false:跑
		bool m_walk;
		// 移动类型
		EMovementType m_movement_type;
		// 位置
		vector3 m_saved_location;
		// 速度
		vector3 m_saved_velocity;
		// 加速度
		vector3 m_saved_acceleration;
		// 方向,弧度为单位
		vector3 m_saved_rotation;
		// 加速度阀值
		float m_acceldotthreshold;
	
		// 统计replicate move次数
		unsigned int m_num_replicates;

		// replicate回调机制
		replicate_move_callback_ptr m_replicate_move_callback;
	};
}

#endif // end of _NEXUS_GAMEFRAMEWORK_SAVEDMOVE_H_