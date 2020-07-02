#include "StdAfx.h"
#include "nterrain_mtl.h"
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"
#include "../image/nimage.h"
#include "../util/perlin.h"
#include "nterrain_actor.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nterrain_mtl, nmaterial_base)
	nDEFINE_NAMED_CLASS(nterrain_mtl_basic, nterrain_mtl)
	nDEFINE_NAMED_CLASS(ntexture_splatting, nterrain_mtl)

	//////////////////////////////////////////////////////////////////////////
	// class nterrain_mtl
	//////////////////////////////////////////////////////////////////////////
	nterrain_mtl::nterrain_mtl(const nstring& name_str):nmaterial_base(name_str)
	{
	}

	nterrain_mtl::~nterrain_mtl(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// class nterrain_mtl_basic
	//////////////////////////////////////////////////////////////////////////
	nterrain_mtl_basic::nterrain_mtl_basic(const nstring& name_str):nterrain_mtl(name_str)
	{}
	
	nterrain_mtl_basic::~nterrain_mtl_basic(void)
	{}

	void nterrain_mtl_basic::create(const resource_location& texture_loc)
	{
		m_template = nresource_manager::instance()->load_material_template_script(
			resource_location(_T("engine_data:material/TRN_mtl_basic.hlsl"))
			);

		m_diffuse_map = nresource_manager::instance()->load_texture_2d(texture_loc);
	}

	void nterrain_mtl_basic::draw_effect_param(nshading_effect* effect_ptr) const
	{
		effect_ptr->set_texture("MTL_DiffuseMap", m_diffuse_map->get_render_texture());
	}

	//////////////////////////////////////////////////////////////////////////
	// class ntexture_splatting
	//////////////////////////////////////////////////////////////////////////
	ntexture_splatting::ntexture_splatting(const nstring& name_str):nterrain_mtl(name_str)
	{}

	ntexture_splatting::~ntexture_splatting(void)
	{}

	void ntexture_splatting::_init(size_t w, size_t h)
	{
		m_alpha_size.x = w;
		m_alpha_size.y = h;

		m_alpha_tex.reset(
			nengine::instance()->get_render_res_mgr()->alloc_alphamap()
			);
		m_alpha_tex->create(w, h);

		//--
		nstring engine_pkg_name = nengine::instance()->get_engine_package_name();
		m_template = nresource_manager::instance()->load_material_template_script(
			resource_location(engine_pkg_name, _T("material/TRN_tex_splatting.hlsl"))
			);		
	}

	void ntexture_splatting::set_layer_texture(size_t layer_index, const resource_location& texture_loc)
	{
		nASSERT(layer_index < MAX_LAYERS);

		layer_data& layer = m_layers[layer_index];

		layer.diffuse_tex = nresource_manager::instance()->load_texture_2d(texture_loc);
		layer.alpha_map.reset( new nalpha_map );
		layer.alpha_map->create(m_alpha_size.x, m_alpha_size.y, 0);
		post_alphamap_change( layer_index, nrect(0,0,m_alpha_size.x,m_alpha_size.y) );
	}

	void ntexture_splatting::set_layer_uv_param(size_t layer_index, const vector2& scale, float rotate)
	{
		nASSERT(layer_index < MAX_LAYERS);

		layer_data& layer = m_layers[layer_index];
		layer.uv_scale = scale;
		layer.uv_rotate = rotate;
	}

	void ntexture_splatting::import_layer_alpha(size_t layer_index, const nstring& img_file_name)
	{
		nASSERT(layer_index < MAX_LAYERS);
		nASSERT(layer_index >= 0);

		layer_data& layer = m_layers[layer_index];
		if( !layer.alpha_map )
		{
			layer.alpha_map.reset( new nalpha_map );
			layer.alpha_map->create(m_alpha_size.x, m_alpha_size.y, 0);
		}

		nalpha_map::ptr amap = layer.alpha_map;

		//-- load image
		nimage img;
		img.load_from_file(img_file_name);
		img.convert_format(EPF_R8G8B8);

		size_t img_w = img.get_width();
		size_t img_h = img.get_height();

		nASSERT(img_w == m_alpha_size.x);
		nASSERT(img_h == m_alpha_size.y);

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

		//--
		post_alphamap_change( layer_index, nrect(0,0,img_w,img_h) );
	}

	void ntexture_splatting::draw_effect_param(nshading_effect* effect_ptr) const
	{
		matrix44 uv_mat_array[MAX_LAYERS];
		for(int i=0; i<MAX_LAYERS; i++)
			uv_mat_array[i] = m_layers[i].uv_mat;
		effect_ptr->set_matrix_array("TRN_UVTransform", uv_mat_array, MAX_LAYERS);
		
		effect_ptr->set_texture("MTL_LayerAlpha", m_alpha_tex.get());

		for(int i=0; i<MAX_LAYERS; i++)
		{
			const layer_data& layer = m_layers[i];
			if( layer.diffuse_tex.get() )
			{
				std::ostringstream oss;
				oss << "MTL_LayerDiffuse" << i;

				effect_ptr->set_texture(oss.str().c_str(), layer.diffuse_tex->get_render_texture());
			}
		}

	}

	void ntexture_splatting::post_alphamap_change(size_t layer_index, const nrect& region)
	{
		if( layer_index == 0)	// 第0层不需要alpha
			return;

		layer_data& layer = m_layers[layer_index];
		
		if( layer.alpha_map )
		{
			size_t channel = layer_index-1;
			m_alpha_tex->copy_alpha(channel,
				region,
				layer.alpha_map.get()
				);
		}
	}

	void ntexture_splatting::generate_noise(size_t layer_index, nrect rc, int numOctaves, float amplitude, float frequency)
	{
		nASSERT(layer_index < MAX_LAYERS);		

		layer_data& layer = m_layers[layer_index];
		if( !layer.alpha_map )
			return;

		nalpha_map::ptr layer_alpha_map = layer.alpha_map;

		if( rc.right == -1 )
			rc.right = layer_alpha_map->get_width();
		if( rc.bottom == -1 )
			rc.bottom = layer_alpha_map->get_height();

		_clip_rect(rc.left, rc.top, rc.right, rc.bottom, 
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

		//--
		post_alphamap_change(layer_index, rc);
	}
}//namespace nexus