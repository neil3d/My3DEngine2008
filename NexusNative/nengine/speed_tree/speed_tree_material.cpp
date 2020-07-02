#include "StdAfx.h"
#include "speed_tree_material.h"
#include "../resource/nresource_manager.h"
#include "IdvFilename.h"


namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nspt_material, nmtl_base)
	nDEFINE_NAMED_CLASS(nspt_branch_material, nspt_material)
	nDEFINE_NAMED_CLASS(nspt_frond_material, nspt_material)
	nDEFINE_NAMED_CLASS(nspt_leaf_material, nspt_material)

	const TCHAR* const MTL_TECH_SPT_BRANCH = _T("MtlTechSptBranch");
	const TCHAR* const MTL_TECH_SPT_FROND = _T("MtlTechSptFrond");
	const TCHAR* const MTL_TECH_SPT_LEAF = _T("MtlTechSptLeaf");

	void nspt_branch_material::create(boost::shared_ptr<CSpeedTreeRT> spt, const resource_location& spt_loc)
	{
		// query & set materials
		set_const(spt->GetBranchMaterial());
		
		// scale ambient values
		CSpeedTreeRT::SLightShaderParams	m_sLightScalars;            // values controlled in SpeedTreeCAD for adjusting overall brightness
		spt->GetLightShaderParams(m_sLightScalars);
		scale_ambient(m_sLightScalars.m_fAmbientScalar);

		//---- textures (failure doesn't affect bSuccess)
		// query the textures from RT
		CSpeedTreeRT::SMapBank sMapBank;
		spt->GetMapBank(sMapBank);

		std::string strTexturePath = IdvPath(conv_string(spt_loc.file_name));
		// try to load any references textures
		for (int nLayer = CSpeedTreeRT::TL_DIFFUSE; nLayer < CSpeedTreeRT::TL_SHADOW; ++nLayer)
		{
			// branches
			std::string strTextureFilename = IdvNoPath(sMapBank.m_pBranchMaps[nLayer]);
			if (!strTextureFilename.empty( ))
			{
				std::string strFullPath = strTexturePath + strTextureFilename;

				resource_location tex_loc(spt_loc.pkg_name,
					conv_string(strFullPath));

				try
				{
					if (nLayer == CSpeedTreeRT::TL_DIFFUSE || nLayer == CSpeedTreeRT::TL_NORMAL
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
						|| nLayer == CSpeedTreeRT::TL_DETAIL
#endif
						)
					{
						m_texBranchMaps[nLayer] =
							nresource_manager::instance()->load_texture_2d(tex_loc, EIO_Block);
					}
				}
				catch(nexception& ne)
				{
					//m_texBranchMaps[nLayer] = load_fallback_texture(nLayer);
				}
			}
		}// end of for

		if (m_texBranchMaps[CSpeedTreeRT::TL_DETAIL])
		{
			s_shader_modifier.add_macro(shader_define("SPEEDTREE_BRANCH_DETAIL_LAYER","1"));
		}

		if (m_texBranchMaps[CSpeedTreeRT::TL_NORMAL])
		{
			s_shader_modifier.add_macro(shader_define("SPEEDTREE_BRANCH_NORMAL_MAPPING","1"));
		}

		//-- 创建tech
		resource_location shader_loc(_T("engine_data:material/SPT_Branch.hlsl"));
		create_from_hlsl(MTL_TECH_SPT_BRANCH, shader_loc);
	}


	void nspt_branch_material::setup_effect(nshading_effect* effect_ptr) const
	{
		{
			effect_ptr->set_texture("SPT_BranchDiffuseMap", 
				m_texBranchMaps[CSpeedTreeRT::TL_DIFFUSE]->get_render_texture() );

			effect_ptr->set_texture("SPT_BranchNormalMap", 
				m_texBranchMaps[CSpeedTreeRT::TL_NORMAL]->get_render_texture() );		
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
			if( m_texBranchMaps[CSpeedTreeRT::TL_DETAIL] )
			{
				effect_ptr->set_texture("SPT_BranchDetailMap", 
					m_texBranchMaps[CSpeedTreeRT::TL_DETAIL]->get_render_texture() );
			}
#endif
		}
	}

	nresource_texture::ptr nspt_branch_material::load_fallback_texture(int nLayer)
	{
		// todo
		return nresource_texture::ptr(NULL);
	}

	void nspt_frond_material::create(boost::shared_ptr<CSpeedTreeRT> spt, const resource_location& spt_loc)
	{
		// query & set materials
		set_const(spt->GetFrondMaterial());

		// scale ambient values
		CSpeedTreeRT::SLightShaderParams	m_sLightScalars;            // values controlled in SpeedTreeCAD for adjusting overall brightness
		spt->GetLightShaderParams(m_sLightScalars);
		scale_ambient(m_sLightScalars.m_fAmbientScalar);

		//---- textures (failure doesn't affect bSuccess)
		// query the textures from RT
		CSpeedTreeRT::SMapBank sMapBank;
		spt->GetMapBank(sMapBank);

		std::string strTexturePath = IdvPath(conv_string(spt_loc.file_name));

		// find composite leaf/frond diffuse map filename
		std::string strTextureFilename = IdvNoPath(sMapBank.m_pCompositeMaps[CSpeedTreeRT::TL_DIFFUSE]);
		std::string strFullPath = strTexturePath + strTextureFilename;

		// load leaf composite diffuse map
		{
			resource_location tex_loc(spt_loc.pkg_name,
				conv_string(strFullPath));
			
			m_texLeavesAndFronds[CSpeedTreeRT::TL_DIFFUSE] = 
				nresource_manager::instance()->load_texture_2d(tex_loc, EIO_Block);
		}

#if defined(SPEEDTREE_FROND_NORMAL_MAPPING) || defined(SPEEDTREE_LEAF_NORMAL_MAPPING)
		// load leaf composite normal map
		{
			// find composite leaf/frond normal map filename
			strTextureFilename = IdvNoPath(sMapBank.m_pCompositeMaps[CSpeedTreeRT::TL_NORMAL]);
			strFullPath = strTexturePath + strTextureFilename;

			resource_location tex_loc(spt_loc.pkg_name,
				conv_string(strFullPath));

			m_texLeavesAndFronds[CSpeedTreeRT::TL_NORMAL] = 
				nresource_manager::instance()->load_texture_2d(tex_loc, EIO_Block);

			if (m_texLeavesAndFronds[CSpeedTreeRT::TL_NORMAL])
			{
				s_shader_modifier.add_macro(shader_define("SPEEDTREE_FROND_NORMAL_MAPPING",""));
			}
		}
#endif

		//-- 创建pass
		resource_location shader_loc(_T("engine_data:material/SPT_Frond.hlsl"));

		create_from_hlsl(MTL_TECH_SPT_FROND, shader_loc);
	}

	void nspt_frond_material::setup_effect(nshading_effect* effect_ptr) const
	{
		{

			effect_ptr->set_texture("SPT_CompositeDiffuseLeafMap", 
				m_texLeavesAndFronds[CSpeedTreeRT::TL_DIFFUSE]->get_render_texture() );

#if defined(SPEEDTREE_FROND_NORMAL_MAPPING)
			effect_ptr->set_texture("SPT_CompositeNormalLeafMap", 
				m_texLeavesAndFronds[CSpeedTreeRT::TL_NORMAL]->get_render_texture() );		
#endif
		}
	}

	void nspt_leaf_material::create(nresource_texture::ptr composite_diffuse, nresource_texture::ptr composite_normal)
	{
		m_composite_diffuse = composite_diffuse;
		m_composite_normal = composite_normal;
		if (composite_normal)
		{
			s_shader_modifier.add_macro(shader_define("SPEEDTREE_LEAF_NORMAL_MAPPING",""));
		}

		//-- 创建Pass 
		resource_location shader_loc(_T("engine_data:material/SPT_Leaf.hlsl"));		
		create_from_hlsl(MTL_TECH_SPT_LEAF, shader_loc);
	}

	void nspt_leaf_material::setup_effect(nshading_effect* effect_ptr) const
	{
		{
			effect_ptr->set_texture("SPT_CompositeDiffuseLeafMap", 
				m_composite_diffuse->get_render_texture() );

#if defined(SPEEDTREE_LEAF_NORMAL_MAPPING)
			effect_ptr->set_texture("SPT_CompositeNormalLeafMap", 
				m_composite_normal->get_render_texture() );		
#endif
		}
	}
}//namespace nexus
