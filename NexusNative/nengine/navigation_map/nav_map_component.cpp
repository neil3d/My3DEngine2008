#include "stdafx.h"
#include "nav_map_component.h"
#include "nav_map_vertex_data.h"
#include "nav_map_actor.h"
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "../../ncore/height_map/nheight_map.h"
#include "../../ncore/height_map/height_map_util.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nav_map_component, nmesh_component)

	nav_map_component::nav_map_component( const nstring& name_str )
		: nmesh_component(name_str),m_mtl(_T("nav_map_mtl"))
	{
		m_cull_dist = 128.0f * 128.0f;
	}
	nav_map_component::~nav_map_component(void)
	{
	}

	void nav_map_component::create( nav_map_actor* nav, int x, int y, int chunk_size, float cull_dist )
	{
		m_hmap_x = x;
		m_hmap_y = y;
		m_chunk_size = chunk_size;
		m_cull_dist = cull_dist;

		//-- render mesh
		create_render_mesh();

		//-- create material		
		resource_location shader_loc(_T("engine_data:material/nav_map.hlsl"));
		m_mtl.default_create( shader_loc );	

		//-- init bounding box
		_update_transform( nav->get_space() );
	}

	void nav_map_component::post_heightmap_change( const nrect& region )
	{
		nrect my_rc(m_hmap_x, m_hmap_y, m_hmap_x+m_chunk_size, m_hmap_y+m_chunk_size);
		if( region.intersect(my_rc) )
		{
			nsize height_range;
			size_t num_vert = m_chunk_size*m_chunk_size;

			nav_map_vertex_data vert;
			vert.init(num_vert);
			update_vertex_data(&vert, height_range);

			m_mesh_ptr->update_vertex_buffer(&vert);

			update_bounds(height_range);
		}
	}

	void nav_map_component::_destroy()
	{
		m_mesh_ptr.reset();
		nmesh_component::_destroy();
	}

	nmtl_base* nav_map_component::get_material( int lod, int mtl_id )
	{
		(void)lod;
		(void)mtl_id;
		return &m_mtl;		
	}

	const matrix44& nav_map_component::get_world_matrix() const
	{
		return m_owner->get_world_matrix();
	}

	void nav_map_component::_update_transform( const object_space& parent_space )
	{
		(void)(parent_space);

		nav_map_actor* nav = ndynamic_cast<nav_map_actor>(m_owner);
		const nheight_map16* hmap = nav->get_height_map();

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
	}

	void nav_map_component::serialize( narchive& ar )
	{
		nmesh_component::serialize(ar);

		nSERIALIZE(ar, m_hmap_x);
		nSERIALIZE(ar, m_hmap_y);
		nSERIALIZE(ar, m_chunk_size);

		if( ar.is_loading() )
		{
			//-- render mesh
			create_render_mesh();
		}
	}

	void nav_map_component::_on_device_lost( int param )
	{
		(void)(param);
		m_mesh_ptr.reset();
	}

	bool nav_map_component::_on_device_reset( int param )
	{
		(void)(param);
		//-- render mesh
		create_render_mesh();
		return true;
	}

	void nav_map_component::update_vertex_data( nav_map_vertex_data* vert_buffer, nsize& out_height_range )
	{
		nav_map_actor* nav_actor = ndynamic_cast<nav_map_actor>(m_owner);
		const nheight_map16* hmap = nav_actor->get_height_map();
		const nbit_map* walkable_map = nav_actor->get_walkable_map();

		vertex_stream* pos_stream = vert_buffer->get_stream(0);
		vector3* pos_ptr = (vector3*)pos_stream->get_data();

		vertex_stream* diffuse_stream = vert_buffer->get_stream(1);
		color4f* diffuse_ptr = (color4f*)(diffuse_stream->get_data());

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

				// 指定顶点位置
				*pos_ptr = vector3(x, h, y);
				pos_ptr++;

				// 指定顶点颜色
				if( !nav_actor->get_walkable_map_visibale() || walkable_map->get_value(x,y) )
				{
					*diffuse_ptr = nav_actor->get_height_nav_color();
				}
				else
				{
					*diffuse_ptr = nav_actor->get_walkable_nav_color();
				}
				diffuse_ptr++;
			}
		}// end of for()

		out_height_range.x = max_h;
		out_height_range.y = min_h;
	}

	void nav_map_component::update_index_buffer( index_buffer16* index_data )
	{
		std::vector<unsigned short>& indices = index_data->data;

		unsigned short uiNumStrips = static_cast<unsigned short>(m_chunk_size-1);
		unsigned short uiLineStep = static_cast<unsigned short>(m_chunk_size);
		unsigned short uiStartVertex = 0;
		unsigned short uiStepX = 1;

		unsigned short uiNumIndices = 2*uiNumStrips*(m_chunk_size+1) - 2;

		indices.resize(uiNumIndices);
		unsigned short *p = &(*indices.begin());

		for (unsigned short j = 0; j < uiNumStrips; j++)
		{ 
			unsigned short uiVert = uiStartVertex;
			for (unsigned short k = 0; k < m_chunk_size; k++) 
			{
				*(p++) = uiVert;
				*(p++) = uiVert + uiLineStep;
				uiVert = uiVert + uiStepX;
			}
			uiStartVertex = uiStartVertex + uiLineStep;
			if (j+1 < uiNumStrips) // add degenerate triangle
			{
				*(p++) = (uiVert-uiStepX)+uiLineStep;
				*(p++) = uiStartVertex;
			}
		}
	}

	void nav_map_component::update_bounds( nsize height_range )
	{
		int max_h = height_range.x;
		int min_h = height_range.y;

		AABBox raw_bounds;
		raw_bounds.m_min = vector3(m_hmap_x, min_h, m_hmap_y);
		raw_bounds.m_max = vector3(m_hmap_x+m_chunk_size, max_h, m_hmap_y+m_chunk_size);

		const vector3& scale = m_owner->get_space().scale;
		const vector3& pos = m_owner->get_space().location;

		raw_bounds.m_max *= scale;
		raw_bounds.m_min *= scale;
		raw_bounds.m_max += pos;
		raw_bounds.m_min += pos;

		m_bounds.box_extent = raw_bounds.m_max-raw_bounds.m_min;
		m_bounds.origin = raw_bounds.get_center();		
		m_bounds.sphere_radius = vec_length(m_bounds.box_extent)*0.5f;
	}

	void nav_map_component::create_render_mesh()
	{
		nsize height_range;
		//-- init vertex data
		size_t num_vert = m_chunk_size*m_chunk_size;
		nASSERT( num_vert < 65536 );

		nav_map_vertex_data vert;
		vert.init(num_vert);
		update_vertex_data(&vert, height_range);

		//-- init indices : 创建LOD最精细基本的三角形list
		index_buffer16 indices;
		update_index_buffer(&indices);

		//-- create render mesh
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		m_mesh_ptr.reset( rres_mgr->alloc_static_mesh_indexed() );
		m_mesh_ptr->create(EDraw_TriangleStrip, &vert, &indices, 0);

		nrender_mesh* mesh = m_mesh_ptr.get();
		m_mesh_elements.clear();
		for(size_t i=0; i<mesh->get_num_section(); i++)
		{	
			nrender_mesh_section* sec = mesh->get_section(i);
			nmtl_base* mtl = get_material(0, sec->get_material_id() );
			m_mesh_elements.push_back(mesh_element(this,mesh,sec,mtl));
		}
	}
} // end of namespace nexus
