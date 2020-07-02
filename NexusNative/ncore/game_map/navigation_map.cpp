#include "stdafx.h"
#include "navigation_map.h"
#include "../height_map/height_map_util.h"

namespace nexus
{
	void navigation_map::create( size_t w, size_t h, const vector3& loc,const vector3& scale )
	{
		nASSERT(w > 0 && h > 0);

		//-- create height map
		m_height_map.create(w, h, 0);

		// Create Walkable map
		m_walkable_map.create(w, h, true);

		set_location(loc);
		set_scale(scale);
	}

	void navigation_map::blit( navigation_map* src_map, size_t x_offest, size_t y_offest )
	{
		m_height_map.blit(src_map->get_height_map(), x_offest, y_offest);
		m_walkable_map.blit(src_map->get_walkable_map(), x_offest, y_offest);
	}

	void navigation_map::resize( size_t w, size_t h )
	{
		nASSERT(w > 0 && h > 0);
		// 高度和宽度为发生变化
		if(m_height_map.get_width() == w
			&& m_height_map.get_height() == h)
		{
			return;
		}

		// 如果高度图还没有创建则创建高度图
		if(!m_height_map.is_valid())
		{
			m_height_map.create(w, h, 0);
			m_walkable_map.create(w, h, true);
			return;
		}

		nheight_map16::ptr old_map = m_height_map.create_sub_map(0,0,m_height_map.get_width(),m_height_map.get_height());
		m_height_map.create(w, h, 0);
		// 重新创建导航图
		m_walkable_map.create(w, h, true);
		if(old_map.get() != NULL)
		{
			m_height_map.blit(old_map.get(),0, 0);
		}
	}

	npoint navigation_map::world2tile( float wx, float wz ) const
	{
		const vector3& scale = m_scale;
		const vector3& pos = m_location;

		wx -= pos.x;
		wz -= pos.z;
		return npoint( long(wx/scale.x), long(wz/scale.z) );
	}


	nexus::npoint navigation_map::safe_world2tile( float x,float z ) const
	{
		const vector3& scale = m_scale;
		const vector3& pos = m_location;

		int hmap_w = get_raw_width();
		int hmap_h = get_raw_height();

		npoint p;
		p.x=long((x-pos.x)/scale.x);
		p.y=long((z-pos.z)/scale.z);
		if(p.x<0)p.x=0;
		if(p.x>=(long)hmap_w)p.x=hmap_w-1;
		if(p.y<0)p.y=0;
		if(p.y>=(long)hmap_h)p.y=hmap_h-1;
		return p;
	}

	vector3 navigation_map::tile2world( int tx, int tz ) const
	{
		const vector3& scale = m_scale;
		const vector3& pos = m_location;
		return vector3(tx*scale.x+pos.x, 
			get_tile_height(tx, tz)+pos.y,
			tz*scale.z+pos.z);
	}

	float navigation_map::get_tile_height( int tx, int tz ) const
	{
		if( m_height_map.is_valid() 
			&& m_height_map.is_valid_index(tx,tz) ) 
			return m_height_map.get_value(tx,tz) * m_scale.y + m_location.y;
		else
			return 0;
	}

	float navigation_map::get_world_height( float wx, float wz ) const
	{
		if( !m_height_map.is_valid() )
			return 0;

		const vector3& scale = m_scale;
		const vector3& pos = m_location;

		npoint pt = world2tile(wx, wz);
		float u = wx - (pt.x*scale.x+pos.x);
		float v = wz - (pt.y*scale.z+pos.z);

		u /= scale.x;
		v /= scale.z;

		/*	1---2
			|	|
			4---3		*/
		float p00 = m_height_map.get_value_safe(pt.x, pt.y);
		float p10 = m_height_map.get_value_safe(pt.x+1, pt.y);
		float p01 = m_height_map.get_value_safe(pt.x, pt.y+1);
		float p11 = m_height_map.get_value_safe(pt.x+1, pt.y+1);

		return quad_lerp(p00, p10, p01, p11, u, v)*scale.y+pos.y;
	}


	void navigation_map::serialize( nfile_system* fs,const nstring& pkg_name,const nstring& file_name, enum EFileMode mode )
	{
		narchive::ptr nav_ap = narchive::open_xml_archive(fs, mode, pkg_name, file_name);
		nstring dummy( _T("navigation_map"));
		nav_ap->object_begin(_T("navigation_map"),dummy);

		nSERIALIZE(*nav_ap, m_location);
		nSERIALIZE(*nav_ap, m_scale);

		//-- height map属性
		int hmap_w=0, hmap_h = 0;
		if( nav_ap->is_loading() )
		{
			nSERIALIZE(*nav_ap, hmap_w);
			nSERIALIZE(*nav_ap, hmap_h);			
		}
		else
		{
			hmap_w = m_height_map.get_width();
			hmap_h = m_height_map.get_height();
			nSERIALIZE(*nav_ap, hmap_w);
			nSERIALIZE(*nav_ap, hmap_h);
		}

		//-- 存取height map
		nstring raw_file_path;
		nstring::size_type fpos = file_name.find_last_of(_T("/"));
		if( fpos != nstring::npos )
			raw_file_path = file_name.substr(0, fpos+1);

		nstring hmap_file_name = raw_file_path + _T("navigation_map.raw");
		nstring walkable_file_name = raw_file_path + _T("walkable_map.raw");

		if( nav_ap->is_loading() )
		{
			if(hmap_w >0
				&& hmap_h>0)
			{
				m_height_map.create(hmap_w, hmap_h, 0);
				load_height_map_as_raw(fs,m_height_map, pkg_name, hmap_file_name);

				// 创建并读取可行走区域信息
				m_walkable_map.create(hmap_w, hmap_h, true);
				m_walkable_map.load_raw(fs, pkg_name, walkable_file_name);
			}
		}
		else
		{
			save_height_map_as_raw(fs,m_height_map, pkg_name, hmap_file_name);

			m_walkable_map.save_raw(fs, pkg_name, walkable_file_name);
		}	

		nav_ap->object_end();
		nav_ap->close();
	}

