#include "StdAfx.h"
#include "nlevel.h"
#include "nengine.h"
#include "../scene/nplain_scene.h"

namespace nexus
{
	nlevel_serialize_callback* nlevel::s_serialize_callback = NULL;

	nDEFINE_NAMED_CLASS(nlevel, nobject)

	nlevel::nlevel(const nstring& name_str): m_name(name_str),
		m_scene_class_name(_T("nplain_scene"))
	{
		m_actor_iter = m_all_actor.end();
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

		m_all_actor.insert(make_pair(actor_ptr->get_name().name_crc, actor_ptr));
		return actor_ptr;
	}

	void nlevel::destroy_actor(const nname& actor_name)
	{
		nactor_map::iterator iter = m_all_actor.find(actor_name.name_crc);			
		if( iter != m_all_actor.end() )
		{
			nactor::ptr actor_ptr = iter->second;
			actor_ptr->_remove_from_level();
			actor_ptr->_destroy();

			m_all_actor.erase(iter);
		}
	}

	nactor::ptr nlevel::find_actor(unsigned int name_crc)
	{
		nactor::ptr ret;

		nactor_map::iterator iter = m_all_actor.find(name_crc);			
		if( iter != m_all_actor.end() )
			ret = iter->second;

		return ret;
	}

	nactor::ptr nlevel::find_actor(const nstring& name_str)
	{
		nname find_nm(name_str);		
		return find_actor(find_nm.name_crc);
	}

	void nlevel::update(float delta_time, const nviewport& view)
	{
		for(nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			nactor::ptr& pa = iter->second;
			pa->update(delta_time, view);
		}
	}

	void nlevel::render(const nviewport& view, nhit_proxy_hash* hph)
	{
		nrenderer_base* rnd = nengine::instance()->get_renderer();

		rnd->frame_begin_level(&view.camera);
		m_scene_ptr->render_visible(view);
		rnd->frame_end_level(hph);
	}

	void nlevel::on_event(nevent_base& evt)
	{
		if( evt.m_event_id == NSCENE_EVENT_ID )
		{
			m_scene_ptr->on_event(evt);
		}
	}

	void nlevel::destroy_all_actors()
	{	
		for(nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			nactor::ptr obj = iter->second;
			obj->_destroy();
		}
		m_all_actor.clear();

		m_scene_ptr->clear_all();
	}

	void nlevel::render_scene_captures(const nviewport& view)
	{
		for(nscene_capture_list::iterator iter = m_all_captures.begin();
			iter != m_all_captures.end();
			++iter)
		{
			if( (*iter)->update_required(view) )
				(*iter)->capture_scene(this);
		}
	}

	void nlevel::add_scene_capture(nscene_capture::ptr cap)
	{
		m_all_captures.push_back( cap );
	}

	void nlevel::remove_scene_capture(nscene_capture::ptr cap)
	{
		m_all_captures.remove( cap );
	}

	void nlevel::_destroy()
	{
		destroy_all_actors();
	}

	void nlevel::serialize(narchive& ar)
	{
		nevent_target::serialize(ar);

		nSERIALIZE(ar, m_name);
		nSERIALIZE(ar, m_scene_class_name);
		
		//-- actor map
		const TCHAR* actor_map_name = _T("all_actors");
		if( ar.is_loading() )
		{
			size_t s = 0;
			ar.array_begin(actor_map_name, s);
			if( s_serialize_callback )
				s_serialize_callback->begin_actors(s);

			for(size_t i=0; i<s; i++)
			{
				nactor::ptr new_obj;
				nserialize(ar, new_obj, ELEM_ArrayItem);
				ar.array_next();
				m_all_actor.insert(make_pair(new_obj->get_name().name_crc, new_obj));
				if( s_serialize_callback )
					s_serialize_callback->actor_loaded(i, new_obj);
			}
			ar.array_end();

			if( s_serialize_callback )
				s_serialize_callback->end_actors();
		}
		else
		{
			//-- 计算需要存盘的对象数量
			size_t s = 0;
			for (nactor_map::iterator iter = m_all_actor.begin();
				iter != m_all_actor.end();
				++iter)
			{
				nactor::ptr obj = iter->second;
				if( obj->get_serializable() )
					s++;
			}

			//--
			if( s_serialize_callback )
				s_serialize_callback->begin_actors(s);

			ar.array_begin(actor_map_name, s);
			size_t i=0;
			for (nactor_map::iterator iter = m_all_actor.begin();
				iter != m_all_actor.end();
				++iter)
			{
				nactor::ptr obj = iter->second;
				if( obj->get_serializable() )
				{
					nserialize(ar, obj, ELEM_ArrayItem);				
					ar.array_next();

					if( s_serialize_callback )
						s_serialize_callback->actor_saved(i, obj);
				}
				i++;
			}
			ar.array_end();
			if( s_serialize_callback )
				s_serialize_callback->end_actors();
		}// end of else

		if( ar.is_loading() )
			serialize_loaded();

	}

	void nlevel::serialize_loaded()
	{
		this->init(m_scene_class_name);
		for (nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			nactor::ptr obj = iter->second;
			obj->_level_loaded(this);
		}
	}

	bool nlevel::line_check(ncheck_result& ret, const vector3& start, const vector3& end)
	{
		//todo : 使用scene mamager加速

		bool hit = false;

		for (nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			nactor::ptr obj = iter->second;
			
			ncheck_result actor_hit;
			if(obj->line_check(actor_hit, start, end))
			{
				hit = true;
				if(actor_hit.dist < ret.dist)
					ret = actor_hit;
			}
		}

		return hit;
	}

	void nlevel::rename_actor(const nstring& old_name, const nstring& new_name)
	{
		nname nm(old_name);
		nactor_map::iterator iter = m_all_actor.find(nm.name_crc);
		if(iter != m_all_actor.end())
		{
			nactor::ptr find_actor = iter->second;
			m_all_actor.erase(iter);
			find_actor->_set_name(new_name);

			m_all_actor.insert(make_pair(find_actor->get_name().name_crc, find_actor));
		}
	}
}//namespace nexus