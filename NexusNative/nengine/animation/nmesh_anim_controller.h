#ifndef _NEXUS_MESH_ANIM_CONTROLLER_H_
#define _NEXUS_MESH_ANIM_CONTROLLER_H_
#include "anim_define.h"
#include "nanim_controller.h"
#include "../resource/nresource_anim_mesh.h"

namespace nexus
{
	class nmesh_anim_controller :
		public nanim_controller
	{
	public:
		typedef shared_ptr<nmesh_anim_controller> ptr;

		nmesh_anim_controller(void);
		virtual ~nmesh_anim_controller(void);

		void init(nresource_anim_mesh::ptr res_ptr, bool auto_play);
		void advance_anim(float delta_time);

		void play_anim(const nstring& seq_name, bool loop, float start_time = 0);
		void stop_anim();
		const nstring& get_anim_playing() const	{	return m_cur_seq_name;}

		const anim_mesh_morph& get_anim_status() const	{	return m_status;}
		box_sphere_bounds get_bounds() const;
	
		void update_render_data(bool do_init=false);
	
	private:
		void copy_vertex_data(nmesh_vertex_data::ptr vb, size_t frm, bool do_init);
	private:
		anim_mesh_morph				m_status;
		nresource_anim_mesh::ptr	m_res;

		nstring			m_cur_seq_name;
		size_t			m_cur_seq;
		size_t			m_cur_frame;
		size_t			m_next_frame;
		bool			m_vert_dirty;

		float			m_frame_time;
		bool			m_playing;
		enum EAnimLoop	m_loop_mode;
	};
}//namespace nexus

#endif //_NEXUS_MESH_ANIM_CONTROLLER_H_