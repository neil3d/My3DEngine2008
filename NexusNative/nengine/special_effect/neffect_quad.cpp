#include "StdAfx.h"
#include "neffect_quad.h"
#include "../framework/nengine.h"

namespace nexus
{
	//-- 
	void neffect_quad_instance::init(shared_ptr<neffect_element> def)
	{
		neffect_element_instance::init(def);

		//--
		neffect_quad* quad_def = dynamic_cast<neffect_quad*>( def.get() );

		m_face_type = quad_def->m_face_type;
		m_tail_fade_speed = 1.0f/quad_def->m_num_tail_faces*quad_def->m_tail_interval;

		//-- init quad array
		int num_faces = quad_def->m_num_tail_faces+1;
		m_faces.resize( num_faces );		
		const nsub_uv& suv = quad_def->m_sub_uv;

		for (int i=0; i<num_faces; i++)
		{
			quad_face& face = m_faces[i];

			face.uv_start.x = suv.get_sub_u(0);
			face.uv_start.y = suv.get_sub_v(0);
			face.uv_end.x = suv.get_sub_u(1);
			face.uv_end.y = suv.get_sub_v(1);
			face.color=color4f(1,1,1,1);
		}

		//-- create memory vertex data
		int num_triangle = num_faces*2;
		int num_vert = num_triangle*3;

		m_vert_data.init(num_vert);

		//-- create render mesh
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		m_mesh.reset( rres_mgr->alloc_dynamic_mesh() );
		m_mesh->create(EDraw_TriangleList, &m_vert_data, 0);
	}

	bool neffect_quad_instance::update_render_data(const nviewport& view, float play_time, const matrix44& owner_mat)
	{		
		(void)view;		
		(void)play_time;

		if( !m_active )
			return false;

		quad_face::build_mesh_vertices(m_vert_data, m_faces, view.camera, owner_mat, m_face_type);
		m_mesh->update_vertices(&m_vert_data);
		m_mesh->set_draw_limit(m_active_face*2);

		return true;
	}

	void neffect_quad_instance::update_tail(neffect_quad* def, float delta_time)
	{
		if(def->m_num_tail_faces <= 0)
			return;

		m_tail_delay += delta_time;
		if(m_tail_delay > def->m_tail_interval)
		{
			m_tail_delay -= def->m_tail_interval;

			//-- 新增一个拖尾片
			m_tail_index++;
			if( m_tail_index >= m_faces.size() )
			{
				m_tail_index = 1;
			}
			m_faces[m_tail_index] = m_faces[0]; // 记录下head face的状态

			m_active_face++;
			if(m_active_face > m_faces.size())
				m_active_face = m_faces.size();
		}

		//-- 计算每个拖尾片的Alpha淡出
		for (unsigned int i=1; i<m_active_face; i++)
		{
			quad_face& face = m_faces[i];
			face.color.A -= m_tail_fade_speed*delta_time;
			if( face.color.A<0 )
				face.color.A = 0;
		}
	}
	
	//-- class neffect_quad
	nDEFINE_CLASS(neffect_quad, neffect_element)
	neffect_quad::neffect_quad(void)
	{
		m_face_type = ESFX_NormalFace;
		m_num_tail_faces = 0;
		m_tail_interval = 1.0f;
	}

	neffect_quad::~neffect_quad(void)
	{
	}

	void neffect_quad::init(const nstring& name)
	{
		neffect_element::init(name);

		m_color.reset( new ndistribution_const<color4f>(color4f(1,1,1,1)) );
		m_size.reset( new ndistribution_const<vector2>(vector2(40,40)) );
	}
	
	neffect_element_instance::ptr neffect_quad::create_instance()
	{
		neffect_quad_instance::ptr new_inst( new neffect_quad_instance );
		
		init_instance_base(new_inst);
		return new_inst;
	}

	void neffect_quad::update_instance(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time)
	{
		if( !update_instance_base(inst, view, delta_time, play_time) )
			return;

		float my_time = play_time - m_start_time;

		//-- update head quad
		neffect_quad_instance::ptr quad_inst = boost::dynamic_pointer_cast<neffect_quad_instance>(inst);

		quad_face& head_face = quad_inst->get_faces()[0];
		head_face.pos = quad_inst->m_pos;
		head_face.revolution = quad_inst->m_revolution;
		head_face.rot=quad_inst->m_rot;
		head_face.half_size = m_size->get_value(my_time)*0.5f;
		head_face.color = m_color->get_value(my_time);

		//-- 更新残影面片
		quad_inst->update_tail(this, delta_time);
	}

	void neffect_quad::serialize(narchive& ar)
	{
		neffect_element::serialize( ar );

		nserialize_enum_as_int(ar, m_face_type, _T("m_face_type"));		
		nSERIALIZE(ar, m_num_tail_faces);
		nSERIALIZE(ar, m_tail_interval);		

		nSERIALIZE_DISTRIBUTION(ar, m_color);
		nSERIALIZE_DISTRIBUTION(ar, m_size);
	}

}//namespace nexus