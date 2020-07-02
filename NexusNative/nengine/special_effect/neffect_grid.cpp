#include "stdafx.h"
#include "neffect_grid.h"
#include "../framework/nengine.h"

namespace nexus
{
	//===================================================================================

	neffect_grid_instance::neffect_grid_instance()
	{

	}

	void neffect_grid_instance::init( boost::shared_ptr<neffect_element> def )
	{
		neffect_element_instance::init(def);

		m_template = boost::dynamic_pointer_cast<neffect_grid>( def );

		//-- 计算顶点个数,使用三角形列表
		m_face_count=(m_template->m_length_div)*(m_template->m_width_div)*2;
		int num_vert = m_face_count*3;

		//-- 初始化vertex data
		m_vert_data.init(num_vert);

		// 初始化uv坐标
		vertex_stream* uv_stream = m_vert_data.get_stream(2);
		vector2* uv_ptr = (vector2*)(uv_stream->get_data());
		const nsub_uv& suv = def->m_sub_uv;

		int count=0;
		for(int i=0;i<m_template->m_length_div-1;++i)
		{
			float u=suv.get_sub_u((float)i/m_template->m_length_div);
			float upp=suv.get_sub_u((float)(i+1)/m_template->m_length_div);
			for(int j=0;j<m_template->m_width_div-1;++j)
			{
				float v=suv.get_sub_v((float)j/m_template->m_width_div);
				float vpp=suv.get_sub_v((float)(j+1)/m_template->m_width_div);

				uv_ptr[count++]=vector2(u,v);
				uv_ptr[count++]=vector2(u,vpp);
				uv_ptr[count++]=vector2(upp,v);

				uv_ptr[count++]=vector2(upp,v);
				uv_ptr[count++]=vector2(u,vpp);
				uv_ptr[count++]=vector2(upp,vpp);
			}
		}

		//-- 创建render mesh
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		m_mesh.reset( rres_mgr->alloc_dynamic_mesh() );
		m_mesh->create(EDraw_TriangleList, &m_vert_data, 0);
	}

	bool neffect_grid_instance::update_render_data( const nviewport& view, float play_time, const matrix44& owner_mat )
	{
		//生成粒子的图元数据
		vertex_stream* pos_stream = m_vert_data.get_stream(0);
		vertex_stream* diffuse_stream = m_vert_data.get_stream(1);

		vector3* pos_ptr = (vector3*)(pos_stream->get_data());
		color4f* diffuse_ptr = (color4f*)(diffuse_stream->get_data());

		int wRes=m_template->m_length_div;
		int hRes=m_template->m_width_div;

		matrix44 mdlView=owner_mat;
		mdlView=mdlView*view.camera.get_view();

		m_view_right=vector3(mdlView._11,mdlView._21,mdlView._31);
		m_view_up=vector3(mdlView._12,mdlView._22,mdlView._32);
		m_view_forward=vector3(mdlView._13,mdlView._23,mdlView._33);

		//--计算变换矩阵
		matrix44 trans,rotate;
		switch (m_template->m_type)
		{
		case neffect_grid_plane:
			{
				mat_set_translation(trans,m_pos);
				mat_set_rotation_yaw_pitch_roll(rotate,m_rot.y,m_rot.x,m_rot.z);
				break;
			}
		case neffect_grid_sphere_billboard:
			{
				vector3 xaxis=m_view_right;
				vector3 yaxis=m_view_up;
				vector3 zaxis=vec_cross(xaxis,yaxis);
				mat_set_rotation_axis(rotate,m_rot.z,zaxis);
				mat_set_translation(trans,m_pos);
				break;
			}
		case neffent_grid_cylinder_billboard:
			{
				vector3 xaxis=m_view_right;
				vector3 yaxis=vector3(0,1,0);
				vector3 zaxis=vec_cross(xaxis,yaxis);
				mat_set_rotation_axis(rotate,m_rot.z,zaxis);
				mat_set_translation(trans,m_pos);
				break;
			}
		}
		m_transform=rotate*trans;
		

		vector3 vec[4];

		int count=0;
		int x,y;
		for(y=0;y<hRes-1;y++)
		{
			for(x=0;x<wRes-1;x++)
			{
				compute_vertex_info(x,y,play_time,vec[0]);
				compute_vertex_info(x,y+1,play_time,vec[1]);
				compute_vertex_info(x+1,y,play_time,vec[2]);
				compute_vertex_info(x+1,y+1,play_time,vec[3]);

				pos_ptr[count]=vec[0];	diffuse_ptr[count]=m_color;		++count;
				pos_ptr[count]=vec[1];	diffuse_ptr[count]=m_color;		++count;
				pos_ptr[count]=vec[2];	diffuse_ptr[count]=m_color;		++count;

				pos_ptr[count]=vec[2];	diffuse_ptr[count]=m_color;		++count;
				pos_ptr[count]=vec[1];	diffuse_ptr[count]=m_color;		++count;
				pos_ptr[count]=vec[3];	diffuse_ptr[count]=m_color;		++count;
			}
		}

		//-- 更新render mesh
		m_mesh->update_vertices(&m_vert_data);
		m_mesh->set_draw_limit(m_face_count);
		return true;
	}

