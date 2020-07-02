#include "StdAfx.h"
#include <ostream>
#include "nterrain_mtl_setup.h"
#include "nterrain_actor.h"
#include "../framework/nengine.h"
#include "../image/nimage.h"
#include "../util/perlin.h"
#include "../resource/nresource_manager.h"
#include "../../ncore/height_map/height_map_util.h"

namespace nexus
{
	nterrain_mtl_setup::nterrain_mtl_setup(void):m_alpha_w(-1),m_alpha_h(-1), m_owner(NULL)
	{
	}

	nterrain_mtl_setup::~nterrain_mtl_setup(void)
	{
	}

	void nterrain_mtl_setup::reset(nterrain_actor* owner)
	{
		m_owner = owner;		
		m_layers.clear();
	}

	void nterrain_mtl_setup::create(const resource_location& basic_texture_loc, int alpha_map_w, int aplha_map_h)
	{
		if( m_owner == NULL)
			return;

		if(alpha_map_w == -1)
			alpha_map_w = m_owner->get_map_width()-1;
		if(aplha_map_h == -1)
			aplha_map_h = m_owner->get_map_height()-1;

		m_basic_texture = basic_texture_loc;
		m_alpha_w = alpha_map_w;
		m_alpha_h = aplha_map_h;

		m_owner->_post_material_create();
	}

	int nterrain_mtl_setup::add_layer(const layer_data_editor& ld)
	{
		if( m_owner == NULL)
			return -1;

		if (m_layers.size()>=TERRAIN_MTL_MAX_LAYER)
			return -1;
		if(m_alpha_w<=0
			|| m_alpha_h<=0) // 未创建多层模式
			return -1;

		layer_data new_data;
		new_data.copy_from(ld);
		m_layers.push_back(new_data);
		//--基础图层（第一个）不需要alpha
		if( m_layers.size() > 1 )
		{
			nalpha_map::ptr map( new nalpha_map );
			map->create(m_alpha_w, m_alpha_h, 0);
			m_layers.back().alpha_map = map;
		}

		m_owner->_post_terrain_layer_change();
		
		return m_layers.size()-1;		
	}

	void nterrain_mtl_setup::update_layer()
	{
		m_owner->_post_terrain_layer_change();
	}

	void nterrain_mtl_setup::remove_layer(size_t i)
	{
		if( m_owner == NULL)
			return;
		if( i<m_layers.size() )
			m_layers.erase( m_layers.begin()+i );
		
		m_owner->_post_terrain_layer_change();
	}

	void nterrain_mtl_setup::shift_up_layer(size_t i)
	{
		if( m_owner == NULL)
			return;
		
		if(i==0 || i>=m_layers.size())
			return;
		
		layer_data tmp = m_layers[i-1];
		m_layers[i-1] = m_layers[i];
		m_layers[i] = tmp;

		m_owner->_post_terrain_layer_change();
	}

	void nterrain_mtl_setup::shift_down_layer(size_t i)
	{
		if( m_owner == NULL)
			return;

		if( i>=m_layers.size()-1 )
			return;

		layer_data tmp = m_layers[i+1];
		m_layers[i+1] = m_layers[i];
		m_layers[i] = tmp;

		m_owner->_post_terrain_layer_change();
	}

	const layer_data& nterrain_mtl_setup::get_layer_data(size_t i) 
	{
		 return m_layers[i];
	}

	void nterrain_mtl_setup::set_layer_data(size_t i,const layer_data_editor& ld )
	{
		if( m_owner == NULL)
			return;

		nASSERT(i<m_layers.size());

		m_layers[i].copy_from(ld);
		update_layer();
	}

	nterrain_mtl_basic::ptr nterrain_mtl_setup::setup_render_mtl_low()
	{
		nterrain_mtl_basic::ptr mtl( new nterrain_mtl_basic(_T("TERRAIN_MTL_LOW")) );
		mtl->create(m_basic_texture);

		return mtl;
	}

	ntexture_splatting::ptr nterrain_mtl_setup::setup_render_mtl_high(int chunk_size)
	{
		ntexture_splatting::ptr mtl( new ntexture_splatting(_T("TERRAIN_MTL_HIGH")) );

		float wr = (float)m_alpha_w/(m_owner->get_map_width()-1);
		float hr = (float)m_alpha_h/(m_owner->get_map_height()-1);

		int s = chunk_size-1;
		mtl->_init(wr*s, hr*s);

		return mtl;
	}

