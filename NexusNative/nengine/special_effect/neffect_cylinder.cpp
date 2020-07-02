#include "StdAfx.h"
#include "neffect_cylinder.h"
#include "../framework/nengine.h"

namespace nexus
{
	nDEFINE_CLASS(neffect_cylinder, neffect_element)

	void neffect_cylinder_instance::init(shared_ptr<neffect_element> def)
	{
		neffect_element_instance::init(def);

		m_template = boost::dynamic_pointer_cast<neffect_cylinder>( def );
		
		// for short
		int div = m_template->m_div;
		vector2 uv_scale = m_template->m_uv_scale;

		//-- 计算顶点个数
		int num_vert = div*2+2; // 使用Triangle Strip，顶部,底部各一个顶点,加上起始边的两个顶点(重复,为了形成封闭形)

		//-- 初始化vertex data
		m_vert_data.init(num_vert);

		// 初始化uv坐标
		vertex_stream* uv_stream = m_vert_data.get_stream(2);
		vector2* uv_ptr = (vector2*)(uv_stream->get_data());
		const nsub_uv& suv = def->m_sub_uv;
		
		float u_step = uv_scale.x/div;
		for (int i=0; i<div+1; i++)
		{			
			*uv_ptr = suv.get_sub_uv(i*u_step, 0); // 顶部顶点
			uv_ptr++;

			*uv_ptr = suv.get_sub_uv(i*u_step, uv_scale.y); // 底部顶点
			uv_ptr++;
		}

		//-- 创建render mesh
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		m_mesh.reset( rres_mgr->alloc_dynamic_mesh() );
		m_mesh->create(EDraw_TriangleStrip, &m_vert_data, 0);
	}

	bool neffect_cylinder_instance::update_render_data(const nviewport& view, float play_time, const matrix44& owner_mat)
	{
		(void)view;
		if( !m_active )
			return false;

		//-- 取得全部变量的当前值
		float num_circle = m_template->m_num_circle->get_value(play_time);
		float height = m_template->m_height->get_value(play_time);
		float top_r = m_template->m_top_radius->get_value(play_time);
		float bottom_r = m_template->m_bottom_radius->get_value(play_time);
		float top_r_inc = m_template->m_div_top_radius_inc->get_value(play_time);
		float bottom_r_inc = m_template->m_div_bottom_radius_inc->get_value(play_time);
		float top_r_shrink = m_template->m_div_top_radius_shrink->get_value(play_time);
		float bottom_r_shrink = m_template->m_div_bottom_radius_shrink->get_value(play_time);
		float top_rise = m_template->m_div_top_rise->get_value(play_time);
		float bottom_rise = m_template->m_div_bottom_rise->get_value(play_time);

		color4f top_color = m_template->m_top_color->get_value(play_time);
		color4f bottom_color = m_template->m_bottom_color->get_value(play_time);

		//-- 计算特效元素的位置旋转
		matrix44 trans, temp_mat;
		mat_set_translation(trans, m_pos);
		mat_set_rotation_yaw_pitch_roll(temp_mat, m_rot.y, m_rot.x, m_rot.z);
		trans = temp_mat*trans*owner_mat;

		//-- 根据当前状态，计算vertex data
		vertex_stream* pos_stream = m_vert_data.get_stream(0);
		vertex_stream* diffuse_stream = m_vert_data.get_stream(1);

		vector3* pos_ptr = (vector3*)(pos_stream->get_data());
		color4f* diffuse_ptr = (color4f*)(diffuse_stream->get_data());

		// for short
		int div = m_template->m_div;
		
		float angle = 0;
		float angle_step = (nPI*2*num_circle)/div;

		float fsin, fcos;
		float top_radius, bottom_radius;
		float top_h, bottom_h;
		int vi = 0;	// 顶点索引

		vector3 pos;

		for(int i=0; i<div; i++)
		{
			//-- 计算循环变量的值
			angle = i*angle_step;
			fsin = sinf(angle);
			fcos = cosf(angle);

			top_radius = top_r + angle*top_r_inc;
			bottom_radius = bottom_r + angle*bottom_r_inc;

			top_h = height+angle*top_rise;
			bottom_h = angle*bottom_rise;			

			if( i%2 )	// 间隔一列顶点，收缩一次，用来生产星形几何体
			{
				top_radius -= top_r_shrink;
				bottom_radius -= bottom_r_shrink;
			}			

			// 增加一条边上的两个顶点
			// 添加顶部顶点
			diffuse_ptr[vi] = top_color;
			pos = vector3(top_radius*fsin, top_h, top_radius*fcos);
			pos_ptr[vi] = pos*trans;
			vi++;

			//添加底部顶点
			diffuse_ptr[vi] = bottom_color;
			pos = vector3(bottom_radius*fsin, bottom_h, bottom_radius*fcos);
			pos_ptr[vi] = pos*trans;
			vi++;
		}

		diffuse_ptr[vi] = top_color;		
		diffuse_ptr[vi+1] = bottom_color;
		if( top_rise>0 || bottom_rise>0 )
		{
			// 如果是带状,则不形成封闭形
			int n = vi-1;
			pos_ptr[vi++] = pos_ptr[n-1];
			pos_ptr[vi++] = pos_ptr[n];
		}
		else
		{
			// 形成封闭形
			pos_ptr[vi++] = pos_ptr[0];
			pos_ptr[vi++] = pos_ptr[1];
		}

		//-- 更新render mesh
		m_mesh->update_vertices(&m_vert_data);
		return true;
	}

