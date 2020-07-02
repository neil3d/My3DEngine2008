#include "stdafx.h"
#include "nterrain_static_mesh_deco_chunk.h"
#include "../special_effect/ndistribution.h"
#include "nterrain_actor.h"
#include "nterrain_chunk.h"

namespace nexus
{
	//===================================================================================
	void static_deco_object::get_mesh_element( int lod,vector<mesh_element>& mesh_elements,nmesh_component* comp )
	{
		nresource_static_mesh::ptr	m_mesh=((nterrain_static_mesh_deco_layer*)m_owner)->get_mesh();
		if( m_mesh && m_mesh->ready() )
		{
			nrender_mesh* mesh = m_mesh->get_render_mesh(lod);

			for(size_t i=0; i<mesh->get_num_section(); i++)
			{	
				nrender_mesh_section* sec = mesh->get_section(i);
				nmtl_base* mtl = m_mesh->get_material(lod, sec->get_material_id() ).get();
				mesh_elements.push_back(mesh_element(m_matrix,mesh,sec,mtl,true));
			}
		}
	}

	static_deco_object::static_deco_object( nterrain_deco_layer* owner )
	{
		m_owner=owner;
	}
	//===================================================================================
	nterrain_static_mesh_deco_chunk::nterrain_static_mesh_deco_chunk(nterrain_deco_layer* owner,nterrain_chunk* terrain_chunk):nterrain_deco_chunk_base(owner,terrain_chunk)
	{
	}

	nterrain_static_mesh_deco_chunk::~nterrain_static_mesh_deco_chunk()
	{
	}

	terrain_deco_object::ptr nterrain_static_mesh_deco_chunk::add_instance( int x,int z,float wx,float wz )
	{
		static_deco_object* obj=new static_deco_object(m_owner);
		if(obj==0) return terrain_deco_object::ptr();
		float wy=m_owner->get_terrain_actor()->get_height(wx,wz)+m_owner->get_height_offset();

		float xv=_get_rand(m_owner->get_random_offset(),-1.0f*m_owner->get_random_offset());
		float yv=0;
		float zv=_get_rand(m_owner->get_random_offset(),-1.0f*m_owner->get_random_offset());
		float rv=_get_rand(m_owner->get_random_rotation(),-1.0f*m_owner->get_random_rotation());
		float sv=_get_rand(m_owner->get_max_random_scale(),m_owner->get_min_random_scale());

		obj->move(vector3(wx+xv,wy+yv,wz+zv),vector3(0,rv,0),vector3(sv,sv,sv));
		obj->m_X=x;	obj->m_Y=z;
		terrain_deco_object::ptr item(obj);
		m_object_list.push_back(item);
		return item;
	}

	bool nterrain_static_mesh_deco_chunk::on_density_map_changed( nterrain_deco_layer* layer ,const nrect& region)
	{
		if(layer!=m_owner) return false;
		if(!m_region.intersect(region) && !region.intersect(m_region))	return false;
		return on_density_map_changed();		
	}

	bool nterrain_static_mesh_deco_chunk::on_density_map_changed()
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
		return true;
	}

	void nterrain_static_mesh_deco_chunk::get_mesh_element( int lod,vector<mesh_element>& mesh_elements,nmesh_component* comp )
	{
		for(size_t i=0;i<m_object_list.size();++i)
		{
			static_deco_object* obj=(static_deco_object*)m_object_list[i].get();
			if(obj)
			{
				obj->get_mesh_element(lod,mesh_elements,comp);
			}
		}
	}
	//====================================================================================
	nDEFINE_CLASS(nterrain_static_mesh_deco_layer,nterrain_deco_layer);
	nterrain_static_mesh_deco_layer::nterrain_static_mesh_deco_layer()
	{
		m_mesh=nresource_static_mesh::ptr(nconstruct<nresource_static_mesh>(L"nresource_static_mesh",L"static_deco_mesh"));
	}

	nterrain_static_mesh_deco_layer::~nterrain_static_mesh_deco_layer()
	{

	}

	boost::shared_ptr<nterrain_deco_chunk_base> nterrain_static_mesh_deco_layer::create_deco_chunk(nterrain_chunk* terrain_chunk,int x,int y,int width,int height)
	{
		nterrain_static_mesh_deco_chunk* chunk=new nterrain_static_mesh_deco_chunk(this,terrain_chunk);
		float w=(float)m_terrain->get_map_width(),h=(float)m_terrain->get_map_height();
		chunk->create(x/w,y/h,width/w,height/h);
		chunk->on_density_map_changed();
		return nterrain_deco_chunk_base::ptr(chunk);
	}

	void nterrain_static_mesh_deco_layer::serialize( narchive& ar )
	{
		nterrain_deco_layer::serialize(ar);
	}
	//===================================================================================
	
};

