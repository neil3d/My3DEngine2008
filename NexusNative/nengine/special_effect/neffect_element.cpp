#include "StdAfx.h"
#include "neffect_element.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	void neffect_element_instance::init(shared_ptr<neffect_element> def)
	{
		m_mtl = def->get_material();
	}

	nmtl_base* neffect_element_instance::get_material(int mtl_id) const
	{
		(void)mtl_id;

		if( m_mtl )
			return m_mtl.get();
		else
			return nresource_manager::instance()->get_default_material_two_side();
	}

	nDEFINE_VIRTUAL_CLASS(neffect_element, nobject)

	neffect_element::neffect_element(void):m_name(_T("default_effect_name")),
		m_start_time(0),
		m_init_pos( 0,0,0 ),
		m_init_revolution( 0,0,0 ),
		m_init_rot( 0,0,0 ),
		m_vel( new ndistribution_const<vector3>() ),		
		m_revolution_vel( new ndistribution_const<vector3>() ),
		m_rotate_vel( new ndistribution_const<vector3>() ),
		m_visible(true),
		m_gravity(0)
	{			
	}

	neffect_element::~neffect_element(void)
	{
	}

	void neffect_element::init( const nstring& name )
	{
		m_name = name;
		//创建默认材质
		m_mtl.reset( new nmtl_static(_T("SFX_Default")) );
		m_mtl->create_from_hlsl(_T("SFX_Default"), resource_location(
			_T("engine_data"),
			_T("material/SFX_Default.hlsl"))
			);
		
		//因为材质系统在alpha混合方面还存在一些bug，所以在修复之前，暂时使用直接修改fx文件的方法打开alpha混合。
		/*m_mtl->set_trans_type(ETrans_UnlitTrans);
		tech->enable_alpha_blend();
		tech->enable_additive_blend();*/
		

		if(m_mtl.get()!=NULL && m_Texture.is_valid())
		{
			m_mtl->set_texture2d_parameter("MTL_DiffuseMap",m_Texture);
		}
	}

	void neffect_element::init_instance_base(neffect_element_instance::ptr inst)
	{
		inst->m_pos = m_init_pos;
		inst->m_revolution = m_init_revolution;
		inst->m_rot=m_init_rot;
	}

	bool neffect_element::update_instance_base(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time)
	{
		(void)view;
		//-- 是否启动
		if( !inst->m_active )
		{
			if(play_time > m_start_time)
				inst->m_active = true;
		}
		if( !inst->m_active )
			return false;

		float my_time = play_time - m_start_time;
		
		vector3 vel = m_vel->get_value(my_time);
		float sg=m_gravity*my_time;
		vel.y+=sg;
		vector3 rot_vel = m_revolution_vel->get_value(my_time);
		vector3 srot_vel = m_rotate_vel->get_value(my_time);

		inst->m_pos += vel*delta_time;
		inst->m_revolution += rot_vel*delta_time;
		inst->m_rot+=srot_vel*delta_time;

		return true;
	}

	void neffect_element::serialize(narchive& ar)
	{
		nobject::serialize( ar );

		nSERIALIZE(ar, m_name);
		nSERIALIZE(ar, m_start_time);
		nSERIALIZE(ar, m_init_pos);
		nSERIALIZE(ar, m_init_revolution);
		nSERIALIZE(ar, m_init_rot);	
		nSERIALIZE(ar, m_gravity);	
		nSERIALIZE(ar, m_Texture);

		nSERIALIZE_DISTRIBUTION(ar, m_vel);
		nSERIALIZE_DISTRIBUTION(ar, m_revolution_vel);
		nSERIALIZE_DISTRIBUTION(ar, m_rotate_vel);
		if(ar.is_loading() && m_mtl.get()!=NULL && m_Texture.is_valid())
		{
			m_mtl->set_texture2d_parameter("MTL_DiffuseMap",m_Texture);
		}
	}
}//namespace nexus