#include "StdAfx.h"
#include "nterrain_chunk.h"
#include "nterrain_actor.h"
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"
#include "nterrain_deco_chunk_base.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nterrain_chunk, nprimitive_component)

	//-- class nterrain_chunk_vertex_data -------------------------------------------------------------
	class nterrain_chunk_vertex_data
		: public nmesh_vertex_data
	{		
	public:
		nterrain_chunk_vertex_data(void)	{}
		virtual ~nterrain_chunk_vertex_data(void)	{}

		virtual size_t get_num_stream() const	{	return 5; }
		virtual vertex_stream* get_stream(size_t stream_index)
		{			
			switch( stream_index )
			{
			case 0:
				return &m_pos_stream;			
			case 1:
				return &m_uv_stream;
			case 2:
				return &m_normal_stream;
			case 3:
				return &m_tangent_stream;
			case 4:
				return &m_binormal_stream;
			}
			return NULL;
		}

		virtual nstring get_vertex_type_name() const
		{
			return _T("terrain_chunk_vertex");
		}

		virtual nstring get_vertex_factory_name() const
		{
			return _T("vfactory_terrain");
		}

		void init(size_t num_vert)
		{
			init_pos_stream(num_vert);

			//-- uv		
			vertex_element_define uv_def = {1, 0, EVET_FLOAT2, EVEU_TexCoord, 0};

			element_array uv_def_array;
			uv_def_array.push_back(uv_def);

			m_uv_stream.init(uv_def_array, num_vert);

			//-- normal
			vertex_element_define norm_def = {2, 0, EVET_FLOAT3, EVEU_Normal, 0};

			element_array norm_def_array;
			norm_def_array.push_back(norm_def);

			m_normal_stream.init(norm_def_array, num_vert);

			//-- tangent
			vertex_element_define tan_def = {3, 0, EVET_FLOAT3, EVEU_Tangent, 0};

			element_array tan_def_array;
			tan_def_array.push_back(tan_def);

			m_tangent_stream.init(tan_def_array, num_vert);

			//-- binormal
			vertex_element_define bi_def = {4, 0, EVET_FLOAT3, EVEU_Binormal, 0};

			element_array binormal_def_array;
			binormal_def_array.push_back(bi_def);

			m_binormal_stream.init(binormal_def_array, num_vert);
		}

	private:
		vertex_stream	m_uv_stream;
		vertex_stream	m_normal_stream;
		vertex_stream   m_tangent_stream;
		vertex_stream   m_binormal_stream;
	};

	//-- class nterrain_chunk_mesh_adapter -----------------------------------------------
	void nterrain_chunk_mesh_adapter::create(const nmesh_vertex_data* vert_data, const index_buffer16* index_data)
	{
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		m_render_mesh.reset( rres_mgr->alloc_static_mesh_indexed() );
		m_render_mesh->create(EDraw_TriangleList, vert_data, index_data, 0);
	}

	void nterrain_chunk_mesh_adapter::add_interlocked_tile_lod(const index_buffer16* index_data)
	{}

	void nterrain_chunk_mesh_adapter::draw_setup_effect(nshading_effect* effect_ptr)
	{
		m_render_mesh->draw_setup_effect(effect_ptr);
	}

	void nterrain_chunk_mesh_adapter::set_chunk(int chunk_size, int terrain_w, int terrain_h)
	{
		m_chunk_uv_scale.x = (terrain_w-1)/(chunk_size-1);
		m_chunk_uv_scale.y = (terrain_h-1)/(chunk_size-1);
	}

	nrender_mesh_section* nterrain_chunk_mesh_adapter::get_section(size_t section_index)
	{
		int sec_sel = 0;

		//-- 根据自身LOD和相邻chunk的LOD来选择section
		return m_render_mesh->get_section(0);
	}

	//-- class nterrain_chunk -------------------------------------------------------------
	nterrain_chunk::nterrain_chunk(const nstring& name_str):nmesh_component(name_str)
	{
		m_editable = false;
		//m_cast_dynamic_shadow = false;
	}

	nterrain_chunk::~nterrain_chunk(void)
	{
	}

	void nterrain_chunk::create(int x, int y, int chunk_size)
	{
		nterrain_actor* trn = ndynamic_cast<nterrain_actor>(m_owner);

		m_hmap_x = x;
		m_hmap_y = y;
		m_chunk_size = chunk_size;

		//-- render mesh
		create_render_mesh();

		//-- init bounding box
		_update_transform( trn->get_space() );
	}

	void nterrain_chunk::update_vertex_data(nterrain_chunk_vertex_data* vert_buffer, nsize& out_height_range)
	{
		nterrain_actor* trn = ndynamic_cast<nterrain_actor>(m_owner);

		nheight_map16* hmap = trn->get_height_map();
		
		vertex_stream* pos_stream = vert_buffer->get_stream(0);
		vector3* pos_ptr = (vector3*)pos_stream->get_data();

		vertex_stream* uv_stream = vert_buffer->get_stream(1);
		vector2* uv_ptr = (vector2*)uv_stream->get_data();

		vertex_stream* norm_stream = vert_buffer->get_stream(2);
		vector3* norm_ptr = (vector3*)norm_stream->get_data();

		vertex_stream* tan_stream = vert_buffer->get_stream(3);
		vector3* tan_ptr = (vector3*)tan_stream->get_data();

		vertex_stream* bi_stream = vert_buffer->get_stream(4);
		vector3* bi_ptr = (vector3*)bi_stream->get_data();

		float uv_w = m_chunk_size-1;
		float uv_h = m_chunk_size-1;
		int max_h = -1;
		int min_h = 65536;
		for(int j=0; j<m_chunk_size; j++)
		{
			int y = j+m_hmap_y;
			for (int i=0; i<m_chunk_size; i++)
			{
				int x = i+m_hmap_x;
				unsigned short h = hmap->get_value(x, y);
				if(h > max_h)
					max_h = h;
				if(h < min_h)
					min_h = h;

				*pos_ptr = vector3(x, h, y);
				pos_ptr++;

				*uv_ptr = vector2(i/uv_w, j/uv_h);
				uv_ptr++;

				//--计算normal
				vector3 scale = m_owner->get_space().scale;
				vector3 n(x, trn->get_raw_height(x, y+1),y+1);
				vector3 s(x, trn->get_raw_height(x, y-1),y-1);
				vector3 w(x - 1, trn->get_raw_height(x-1, y),y);
				vector3 e(x+1,trn->get_raw_height(x+1, y),y);
				n*=scale;
				s*=scale;
				w*=scale;
				e*=scale;

				vector3 c = vector3(x, h, y)*scale;

				vector3 east = vec_normalize(e - c);
				vector3 south = vec_normalize(s - c);
				vector3 west = vec_normalize(w - c);
				vector3 north = vec_normalize(n - c);

				vector3 normal =  vec_cross(east,south);
				normal  +=  vec_cross(south,west);
				normal	 +=  vec_cross(west,north);
				normal  +=  vec_cross(north,east);
				
				*tan_ptr++ = east;
				*bi_ptr++ = south;
				*norm_ptr++ = vec_normalize(normal);
			}
		}// end of for()

		out_height_range.x = max_h;
		out_height_range.y = min_h;
	}

	void nterrain_chunk::update_index_buffer(index_buffer16* indices)
	{			
		nterrain_actor* trn = ndynamic_cast<nterrain_actor>(m_owner);

		nbit_map* viz_map = trn->get_viz_map();

		for (int j=1; j<m_chunk_size; j+=2)
		{
			int y = j+m_hmap_y;
			for(int i=1; i<m_chunk_size; i+=2)
			{
				int x = i+m_hmap_x;

				if( viz_map->get_value(x, y) )
				{
					unsigned short c = get_index(i, j);

					//-- 顺时针建立三角形list
					indices->append_index(c);
					indices->append_index(get_index(i, j-1));
					indices->append_index(get_index(i+1, j-1));

					indices->append_index(c);
					indices->append_index(get_index(i+1, j-1));
					indices->append_index(get_index(i+1, j));

					indices->append_index(c);
					indices->append_index(get_index(i+1, j));
					indices->append_index(get_index(i+1, j+1));

					indices->append_index(c);
					indices->append_index(get_index(i+1, j+1));
					indices->append_index(get_index(i, j+1));					

					indices->append_index(c);
					indices->append_index(get_index(i, j+1));
					indices->append_index(get_index(i-1, j+1));

					indices->append_index(c);
					indices->append_index(get_index(i-1, j+1));
					indices->append_index(get_index(i-1, j));

					indices->append_index(c);
					indices->append_index(get_index(i-1, j));
					indices->append_index(get_index(i-1, j-1));

					indices->append_index(c);
					indices->append_index(get_index(i-1, j-1));
					indices->append_index(get_index(i, j-1));
				}// end of if				
			}// end of for()
		}// end of for()
	}

	void nterrain_chunk::post_vizmap_change(const nrect& region)
	{
		nterrain_actor* trn = ndynamic_cast<nterrain_actor>(m_owner);

		if( region.intersect(get_map_rect()) )
		{
			create_render_mesh();
		}
	}

	void nterrain_chunk::create_render_mesh()
	{
		nterrain_actor* trn = ndynamic_cast<nterrain_actor>(m_owner);

		nsize height_range;
		//-- init vertex data
		size_t num_vert = m_chunk_size*m_chunk_size;
		nASSERT( num_vert < 65536 );

		nterrain_chunk_vertex_data vert;
		vert.init(num_vert);
		update_vertex_data(&vert, height_range);

		//-- init indices : 创建LOD最精细基本的三角形list
		index_buffer16 indices;
		update_index_buffer(&indices);
		
		//-- create render mesh
		m_render_mesh.create(&vert, &indices);

		//-- create interlocked tile 

		//-- set chunk param
		m_render_mesh.set_chunk(m_chunk_size, trn->get_map_width(), trn->get_map_height());
		update_mesh_element();
	}

	void nterrain_chunk::post_heightmap_change(const nrect& region)
	{
		nterrain_actor* trn = ndynamic_cast<nterrain_actor>(m_owner);

		nrect my_rc = get_map_rect();
		if( region.intersect(my_rc)
			|| my_rc.intersect(region) )
		{
			nsize height_range;
			size_t num_vert = m_chunk_size*m_chunk_size;

			nterrain_chunk_vertex_data vert;
			vert.init(num_vert);
			update_vertex_data(&vert, height_range);

			m_render_mesh.update_vertex_buffer(&vert);

			update_bounds(height_range);

			for(size_t i=0;i<m_deco_chunk_list.size();++i)
			{
				uint32 size=m_deco_chunk_list[i]->get_owner()->get_grid_size();
				m_deco_chunk_list[i]->on_density_map_changed();
			}
		}

		// deco chunk重建了render mesh，所以这里也需要重建
		// todo:结构修改
		update_mesh_element();
	}

	void nterrain_chunk::_post_layer_change()
	{
		nterrain_actor* trn = ndynamic_cast<nterrain_actor>(m_owner);
		nterrain_mtl_setup* mtl = trn->get_material();
		m_mtl_high->post_layer_change(mtl, get_map_rect());
		update_mesh_element();
	}

	void nterrain_chunk::_post_layer_alpha_change(const nstring& layer_name, const nrect& region)
	{
		nrect my_rc = get_map_rect();
		if( region.intersect( my_rc )
			|| my_rc.intersect(region) )
		{
			nterrain_actor* trn = ndynamic_cast<nterrain_actor>(m_owner);
			nterrain_mtl_setup* mtl = trn->get_material();
			m_mtl_high->post_alphamap_change(mtl, layer_name, my_rc);
		}
	}

	void nterrain_chunk::update_bounds(nsize height_range)
	{
		int max_h = height_range.x;
		int min_h = height_range.y;
				
		AABBox raw_bounds;
		raw_bounds.m_min = vector3(m_hmap_x, min_h, m_hmap_y);
		raw_bounds.m_max = vector3(m_hmap_x+m_chunk_size-1, max_h, m_hmap_y+m_chunk_size-1);

		const vector3& scale = m_owner->get_space().scale;
		const vector3& pos = m_owner->get_space().location;

		raw_bounds.m_max *= scale;
		raw_bounds.m_min *= scale;
		raw_bounds.m_max += pos;
		raw_bounds.m_min += pos;

		m_bounds.box_extent = (raw_bounds.m_max-raw_bounds.m_min)*0.5f;
		m_bounds.origin = raw_bounds.get_center();		
		m_bounds.sphere_radius = vec_length(m_bounds.box_extent);
	}

	void nterrain_chunk::_update_transform(const object_space& parent_space)
	{
		nterrain_actor* trn = ndynamic_cast<nterrain_actor>(m_owner);
		nheight_map16* hmap = trn->get_height_map();

		int max_h = -1;
		int min_h = 65536;
		for(int j=0; j<m_chunk_size; j++)
		{
			int y = j+m_hmap_y;
			for (int i=0; i<m_chunk_size; i++)
			{
				int x = i+m_hmap_x;
				unsigned short h = hmap->get_value(x, y);
				if(h > max_h)
					max_h = h;
				if(h < min_h)
					min_h = h;
			}
		}// end of for()

		update_bounds( nsize(max_h, min_h) );

		for(size_t i=0;i<m_deco_chunk_list.size();++i)
		{
			uint32 size=m_deco_chunk_list[i]->get_owner()->get_grid_size();
			m_deco_chunk_list[i]->on_density_map_changed();
		}
	}

	nmtl_base* nterrain_chunk::get_material(int lod, int mtl_id)
	{
		const int LOW_MTL_LOD = 1;
		if( m_mtl_low 
			&& m_mtl_high)
		{
			if( lod < LOW_MTL_LOD )
				return m_mtl_high.get();
			else
				return m_mtl_low.get();
		}
		else
		{
			return nresource_manager::instance()->get_default_material();
		}
	}

	void nterrain_chunk::_post_material_create(nterrain_mtl_setup* mtl)
	{
		m_mtl_low = mtl->setup_render_mtl_low();
		m_mtl_high = mtl->setup_render_mtl_high(m_chunk_size);

		update_mesh_element();
	}

	void nterrain_chunk::update_mesh_element()
	{
		nrender_mesh* mesh = &m_render_mesh;
		m_mesh_elements.clear();
		for(size_t i=0; i<mesh->get_num_section(); i++)
		{	
			nrender_mesh_section* sec = mesh->get_section(i);
			nmtl_base* mtl = get_material(0, sec->get_material_id() );
			m_mesh_elements.push_back(mesh_element(this,mesh,sec,mtl));
		}

		for(size_t i=0; i<m_deco_chunk_list.size(); i++)
		{	
			m_deco_chunk_list[i]->get_mesh_element(0,m_mesh_elements,this);
		}
	}

	void nterrain_chunk::_on_device_lost(int param)
	{
		m_render_mesh.destroy();

		for(size_t i=0;i<m_deco_chunk_list.size();++i)
		{
			m_deco_chunk_list[i]->on_device_lost();
		}
	}
	
	bool nterrain_chunk::_on_device_reset(int param)
	{	
		for(size_t i=0;i<m_deco_chunk_list.size();++i)
		{
			m_deco_chunk_list[i]->on_device_reset();
		}

		create_render_mesh();
		return true; 
	}

	void nterrain_chunk::add_deco_layer( nterrain_deco_layer* layer )
	{
		nterrain_deco_chunk_base::ptr chunk=layer->create_chunk(this,m_hmap_x,m_hmap_y,m_chunk_size,m_chunk_size);
		if(chunk!=0)
		{
			m_deco_chunk_list.push_back(chunk);
			update_mesh_element();
		}
	}

	void nterrain_chunk::del_deco_layer( nterrain_deco_layer* layer )
	{
		vector<boost::shared_ptr<nterrain_deco_chunk_base> >::iterator i;
		for(i=m_deco_chunk_list.begin();i!=m_deco_chunk_list.end();++i)
		{
			if((*i)->get_owner()==layer)
			{
				m_deco_chunk_list.erase(i);
				update_mesh_element();
				return;
			}
		}
	}

	void nterrain_chunk::_post_deco_layer_change()
	{
		update_mesh_element();
	}

}//namespace nexus