	void neffect_grid_instance::compute_vertex_info( int x,int y,float time,vector3& pos )
	{
		int wRes=m_template->m_length_div;
		int hRes=m_template->m_width_div;

		float minX=-m_size.x*0.5f;
		float minY=-m_size.y*0.5f;
		float xStep=m_size.x/wRes;
		float yStep=m_size.y/hRes;

		float fx=x+1.0f;
		float fy=y+1.0f;

		float fsin=sinf((time+fx)*m_template->m_wave_speed+fy+fx);
		float fcos=cosf((time+fy)*m_template->m_wave_speed+fx+fy);

		switch (m_template->m_type)
		{
		case neffect_grid_plane:
			{
				pos.x=x*xStep+minX	+m_power.x*(fsin-fcos);
				pos.y=y*yStep+minY	+m_power.y*(fsin+fcos);
				pos.z=m_power.z*fsin;

				pos=pos*m_transform;
				break;
			}
		case neffect_grid_sphere_billboard:
			{
				vector3 xaxis=m_view_right;
				vector3 yaxis=m_view_up;

				float xOffset=x*xStep+minX + m_power.x*(fsin-fcos);
				float yOffset=y*yStep+minY + m_power.y*(fsin+fcos);
				float zOffset=m_power.z * fsin;

				pos=xaxis*xOffset+yaxis*yOffset+m_view_forward*zOffset;
				pos=pos*m_transform;
				break;
			}
		case neffent_grid_cylinder_billboard:
			{
				vector3 xaxis=m_view_right;
				vector3 yaxis=vector3(0,1,0);

				float xOffset=x*xStep+minX + m_power.x*(fsin-fcos);
				float yOffset=y*yStep+minY + m_power.y*(fsin+fcos);
				float zOffset=m_power.z * fsin;
				pos=xaxis*xOffset+yaxis*yOffset+m_view_forward*zOffset;
				pos=pos*m_transform;
				break;
			}
		}
	}

	//==============================================================================================================================
	nDEFINE_CLASS(neffect_grid, neffect_element)
	
	neffect_grid::neffect_grid()
	{
		m_type=neffect_grid_plane;
		m_length_div=50;
		m_width_div=10;
		m_wave_speed=0;
		m_attenuation=0;
		m_color.reset( new ndistribution_const<color4f>(color4f(1,1,1,1)) );
		m_size.reset( new ndistribution_const<vector2>(vector2(40,40)) );
		m_wave_power.reset( new ndistribution_const<vector3>(vector3(0,0,0)) );
	}

	neffect_grid::~neffect_grid()
	{
	}

	void neffect_grid::init( const nstring& name )
	{		
		neffect_element::init(name);
	}

	neffect_element_instance::ptr neffect_grid::create_instance()
	{
		neffect_grid_instance::ptr new_inst( new neffect_grid_instance );

		init_instance_base(new_inst);

		new_inst->m_pos=m_init_pos;
		new_inst->m_rot=m_init_rot;
		new_inst->m_revolution=m_init_revolution;
		return new_inst;
	}

	void neffect_grid::update_instance( neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time )
	{
		(void)view;
		//-- 是否启动
		if( !inst->m_active )
		{
			if(play_time > m_start_time)
				inst->m_active = true;
		}
		if( !inst->m_active )
			return;

		neffect_grid_instance::ptr rinst = boost::dynamic_pointer_cast<neffect_grid_instance>(inst);
		rinst->m_color=m_color->get_value(play_time);
		rinst->m_size=m_size->get_value(play_time);
		rinst->m_power=m_wave_power->get_value(play_time);
		rinst->m_pos+=m_vel->get_value(play_time)*delta_time;
		rinst->m_rot+=m_rotate_vel->get_value(play_time)*delta_time;

	}

	void neffect_grid::serialize( narchive& ar )
	{
		neffect_element::serialize(ar);
	}
	//==============================================================================================================================
};

