#include "StdAfx.h"
#include "nmesh_anim_controller.h"

namespace nexus
{
	nmesh_anim_controller::nmesh_anim_controller(void)
	{
		m_cur_seq = 0;
		m_cur_frame = 0;
		m_next_frame = 0;
		m_frame_time = 0;
		m_playing = false;
		m_vert_dirty = true;
		m_loop_mode = EAnim_Loop;
	}

	nmesh_anim_controller::~nmesh_anim_controller(void)
	{
	}

	void nmesh_anim_controller::init(nresource_anim_mesh::ptr res_ptr, bool auto_play)
	{
		nASSERT(res_ptr);
		nASSERT(res_ptr->ready());

		m_res = res_ptr;
		
		m_cur_seq = 0;
		m_cur_frame = 0;
		m_next_frame = 0;
		m_frame_time = 0;		

		m_playing = auto_play;

		//-- create vertex data
		nanim_mesh_sequence::ptr seq = m_res->get_mesh_data()->m_sequence_array[m_cur_seq];

		shared_ptr<nmesh_vertex_data_common_morph> src_vb( nNew nmesh_vertex_data_common_morph );
		src_vb->init(seq->get_num_vert(), 2, 1);

		shared_ptr<nmesh_vertex_data_common_morph> dst_vb( nNew nmesh_vertex_data_common_morph );
		dst_vb->init(seq->get_num_vert(), 2, 1);

		m_status.src_frame_vert = src_vb;
		m_status.dst_frame_vert = dst_vb;
		m_status.alpha = 0;

		m_vert_dirty = true;
		update_render_data(true);
	}

	void nmesh_anim_controller::update_render_data(bool do_init)
	{
		if( m_vert_dirty )
		{
			copy_vertex_data(m_status.src_frame_vert, m_cur_frame, do_init);
			copy_vertex_data(m_status.dst_frame_vert, m_next_frame, do_init);		

			m_vert_dirty = false;
		}
	}

	void nmesh_anim_controller::copy_vertex_data(nmesh_vertex_data::ptr vb, size_t frm, bool do_init)
	{
		nanim_mesh_sequence::ptr seq = m_res->get_mesh_data()->m_sequence_array[m_cur_seq];

		vertex_stream* pos_stream = vb->get_stream(0);
		vertex_stream* second_stream = vb->get_stream(1);

		struct vertex
		{
			vector3	normal;
			vector3	tangent;
			vector2	uv;
		};

		vector3* vert_pos = reinterpret_cast<vector3*>(pos_stream->get_data());
		vertex*	vert_sec = reinterpret_cast<vertex*>(second_stream->get_data());

		size_t num_vert = seq->get_num_vert();

		if( seq->has_pos_keys() )
		{
			for(size_t i=0; i<num_vert; i++)
			{
				const nanim_mesh_track& track = seq->m_tracks[i];
				*vert_pos = track.m_pos_keys[frm];
				vert_pos++;
			}
		}
		else
		{
			if( do_init )
			{
				for(size_t i=0; i<num_vert; i++)
				{
					const nanim_mesh_track& track = seq->m_tracks[i];
					*vert_pos = track.m_pos_keys[0];
					vert_pos++;
				}
			}//end of if
		}

		if( seq->has_normal_keys() )
		{
			vertex* vert_iter = vert_sec;
			for(size_t i=0; i<num_vert; i++)
			{
				const nanim_mesh_track& track = seq->m_tracks[i];
				vert_iter->normal = track.m_normal_keys[frm];
				vert_iter++;
			}
		}
		else
		{
			if( do_init )
			{
				vertex* vert_iter = vert_sec;
				for(size_t i=0; i<num_vert; i++)
				{
					const nanim_mesh_track& track = seq->m_tracks[i];
					vert_iter->normal = track.m_normal_keys[0];
					vert_iter++;
				}
			}// end of if
		}


		if( seq->has_tangent_keys() )
		{
			vertex* vert_iter = vert_sec;
			for(size_t i=0; i<num_vert; i++)
			{
				const nanim_mesh_track& track = seq->m_tracks[i];
				vert_iter->tangent = track.m_tangent_keys[frm];
				vert_iter++;
			}
		}
		else
		{
			if( do_init )
			{
				vertex* vert_iter = vert_sec;
				for(size_t i=0; i<num_vert; i++)
				{
					const nanim_mesh_track& track = seq->m_tracks[i];
					vert_iter->tangent = track.m_tangent_keys[0];
					vert_iter++;
				}
			}// end of if
		}

		if( seq->has_uv_keys() )
		{
			vertex* vert_iter = vert_sec;
			for(size_t i=0; i<num_vert; i++)
			{
				const nanim_mesh_track& track = seq->m_tracks[i];
				vert_iter->uv = track.m_uv_keys[frm];
				vert_iter++;
			}
		}
		else
		{
			if( do_init )
			{
				vertex* vert_iter = vert_sec;
				for(size_t i=0; i<num_vert; i++)
				{
					const nanim_mesh_track& track = seq->m_tracks[i];
					vert_iter->uv = track.m_uv_keys[0];
					vert_iter++;
				}
			}
		}
	}

	void nmesh_anim_controller::advance_anim(float delta_time)
	{
		if( !m_playing )
			return;

		nanim_mesh_sequence::ptr seq = m_res->get_mesh_data()->m_sequence_array[m_cur_seq];
		size_t num_frame = seq->get_num_frame();

		//--
		m_frame_time += delta_time;
		while( m_frame_time > seq->get_frame_time(m_cur_frame))
		{
			m_frame_time -= seq->get_frame_time(m_cur_frame);
			m_cur_frame++;			
			m_vert_dirty = true;

			if( m_cur_frame >= num_frame )
			{
				if( m_loop_mode == EAnim_Loop )
					m_cur_frame = 0;
				else if( m_loop_mode == EAnim_PlayOnce )
				{
					m_cur_frame = num_frame-1;
					m_frame_time = 0;
					m_playing = false;					
					break;
				}
			}//end of if			
		}

		//--
		m_next_frame = m_cur_frame+1;
		if(m_next_frame >= num_frame)
		{
			if( m_loop_mode == EAnim_Loop )
				m_next_frame = 0;
			else if( m_loop_mode == EAnim_PlayOnce )
			{
				m_next_frame = m_cur_frame;				
			}
		}

		m_status.alpha = 1.0f-( m_frame_time/seq->get_frame_time(m_cur_frame) );
	}

	void nmesh_anim_controller::play_anim(const nstring& seq_name, bool loop, float start_time)
	{
		nanim_mesh::ptr mesh = m_res->get_mesh_data();
		for(size_t i=0; i<mesh->m_sequence_array.size(); i++)
		{
			if( seq_name == mesh->m_sequence_array[i]->m_name )
			{
				m_cur_seq = i;
			}
		}

		m_cur_frame = 0;
		m_frame_time = 0;

		m_loop_mode = loop?EAnim_Loop:EAnim_PlayOnce;
		m_playing = true;

		advance_anim(start_time);
	}

	void nmesh_anim_controller::stop_anim()
	{
		m_playing = false;
	}

	box_sphere_bounds nmesh_anim_controller::get_bounds() const
	{
		nanim_mesh_sequence::ptr seq = m_res->get_mesh_data()->m_sequence_array[m_cur_seq];

		if( seq->m_frame_bounds.size() < 1)
			return seq->m_frame_bounds[0];

		size_t frm = 0;
		if( m_status.alpha > 0.5f )
			frm = m_next_frame;
		else
			frm = m_cur_frame;

		return seq->m_frame_bounds[frm];
	}
}//namespace nexus