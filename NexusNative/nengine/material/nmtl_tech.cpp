#include "StdAfx.h"
#include "nmtl_tech.h"
#include "../renderer/nrender_mesh.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nmtl_tech, nobject)

	nmtl_tech::nmtl_tech(const nstring& name_str):m_name(name_str)
	{		
		m_two_side = false;
		m_alpha_masked = false;
	}

	nmtl_tech::~nmtl_tech(void)
	{
	}

	void nmtl_tech::_init(nmtl_tech_shader::ptr shader)
	{
		m_shader_res = shader;
		m_render_states.clear();
	}

	void nmtl_tech::set_render_state(enum ERenderState es, int value)
	{
		//-- 如果已经设置过这个状态，则更新其值
		for (size_t i=0; i<m_render_states.size(); i++)
		{
			if( m_render_states[i].first == es )
			{
				m_render_states[i].second = value;
				return;
			}
		}// end of for()

		//-- 新加一个<状态,值>
		std::pair<enum ERenderState, int> rs(es, value);
		m_render_states.push_back(rs);
	}

	void nmtl_tech::enable_two_side()
	{
		set_render_state(ERS_CullMode, ECM_None);
		m_two_side = true;
	}

	void nmtl_tech::enable_alpha_mask(int alpha_mask)
	{
		set_render_state(ERS_AlphaTestEnable, true);
		set_render_state(ERS_AlphaRef, alpha_mask);
		set_render_state(ERS_AlphaFunc, ECMP_GreaterEqual);
		m_alpha_masked = true;
	}

	void nmtl_tech::enable_alpha_blend()
	{
		set_render_state(ERS_AlphaBlendEnable, true);
		set_render_state(ERS_SrcBlend, EAB_SrcAlpha);
		set_render_state(ERS_DestBlend, EAB_InvSrcAlpha);
	}

	void nmtl_tech::enable_additive_blend()
	{
		set_render_state(ERS_AlphaBlendEnable, true);
		set_render_state(ERS_SrcBlend, EAB_One);
		set_render_state(ERS_DestBlend, EAB_One);
	}

	void nmtl_tech::set_effect_state(nshading_effect* eft)
	{
		for (size_t i=0; i<m_render_states.size(); i++)
		{
			const std::pair<enum ERenderState, int>& item = m_render_states[i];
			eft->set_render_state(item.first, item.second);			
		}// end of for()
	}

	void nmtl_tech::set_render_state(enum ERenderState es, float value)
	{
		set_render_state( es, *((int*)&value) );
	}

	template<>
	inline narchive& nserialize(narchive& ar, std::pair<enum ERenderState, int>& val, const TCHAR* obj_name)
	{
		nstring class_name(_T("render_state_pair"));
		ar.object_begin(obj_name, class_name);
		nserialize_enum_as_int(ar, val.first, _T("render_state"));
		nserialize(ar, val.second, _T("val"));
		ar.object_end();

		return ar;
	}

	void nmtl_tech::serialize(narchive& ar)
	{
		nSERIALIZE(ar, m_name);

		resource_location shader_loc;
		if(ar.is_loading())
		{
			nSERIALIZE(ar, shader_loc);
			m_shader_res = nresource_manager::instance()->load_mtl_shader(shader_loc, EIO_Block);
		}
		else
		{
			shader_loc = m_shader_res->get_location();
			nSERIALIZE(ar, shader_loc);
		}

		nSERIALIZE(ar, m_render_states);
		nSERIALIZE(ar, m_two_side);
		nSERIALIZE(ar, m_alpha_masked);
	}
}//namespace nexus