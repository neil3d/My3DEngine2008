#include "stdafx.h"
#include "nterrain_grass_deco_chunk.h"
#include "../special_effect/ndistribution.h"
#include "nterrain_actor.h"
#include "nterrain_chunk.h"

namespace nexus
{
	//===================================================================================
	nDEFINE_CLASS(nterrain_grass_deco_layer,nterrain_deco_layer);

	boost::shared_ptr<nterrain_deco_chunk_base> nterrain_grass_deco_layer::create_deco_chunk( nterrain_chunk* terrain_chunk,int x,int y,int width,int height )
	{
		nterrain_grass_deco_chunk* chunk=new nterrain_grass_deco_chunk(this,terrain_chunk);
		float w=(float)m_terrain->get_map_width(),h=(float)m_terrain->get_map_height();
		chunk->create(x/w,y/h,width/w,height/h);
		chunk->set_cross(m_cross);
		chunk->set_material(m_material);
		chunk->on_density_map_changed();
		return nterrain_deco_chunk_base::ptr(chunk);
	}

	void nterrain_grass_deco_layer::set_resource_loc( const resource_location& loc )
	{
		m_resource=loc;
		m_material=nmtl_base::create_from_file(loc);
		for each(nterrain_deco_chunk_base::ptr _chunk in m_chunks)
		{
			nterrain_grass_deco_chunk* chunk=(nterrain_grass_deco_chunk*)_chunk.get();
			if(chunk)
			{
				chunk->set_material(m_material);
			}
		}
	}

	nterrain_grass_deco_layer::nterrain_grass_deco_layer()
	{
		m_width=100.0f;	m_height=100.0f;	m_cross=false;
	}

	nterrain_grass_deco_layer::~nterrain_grass_deco_layer()
	{

	}

	void nterrain_grass_deco_layer::set_cross( bool value )
	{
		m_cross=value;
		nrect rc(0,0,m_grid_size,m_grid_size);
		for each(nterrain_deco_chunk_base::ptr _chunk in m_chunks)
		{
			nterrain_grass_deco_chunk* chunk=(nterrain_grass_deco_chunk*)_chunk.get();
			if(chunk)
			{
				chunk->set_cross(m_cross);
				chunk->on_density_map_changed(this,rc);
			}
		}
	}

	void nterrain_grass_deco_layer::serialize( narchive& ar )
	{
		nterrain_deco_layer::serialize(ar);
		nSERIALIZE(ar,m_width);
		nSERIALIZE(ar,m_height);
		nSERIALIZE(ar,m_cross);
	}
	//===================================================================================
	nterrain_grass_deco_chunk::nterrain_grass_deco_chunk(nterrain_deco_layer* owner,nterrain_chunk* terrain_chunk):nterrain_deco_chunk_base(owner,terrain_chunk)
	{
		m_grass_buffer.reset(new grass_deco_buffer());
	}

	nterrain_grass_deco_chunk::~nterrain_grass_deco_chunk()
	{
	}

	terrain_deco_object::ptr nterrain_grass_deco_chunk::add_instance( int x,int z,float wx,float wz )
	{
		grass_deco_object* obj=new grass_deco_object(m_owner);
		if(obj==0) return terrain_deco_object::ptr();
		nterrain_grass_deco_layer* owner=(nterrain_grass_deco_layer*)m_owner;
		float wy=owner->get_terrain_actor()->get_height(wx,wz)+m_owner->get_height_offset();

		float xv=_get_rand(owner->get_random_offset(),-1.0f*owner->get_random_offset());
		float yv=0;
		float zv=_get_rand(owner->get_random_offset(),-1.0f*owner->get_random_offset());

		float rv=_get_rand(owner->get_random_rotation(),-1.0f*owner->get_random_rotation())*(nPI/180.0f);
		float sv=_get_rand(owner->get_max_random_scale(),owner->get_min_random_scale());

		obj->move(vector3(wx+xv,wy+yv,wz+zv),vector3(0,rv,0),vector3(owner->get_width()*sv,owner->get_height()*sv,owner->get_width()*sv));

		obj->m_X=x;	obj->m_Y=z;
		terrain_deco_object::ptr item(obj);
		m_object_list.push_back(item);
		m_grass_buffer->add_instance(obj->m_matrix);
		return item;
	}

	void nterrain_grass_deco_chunk::get_mesh_element( int lod,vector<mesh_element>& mesh_elements,nmesh_component* comp)
	{
		m_grass_buffer->get_mesh_element(lod,mesh_elements,comp);
	}

	bool nterrain_grass_deco_chunk::on_density_map_changed( nterrain_deco_layer* layer,const nrect& region )
	{
		if(layer!=m_owner) return false;
		if(!m_region.intersect(region) && !region.intersect(m_region))	return false;
		return on_density_map_changed();		
	}

	bool nterrain_grass_deco_chunk::on_density_map_changed()
	{
		nalpha_map::ptr map=m_owner->get_density_map();
		int x=m_x*map->get_width();
		int y=m_y*map->get_height();
		int width=m_width*map->get_width();
		int height=m_height*map->get_height();

		float terrain_width=m_owner->get_terrain_actor()->get_scale().x*(float)m_owner->get_terrain_actor()->get_map_width();
		float terrain_height=m_owner->get_terrain_actor()->get_scale().z*(float)m_owner->get_terrain_actor()->get_map_height();
		vector3 offset=m_owner->get_terrain_actor()->get_space().location;

		//种植网格中的半格，对应世界空间中的大小
		float half_x=1.0f/map->get_width()*terrain_width;
		float half_y=1.0f/map->get_height()*terrain_height;

		m_grass_buffer->begin_update();
		m_object_list.clear();
		for(int i=0;i<width;++i)
		{
			int ix=i+x;
			float tx=ix/(float)map->get_width();
			for(int j=0;j<height;++j)
			{
				int jy=j+y;
				float ty=jy/(float)map->get_height();
				uint32 v=map->get_value(ix,jy);
				if(v>0)
				{
					float pos_x=tx*terrain_width+offset.x;
					float pos_y=ty*terrain_height+offset.z;
					uint32 count=(uint32)(m_owner->get_density()*v);
					for(uint32 k=0;k<count;++k)
					{
						add_instance(ix,jy,pos_x+_get_rand(half_x,-1.0f*half_x),pos_y+_get_rand(half_y,-1.0f*half_y));
					}
				}
			}
		}
		m_grass_buffer->end_update();
		return true;
	}

	void nterrain_grass_deco_chunk::set_material( nmtl_base::ptr material )
	{
		m_grass_buffer->set_material(material);
	}
	//==============================================================================================================================
	
};

