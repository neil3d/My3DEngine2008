#include "StdAfx.h"
#include "nchunked_terrain_impl.h"
#include "../resource/nresource_manager.h"
#include "../util/perlin.h"
#include "../framework/nengine.h"
#include "../image/nimage.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nterrain_chunk, nprimitive_component)

	////////////////////////////////////////////////////////////////////////////
	//	class nterrain_chunk_vertex_data
	////////////////////////////////////////////////////////////////////////////
	nstring nterrain_chunk_vertex_data::get_vertex_type_name() const
	{
		return _T("terrain_chunk_vertex");
	}

	nstring nterrain_chunk_vertex_data::get_vertex_factory_name() const
	{
		return _T("vfactory_terrain");
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nchunked_terrain_impl
	////////////////////////////////////////////////////////////////////////////
	nchunked_terrain_impl::nchunked_terrain_impl(void)
	{
	}

	nchunked_terrain_impl::~nchunked_terrain_impl(void)
	{
	}

	unsigned short _get_chunk_vert_index(size_t chunk_size, size_t x, size_t y)
	{
		return y*chunk_size + x;
	}

	void nchunked_terrain_impl::create(size_t w, size_t h, short init_h, size_t chunk_size)
	{
		nASSERT(w > 0 && h > 0);
		nASSERT(chunk_size < 256);

		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		//-- create height map
		m_height_map.create(w, h, init_h);

		//-- create render heightmap texutre
		m_heightmap_tex.reset( rres_mgr->alloc_heightmap() );
		m_heightmap_tex->create(w, h);

		//-- create chunk mesh
		// create vertex data
		m_vert_data.init(chunk_size*chunk_size);

		vector3* vert = static_cast<vector3*>( m_vert_data.get_stream_data(0) );
		
		for(size_t y=0; y<chunk_size; y++)
		{
			for(size_t x=0; x<chunk_size; x++)
			{
				vert->x = x;
				vert->y = 0;
				vert->z = y;
				vert++;
			}
		}

		// create index data
		vector<unsigned short>&	ib = m_index_data.data;		

		for(size_t y=1; y<chunk_size; y+=2)
		{
			for(size_t x=1; x<chunk_size; x+=2)
			{
				ib.push_back( _get_chunk_vert_index(chunk_size, x, y) );
				ib.push_back( _get_chunk_vert_index(chunk_size, x+1, y-1) );
				ib.push_back( _get_chunk_vert_index(chunk_size, x-1, y-1) );				

				ib.push_back( _get_chunk_vert_index(chunk_size, x, y) );
				ib.push_back( _get_chunk_vert_index(chunk_size, x+1, y+1) );
				ib.push_back( _get_chunk_vert_index(chunk_size, x+1, y-1) );				

				ib.push_back( _get_chunk_vert_index(chunk_size, x, y) );				
				ib.push_back( _get_chunk_vert_index(chunk_size, x-1, y+1) );
				ib.push_back( _get_chunk_vert_index(chunk_size, x+1, y+1) );
				
				ib.push_back( _get_chunk_vert_index(chunk_size, x, y) );	
				ib.push_back( _get_chunk_vert_index(chunk_size, x-1, y-1) );
				ib.push_back( _get_chunk_vert_index(chunk_size, x-1, y+1) );				
			}
		}// end of for
		
		m_chunk_mesh.reset( rres_mgr->alloc_static_mesh_indexed() );
		m_chunk_mesh->create(EDraw_TriangleList, &m_vert_data, &m_index_data, 0);
	}

	void nchunked_terrain_impl::destroy()
	{
		m_height_map.destroy();

		for(int i=0; i<MAX_ALPHA_LAYER; i++)
			m_layer_map[i].destroy();
	}

	void nchunked_terrain_impl::generate_noise(nrect rc, int numOctaves, float amplitude, float frequency)
	{
		if( rc.left < 0 )
			rc.left = 0;
		if( rc.top <0 )
			rc.top = 0;
		if( rc.right > m_height_map.get_width() )
			rc.right = m_height_map.get_width();
		if( rc.bottom > m_height_map.get_height() )
			rc.bottom = m_height_map.get_height();

		if( rc.get_width() <= 0
			|| rc.get_height() <= 0)
			return;

		Perlin pn(numOctaves, frequency, amplitude, rand());

		int ix,iy;
		float xStep = 1.0f/(rc.right-rc.left);
		float yStep = 1.0f/(rc.bottom-rc.top);

		for(iy=rc.top; iy<rc.bottom; iy++)
		{
			for(ix=rc.left; ix<rc.right; ix++)
			{
				float n = pn.Get(ix*xStep, iy*yStep);
				m_height_map.set_value(ix, iy, short(n));
			}
		}//end of for	

		post_height_changed(rc);
	}

	void nchunked_terrain_impl::post_height_changed(const nrect& rc)
	{
		m_heightmap_tex->copy_heightmap( &m_height_map );
	}

	void nchunked_terrain_impl::import_heightmap(const nstring& img_file_name)
	{
		nimage img;
		img.load_from_file(img_file_name);
		img.convert_format(EPF_R8G8B8);

		size_t img_w = img.get_width();
		size_t img_h = img.get_height();

		nASSERT(img_w == m_height_map.get_width());
		nASSERT(img_h == m_height_map.get_height());

		pixel_R8G8B8* pixel = static_cast<pixel_R8G8B8*>(img.get_pixel());
		
		for(size_t y=0; y<img_h; y++)
		{
			for(size_t x=0; x<img_w; x++)
			{
				m_height_map.set_value(x, y, pixel->R);
				pixel++;
			}
		}

		img.destroy();

		post_height_changed(nrect(0,0,img_w,img_h));
	}

	nmaterial_base* nchunked_terrain_impl::get_material() const
	{
		if( m_material )
			return m_material.get();
		else
			return nresource_manager::instance()->get_default_material();
	}

	void nchunked_terrain_impl::create_material_basic(const resource_location& texture_loc)
	{
		shared_ptr<nterrain_mtl_basic> mtl( nNew nterrain_mtl_basic );
		mtl->create(texture_loc);

		//--
		m_material = mtl;
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nterrain_chunk_render_mesh
	////////////////////////////////////////////////////////////////////////////
	nterrain_chunk_render_mesh::nterrain_chunk_render_mesh(void)
		: m_chunk_mesh(NULL), m_owner(NULL)
	{}

	nterrain_chunk_render_mesh::~nterrain_chunk_render_mesh(void)
	{}

	void nterrain_chunk_render_mesh::draw_setup_effect(nshading_effect* effect_ptr)
	{
		effect_ptr->set_param("TRN_MapBias", m_owner->get_map_bias());
		effect_ptr->set_texture("TRN_HeightMap", m_heightmap_tex);
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nterrain_chunk
	////////////////////////////////////////////////////////////////////////////
	nterrain_chunk::nterrain_chunk(const nstring& name_str)
		: nprimitive_component(name_str),m_terrain(NULL)
	{
		m_trans_flag.set(ETrans_Opaque);
	}

	nterrain_chunk::~nterrain_chunk(void)
	{}

	void nterrain_chunk::create(const npoint& map_bias, nchunked_terrain_impl* terrain)
	{
		m_map_bias = vector3(map_bias.x, 0, map_bias.y);
		m_terrain = terrain;

		m_mesh.init(this, terrain->get_chunk_mesh(), terrain->get_heightmap_texture());

		//-- todo
		float temp = 512*1024;
		m_bounds.box_extent.x = temp/2;
		m_bounds.box_extent.y = temp;
		m_bounds.box_extent.z = temp/2;
		m_bounds.sphere_radius = temp/2;		
	}
}//namespace nexus