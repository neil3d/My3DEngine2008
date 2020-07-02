#include "stdafx.h"
#include "nterrain_deco_chunk_base.h"
#include "nterrain_actor.h"
#include "../special_effect/ndistribution.h"
#include "../framework/nengine.h"
#include "../../ncore/height_map/height_map_util.h"
#include "nterrain_chunk.h"

namespace nexus
{

	//==============================================================================================================================
	nterrain_deco_chunk_base::nterrain_deco_chunk_base(nterrain_deco_layer* owner,nterrain_chunk* terrain_chunk):m_owner(owner),m_terrain_chunk(terrain_chunk)
	{
	}

	nterrain_deco_chunk_base::~nterrain_deco_chunk_base()
	{
	}

	void nterrain_deco_chunk_base::create( float x, float y, float width,float height )
	{
		m_x=x;	m_y=y;	m_width=width;	m_height=height;
		if(m_owner)
		{
			m_region.left=m_x*m_owner->get_grid_size();
			m_region.right=(m_x+m_width)*m_owner->get_grid_size();
			m_region.top=m_y*m_owner->get_grid_size();
			m_region.bottom=(m_y+m_height)*m_owner->get_grid_size();
		}
	}
	//==============================================================================================================================
	nDEFINE_VIRTUAL_CLASS(nterrain_deco_layer, nobject)
	nterrain_deco_layer::nterrain_deco_layer():m_max_random_scale(1.0f),m_min_random_scale(1.0f)
	{
		m_terrain=0;	m_grid_size=256;	m_random_rotation=180;		m_random_seed=0;	m_random_offset=100;
		m_density=0.05f;					m_height_offset=0;
		m_density_map.reset(new nalpha_map());
	}

	nterrain_deco_layer::~nterrain_deco_layer()
	{
	}

	void nterrain_deco_layer::on_update_density_map(const nrect& region,chunk_list& chunks)
	{
		if(m_density_map->get_width()>0)
		{
			for(int i=0;i<(int)m_chunks.size();++i)
			{
				if(m_chunks[i]->on_density_map_changed(this,region))
				{
					chunks.insert(m_chunks[i]->m_terrain_chunk);
				}
			}
		}
	}

	void nterrain_deco_layer::initialize( nterrain_actor* terrain)
	{
		m_terrain=terrain;
	}

	nterrain_deco_chunk_base::ptr nterrain_deco_layer::create_chunk(nterrain_chunk* terrain_chunk, int x,int y,int width,int height )
	{
		nterrain_deco_chunk_base::ptr chunk=create_deco_chunk(terrain_chunk,x,y,width,height);
		if(chunk!=0)
		{
			m_chunks.push_back(chunk);
		}
		return chunk;
	}

	void nterrain_deco_layer::set_random_seed( int value )
	{
		m_random_seed=value;
		rebuild();
	}


	void nterrain_deco_layer::do_test()
	{
		for(uint32 i=0;i<m_grid_size;++i)
		{
			for(uint32 j=0;j<m_grid_size;++j)
			{
				m_density_map->set_value(i,j,_get_rand(1,255));
			}
		}
		chunk_list chunks;
		on_update_density_map(nrect(0,0,m_grid_size,m_grid_size),chunks);
		for each(nterrain_chunk* chunk in chunks)
		{
			chunk->_post_deco_layer_change();
		}
	}

	void nterrain_deco_layer::serialize( narchive& ar )
	{
		nSERIALIZE(ar,m_name);
		nSERIALIZE(ar,m_resource);
		nSERIALIZE(ar,m_random_rotation);
		nSERIALIZE(ar,m_random_offset);
		nSERIALIZE(ar,m_height_offset);
		nSERIALIZE(ar,m_min_random_scale);
		nSERIALIZE(ar,m_max_random_scale);
		nSERIALIZE(ar,m_density);
		nSERIALIZE(ar,m_grid_size);
		nSERIALIZE(ar,m_random_seed);

		if(ar.is_loading())
		{
			set_resource_loc(m_resource);
		}
		
		//序列化分布图
		nstring pkg_name = ar.get_file()->get_package();
		nstring file_name = ar.get_file()->get_file_name();
		nstring folder;

		nstring::size_type fpos = file_name.find_last_of(_T("/"));
		if( fpos != nstring::npos )
			folder = file_name.substr(0, fpos+1);

		nstring map_file_name = folder+m_name;
		map_file_name += _T("_density_map.raw");

		nfile_system* fs = nengine::instance()->get_file_sys();
		if( ar.is_loading() )
		{
			m_density_map->create(m_grid_size, m_grid_size, false);
			load_height_map_as_raw(fs,*m_density_map.get(),pkg_name, map_file_name);
		}
		else
		{
			save_height_map_as_raw(fs,*m_density_map.get(),pkg_name, map_file_name);
		}
	}

	void nterrain_deco_layer::set_density( float value )
	{
		m_density=value;
		rebuild();
	}

	void nterrain_deco_layer::rebuild()
	{
		for each(nterrain_deco_chunk_base::ptr chunk in m_chunks)
		{
			if(chunk)
			{
				chunk->on_density_map_changed();
				chunk->m_terrain_chunk->_post_deco_layer_change();
			}
		}
	}

	void nterrain_deco_layer::set_grid_size(size_t value)
	{
		m_grid_size=value;
		if(m_grid_size==0)
		{
			m_grid_size=__max(m_terrain->get_map_width(),m_terrain->get_map_height());
		}
			
		m_density_map->create(m_grid_size,m_grid_size,0);
	}
};

