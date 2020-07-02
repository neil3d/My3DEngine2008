#include "StdAfx.h"
#include "nterrain_actor.h"
#include "nquad_tree_terrain.h"
#include "terrain_water.h"
#include "../util/perlin.h"
#include "../image/nimage.h"
#include "../framework/nengine.h"
#include "height_map_util.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nterrain_actor, nactor)

	nterrain_actor::nterrain_actor(const nstring& name_str)
		: nactor(name_str), m_chunk_size(65)
	{
		m_frozen = true;
	}

	nterrain_actor::~nterrain_actor(void)
	{
	}

	const TCHAR* const TRN_RENDER_COMPONENT_NAME = _T("quad_tree_trn_render");
	const TCHAR* const TRN_WATER_COMPONENT_NAME = _T("terrain_water_comp");
	void nterrain_actor::create(size_t w, size_t h, unsigned short init_h, size_t chunk_size)
	{
		nASSERT(w > 0 && h > 0);
		nASSERT(chunk_size < 256);

		m_chunk_size = chunk_size;
		//-- create height map
		m_height_map.create(w, h, init_h);

		//--
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		//-- create render heightmap texutre
		m_heightmap_tex.reset( rres_mgr->alloc_heightmap() );
		m_heightmap_tex->create(w, h);

		//-- create components
		shared_ptr<nquad_tree_terrain> comp = create_component_t<nquad_tree_terrain>(TRN_RENDER_COMPONENT_NAME);
		comp->create(&m_height_map, m_heightmap_tex.get(), chunk_size);

		//--
		m_trn_component = comp;
	}

	void nterrain_actor::create_water(int water_h, size_t chunk_size)
	{
		shared_ptr<nterrain_water> water_comp = create_component_t<nterrain_water>(TRN_WATER_COMPONENT_NAME);
		water_comp->create(water_h, chunk_size);

		//--
		m_water_component = water_comp;
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

	void nterrain_actor::create_water_material_basic(const resource_location& texture_loc)
	{
		m_water_component->create_material_basic(texture_loc);
	}

	void nterrain_actor::create_water_material_natural(int render_target_w, int render_target_h,
		const resource_location& detail_map, const resource_location& bump_map)
	{
		m_water_component->create_material_natural(render_target_w, render_target_h,
			detail_map, bump_map);
	}

	void nterrain_actor::import_heightmap(const nstring& img_file_name)
	{
		nimage img;
		img.load_from_file(img_file_name);
		img.convert_format(EPF_R8G8B8);

		size_t img_w = img.get_width();
		size_t img_h = img.get_height();

		if(img_w != m_height_map.get_width()
			||img_h != m_height_map.get_height())
		{
			nthrow(_T("Image-Terrain Size Mismatch."));
		}

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
		float step = std::min(m_space.scale.x, m_space.scale.z);		
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
						ret.dist = test_len;
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

		const vector3& scale = m_space.scale;
		const vector3& pos = m_space.location;

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

	void nterrain_actor::serialize(narchive& ar)
	{
		nactor::serialize(ar);

		//------------------------------------------
		nSERIALIZE(ar, m_chunk_size);

		//-- height mapÊôÐÔ
		int hmap_w=0, hmap_h = 0;
		if( ar.is_loading() )
		{
			nSERIALIZE(ar, hmap_w);
			nSERIALIZE(ar, hmap_h);			
		}
		else
		{
			hmap_w = m_height_map.get_width();
			hmap_h = m_height_map.get_height();
			nSERIALIZE(ar, hmap_w);
			nSERIALIZE(ar, hmap_h);
		}
		

		//-- ´æÈ¡height map
		nstring pkg_name = ar.get_file()->get_package();
		nstring file_name = ar.get_file()->get_file_name();
		nstring hmap_file_name;

		nstring::size_type fpos = file_name.find_last_of(_T("/"));
		if( fpos != nstring::npos )
			hmap_file_name = file_name.substr(0, fpos+1);
		
		hmap_file_name += m_name.name_str;
		hmap_file_name += _T("_hmap.raw");

		if( ar.is_loading() )
		{
			m_height_map.create(hmap_w, hmap_h, 0);
			load_height_map_as_raw(m_height_map, pkg_name, hmap_file_name);
		}
		else
		{
			save_height_map_as_raw(m_height_map, pkg_name, hmap_file_name);
		}		
	}

	void nterrain_actor::_level_loaded(nlevel* level_ptr)
	{
		m_owner = level_ptr;

		//-- create render heightmap texutre
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		m_heightmap_tex.reset( rres_mgr->alloc_heightmap() );
		m_heightmap_tex->create(m_height_map.get_width(),
			m_height_map.get_height());

		//--
		for(st_component_list::iterator iter = m_component_list.begin();
			iter != m_component_list.end();
			++iter)
		{
			nactor_component::ptr comp = *iter;
			if( comp->get_name().name_str == TRN_RENDER_COMPONENT_NAME )
				m_trn_component = boost::dynamic_pointer_cast<nquad_tree_terrain>(comp);
			else if( comp->get_name().name_str == TRN_WATER_COMPONENT_NAME )
				m_water_component = boost::dynamic_pointer_cast<nterrain_water>(comp);
			comp->_level_loaded(this);
		}

		nrect rc(0,0,
			m_height_map.get_width(),
			m_height_map.get_height()
			);
		this->post_heightmap_change(rc);
	}

	void nterrain_actor::_destroy()
	{
		nactor::_destroy();

		m_height_map.destroy();
		m_heightmap_tex.reset();
		m_trn_component.reset();
		m_water_component.reset();
	}
}//namespace nexus