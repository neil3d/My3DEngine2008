#ifndef _NEXUS_MESH_ANIM_CONTROLLER_H_
#define _NEXUS_MESH_ANIM_CONTROLLER_H_
#include "anim_define.h"
#include "nanim_controller.h"
#include "../resource/nresource_anim_mesh.h"

namespace nexus
{
	class nAPI nmesh_anim_controller :
		public nanim_controller
	{
	public:
		typedef shared_ptr<nmesh_anim_controller> ptr;

		nmesh_anim_controller(void);
		virtual ~nmesh_anim_controller(void);

		void init(nresource_anim_mesh::ptr res_ptr, bool auto_play);
		
		virtual void tick_anim(float delta_time);
		//void advance_anim(float delta_time);

		virtual nanim_mesh_sequence::ptr find_anim_sequence(const nstring& sequence) const;
		virtual void set_anim(const nstring& sequence);
		virtual void play_anim(bool loop, float rate, float start_time);
		virtual void stop_anim();
		virtual void set_position(float new_time, bool fire_notifies);

		virtual float get_time_left() const;
		virtual float get_anim_length() const;
		virtual size_t get_anim_frame() const;
		virtual const TCHAR* get_anim_playing() const;

		virtual void advance_by(float move_deta, float delta_time, bool fire_notifies); 
		virtual void issue_notifies(float delta_time);
		virtual void on_anim_end(const nstring& sequence, float played_time);

		//void play_anim(const nstring& seq_name, bool loop, float start_time = 0);

		const anim_mesh_morph& get_anim_status() const	{	return m_status;}
		box_sphere_bounds get_bounds() const;
	
		void update_render_data(bool do_init=false);
	
	private:
		void copy_vertex_data(nmesh_vertex_data::ptr vb, size_t frm, bool do_init);
	private:
		anim_mesh_morph				m_status;
		nresource_anim_mesh::ptr	m_res;

		nanim_mesh_sequence::ptr m_cur_seq_ptr;
		float			m_rate;
		//size_t			m_cur_seq;
		size_t			m_cur_frame;
		size_t			m_next_frame;
		bool			m_vert_dirty;

		// 当前Sequence播放的时间
		float			m_frame_time;
		bool			m_playing;
		enum EAnimLoop	m_loop_mode;
	};
}//namespace nexus

#endif //_NEXUS_MESH_ANIM_CONTROLLER_H_