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
		virtual void advance_anim(float delta_time);

		void play_anim(const nstring& seq_name, bool loop, float start_time = 0);
		void stop_anim();

		void update_matrix_palette();

		virtual const nskeleton_define* get_skeleton() const
		{
			if( m_anim_set )
				return &(m_anim_set->get_skeleton());
			else
				return NULL;
		}
	private:
		nresource_skeletal_anim_set::ptr	m_anim_set;
		nskeletal_anim_sequence::ptr		m_cur_sequence;

		size_t			m_cur_frame;
		size_t			m_next_frame;
		bool			m_matrix_dirty;

		float			m_frame_time;
		bool			m_playing;
		enum EAnimLoop	m_loop_mode;

		nDECLARE_CLASS(nskel_anim_play)
	};
}//namespace nexus

#endif //_NEXUS_SKEL_ANIM_PLAY_H_