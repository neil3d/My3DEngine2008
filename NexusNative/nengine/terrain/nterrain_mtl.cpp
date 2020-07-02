#include "StdAfx.h"
#include <sstream>
#include "nterrain_mtl.h"
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"
#include "../image/nimage.h"
#include "../util/perlin.h"
#include "nterrain_actor.h"
#include "../../ncore/height_map/height_map_util.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nterrain_mtl, nmtl_base)
	nDEFINE_NAMED_CLASS(nterrain_mtl_basic, nterrain_mtl)
	nDEFINE_NAMED_CLASS(ntexture_splatting, nterrain_mtl)

	//////////////////////////////////////////////////////////////////////////
	// class nterrain_mtl
	//////////////////////////////////////////////////////////////////////////
	nterrain_mtl::nterrain_mtl(const nstring& name_str):nmtl_base(name_str)
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

	const TCHAR* const TRN_MTL_BASIC_NAME = _T("TRNMtlTechBasic");
	void nterrain_mtl_basic::create(const resource_location& texture_loc)
	{
		//-- 创建pass对象		
		resource_location shader_loc(_T("engine_data:material/TRN_mtl_basic.hlsl"));
		create_from_hlsl(TRN_MTL_BASIC_NAME, shader_loc);

		//-- 加载贴图
		m_diffuse_map = nresource_manager::instance()->load_texture_2d(texture_loc);
	}

	void nterrain_mtl_basic::setup_effect( nshading_effect* effect_ptr) const
	{
		{
			effect_ptr->set_texture("MTL_DiffuseMap", m_diffuse_map->get_render_texture());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// class ntexture_splatting
	//////////////////////////////////////////////////////////////////////////
	ntexture_splatting::ntexture_splatting(const nstring& name_str):nterrain_mtl(name_str)
	{}

	ntexture_splatting::~ntexture_splatting(void)
	{}

	const TCHAR* const TRN_MTL_SPLAT_NAME = _T("TRNMtlTechSplat");		
	void ntexture_splatting::_init(size_t w, size_t h)
	{
		//-- 创建Alpha texture
		m_alpha_tex.reset(
			nengine::instance()->get_render_res_mgr()->alloc_alphamap()
			);
		m_alpha_tex->create(w, h);
		
		//-- 创建tech

		resource_location shader_loc(_T("engine_data:material/TRN_tex_splatting.hlsl"));		
		
		create_from_hlsl(TRN_MTL_SPLAT_NAME, shader_loc);
	}

	void ntexture_splatting::set_layer_texture(size_t layer_index, 
		const resource_location& texture_loc,
		const resource_location& normal_tex_loc )
	{
		nASSERT(layer_index < m_layers.size());

		mtl_layer_data& layer = m_layers[layer_index];
		layer.diffuse_tex = nresource_manager::instance()->load_texture_2d(texture_loc);
		if (normal_tex_loc.is_valid())
		{
			layer.normal_tex =  nresource_manager::instance()->load_texture_2d(normal_tex_loc);
		}
	}

	void ntexture_splatting::set_layer_uv_param(size_t layer_index, const vector2& scale, float rotate)
	{
		nASSERT(layer_index < MAX_LAYERS);

		mtl_layer_data& layer = m_layers[layer_index];		
		layer.update_uv_mat(scale, rotate);
	}


	void ntexture_splatting::setup_effect(nshading_effect* effect_ptr) const
	{
		if (m_layers.empty())
		{
			return;
		}

		vector<matrix44> uv_mat_array(m_layers.size());
		float specs[5];
		float spec_powers[5];
		
		for(int i=0; i<m_layers.size(); i++)
		{
			const mtl_layer_data& layer = m_layers[i];
			nrender_texture* rtx = NULL;
			rtx = layer.diffuse_tex->get_render_texture();
			effect_ptr->set_texture(layer.diffuse_name, rtx);	
			if (layer.normal_tex)
			{
				rtx = layer.normal_tex->get_render_texture();
				effect_ptr->set_texture(layer.normal_name, rtx);	
			}
			
			specs[i] = layer.spec;
			spec_powers[i] = layer.spec_power;
			uv_mat_array[i] = m_layers[i].uv_mat;
		}// end of for()
		
		effect_ptr->set_matrix_array("TRN_UVTransform", &uv_mat_array[0], m_layers.size());
		effect_ptr->set_texture("MTL_LayerAlpha", m_alpha_tex.get());	
		effect_ptr->set_float_array("MTL_LayerSpec",specs,5);
		effect_ptr->set_float_array("MTL_LayerSpecPower",spec_powers,5);
	}

	void ntexture_splatting::post_alphamap_change(nterrain_mtl_setup* trn_mtl, const nstring& layer_name, const nrect& chunk_rc)
	{
		// 检测mtl setup中对这个chunk有效的前5个图层，并保证其顺序

		//--临时：重新计算所有相关图层
		post_layer_change(trn_mtl, chunk_rc);
	}

	void ntexture_splatting::mtl_layer_data::operator = (const layer_data& layer)
	{
		update_uv_mat(layer.uv_scale, layer.uv_rotate);
		spec = layer.spec;
		spec_power = layer.spec_power;
		diffuse_tex = layer.diffuse_tex;
		normal_tex = layer.normal_tex;
	}

	void ntexture_splatting::post_layer_change(nterrain_mtl_setup* trn_mtl, nrect chunk_rc)
	{
		chunk_rc.right -= 1;
		chunk_rc.bottom -= 1;

		// 地形图层产生了增减，移动等，这里简单处理，也就是重建所有图层
		clear_layers();
		s_shader_modifier.clear();
		vector2 uv_scale;
		float uv_rotate;
		/*resource_location texture_loc;
		resource_location normal_tex_loc;*/

		size_t num_setup_layer = trn_mtl->get_num_layers();
		int j = 0;
		for (size_t i=0; i<num_setup_layer; i++)
		{
			//-- 第一个图层没有alpha
			if( i == 0)
			{
				mtl_layer_data ld;
				ld = trn_mtl->get_layer_data(i);
				ld.set_index(j);

				if (ld.normal_tex)
				{
					std::ostringstream oss_marco;
					oss_marco << "N" << ld.index;
					s_shader_modifier.add_macro(shader_define(oss_marco.str(),"1"));
				}

				m_layers.push_back(ld);
				j++;
			}// end of if
			else
			{
				if( trn_mtl->calulate_alpha_relative(i, chunk_rc) )
				{
					mtl_layer_data ld;
					const layer_data& layer = trn_mtl->get_layer_data(i);
					ld = layer;
					ld.set_index(j);

					std::ostringstream oss_marco;
					oss_marco << "LAY" <<  ld.index;
					s_shader_modifier.add_macro(shader_define(oss_marco.str(),"1"));

					if (ld.normal_tex)
					{
						std::ostringstream oss_marco;
						oss_marco << "N" << ld.index;
						s_shader_modifier.add_macro(shader_define(oss_marco.str(),"1"));
					}

					m_layers.push_back(ld);

					//-- 更新Alpha贴图
					nalpha_map::ptr amap = trn_mtl->get_layer_alpha(i);
					size_t channel = j-1;
					m_alpha_tex->copy_alpha( channel, chunk_rc.left, chunk_rc.top, amap.get() );
					j++;
				}
			}// end of else

			//-- 如果已经达到最大图层数，则忽略其他
			if( j>= MAX_LAYERS )
				break;

		}// end of for()	
	}

	void ntexture_splatting::clear_layers()
	{
		for(size_t i=0; i<m_layers.size(); i++)
		{
			mtl_layer_data& ld = m_layers[i];

			mat_set_identity(ld.uv_mat);
			ld.diffuse_tex.reset();			
		}

		if( m_alpha_tex )
		{
			for (int i=0; i<MAX_ALPHA_LAYER; i++)
			{
				m_alpha_tex->set_alpha(i, 0);
			}
		}// end of if()

		m_layers.clear();
		s_shader_modifier.clear();
	}

	int ntexture_splatting::get_layer_index(const nstring& layer_name)
	{
		for(int i=0; i<m_layers.size(); i++)
		{
			const mtl_layer_data& ld = m_layers[i];
			if( ld.diffuse_tex
				&& layer_name == ld.diffuse_tex->get_location().to_string() )
				return i;
		}
		return -1;
	}
}//namespace nexus