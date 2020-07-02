#pragma once
#include "render_proxy.h"
#include "d3d9_vertex_factory_common.h"
#include "../nengine/actor/nmesh_component.h"

namespace nexus
{
	//并将顶点申明替换为实例的顶点声明
	class d3d9_vertex_factory_instance :
		public d3d9_vertex_factory
	{
	public:
		static const UINT inst_stream_index = 8;

		d3d9_vertex_factory_instance(d3d9_vertex_factory* vertex_factory,const vertex_stream* vs) 
		{
			nASSERT(vertex_factory);
			m_vertex_factory = vertex_factory;
			instance_vb = create_d3d_vb_from_stream(vs,D3DPOOL_MANAGED,true);
			instance_count = vs->get_num_vert();
			stride = vs->get_vertex_size();
			m_type = vertex_factory->get_inst_type();
			m_num_vert = vertex_factory->get_num_vert();
		};

		virtual ~d3d9_vertex_factory_instance(void)  
		{
		};

		virtual void destroy()
		{
			m_vertex_factory->destroy();
		}

		virtual void draw_set_pos_only()
		{
			m_vertex_factory->draw_set_pos_only();
		}

		virtual bool draw_set_full()
		{
			IDirect3DDevice9*  d3d_device = d3d_device_manager::instance()->get_device();
			HRESULT hr;

			m_type->draw_set_vert_decl();

			const vector<d3d_vb_ptr>& vert_streams = m_vertex_factory->get_vert_streams();

			const vector<UINT>&  vert_stride = m_vertex_factory->get_stream_stride();
			for(UINT i=0; i<vert_streams.size(); i++)
			{
				d3d_device->SetStreamSource(i, vert_streams[i].get(), 0, vert_stride[i]);
				d3d_device->SetStreamSourceFreq( i, D3DSTREAMSOURCE_INDEXEDDATA | instance_count ) ;
			}

			hr = d3d_device->SetStreamSource( inst_stream_index, instance_vb.get(), 0, stride  );
			hr = d3d_device->SetStreamSourceFreq( inst_stream_index, D3DSTREAMSOURCE_INSTANCEDATA | 1 );

			return true;
		}

		virtual void draw_reset()
		{
			IDirect3DDevice9*  d3d_device = d3d_device_manager::instance()->get_device();

			const vector<d3d_vb_ptr>& vert_streams = m_vertex_factory->get_vert_streams();
			for(UINT i=0; i<vert_streams.size(); i++)
			{
				d3d_device->SetStreamSourceFreq(i,1);
			}

			d3d_device->SetStreamSourceFreq(inst_stream_index,1);
		}

		virtual vertex_factory_type* get_type()	
		{	
			return m_type;
		}

		size_t get_num_vert() const		{	return m_vertex_factory->get_num_vert();}

	protected:
		d3d9_vertex_factory* m_vertex_factory;
		d3d_vb_ptr	instance_vb;
		unsigned int instance_count,stride;
		vertex_factory_type* m_type;
	};

	//用于收集实例数据，并将其组合成一个流.
	class mesh_instance_adapter : public nrender_mesh
	{
		struct instance_data
		{
			matrix44 local_to_world;

			instance_data()
			{}

			instance_data(matrix44 mat):
			local_to_world(mat)
			{}
		};

	public:
		nrender_mesh*	m_mesh;
		nrender_mesh_section* m_mesh_section;
		nmtl_base* m_mtl;
		d3d9_vertex_factory* m_vertex_factory;
		vector<instance_data> instance_datas;

		mesh_instance_adapter(
			nrender_mesh*	mesh,
			nrender_mesh_section* mesh_section,
			nmtl_base* mtl
			):m_mesh(mesh),
			m_mesh_section(mesh_section),
			m_mtl(mtl),
			m_vertex_factory(NULL)
		{
		}

		virtual  ~mesh_instance_adapter()
		{
			delete m_vertex_factory;
		}
		
		bool empty()
		{
			return instance_datas.empty();
		}

		void add_mesh(nmesh_component* mesh)
		{
			instance_datas.push_back(instance_data(mesh->get_world_matrix()));
		}

		void add_instance(const mesh_element* element)
		{
			instance_datas.push_back(instance_data(element->comp->get_world_matrix()));
		}

		void init()
		{
			if(instance_datas.empty())
			{
				return;
			}

			vector<vertex_element_define> packed_array;

			unsigned short offset = 0;
			unsigned short stream_index = d3d9_vertex_factory_instance::inst_stream_index;

			for ( size_t i = 0; i < 4; i ++)
			{
				vertex_element_define def = {stream_index, offset, EVET_FLOAT4, EVEU_Color, i};
				offset += sizeof(float)*4;
				packed_array.push_back(def);
			}

			vertex_stream vs;
			vs.init(packed_array,instance_datas.size());
			memcpy(vs.get_data(), &instance_datas[0], vs.get_data_size());
			
			m_vertex_factory = new d3d9_vertex_factory_instance((d3d9_vertex_factory*)m_mesh->get_vertex_factory(),&vs);
		}

		virtual nvertex_factory* get_vertex_factory()
		{
			return m_vertex_factory;
		}
	
		virtual nshader_modifier* get_shader_mod()	{	return m_mesh->get_shader_mod(); }
		virtual void draw_setup_effect(nshading_effect* effect_ptr)	
		{
			 m_mesh->draw_setup_effect(effect_ptr);
		}

		virtual size_t get_num_section()	{	return 1;}	
		virtual nrender_mesh_section* get_section(size_t section_index)
		{
			(void)section_index;
			return m_mesh_section;
		}
	};

	//实例渲染器，使用map管理mesh_instance_adapter
	class mesh_instance_renderer
	{
		typedef std::map<std::pair<nrender_mesh_section*, nmtl_base*>, mesh_instance_adapter*> instance_map;
		instance_map instances;
	public:
		mesh_instance_renderer() {};
		~mesh_instance_renderer();
		void clear();
		void add_mesh(const mesh_element* element);
		void init();
		void draw_all(const nview_info* view,drawing_policy* dp);
	};
}