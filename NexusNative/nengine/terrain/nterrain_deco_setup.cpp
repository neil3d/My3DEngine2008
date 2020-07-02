#include "stdafx.h"
#include "nterrain_deco_setup.h"
#include "nterrain_chunk.h"
#include "nterrain_actor.h"
#include "nterrain_static_mesh_deco_chunk.h"
#include "nterrain_grass_deco_chunk.h"
namespace nexus
{
	nDEFINE_CLASS(nterrain_deco_setup,nobject);

	typedef std::vector<boost::shared_ptr<nterrain_chunk> > st_chunk_array;
	//==============================================================================================================================
	nterrain_deco_setup::nterrain_deco_setup()
	{
	}

	nterrain_deco_setup::~nterrain_deco_setup()
	{
	}

	boost::shared_ptr<nterrain_deco_layer> nterrain_deco_setup::get_deco_layer( size_t index )
	{
		return m_deco_layers[index];
	}

	nterrain_deco_layer::ptr nterrain_deco_setup::get_deco_layer( const nstring& name )
	{
		for(uint32 i=0;i<m_deco_layers.size();++i)
		{
			if(m_deco_layers[i]->get_name()==name)
			{
				return m_deco_layers[i];
			}
		}
		return nterrain_deco_layer::ptr();
	}

	nterrain_deco_layer::ptr nterrain_deco_setup::add_deco_layer(deco_layer_type type,size_t size)
	{
		nterrain_deco_layer::ptr layer;
		switch (type)
		{
		case deco_layer_static_mesh:
			{
				layer.reset(new nterrain_static_mesh_deco_layer());
				if(layer!=0)
				{
					layer->initialize(m_terrain);
					layer->set_grid_size(size);
					for( size_t i=0;i<m_terrain->get_chunk_count();++i)
					{
						m_terrain->get_chunk(i)->add_deco_layer(layer.get());
					}
					m_deco_layers.push_back(layer);
				}
				break;
			}
		case deco_layer_grass:
			{
				layer.reset(new nterrain_grass_deco_layer());
				if(layer!=0)
				{
					layer->initialize(m_terrain);
					layer->set_grid_size(size);
					for( size_t i=0;i<m_terrain->get_chunk_count();++i)
					{
						m_terrain->get_chunk(i)->add_deco_layer(layer.get());
					}
					m_deco_layers.push_back(layer);
				}
				break;
			}
		}

		return layer;
	}

	nterrain_deco_layer::ptr nterrain_deco_setup::add_deco_layer( deco_layer_type type,const nstring& name,size_t size )
	{
		//首先寻找是否已经有重复命名的层
		nterrain_deco_layer::ptr layer=get_deco_layer(name);
		if(layer!=0) return nterrain_deco_layer::ptr();
		return add_deco_layer(type,size);
	}

	void nterrain_deco_setup::del_deco_layer( boost::shared_ptr<nterrain_deco_layer> layer )
	{
		nASSERT(m_terrain);
		for( size_t i=0;i<m_terrain->get_chunk_count();++i)
		{
			m_terrain->get_chunk(i)->del_deco_layer(layer.get());
		}
		std::vector<boost::shared_ptr<nterrain_deco_layer> >::iterator i=find(m_deco_layers.begin(),m_deco_layers.end(),layer);
		if(i!=m_deco_layers.end())
		{
			m_deco_layers.erase(i);
		}
	}

	void nterrain_deco_setup::del_deco_layer( int index )
	{
		if(index<0) return;
		std::vector<boost::shared_ptr<nterrain_deco_layer> >::iterator i=m_deco_layers.begin();
		advance(i,index);
		if(i!=m_deco_layers.end())
		{
			nterrain_deco_layer::ptr layer=*i;
			for( size_t j=0;j<m_terrain->get_chunk_count();++j)
			{
				m_terrain->get_chunk(j)->del_deco_layer(layer.get());
			}
			m_deco_layers.erase(i);
		}
	}

	void nterrain_deco_setup::reset( nterrain_actor* owner )
	{
		m_terrain=owner;
		m_deco_layers.clear();
	}

	void nterrain_deco_setup::post_deco_map_changed( nterrain_deco_layer* layer,const nrect& region )
	{
		std::set<nterrain_chunk*>	chunk_list;
		for(uint32 i=0;i<m_deco_layers.size();++i)
		{
			if(m_deco_layers[i].get()==layer)
			{
				m_deco_layers[i]->on_update_density_map(region,chunk_list);
				break;
			}
		}
		for each(nterrain_chunk* chunk in chunk_list)
		{
			chunk->_post_deco_layer_change();
		}
	}

	void nterrain_deco_setup::serialize( narchive& ar )
	{
		nSERIALIZE(ar,m_deco_layers);
		if(ar.is_loading())
		{
			for(uint32 i=0;i<m_deco_layers.size();++i)
			{
				m_deco_layers[i]->initialize(m_terrain);
			}
		}
	}

	void nterrain_deco_setup::create_chunks()
	{
		for(uint32 i=0;i<m_deco_layers.size();++i)
		{
			for( size_t j=0;j<m_terrain->get_chunk_count();++j)
			{
				m_terrain->get_chunk(j)->add_deco_layer(m_deco_layers[i].get());
			}
			m_deco_layers[i]->rebuild();
		}		
	}
	//==============================================================================================================================
};