	nstring nterrain_mtl_setup::get_layer_name(size_t i) const
	{
		nASSERT(i<m_layers.size());
		const layer_data& ld = m_layers[i];
		return ld.get_name();
	}

	void nterrain_mtl_setup::import_layer_alpha(const nstring& layer_name, const nstring& img_file_name)
	{
		if( m_owner == NULL)
			return;

		int layer_index = find_layer_by_name( layer_name );
		if( layer_index == -1)
			return;

		layer_data& layer = m_layers[layer_index];
		if( !layer.alpha_map )
		{
			layer.alpha_map.reset( new nalpha_map );
			layer.alpha_map->create(m_alpha_w, m_alpha_h, 0);
		}

		nalpha_map::ptr amap = layer.alpha_map;

		//-- load image
		nimage img;
		img.load_from_file(img_file_name);
		img.convert_format(EPF_R8G8B8);

		size_t img_w = img.get_width();
		size_t img_h = img.get_height();

		nASSERT(img_w == (size_t)m_alpha_w);
		nASSERT(img_h == (size_t)m_alpha_h);

		//-- 拷贝像素
		pixel_R8G8B8* pixel = static_cast<pixel_R8G8B8*>(img.get_pixel());

		for(size_t y=0; y<img_h; y++)
		{
			for(size_t x=0; x<img_w; x++)
			{
				amap->set_value(x, y, pixel->R);
				pixel++;
			}
		}

		img.destroy();

		//-- 通知actor更新渲染
		post_layer_alpha_change( layer_name, nrect(0,0,m_alpha_w,m_alpha_h) );
	}

