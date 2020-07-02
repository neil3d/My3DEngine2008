#include "StdAfx.h"
#include "nlevel.h"
#include "../scene/nplain_scene.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nlevel, nobject)

	nlevel::nlevel(const nstring& name_str): m_name(name_str),
		m_scene_class_name(_T("nplain_scene"))
	{
	}

	nlevel::~nlevel(void)
	{
	}

	void nlevel::init(const nstring& scene_class_name)
	{
		m_scene_class_name = scene_class_name;
		//-- create scene manager
		nscene_manager* psm = (nscene_manager*)nclass_factory::instance()->alloc_object(m_scene_class_name);
		if(psm == NULL)
			nthrow(_T("create scene manager failed."));
		m_scene_ptr.reset(psm);
	}

	nactor::ptr nlevel::create_actor(const nstring& actor_name, const nstring& class_name)
	{
		nactor::ptr actor_ptr( nconstruct<nactor>(class_name, actor_name) );
		nASSERT( actor_ptr );		

		actor_ptr->_init(this);

		m_all_actor.insert(make_pair(actor_ptr->get_name(), actor_ptr));
		return actor_ptr;
	}

	void nlevel::free_actor(const nname& actor_name)
	{
		nactor_map::iterator iter = m_all_actor.find(actor_name);			
		if( iter != m_all_actor.end() )
		{
			nactor::ptr actor_ptr = iter->second;
			actor_ptr->_remove_from_level();

			m_all_actor.erase(iter);
		}
	}

	void nlevel::update(float delta_time, const view_info& view)
	{
		for(nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			nactor::ptr& pa = iter->second;
			pa->update_component(delta_time, view);
		}
	}

	void nlevel::render(const view_info& view)
	{
		m_scene_ptr->render_visible(view);
	}

	void nlevel::on_event(nevent_base& evt)
	{
		if( evt.m_event_id == NSCENE_EVENT_ID )
		{
			m_scene_ptr->on_event(evt);
		}
	}

	void nlevel::clear_actor()
	{	
		m_all_actor.clear();
		m_scene_ptr->clear_all();
	}
}//namespace nexus