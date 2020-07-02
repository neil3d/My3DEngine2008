/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/
#ifndef _NEXUS_TERRAIN_MTL_H_
#define _NEXUS_TERRAIN_MTL_H_
#include "../material/nmaterial.h"

namespace nexus
{
	/** 地形材质的基类 */
	class nAPI nterrain_mtl
		: public nmaterial_base
	{
	public:
		typedef shared_ptr<nterrain_mtl> ptr;

		explicit nterrain_mtl(const nstring& name_str);
		virtual ~nterrain_mtl(void);

		virtual enum ETransparentType get_trans_type() const
		{
			return ETrans_Opaque;
		}
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
		explicit nterrain_mtl_basic(const nstring& name_str);
		virtual ~nterrain_mtl_basic(void);

		void create(const resource_location& texture_loc);
		
		virtual void draw_effect_param(nshading_effect* effect_ptr) const;

		virtual void serialize(narchive& ar);
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

		void set_layer_texture(size_t layer_index, const resource_location& texture_loc);
		void set_layer_uv_param(size_t layer_index, const vector2& scale, float rotate);
		void get_layer_param(size_t layer_index, resource_location& out_tex, vector2& out_scale, float& out_rotate);

		void import_layer_alpha(size_t layer_index, const nstring& img_file_name);
		void generate_noise(size_t layer_index, nrect rc, int numOctaves, float amplitude, float frequency);

		void _init(size_t w, size_t h);
		virtual void draw_effect_param(nshading_effect* effect_ptr) const;

		void post_alphamap_change(size_t layer_index, const nrect& region);

		nalpha_map* get_alpha_map(size_t layer_index)
		{
			return m_layers[layer_index].alpha_map.get();
		}

		virtual void serialize(narchive& ar);
	private:
		struct layer_data
		{
			vector2			uv_scale;
			float			uv_rotate;
			matrix44		uv_mat;

			nalpha_map::ptr			alpha_map;
			nresource_texture::ptr	diffuse_tex;
			
			layer_data():uv_scale(10,10),uv_rotate(0)
			{
				update_uv_mat();
			}

			void update_uv_mat()
			{
				mat_set_rotation_Z(uv_mat, uv_rotate);
				mat_scale(uv_mat, uv_scale.x, uv_scale.y, 1.0f);
			}
		};

		nsize	m_alpha_size;

		layer_data							m_layers[MAX_LAYERS];
		render_res_ptr<nrender_alphamap>	m_alpha_tex;

		nDECLARE_NAMED_CLASS(ntexture_splatting)
	};
}//namespace nexus

#endif //_NEXUS_TERRAIN_MTL_H_