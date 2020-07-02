#include "stdafx.h"
#include "nterrain_deco_grass.h"
#include "../framework/nengine.h"
#include "../renderer/nrender_resource_manager.h"

namespace nexus
{
	//===================================================================================
	void grass_deco_buffer::set_material( nmtl_base::ptr matrial )
	{
		m_material=matrial;
	}

	void grass_deco_buffer::begin_update()
	{
		m_pos_stream.clear();
		m_second_stream.clear();
		m_index.data.clear();
	}

	void grass_deco_buffer::add_instance( const matrix44& matrix )
	{
		uint32 vertex=m_cross?12:6;
		for(uint32 i=0;i<vertex;++i)
		{
			vector4 position,normal;
			position = m_pos[i];
			vector3 pos;
			pos=mat_transform(position,matrix);
			m_pos_stream.push_back(pos);
			m_second_stream.push_back(m_second_part[i]);
			m_index.append_index(m_pos_stream.size()-1);
		}
	}

	void grass_deco_buffer::end_update()
	{
		if(m_pos_stream.size()==0) return;
		m_vertex.reset(new nmesh_vertex_data_grass());
		m_vertex->init(m_pos_stream.size());
		vertex_stream* stream=m_vertex->get_stream(0);
		vertex_stream* stream2=m_vertex->get_stream(1);
		for(uint32 i=0;i<m_pos_stream.size();++i)
		{
			stream->set_element(i,0,m_pos_stream[i]);
			stream2->set_element(i,0,m_second_stream[i].normal);
			stream2->set_element(i,1,m_second_stream[i].uv);
			stream2->set_element(i,2,m_second_stream[i].weight);
		}
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		m_render_mesh.reset(rres_mgr->alloc_static_mesh());
		m_render_mesh->create(EDraw_TriangleList, (m_vertex.get()) );
	}

	void grass_deco_buffer::get_mesh_element( int lod,vector<mesh_element>& mesh_elements,nmesh_component* comp )
	{
		if(m_index.get_index_count()>0 && m_render_mesh!=0 && m_material!=0)
		{
			for(size_t i=0; i<m_render_mesh->get_num_section(); i++)
			{	
				nrender_mesh_section* sec = m_render_mesh->get_section(i);
				mesh_elements.push_back(mesh_element(NULL,m_render_mesh.get(),sec,m_material.get(),false));
			}
		}
	}

	grass_deco_buffer::grass_deco_buffer()
	{
		m_cross=false;
		//生成十字交叉面片的顶点数据
		m_pos[0]=vector3(-0.5f,1.0f,0);
		m_pos[1]=vector3(-0.5f,0,0);
		m_pos[2]=vector3(0.5f,1.0f,0);
		m_pos[3]=vector3(0.5f,1.0f,0);
		m_pos[4]=vector3(-0.5f,0,0);
		m_pos[5]=vector3(0.5f,0,0);

		m_pos[6]=vector3(0,1.0f,-0.5f);
		m_pos[7]=vector3(0,0,-0.5f);
		m_pos[8]=vector3(0,1.0f,0.5f);
		m_pos[9]=vector3(0,1.0f,0.5f);
		m_pos[10]=vector3(0,0,-0.5f);
		m_pos[11]=vector3(0,0,0.5f);

		m_second_part[0].uv=vector2(0,0);
		m_second_part[1].uv=vector2(0,1);
		m_second_part[2].uv=vector2(1,0);
		m_second_part[3].uv=vector2(1,0);
		m_second_part[4].uv=vector2(0,1);
		m_second_part[5].uv=vector2(1,1);

		m_second_part[6].uv=vector2(0,0);
		m_second_part[7].uv=vector2(0,1);
		m_second_part[8].uv=vector2(1,0);
		m_second_part[9].uv=vector2(1,0);
		m_second_part[10].uv=vector2(0,1);
		m_second_part[11].uv=vector2(1,1);

		for (int i=0;i<12;++i)
		{
			m_second_part[i].normal=vector3(0,1,0);
			if(m_pos[i].y<0.4f)
			{
				m_second_part[i].weight=0;
			}
			else
			{
				m_second_part[i].weight=1.0f;
			}
		}
	}

	grass_deco_buffer::~grass_deco_buffer()
	{

	}

	void grass_deco_buffer::on_device_lost()
	{
		m_vertex.reset();
		m_render_mesh.reset();
	}	

	void grass_deco_buffer::on_device_reset()
	{
		end_update();
	}
	//=====================================================================================

	nmesh_vertex_data_grass::nmesh_vertex_data_grass( void )
	{

	}

	nmesh_vertex_data_grass::~nmesh_vertex_data_grass( void )
	{

	}

	void nmesh_vertex_data_grass::init( size_t num_vert )
	{
		init_pos_stream(num_vert);

		//-- 初始化second stream
		unsigned short offset = 0;
		element_array elem_array;

		vertex_element_define norm_def = {1, offset, EVET_FLOAT3, EVEU_Normal, 0};
		offset += sizeof(float)*3;
		elem_array.push_back(norm_def);

		vertex_element_define uv_def = {1, offset, EVET_FLOAT2, EVEU_TexCoord, 0};
		offset += sizeof(float)*2;
		elem_array.push_back(uv_def);

		// bone index
		vertex_element_define idx_def = {1, offset, EVET_FLOAT1, EVEU_BlendWeight, 0};
		offset += sizeof(float);
		elem_array.push_back(idx_def);

		// init stream
		m_second_stream.init(elem_array, num_vert);

		// make macro define
		shader_define normal_defin("VERT_NUM_NORMAL",boost::lexical_cast<string>(1));
		shader_define uv_defin("VERT_NUM_TEXCOORD",boost::lexical_cast<string>(1));

		m_macro_array.push_back(normal_defin);
		m_macro_array.push_back(uv_defin);
	}

	nstring nmesh_vertex_data_grass::get_vertex_type_name() const
	{
		return _T("vertex_grass");
	}

	nstring nmesh_vertex_data_grass::get_vertex_factory_name() const
	{
		return _T("vfactory_grass");
	}
};

