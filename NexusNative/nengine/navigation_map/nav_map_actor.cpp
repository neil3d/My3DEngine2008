#include "stdafx.h"
#include "nav_map_actor.h"
#include "../../ncore/height_map/height_map_util.h"
#include "../../ncore/height_map/nheight_map.h"
#include "../image/nimage.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nav_map_actor, nactor)

		const int NAV_MAP_CHUNK_SIZE = 65.0f;

	nav_map_actor::nav_map_actor( const nstring& name_str )
		: nactor(name_str)
	{
		m_serializable = false;
		m_kinematic = false;
		m_frozen = true;
		m_chunk_size = NAV_MAP_CHUNK_SIZE;
		m_cull_dist = 128.0f * 128.0f;
		m_height_nav_color = color4f(0.45f, 0.53f, 0.78f, 1.0f);
		m_walkable_nav_color = color4f(0.85f, 0.55f, 0.38f, 1.0f);
		m_walkable_map_visibale= true;
	}
	nav_map_actor::~nav_map_actor(void)
	{
	}

	void nav_map_actor::create( navigation_map::ptr nav_map)
	{
		// handle height map ptr
		m_nav_map = nav_map;

		recreate_chunks();

		if( !empty() )
		{
			// 移动actor到目标位置
			this->move(m_nav_map->get_location(), m_nav_map->get_scale());
		}
	}


	void nav_map_actor::move( const vector3& pos, const vector3& rot, const vector3& scale )
	{
		(void)rot;
		nactor::move(pos, vector3::zero, scale);
	}

	void nav_map_actor::move( const vector3& pos, const vector3& scale )
	{
		nactor::move(pos, vector3::zero, scale);
	}

	void nav_map_actor::post_heightmap_change( const nrect& region )
	{
		if( empty() )
			return;

		if(region.get_width() > 0
			&& region.get_height() > 0)
		{
			// 目前是一个简单的遍历，如果需要的话可以使用quad tree去优化
			for( st_chunk_array::iterator iter = m_chunks.begin();
				iter != m_chunks.end();
				++iter )
			{
				(*iter)->post_heightmap_change(region);
			}// end of for()
		}
	}

	void nav_map_actor::import_heightmap( const nstring& img_file_name )
	{
		if( empty() )
			return;

		if(!m_nav_map)
			return;

		nimage img;
		img.load_from_file(img_file_name);
		img.convert_format(EPF_R8G8B8);

		size_t img_w = img.get_width();
		size_t img_h = img.get_height();

		m_nav_map->resize(img_w, img_h);

		pixel_R8G8B8* pixel = static_cast<pixel_R8G8B8*>(img.get_pixel());
		for(size_t y=0; y<img_w; y++)
		{
			for(size_t x=0; x<img_h; x++)
			{
				m_nav_map->set_raw_height(x, y, pixel->R);
				pixel++;
			}
		}

		img.destroy();

		//--
		recreate_chunks( );

		post_heightmap_change(nrect(0, 0, img_w, img_h));
	}

	void nav_map_actor::recreate_chunks()
	{
		// 先移除所有的chunks
		for(st_chunk_array::iterator iter = m_chunks.begin();
			iter != m_chunks.end();
			++iter)
		{
			remove_component((*iter)->get_name());
		}
		// clear old data
		m_chunks.clear();

		if( empty() )
			return;

		int width = m_nav_map->get_raw_width(); 
		int height = m_nav_map->get_raw_height(); 

		if( width <= 0 || height <= 0)
			return;

		// create chunk components
		//-- create chunk components
		int yc = (height-1)/(m_chunk_size-1);
		int xc = (width-1)/(m_chunk_size-1);
		for(int y=0; y<yc; y++)
		{
			for(int x=0; x<xc; x++)
			{
				std::wostringstream ss;
				ss << _T("trn_chunk_") << x << _T('_') << y;

				nav_map_component::ptr ck = create_component_t<nav_map_component>( ss.str() );
				int px = x*(m_chunk_size-1);
				int py = y*(m_chunk_size-1);
				ck->create( this, px, py, m_chunk_size, m_cull_dist);

				//-- 虽然基类已经保存了components list，这里重复保存一次，方便处理
				m_chunks.push_back(ck);
			}
		}//end of for()
	}

	bool nav_map_actor::line_check( ncheck_result& ret, const vector3& start, const vector3& end, ELineCheckType check_type )
	{
		if(!(check_type&ELCT_Navigation))
			return false;

		if( empty() )
			return false;

		return m_nav_map->line_of_sight(start, end, false, ret);
	}

	int nav_map_actor::classify_side( const vector3& wpt )
	{
		if( empty() )
			return 0;

		return m_nav_map->classify_side(wpt);
	}


} // end of namespace nexus
