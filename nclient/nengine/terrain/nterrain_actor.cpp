#include "StdAfx.h"
#include "nterrain_actor.h"
#include "nquad_tree_terrain.h"
#include "../util/perlin.h"
#include "../image/nimage.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nterrain_actor, nactor)

	nterrain_actor::nterrain_actor(const nstring& name_str)
		: nactor(name_str), m_scale(1,1,1)
	{
	}

	nterrain_actor::~nterrain_actor(void)
	{
	}

	void nterrain_actor::create(size_t w, size_t h, unsigned short init_h, size_t chunk_size)
	{
		nASSERT(w > 0 && h > 0);
		nASSERT(chunk_size < 256);

		//-- create height map
		m_height_map.create(w, h, init_h);

		//-- create component
		shared_ptr<nquad_tree_terrain> comp = create_component_t<nquad_tree_terrain>(_T("quad_tree_trn_render"));
		comp->create(&m_height_map, chunk_size);

		//--
		m_trn_component = comp;
	}

	void nterrain_actor::generate_noise(nrect rc, int numOctaves, float amplitude, float frequency)
	{
		_clip_rect(rc.left, rc.top, rc.right, rc.bottom, 
			m_height_map.get_width(), m_height_map.get_height());

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
				float n = pn.Get(ix*xStep, iy*yStep)+0.5f*amplitude;
				m_height_map.set_value(ix, iy, unsigned short(fabsf(n)));
			}
		}//end of for	

		//--
		m_trn_component->post_heightmap_change(rc, &m_height_map);
	}

	void nterrain_actor::create_material_basic(const resource_location& texture_loc)
	{
		m_trn_component->create_material_basic(texture_loc);
	}

	void nterrain_actor::import_heightmap(const nstring& img_file_name)
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

		//--
		m_trn_component->post_heightmap_change(nrect(0,0,img_w,img_h),
			&m_height_map);
	}

	void nterrain_actor::set_scale(const vector3& scale)
	{
		m_scale = scale;
		m_trn_component->set_scale(scale);
	}

	void nterrain_actor::set_position(const vector3& pos)
	{
		m_pos = pos;
		m_trn_component->set_position(pos);
	}

	ntexture_splatting::ptr nterrain_actor::create_texture_splatting(size_t alpha_w, size_t alpha_h)
	{
		return m_trn_component->create_texture_splatting(alpha_w, alpha_h);
	}

	bool nterrain_actor::line_check(ncheck_result& ret, const vector3& start, const vector3& end)
	{
		if( !m_height_map.is_valid() )
			return false;

		bool find = false;
		npoint pt = world2tile(start.x, start.z);		
		int old_side = 0; 
		float step = std::min(m_scale.x, m_scale.z);		
		float test_len = step;
		float max_test_len = vec_distance(start, end);
		vector3 line_dir = vec_normalize(end-start);

		vector3 wpt;
		while(true)
		{
			wpt = start + line_dir*test_len;
			pt = world2tile(wpt.x, wpt.z);

			if( m_height_map.is_valid_index(pt.x, pt.y) )
			{			
				if(old_side == 0)
					old_side = classify_side(wpt); // first valid side
				else
				{
					if(classify_side(wpt) != old_side)
					{
						find = true;
						ret.location = tile2world(pt.x, pt.y);
						break;
					}
				}
			}

			test_len += step;

			if(test_len > max_test_len)
				break;
		}

		return find;
	}

	int nterrain_actor::classify_side(const vector3& wpt)
	{
		float map_h = get_height(wpt.x, wpt.z);

		if(map_h > wpt.y)
			return -1;
		else
			return 1;
	}

	float nterrain_actor::get_height(float wx, float wz) const
	{
		if( !m_height_map.is_valid() )
			return 0;

		npoint pt = world2tile(wx, wz);
		float u = wx - (pt.x*m_scale.x+m_pos.x);
		float v = wz - (pt.y*m_scale.z+m_pos.z);

		u /= m_scale.x;
		v /= m_scale.z;

		/*	1---2
			|	|
			4---3		*/
		float p00 = m_height_map.get_value_safe(pt.x, pt.y);
		float p10 = m_height_map.get_value_safe(pt.x+1, pt.y);
		float p01 = m_height_map.get_value_safe(pt.x, pt.y+1);
		float p11 = m_height_map.get_value_safe(pt.x+1, pt.y+1);

		return quad_lerp(p00, p10, p01, p11, u, v)*m_scale.y+m_pos.y;
	}

	void nterrain_actor::post_heightmap_change(const nrect& region)
	{
		if( !m_height_map.is_valid() )
			return;

		if(region.get_width() > 0
			&& region.get_height() > 0)
		{
			m_trn_component->post_heightmap_change(region, &m_height_map);
		}
	}

	nmaterial_base* nterrain_actor::get_material()
	{
		return m_trn_component->get_material();
	}
}//namespace nexus