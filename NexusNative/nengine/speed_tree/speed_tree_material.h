#ifndef _NEXUS_SPEED_TREE_MATERIAL_H_
#define _NEXUS_SPEED_TREE_MATERIAL_H_
#include "spt_cfg.h"
#include "../material/nmaterial.h"
#include "../resource/nresource_texture.h"

namespace nexus
{
	class nspt_material
		: public nmaterial_base
	{
	public:
		explicit nspt_material(const nstring& name_str):nmaterial_base(name_str),
			m_afAmbient(0,0,0,1),m_afDiffuse(0,0,0,1),m_afSpecular(0,0,0,1),m_afEmission(0,0,0,1),
			m_fShininess(5)
		{}
		virtual ~nspt_material(void)	{}

	protected:
		void set_const(const float afMaterial[13])
		{
			nASSERT(afMaterial);

			// assumes CSpeedTreeRT-format data packing
			memcpy(&m_afDiffuse, afMaterial, 3 * sizeof(float));
			memcpy(&m_afAmbient, afMaterial + 3, 3 * sizeof(float));

			memcpy(&m_afSpecular, afMaterial + 6, 3 * sizeof(float));
			memcpy(&m_afEmission, afMaterial + 9, 3 * sizeof(float));
			m_fShininess = afMaterial[12];
		}

		void scale_ambient(float fScalar)
		{
			m_afAmbient.R *= fScalar;
			m_afAmbient.G *= fScalar;
			m_afAmbient.B *= fScalar;
		}

		color4f           m_afAmbient;         // ambient color (r, g, b, a) [each range from 0.0 to 1.0]
		color4f           m_afDiffuse;         // diffuse color (r, g, b, a) [each range from 0.0 to 1.0]
		color4f           m_afSpecular;        // specular color (r, g, b, a) [each range from 0.0 to 1.0]
		color4f           m_afEmission;        // emission color (r, g, b, a) [each range from 0.0 to 1.0]
		float			  m_fShininess;           // shininess [ranges from 0.0 to 128.0]

		nDECLARE_VIRTUAL_CLASS(nspt_material)
	};

	class nspt_branch_material
		: public nspt_material
	{
	public:
		explicit nspt_branch_material(const nstring& name_str):nspt_material(name_str)
		{}
		virtual ~nspt_branch_material(void)	{}

		void create(boost::shared_ptr<CSpeedTreeRT> spt, const resource_location& spt_loc);
		virtual void draw_effect_param(nshading_effect* effect_ptr) const;

	protected:
		nresource_texture::ptr load_fallback_texture(int nLayer);
		// textures
		nresource_texture::ptr	m_texBranchMaps[CSpeedTreeRT::TL_NUM_TEX_LAYERS]; // texture for each layer

		nDECLARE_NAMED_CLASS(nspt_branch_material)
	};

	class nspt_frond_material
		: public nspt_material
	{
	public:
		explicit nspt_frond_material(const nstring& name_str):nspt_material(name_str)
		{}
		virtual ~nspt_frond_material(void)	{}

		void create(boost::shared_ptr<CSpeedTreeRT> spt, const resource_location& spt_loc);
		virtual void draw_effect_param(nshading_effect* effect_ptr) const;

		virtual enum ETransparentType get_trans_type() const	
		{	
			return ETrans_AlphaMasked; 
		}

		nresource_texture::ptr get_texture(int layer) const
		{
			return m_texLeavesAndFronds[layer];
		}

	protected:
		nresource_texture::ptr	m_texLeavesAndFronds[CSpeedTreeRT::TL_NUM_TEX_LAYERS];  // leaf/frond composite texture for all supported layers (CAD exported)
		nresource_texture::ptr	m_texBillboards[CSpeedTreeRT::TL_NUM_TEX_LAYERS];       // billboard composite texture for all supported layers (CAD exported)
		nresource_texture::ptr	m_texSelfShadow;                // shared composite self-shadow, cast onto branch and frond geometry (CAD exported)

		nDECLARE_NAMED_CLASS(nspt_frond_material)
	};

	class nspt_leaf_material
		: public nspt_material
	{
	public:
		explicit nspt_leaf_material(const nstring& name_str):nspt_material(name_str)
		{}
		virtual ~nspt_leaf_material(void)	{}

		void create(nresource_texture::ptr composite_diffuse, nresource_texture::ptr composite_normal);
		virtual void draw_effect_param(nshading_effect* effect_ptr) const;

		virtual enum ETransparentType get_trans_type() const	
		{	
			return ETrans_AlphaMasked; 
		}

	protected:
		nresource_texture::ptr	m_composite_diffuse, 
								m_composite_normal;

		nDECLARE_NAMED_CLASS(nspt_leaf_material)
	};
}//namespace nexus

#endif //_NEXUS_SPEED_TREE_MATERIAL_H_