#include "StdAfx.h"
#include "navatar_actor.h"
#include "../resource/nresource_manager.h"
#include "navatar_skeletal_mesh.h"
#include "navatar_attachment.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(navatar_actor, nactor)

	navatar_actor::navatar_actor(const nstring& name_str):nactor(name_str)
	{
	}

	navatar_actor::~navatar_actor(void)
	{
	}

	void navatar_actor::create(const resource_location& anim_set_loc)
	{
		nresource_skeletal_anim_set::ptr anim_set 
			= nresource_manager::instance()->load_skeletal_anim_set( anim_set_loc );

		m_anim_ctrl.reset( new nskel_anim_play );
		m_anim_ctrl->init(anim_set, true);
	}

	void navatar_actor::update(float delta_time, const nviewport& view)
	{
		if(m_anim_ctrl)
			m_anim_ctrl->tick_anim(delta_time);

		nactor::update(delta_time, view);
	}

	void navatar_actor::bind_skeletal_mesh(const nstring& comp_name, const resource_location& res_loc)
	{
		//-- 在现有组件中查找，如果找到了则令该组件去更新模型
		nactor_component::ptr comp = get_component(comp_name);
				
		if(comp)
		{
			boost::shared_ptr<navatar_skeletal_mesh> skm = boost::dynamic_pointer_cast<navatar_skeletal_mesh>(comp);
			//skm->replace_resource(res_loc);
		}
		else
		{
			//-- 没有找到，则新建一个
			boost::shared_ptr<navatar_skeletal_mesh> skm = create_component_t<navatar_skeletal_mesh>(comp_name);
			skm->reset_resource(res_loc, true);
			skm->bind_anim_controller(m_anim_ctrl);
		}

	}

	void navatar_actor::bind_attachment(const nstring& comp_name, const nstring& attach_bone, const resource_location& res_loc)
	{
		//-- 在现有组件中查找，如果找到了则令该组件去更新模型
		nactor_component::ptr comp = get_component(comp_name);

		if(comp)
		{
			boost::shared_ptr<navatar_attachment> attach = boost::dynamic_pointer_cast<navatar_attachment>(comp);
			//attach->replace_resource(res_loc);
		}
		else
		{
			//-- 没有找到，则新建一个
			boost::shared_ptr<navatar_attachment> attach = create_component_t<navatar_attachment>(comp_name);
			attach->reset_resource(res_loc);
			if(m_anim_ctrl)
				attach->bind_to_bone(attach_bone, m_anim_ctrl->get_skeleton());
		}

	}

	void navatar_actor::set_anim( const nstring& sequence )
	{
		if(m_anim_ctrl)
		{
			m_anim_ctrl->set_anim(sequence);
		}
		else
		{
			nLog_Debug(_T("Unknow animation controller!"));
		}
	}

	void navatar_actor::play_anim( bool loop, float rate, float start_time )
	{
		if(m_anim_ctrl)
		{
			m_anim_ctrl->play_anim(loop,rate,start_time);
		}
		else
		{
			nLog_Debug(_T("Unknow animation controller!"));
		}
	}

	void navatar_actor::stop_anim()
	{
		if(m_anim_ctrl)
		{
			m_anim_ctrl->stop_anim();
		}
		else
		{
			nLog_Debug(_T("Unknow animation controller!"));
		}
	}

	void navatar_actor::set_position( float new_time, bool fire_notifies )
	{
		if(m_anim_ctrl)
		{
			m_anim_ctrl->set_position(new_time,fire_notifies);
		}
		else
		{
			nLog_Debug(_T("Unknow animation controller!"));
		}
	}

	float navatar_actor::get_time_left() const
	{
		if(m_anim_ctrl)
		{
			return m_anim_ctrl->get_time_left();
		}
		else
		{
			nLog_Debug(_T("Unknow animation controller!"));
			return -1.0f;
		}
	}

	float navatar_actor::get_anim_length() const
	{
		if(m_anim_ctrl)
		{
			return m_anim_ctrl->get_anim_length();
		}
		else
		{
			nLog_Debug(_T("Unknow animation controller!"));
			return -1.0f;
		}
	}

	size_t navatar_actor::get_anim_frame() const
	{
		if(m_anim_ctrl)
		{
			return m_anim_ctrl->get_anim_frame();
		}
		else
		{
			nLog_Debug(_T("Unknow animation controller!"));
			return 0;
		}
	}

	const TCHAR* navatar_actor::get_anim_playing() const
	{
		if(m_anim_ctrl)
		{
			return m_anim_ctrl->get_anim_playing();
		}
		else
		{
			nLog_Debug(_T("Unknow animation controller!"));
			return NULL;
		}
	}
}//namespace nexus