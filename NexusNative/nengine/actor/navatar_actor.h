#ifndef _NEXUS_AVATAR_ACTOR_H_
#define _NEXUS_AVATAR_ACTOR_H_
#include "../framework/nactor.h"
#include "../resource/nresource.h"
#include "../animation/nskel_anim_play.h"
namespace nexus
{
	class nAPI navatar_actor :
		public nactor
	{
	public:
		typedef shared_ptr<navatar_actor> ptr;

		navatar_actor(const nstring& name_str);
		virtual ~navatar_actor(void);

		void create(const resource_location& anim_set_loc);

		virtual void update(float delta_time, const nviewport& view);

		/**
		 *	添加一个skeletal mesh组件，用来显示游戏角色的盔甲等随动作变形的装备
		*/
		void bind_skeletal_mesh(const nstring& comp_name, const resource_location& res_loc);
		
		/**
		 *	添加一个挂件，并绑定到指定的骨骼，用来显示游戏角色的武器等硬质的东西
		*/
		void bind_attachment(const nstring& comp_name, const nstring& attach_bone, const resource_location& res_loc);
		

		/**
		 *	使用基类的remove_component()接口可以去除某个组件
		*/

		nskel_anim_controller::ptr get_anim_ctrl() const	{	return m_anim_ctrl;}


		/**
		* 添加动画播放接口方便CLI层封装
		*/
		virtual void set_anim(const nstring& sequence);
		virtual void play_anim(bool loop, float rate, float start_time);
		virtual void stop_anim();
		virtual void set_position(float new_time, bool fire_notifies);
		// 获取当前anim sequence剩余时间
		virtual float get_time_left() const;
		// 获取当前anim sequence总长度
		virtual float get_anim_length() const;
		// 获取当前anim sequence总帧数
		virtual size_t get_anim_frame() const;
		// 获取当前anim sequence
		virtual const TCHAR* get_anim_playing() const;

	protected:
		shared_ptr<nskel_anim_play>	m_anim_ctrl;

		nDECLARE_NAMED_CLASS(navatar_actor)
	};
	
}//namespace nexus

#endif //_NEXUS_AVATAR_ACTOR_H_