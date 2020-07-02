#ifndef _NEXUS_SPEED_TREE_RENDERER_H
#define _NEXUS_SPEED_TREE_RENDERER_H
#include "../nengine/framework/view_info.h"
#include "drawing_policy.h"
#include "d3d9_vertex_factory.h"
#include "d3d_device_manager.h"

namespace nexus
{
	enum ESptMeshType
	{
		ESpt_Branch,
		ESpt_Frond,
		ESpt_Leafcard,
		ESpt_Leafmesh,
	};

	class nrender_mesh_lod_adapter
		: public nrender_mesh
	{
	public:
		nrender_mesh*	m_mesh;
		int				m_lod;

		nrender_mesh_lod_adapter(void):m_mesh(0),m_lod(0)
		{}

		virtual nvertex_factory* get_vertex_factory()
		{
			return m_mesh->get_vertex_factory();
		}
		virtual size_t get_num_section()	{	return 1;}	
		virtual nrender_mesh_section* get_section(size_t section_index)
		{
			(void)section_index;
			return m_mesh->get_section(m_lod);
		}
	};

	struct instance_data
	{
		matrix44 data;

		instance_data(vector4 pos,vector4 rot,float offset ):
		data(
			pos.x,pos.y,pos.z,pos.w,
			rot.x,rot.y,rot.z,rot.w,
			offset,0,0,0,
			0,0,0,0
			)
		{}
	};

	class spt_vertex_factory_instance :
		public d3d9_vertex_factory
	{
	public:
		static const UINT inst_stream_index = 8;

		spt_vertex_factory_instance(d3d9_vertex_factory* vertex_factory,const vertex_stream* vs,d3d_vb_ptr vb) 
		{
			nASSERT(vertex_factory);
			m_vertex_factory = vertex_factory;
			m_num_vert = vertex_factory->get_num_vert();
			IDirect3DDevice9*  d3d_device = d3d_device_manager::instance()->get_device();

			instance_vb = vb;
			instance_count = vs->get_num_vert();
			stride = vs->get_vertex_size();

			m_type = vertex_factory->get_inst_type();
		};

		virtual ~spt_vertex_factory_instance(void)  
		{
		};

		virtual void destroy()
		{
			m_vertex_factory->destroy();
		}
		virtual void draw_set_pos_only()
		{
			draw_set_full();
		}

		virtual bool  draw_set_full()
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
	
	protected:
		d3d9_vertex_factory* m_vertex_factory;
		d3d_vb_ptr	instance_vb;
		unsigned int instance_count,stride;
		d3d_ptr<IDirect3DVertexDeclaration9> m_vert_decl;
	};

	class spt_instance_adapter
		: public nrender_mesh
	{
	public:
		nrender_mesh*	m_mesh;
		const nspt_instance* tree;
		vector<instance_data> instance_datas;
		ESptMeshType mesh_type;
			
		spt_instance_adapter(void):m_mesh(0),
		m_vertex_factory(NULL)
		{
		}

		virtual  ~spt_instance_adapter()
		{
			delete m_vertex_factory;
		}

		void create_vertex_buffer()
		{
			vector<vertex_element_define> packed_array;

			unsigned short offset = 0;
			unsigned short stream_index = spt_vertex_factory_instance::inst_stream_index;
		
			for ( size_t i = 0; i < 4; i ++)
			{
				vertex_element_define def = {stream_index, offset, EVET_FLOAT4, EVEU_Color, i};
				offset += sizeof(float)*4;
				packed_array.push_back(def);
			}

			vs.init(packed_array,instance_datas.size());

			memcpy(vs.get_data(), &instance_datas[0], vs.get_data_size());
			
			instance_vb = create_d3d_vb_from_stream(&vs,D3DPOOL_MANAGED,true);
		}
		
		void create_instance_vertex_factory(d3d9_vertex_factory* vertex_factory)
		{
			if (m_vertex_factory)
			{
				delete m_vertex_factory;
			}

			m_vertex_factory = new spt_vertex_factory_instance(vertex_factory,&vs,instance_vb);
		}

		virtual nvertex_factory* get_vertex_factory()
		{
			return m_vertex_factory;
		}

		virtual size_t get_num_section()	{	return 1;}	
		virtual nrender_mesh_section* get_section(size_t section_index)
		{
			(void)section_index;
			return m_mesh->get_section(0);
		}

		virtual void draw_setup_effect(nshading_effect* effect_ptr);

		d3d9_vertex_factory* m_vertex_factory;
		vertex_stream vs;
		d3d_vb_ptr instance_vb;
	};

	class nspeed_tree_renderer
	{
	public:	
		typedef std::vector<const nspt_instance*> st_tree_array;
		struct tree_group
		{
			st_tree_array trees;
			spt_instance_adapter inst_adapter;
		};

		typedef std::map<nname, tree_group*> st_tree_map;//base tree name <-> tree instance array
		nspeed_tree_renderer(void);
		virtual ~nspeed_tree_renderer(void);

		void clear();
		void attach_tree(const nspt_instance* tree);
		void draw_all_trees(const nview_info* view, drawing_policy* dp, bool enable_instance);
		void frame_begin(bool bAdvance);
		const st_tree_map& get_tree_map()
		{
			return m_tree_map;
		}

	private:
		void instance_draw(const nview_info* view, drawing_policy* dp);
		void draw_no_instance(const nview_info* view, drawing_policy* dp);
		void draw_leaf_no_instance(int index, const nspt_instance* spt, const nview_info* view, drawing_policy* dp);
	private:
		st_tree_map	m_tree_map;
		bool binstance;
		boost::timer m_timer;
		double		m_last_time;
	};
}//namespace nexus
#endif //_NEXUS_SPEED_TREE_RENDERER_H