	void nterrain_mtl_setup::generate_noise(const nstring& layer_name, nrect rc, int numOctaves, float amplitude, float frequency)
	{
		if( m_owner == NULL)
			return;

		int layer_index = find_layer_by_name( layer_name );
		if( layer_index == -1)
			return;

		layer_data& layer = m_layers[layer_index];
		if( !layer.alpha_map )
			return;

		nalpha_map::ptr layer_alpha_map = layer.alpha_map;

		if( rc.right == -1 )
			rc.right = layer_alpha_map->get_width();
		if( rc.bottom == -1 )
			rc.bottom = layer_alpha_map->get_height();

		clip_rect(rc.left, rc.top, rc.right, rc.bottom, 
			layer_alpha_map->get_width(), layer_alpha_map->get_height());

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
				layer_alpha_map->set_value(ix, iy, unsigned char(fabsf(n)));
			}
		}//end of for	

		//-- 通知actor更新渲染
		post_layer_alpha_change( layer_name, nrect(0,0,m_alpha_w,m_alpha_h) );
	}

	int nterrain_mtl_setup::find_layer_by_name(const nstring& layer_name) const
	{
		for (size_t i=0; i<m_layers.size(); i++)
		{
			const layer_data& ld = m_layers[i];
			if(layer_name == ld.get_name())
				return i;
		}
		return -1;
	}

	nalpha_map::ptr nterrain_mtl_setup::get_layer_alpha(const nstring& layer_name) const
	{
		nalpha_map::ptr ret;
		if( m_owner == NULL)
			return ret;

		int layer_index = find_layer_by_name( layer_name );
		if( layer_index == -1)
			return ret;

		return m_layers[layer_index].alpha_map;
	}

	nalpha_map::ptr nterrain_mtl_setup::get_layer_alpha(size_t i) const
	{
		nalpha_map::ptr ret;
		if( m_owner == NULL)
			return ret;
		
		if( i >= m_layers.size() )
			return ret;

		return m_layers[i].alpha_map;
	}

	void nterrain_mtl_setup::post_layer_alpha_change(const nstring& layer_name, nrect rc)
	{
		if(m_owner)
			m_owner->_post_layer_alpha_change(layer_name, rc);
	}

	bool nterrain_mtl_setup::calulate_alpha_relative(size_t layer_index, const nrect& rc)
	{
		if( layer_index >= m_layers.size())
			return false;

		const layer_data& ld = m_layers[layer_index];
		if( !ld.alpha_map )
			return false;

		nalpha_map::ptr amap = ld.alpha_map;
		
		const unsigned char MINI_RELATIVE = 8; // 小于这个值，则认为是不相关
		for (int y=rc.top; y<rc.bottom; y++)
		{
			for (int x=rc.left; x<rc.right; x++)
			{
				if( amap->get_value(x,y) >= MINI_RELATIVE )
					return true;
			}
		}

		return false;
	}

	int nterrain_mtl_setup::get_layer_index(const nstring& layer_name) const
	{
		return find_layer_by_name(layer_name);
	}

	void nterrain_mtl_setup::serialize(narchive& ar)
	{
		nSERIALIZE(ar, m_alpha_w);
		nSERIALIZE(ar, m_alpha_h);

		nSERIALIZE(ar, m_basic_texture);

		if( ar.is_loading() )
		{
			create(m_basic_texture, m_alpha_w, m_alpha_h);
		}

		//-- layers		
		nstring pkg_name = ar.get_file()->get_package();

		// 在关卡存盘时，每个图层存盘成一个单独的raw文件用来保存alpha
		const TCHAR* const LAYER_ARRAY_NAME = _T("terrain_mtl_setup_layers");
		nstring layer_class(_T("layer_data_c"));			

	/*	vector2 uv_scale;
		float uv_rotate;
		float spec,spec_power;*/
		resource_location texture_loc;
		resource_location normal_tex_loc;

		nfile_system* fs = nengine::instance()->get_file_sys();

		if( ar.is_loading() )
		{
			size_t num_layers = 0;
			ar.array_begin(LAYER_ARRAY_NAME, num_layers);
			for (size_t i=0; i<num_layers; i++)
			{
				layer_data ld;
				ar.object_begin(ELEM_ArrayItem, layer_class);
				nSERIALIZE(ar, ld.uv_scale);
				nSERIALIZE(ar, ld.uv_rotate);
				nSERIALIZE(ar, ld.spec);
				nSERIALIZE(ar, ld.spec_power);
				nSERIALIZE(ar, texture_loc);
				nSERIALIZE(ar, normal_tex_loc);

				ar.object_end();
				ar.array_next();
				
				ld.diffuse_tex = nresource_manager::instance()->load_texture_2d(texture_loc);
				if (normal_tex_loc.is_valid())
				{
					ld.normal_tex = nresource_manager::instance()->load_texture_2d(normal_tex_loc);
				}
				else
				{
					ld.normal_tex = NULL;
				}

				int layer_index = add_layer(ld);
				nASSERT( (int)i==layer_index );
				if( layer_index>0 )
				{
					load_height_map_as_raw(fs, *m_layers[i].alpha_map, pkg_name,
						get_serialize_alpha_map_name(ar, i) );					
				}
			}
			ar.array_end();
		}
		else
		{			
			size_t num_layers = m_layers.size();			
			
			ar.array_begin(LAYER_ARRAY_NAME, num_layers);
			for (size_t i=0; i<num_layers; i++)
			{
				layer_data ld = get_layer_data(i);
				texture_loc = ld.diffuse_tex->get_location();
				if(ld.normal_tex!=0)	
				{
					normal_tex_loc = ld.normal_tex->get_location();
				}
				ar.object_begin(ELEM_ArrayItem, layer_class);
				nSERIALIZE(ar, ld.uv_scale);
				nSERIALIZE(ar, ld.uv_rotate);
				nSERIALIZE(ar, ld.spec);
				nSERIALIZE(ar, ld.spec_power);
				nSERIALIZE(ar, texture_loc);
				nSERIALIZE(ar, normal_tex_loc);
				ar.object_end();

				//-- save alpha map as individual raw file
				if( i>0 )
				{
					save_height_map_as_raw(fs, *m_layers[i].alpha_map, pkg_name,
						get_serialize_alpha_map_name(ar, i) );					
				}// end of if

				ar.array_next();
			}
			ar.array_end();
		}// end of else
	}

	nstring nterrain_mtl_setup::get_serialize_alpha_map_name(narchive& ar, int layer_index) const
	{
		nstring pkg_name = ar.get_file()->get_package();
		nstring file_name = ar.get_file()->get_file_name();
		nstring folder;

		nstring::size_type fpos = file_name.find_last_of(_T("/"));
		if( fpos != nstring::npos )
			folder = file_name.substr(0, fpos+1);

		wostringstream oss;
		oss << folder << m_owner->get_name_str()
			<< _T("_alpha_")<<layer_index<<_T(".raw");				

		return oss.str();
	}
}//namespace nexus