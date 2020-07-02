#ifndef _NEXUS_GAMEFRAMEWORK_CHARACTER_CONTROLLER_H_
#define _NEXUS_GAMEFRAMEWORK_CHARACTER_CONTROLLER_H_
#include "ncore.h"
#include "ntype_def.h"
#include "nmovement_define.h"
#include "../world/ngame_level.h"
#include "../savedmove/nsavedmove.h"

namespace gameframework
{
	class ndebug_draw_interface;
	
	class character_evnet_arg : public nevent_arg
	{
	public:
		ncharacter_controller* m_character;

		explicit character_evnet_arg(ncharacter_controller* _character) : m_character(_character) {};
	};

	class character_hit_evnet_arg : public character_evnet_arg
	{
	public:
		ncheck_result*		m_hit;

		explicit character_hit_evnet_arg(ncharacter_controller* _character, ncheck_result* hit) 
			: character_evnet_arg(_character),m_hit(hit)
		{};
	};



	/**
	* 依赖于导航图的角色控制基类，该类定义了角色运动相关的参数和角色运动控制
	* 游戏逻辑层可以继承该类实现不同策略的角色控制
	*/
	class nGameFrameworkAPI ncharacter_controller 
		: public nexus::nobject
	{
	public:
		typedef boost::shared_ptr<ncharacter_controller> ptr;

		// 事件类型
		typedef nevent_handler<void, character_evnet_arg*> character_event;
		typedef nevent_handler<void, character_hit_evnet_arg*> character_hit_event;


		ncharacter_controller();
		virtual ~ncharacter_controller();
	
		// 初始化
		virtual bool init() { return true; }
		
		// 销毁时调用
		virtual void destroy() {}

		// update,需要在每个Tick中调用
		virtual void update(float delta_time);

		// 设置savedmove，一般只设置一次即可
		void set_saved_move(nsavedmove* savedmove_ptr);

		// 移动角色
		// level: 在哪个场景中运动，决定角色与场景的碰撞
		// delta_time: 时间以s为单位
		virtual void move(ngame_level* level, float delta_time);

		// 传动到某个点
		// param dest_pos: 需要传动到的目标点
		// param place_nav_above: 是否需要将角色放置到导航图的上面
		virtual bool transport(ngame_level* level, const vector3& dest_pos, bool place_nav_above);

		// debug draw interface
		virtual void draw_debug_info(ndebug_draw_interface* ddi);

		// 当前是否允许跳跃
		virtual bool can_jump () const;

		// 切换到跳跃,执行跳跃命令
		virtual void jump();

		// 当前是否允许飞行
		virtual bool can_fly() const;
		// 切换到飞行状态
		virtual void fly();

		//-- 目标测试接口
		bool reached_destination(const vector3& test_position, const vector3&  destination)
		{
			return reach_threshold_test(test_position, destination, 40, 40, 40);
		}

		// 根据阀值计算有没有到达目标点
		bool reach_threshold_test(const vector3&  test_position, const vector3&  destination, float upThresholdAdjust, float downThresholdAdjust, float thresholdAdjust);

		// 重置角色位置
		void set_position(const vector3& position);
		// 获取当前角色位置
		const vector3& get_position() const;

		// 重置角色方向
		void set_rotation(const vector3& rot);
		// 获取当前角色方向
		const vector3& get_rotation() const;

		// 设置角色的加速度
		void set_acceleration(const vector3& accel)
		{
			m_acceleration = vec_normalize(accel);
		}
		// 获取角色当前加速度
		const vector3& get_acceleration() const
		{
			return m_acceleration;
		}

		// 设置角色的加速度
		void set_velocity(const vector3& velocity)
		{
			m_velocity = velocity;
		}
		// 获取角色当前加速度
		const vector3& get_velocity() const
		{
			return m_velocity;
		}

		EMovementType get_current_movement_type() const { return m_current_movement_type; }

		// 改变当前移动类型
		virtual void set_current_movement_type(EMovementType move_type);

		// 是走/跑
		bool get_walk() const {  return m_walk; }
		void set_walk(bool value) { m_walk = value; }
		
		// 眼睛的高度
		float get_eyes_height() const { return m_eyes_height; }
		void set_eyes_height(float value) { m_eyes_height = value; } 

		float get_jump_speed() const {  return m_jump_speed; }
		void set_jump_speed(float value) { m_jump_speed = value; }

		float get_speedup() const {  return m_speedup; }
		void set_speedup(float value) { m_speedup = value; }

		float get_max_ground_speed() const {  return m_max_ground_speed; }
		void set_max_ground_speed(float value) { m_max_ground_speed = value; }

		float get_max_fly_speed() const {  return m_max_fly_speed; }
		void set_max_fly_speed(float value) { m_max_fly_speed = value; }

		uint64 get_character_id() const {  return m_character_id; }
		void set_character_id(uint64 value) { m_character_id = value; }

		// 落地(移动时触到地面)，内部调用
		void _on_land();

	protected:
		// 计算当前所有的合理和根据运动状态返回的最大速度
		// param force: 返回所有合力
		// param hor_max_speed: 水平最大速度
		// param vert_max_speed: 垂直最大速度
		virtual void composite_force(vector3 &force, float &hor_max_speed, float &vert_max_speed);

		// 在每次调用完move函数之后调用，用于重载该函数以实现网络同步等功能
		virtual void post_move() {}

		// 供子类重载自定义的移动参数计算
		virtual void custom_move(vector3& /*force*/, float& /*hor_max_speed*/, float& /*vert_max_speed*/) {}

		// 计算最大速度限制率，返回[0,1]
		virtual float maxspeed_modifier();

		// 移动方式发生改变的回调
		virtual void notify_ground();
		virtual void notify_jump();
		virtual void notify_fly();
		virtual void notify_custom();
		virtual void notify_hit(ncheck_result* hit);

	public:
		// 定义事件，外部使用方式如下,参考/event_delegate/sample.txt：
		// test_delegate_class test_class;
		// ground_event += new character_event::T<test_delegate_class>(&test_class,&test_delegate_class::do_somthing);
		// 角色切换到EMove_Ground移动状态事件
		character_event		ground_event;
		// 角色切换到EMove_Jump移动状态事件
		character_event		jump_event;
		// 角色切换到EMove_Fly移动状态事件
		character_event		fly_event;
		// 角色切换到EMove_Custom移动状态事件
		character_event		custom_event;

		// 移动是遇到阻挡
		character_hit_event	hit_event;

	private:
		// 角色ID
		uint64		m_character_id;

		// 角色位置
		vector3		m_location;
		// 角色朝向
		vector3		m_rotation;
		// 当前速度，收内部计算不断在变化
		vector3		m_velocity;
		// 角色加速度,只收外部设置改变
		vector3		m_acceleration;
		// 当前移动类型
		EMovementType m_current_movement_type;

		// 是走/跑， true：走 false：跑
		bool		m_walk;

		//-- 角色运动能力属性
		// 角色所能爬的最大坡度,~= cos( *°)
		float		m_slope_limit;
		// 步阶
		float		m_step_height;

		// 加速率
		float		m_accel_rate;

		// 起跳速度
		float		m_jump_speed;

		// 在地面上跑步的最大速度
		float		m_max_ground_speed;
		// 走的最大速度 = 跑的最大速度 * m_walking_modifier
		float		m_walking_modifier;
		float		m_max_fly_speed;
		// 加速指，供外部设置最大速度限制用
		float		m_speedup;

		// 眼睛的高度
		float		m_eyes_height;

		// 用于判断是否需要移动同步
		nsavedmove* m_saved_move;
	};
}

#endif // end of _NEXUS_GAMEFRAMEWORK_CHARACTER_CONTROLLER_H_