#ifndef _NEXUS_SKEL_ANIM_PLAY_H_
#define _NEXUS_SKEL_ANIM_PLAY_H_
#include "nskel_anim_controller.h"

namespace nexus
{
	class nAPI nskel_anim_play :
		public nskel_anim_controller
	{
	public:
		nskel_anim_play(void);
		virtual ~nskel_anim_play(void);

		void init(nresource_skeletal_anim_set::ptr anim_set, bool auto_play);

		virtual void tick_anim(float delta_time);

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

		// advance animation
		virtual void advance_by(float move_deta, float delta_time, bool fire_notifies); 

		virtual void issue_notifies(float delta_time);

		virtual void on_anim_end(const nstring& sequence, float played_time);


		void update_matrix_palette();

		virtual const nskeleton_define* get_skeleton() const
		{
			if( m_anim_set )
				return &(m_anim_set->get_skeleton());
			else
				return NULL;
		}
	private:
		virtual void set_anim(nskeletal_anim_sequence::ptr sequence);

	private:
		nresource_skeletal_anim_set::ptr	m_anim_set;
		nskeletal_anim_sequence::ptr		m_cur_sequence;

		size_t			m_cur_frame;
		size_t			m_next_frame;
		bool			m_matrix_dirty;

		float			m_rate;
		float			m_frame_time;
		bool			m_playing;
		enum EAnimLoop	m_loop_mode;

		nDECLARE_CLASS(nskel_anim_play)
	};
}//namespace nexus

#endif //_NEXUS_SKEL_ANIM_PLAY_H_