	bool navigation_map::get_tile_walkable( int tx, int tz ) const
	{
		// 使用高度图的index来判断可行走区域范围，因为高度图和可行走位图大小始终是一致的
		if( m_height_map.is_valid() 
			&& m_height_map.is_valid_index(tx,tz) ) 
		{
			return m_walkable_map.get_value(tx,tz);
		}
		else
		{
			return false;
		}
	}

	bool navigation_map::get_world_walkable( float wx, float wz ) const
	{
		if( !m_height_map.is_valid() )
			return false;

		npoint pt = world2tile(wx, wz);
		return get_tile_walkable(pt.x, pt.y);

	}

	void navigation_map::set_height( int x, int y, float value )
	{
		m_scale.y==0.0f ? 0.0f : m_height_map.checked_set(x, y, (long)((value-m_location.y)/m_scale.y));
	}

	bool navigation_map::line_of_sight( const vector3& start, const vector3& end, bool walkable_check, ncheck_result& out_hit)
	{
		bool find = false;
		npoint pt = world2tile(start.x, start.z);		
		float step = std::min(m_scale.x, m_scale.z);		
		float test_len = step;
		float max_test_len = vec_distance(start, end);
		vector3 line_dir = vec_normalize(end-start);
		int old_side = classify_side(start); 

		vector3 wpt;
		do
		{
			wpt = start + line_dir*test_len;
			pt = world2tile(wpt.x, wpt.z);

			test_len += step;

			if(walkable_check && !get_tile_walkable(pt.x, pt.y))
			{
				find = true;
				out_hit.location = tile2world(pt.x, pt.y);
				out_hit.dist = test_len;
				break;
			}

			if( is_valid_index(pt.x, pt.y) )
			{			
				if(classify_side(wpt) != old_side)
				{
					find = true;
					out_hit.location  = tile2world(pt.x, pt.y);
					out_hit.dist = test_len;
					break;
				}
			}
		}
		while(test_len < max_test_len);

		return find;
	}

	int navigation_map::classify_side( const vector3& wpt )
	{

		float map_h = get_world_height(wpt.x, wpt.z);

		if(map_h > wpt.y)
			return -1;
		else
			return 1;
	}

	int navigation_map::classify_side( const npoint& tile_pt, float test_height )
	{
		float map_h = get_tile_height(tile_pt.x, tile_pt.y);
		if(map_h > test_height)
			return -1;
		else
			return 1;
	}


	nexus::vector3 navigation_map::get_tile_normal( int x, int z ) const
	{
		//--see <<Game Programming Gems 3>>,4.2,Fast Heightfield normal cal
		/*
				h2
				|
				|
		h3______h______h1
				|
				|
				h4
		*/
		vector3 ret;
		ret.x=(get_tile_height(x-4,z) - get_tile_height(x+4,z));	//((h1-h3)
		ret.z=(get_tile_height(x,z-4) - get_tile_height(x,z+4));	//(h2-h4)
		ret.y=8.0f * m_scale.x;
		return vec_normalize(ret);

	}

	bool navigation_map::can_walkable_to( const npoint& start_pt,const npoint& end_pt, npoint& out_near_pos )
	{
		out_near_pos.x=start_pt.x;
		out_near_pos.y=start_pt.y;

		if(start_pt == end_pt)
		{
			return true;
		}

		int dx = end_pt.x >= start_pt.x ? end_pt.x - start_pt.x : start_pt.x - end_pt.x;
		int s1 = end_pt.x >= start_pt.x ? 1 : -1;
		int dy = end_pt.y >= start_pt.y ? end_pt.y - start_pt.y : start_pt.y - end_pt.y;
		int s2 = end_pt.y >= start_pt.y ? 1 : -1;

		int interchange = dy > dx ? 1 : 0;

		if( dy > dx ) 
			std::swap(dx, dy);

		int f = dy + dy - dx;

		npoint trace_pt = start_pt;
		for( int i=1; i<=dx; i++ )
		{
			if( f >= 0 )  
			{
				if( interchange ) 
					trace_pt.x += s1;
				else 
					trace_pt.y += s2;

				f -= dx + dx;
			}

			if( interchange ) 
				trace_pt.y += s2;
			else 
				trace_pt.x += s1;

			f += dy + dy;

			if( !get_tile_walkable(trace_pt.x,trace_pt.y) )
				return false;

			out_near_pos.x=trace_pt.x;
			out_near_pos.y=trace_pt.y;
		}

		//由于画线算法可能会漏掉终点，所以最后再判断一下终点是否可通过
		if(!get_tile_walkable(end_pt.x,end_pt.y))
		{
			return false;
		}

		return true;
	}
} // end of namespace nexus