	nrender_mesh* neffect_cylinder_instance::get_render_mesh() const
	{
		return m_mesh.get();
	}

	//-- class neffect_cylinder ------------------------------------------------------------------
	neffect_cylinder::neffect_cylinder(void)
	{
	}

	neffect_cylinder::~neffect_cylinder(void)
	{
	}

	neffect_element_instance::ptr neffect_cylinder::create_instance()
	{
		shared_ptr<neffect_cylinder_instance> new_inst( new neffect_cylinder_instance );
		init_instance_base(new_inst);
		return new_inst;
	}

	void neffect_cylinder::update_instance(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time)
	{
		if( !update_instance_base(inst, view, delta_time, play_time) )
			return;
	}

	void neffect_cylinder::init(const nstring& name)
	{
		neffect_element::init(name);

		//-- 静态数据
		m_div = 12;
		m_uv_scale = vector2(1, 1);

		//-- 动态数据
		m_num_circle.reset( new ndistribution_const<float>(1.0f) );
		m_height.reset( new ndistribution_const<float>(100.0f) );
		m_top_radius.reset( new ndistribution_const<float>(100.0f) );
		m_bottom_radius.reset( new ndistribution_const<float>(100.0f) );
		m_div_top_radius_inc.reset( new ndistribution_const<float>(0.0f) );
		m_div_bottom_radius_inc.reset( new ndistribution_const<float>(0.0f) );
		m_div_top_radius_shrink.reset( new ndistribution_const<float>(0.0f) );
		m_div_bottom_radius_shrink.reset( new ndistribution_const<float>(0.0f) );
		m_div_top_rise.reset( new ndistribution_const<float>(0.0f) );
		m_div_bottom_rise.reset( new ndistribution_const<float>(0.0f) );

		m_top_color.reset( new ndistribution_const<color4f>(color4f(1,1,1,1)) );
		m_bottom_color.reset( new ndistribution_const<color4f>(color4f(1,1,1,1)) );
	}

	void neffect_cylinder::serialize(narchive& ar)
	{
		neffect_element::serialize( ar );

		nSERIALIZE(ar, m_div);
		nSERIALIZE(ar, m_uv_scale);

		nSERIALIZE_DISTRIBUTION(ar, m_num_circle);
		nSERIALIZE_DISTRIBUTION(ar, m_height);
		nSERIALIZE_DISTRIBUTION(ar, m_top_radius);
		nSERIALIZE_DISTRIBUTION(ar, m_bottom_radius);
		nSERIALIZE_DISTRIBUTION(ar, m_div_top_radius_inc);
		nSERIALIZE_DISTRIBUTION(ar, m_div_bottom_radius_inc);
		nSERIALIZE_DISTRIBUTION(ar, m_div_top_radius_shrink);
		nSERIALIZE_DISTRIBUTION(ar, m_div_bottom_radius_shrink);
		nSERIALIZE_DISTRIBUTION(ar, m_div_top_rise);
		nSERIALIZE_DISTRIBUTION(ar, m_div_bottom_rise);		
	}
}//namespace nexus