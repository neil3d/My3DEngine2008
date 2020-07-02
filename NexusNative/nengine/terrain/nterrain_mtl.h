/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/
#ifndef _NEXUS_TERRAIN_MTL_H_
#define _NEXUS_TERRAIN_MTL_H_
#include "../material/nmtl_base.h"

namespace nexus
{
	class nterrain_mtl_setup;

	/** 地形材质的基类 */
	class nAPI nterrain_mtl
		: public nmtl_base
	{
	public:
		typedef shared_ptr<nterrain_mtl> ptr;

		explicit nterrain_mtl(const nstring& name_str);
		virtual ~nterrain_mtl(void);
		
	protected:		
		nDECLARE_VIRTUAL_CLASS(nterrain_mtl)
	};

	/**
	 *	只使用一层贴图的简单地形材质
	*/
	class nterrain_mtl_basic
		: public nterrain_mtl
	{
	public:
		typedef shared_ptr<nterrain_mtl_basic> ptr;

		explicit nterrain_mtl_basic(const nstring& name_str);
		virtual ~nterrain_mtl_basic(void);

		void create(const resource_location& texture_loc);
		
		virtual void setup_effect(nshading_effect* effect_ptr) const;
	private:		
		nresource_texture::ptr	m_diffuse_map;

		nDECLARE_NAMED_CLASS(nterrain_mtl_basic)
	};

	/**
	 *	使用4张贴图的texture splatting算法的地形材质
	*/
	class nAPI ntexture_splatting
		: public nterrain_mtl
	{
		enum const_val
		{
			MAX_ALPHA_LAYER = 4,
			MAX_LAYERS,
		};
	public:
		typedef shared_ptr<ntexture_splatting> ptr;

		explicit ntexture_splatting(const nstring& name_str);
		virtual ~ntexture_splatting(void);

		void _init(size_t w, size_t h);
		void post_alphamap_change(nterrain_mtl_setup* trn_mtl, const nstring& layer_name, const nrect& chunk_rc);
		void post_layer_change(nterrain_mtl_setup* trn_mtl, nrect chunk_rc);

		void set_layer_texture(size_t layer_index, const resource_location& texture_loc,const resource_location& normal_tex_loc = resource_location());
		void set_layer_uv_param(size_t layer_index, const vector2& scale, float rotate);
		virtual void setup_effect(nshading_effect* effect_ptr) const;
	private:
		int get_layer_index(const nstring& layer_name);
		void clear_layers();
	private:
		struct mtl_layer_data
		{
			matrix44				uv_mat;
			float						spec,spec_power;
			nresource_texture::ptr	diffuse_tex;
			nresource_texture::ptr	normal_tex;
			std::string diffuse_name;
			std::string normal_name;
			int index;

			mtl_layer_data():index(0)
			{
				mat_set_identity(uv_mat);
			}

			void set_index(int i)
			{
				index = i;
				std::ostringstream oss;
				oss << "MTL_LayerDiffuse" << i;
				diffuse_name = oss.str();
				std::ostringstream oss_normal;
				oss_normal << "MTL_LayerNormal" << i;
				normal_name = oss_normal.str();
			}

			void operator = (const struct layer_data& layer);
		
			void update_uv_mat(vector2 uv_scale, float uv_rotate)
			{
				mat_set_rotation_Z(uv_mat, uv_rotate);
				mat_scale(uv_mat, uv_scale.x, uv_scale.y, 1.0f);
			}
		};

		vector<mtl_layer_data>					m_layers;
		render_res_ptr<nrender_alphamap>	m_alpha_tex;
		
		nDECLARE_NAMED_CLASS(ntexture_splatting)
	};
}//namespace nexus

#endif //_NEXUS_TERRAIN_MTL_H_