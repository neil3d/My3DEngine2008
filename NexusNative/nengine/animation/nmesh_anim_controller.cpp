#include "StdAfx.h"
#include "nmesh_anim_controller.h"

namespace nexus
{
	nmesh_anim_controller::nmesh_anim_controller(void)
	{
		m_rate = 1.0f;
		m_cur_frame = 0;
		m_next_frame = 0;
		m_frame_time = 0.f;
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
		m_frame_time = 0.f;		
		m_cur_frame = 0;
		m_next_frame = 0;

		//-- create vertex data
		nanim_mesh::ptr lod_mesh = m_res->get_lod_mesh(0);
		nASSERT(lod_mesh->m_sequence_array.size() > 0);
		nanim_mesh_sequence::ptr seq = lod_mesh->m_sequence_array[0];
		if(auto_play)
		{
			set_anim(seq->m_name);
			play_anim(true, 1.0f, 0.f);
		}

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
		if(!m_cur_seq_ptr)
			return;

		nanim_mesh_sequence::ptr seq = m_cur_seq_ptr;

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

	//void nmesh_anim_controller::advance_anim(float delta_time)
	//{
	//	if( !m_playing )
	//		return;

	//	nanim_mesh::ptr lod_mesh = m_res->get_lod_mesh(0);
	//	nASSERT(m_cur_seq < lod_mesh->m_sequence_array.size());
	//	nanim_mesh_sequence::ptr seq = lod_mesh->m_sequence_array[m_cur_seq];
	//	float frame_time = seq->get_frame_time(m_cur_frame);
	//	size_t num_frame = seq->get_num_frame();

	//	//--
	//	m_vert_dirty = true;
	//	m_frame_time += delta_time;
	//	while( m_frame_time > frame_time)
	//	{
	//		m_frame_time -= frame_time;
	//		++ m_cur_frame;			

	//		if( m_cur_frame >= num_frame )
	//		{
	//			if( m_loop_mode == EAnim_Loop )
	//			{
	//				m_cur_frame = 0;
	//			}
	//			else if( m_loop_mode == EAnim_PlayOnce )
	//			{
	//				nLog_Debug(_T("Play animation complete [%s]"), get_anim_playing());

	//				// 终止动画在最后一帧
	//				m_playing = false;		
	//				m_cur_seq = 0;
	//				m_cur_frame = num_frame-1;
	//				m_next_frame = 0;
	//				m_frame_time = 0;

	//				return;
	//			}
	//		}//end of if	

	//		frame_time = seq->get_frame_time(m_cur_frame);
	//	}

	//	//--
	//	m_next_frame = m_cur_frame+1;
	//	if(m_next_frame >= num_frame)
	//	{
	//		//nASSERT(m_loop_mode == EAnim_Loop);
	//		m_next_frame = 0;
	//	}

	//	nASSERT(m_next_frame <= num_frame);
	//	m_status.alpha = 1.0f-( m_frame_time/seq->get_frame_time(m_cur_frame) );
	//}

	//void nmesh_anim_controller::play_anim(const nstring& seq_name, bool loop, float start_time)
	//{
	//	size_t old_seq = m_cur_seq;

	//	bool has_seq = false;
	//	nanim_mesh::ptr mesh = m_res->get_lod_mesh(0);
	//	for(size_t i=0; i<mesh->m_sequence_array.size(); i++)
	//	{
	//		if( seq_name == mesh->m_sequence_array[i]->m_name )
	//		{
	//			m_cur_seq = i;
	//			has_seq = true;
	//			break;
	//		}
	//	}

	//	if(!has_seq)
	//	{
	//		nLog_Warning(_T("Can't find animation sequene [%s]...\r\n"), seq_name.c_str());
	//		return;
	//	}

	//	// 如果调用的动画就是当前播放的动画,而且start_time为负数,则不重新开始播放
	//	m_loop_mode = loop ? EAnim_Loop : EAnim_PlayOnce;
	//	m_playing = true;
	//	m_vert_dirty = true;

	//	if(m_cur_seq != old_seq	)
	//	{
	//		m_cur_frame = 0;
	//		m_next_frame = 0;
	//		m_frame_time = 0;			
	//	}

	//	if(start_time > nEPS)
	//		advance_anim(start_time);
	//}

	//void nmesh_anim_controller::stop_anim()
	//{
	//	m_playing = false;
	//	m_cur_seq = 0;
	//	m_cur_frame = 0;
	//	m_next_frame = 0;
	//	m_frame_time = 0;
	//}


	//void nmesh_anim_controller::pause_anim()
	//{
	//	m_playing = false;
	//}

	box_sphere_bounds nmesh_anim_controller::get_bounds() const
	{
		nanim_mesh_sequence::ptr seq;
		if(!m_cur_seq_ptr)
		{
			nanim_mesh::ptr lod_mesh = m_res->get_lod_mesh(0);
			seq = lod_mesh->m_sequence_array[0];
		}
		else
		{
			seq = m_cur_seq_ptr;
		}


		if( seq->m_frame_bounds.size() < 1)
			return seq->m_frame_bounds[0];

		size_t frm = 0;
		if( m_status.alpha > 0.5f )
			frm = m_next_frame;
		else
			frm = m_cur_frame;

		return seq->m_frame_bounds[frm];
	}

	
	void nmesh_anim_controller::set_anim( const nstring& sequence )
	{
		nanim_mesh_sequence::ptr ret = find_anim_sequence(sequence);
		if(ret)
		{
			m_cur_seq_ptr = ret;
		}
		else
		{
			m_cur_seq_ptr.reset();
			stop_anim();
			nLog_Warning(_T("Can't find animation sequene [%s]...\r\n"), sequence.c_str());
		}
	}

	nanim_mesh_sequence::ptr nmesh_anim_controller::find_anim_sequence( const nstring& sequence ) const
	{
		nanim_mesh_sequence::ptr ret;

		nanim_mesh::ptr mesh = m_res->get_lod_mesh(0);
		for(size_t i=0; i<mesh->m_sequence_array.size(); i++)
		{
			if( sequence == mesh->m_sequence_array[i]->m_name )
			{
				ret = mesh->m_sequence_array[i];

				break;
			}
		}
		return ret;
	}

	void nmesh_anim_controller::play_anim( bool loop, float rate, float start_time )
	{
		m_loop_mode = loop ? EAnim_Loop : EAnim_PlayOnce;
		m_rate = rate;
		m_playing = true;
		m_vert_dirty = true;

		set_position(start_time,false);
	}

	void nmesh_anim_controller::stop_anim()
	{
		m_playing = false;
	}

	void nmesh_anim_controller::set_position( float new_time, bool fire_notifies )
	{
		//const float anim_length = m_cur_seq_ptr ? m_cur_seq_ptr->get_length() : 0.f;
		//new_time = clamp<float>(new_time, 0.f, anim_length + nEPS);

		const float delta_time = new_time - m_frame_time;
		const float move_deta = m_rate * delta_time;
		advance_by(move_deta, delta_time, fire_notifies);
	}

	float nmesh_anim_controller::get_time_left() const
	{
		if(m_cur_seq_ptr)
		{
			return std::max(m_cur_seq_ptr->get_length() - m_frame_time, 0.f);
		}
		return 0.f;
	}

	float nmesh_anim_controller::get_anim_length() const
	{
		return m_cur_seq_ptr ? m_cur_seq_ptr->get_length() : 0.f;
	}

	size_t nmesh_anim_controller::get_anim_frame() const
	{
		return m_cur_seq_ptr ? m_cur_seq_ptr->get_num_frame() : 0;
	}


	const TCHAR* nmesh_anim_controller::get_anim_playing() const
	{
		return m_cur_seq_ptr ? m_cur_seq_ptr->m_name.c_str() : NULL;
	}

	void nmesh_anim_controller::advance_by( float move_deta, float delta_time, bool fire_notifies )
	{
		if(!m_cur_seq_ptr || move_deta == 0.f || delta_time == 0.f)
		{
			return;
		}

		if(fire_notifies && move_deta > nEPS)
		{
			issue_notifies(move_deta);
		}

		// 增加时间
		m_frame_time += move_deta;

		const float anim_length = m_cur_seq_ptr->get_length();
		if(m_frame_time - anim_length > FLT_EPSILON)
		{
			if(m_loop_mode == EAnim_Loop)
			{
				m_frame_time = fmodf(m_frame_time, anim_length);
			}
			else
			{
				const float move_rate = move_deta / delta_time;
				const float excess_time = (m_frame_time - anim_length) / move_rate;
				m_frame_time = anim_length;
				stop_anim();

				on_anim_end(m_cur_seq_ptr->m_name, delta_time - excess_time);
			}
		}
		else if(m_frame_time < FLT_EPSILON)
		{
			if(m_loop_mode == EAnim_Loop)
			{
				m_frame_time = fmodf(m_frame_time, anim_length);
				if( m_frame_time < 0.f )
				{
					m_frame_time += anim_length;
				}
			}
			else 
			{
				// find rate of this movement.
				const float move_rate = move_deta / delta_time;

				// figure out by how much we've reached beyond beginning of animation.
				// this is useful for transitions.
				const float excess_time = m_frame_time / abs(move_rate);
				m_frame_time = anim_length;
				stop_anim();

				// notify that animation finished playing
				on_anim_end(m_cur_seq_ptr->m_name, delta_time - excess_time);
			}
		}

		/// 设置顶点更新标志
		m_vert_dirty = true;

		float frame_time = 0.f;
		size_t num_frame = m_cur_seq_ptr->get_num_frame();
		size_t i = 0;
		for (; i < num_frame; i++)
		{
			float temp_frame_time = m_cur_seq_ptr->get_frame_time(i);
			frame_time += temp_frame_time;
			if( m_frame_time <= frame_time)
			{
				m_status.alpha = clamp(1.0f - ((m_frame_time- (frame_time - temp_frame_time ))/temp_frame_time), 0.0f, 1.0f);
				break;
			}
		}
	
		if(m_cur_frame != i)
		{
			m_cur_frame = i;
			m_next_frame = m_cur_frame + 1;
			if(m_next_frame == num_frame)
			{
				m_next_frame = (m_loop_mode == EAnim_Loop) ? (m_next_frame = 0) : (m_next_frame = m_cur_frame);
			}
			nASSERT(m_next_frame < num_frame);
		}
	}

	void nmesh_anim_controller::tick_anim( float delta_time )
	{
		// Can only do something if we are currently playing a valid AnimSequence
		if( m_playing && m_cur_seq_ptr )
		{
			// Time to move forwards by - DeltaSeconds scaled by various factors.
			const float move_deta = m_rate * delta_time;
			advance_by(move_deta, delta_time, true);
		}
	}

	void nmesh_anim_controller::issue_notifies( float delta_time )
	{
		(void)delta_time;
	}


	void nmesh_anim_controller::on_anim_end( const nstring& sequence, float played_time )
	{
		(void)sequence;
		(void)played_time;
	}
}//namespace nexus