#include "StdAfx.h"
#include "nlevel.h"
#include "nengine.h"
#include "world/nlevel_info_resource.h"
#include "../sky/sky_base.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nlevel, nobject)

	nlevel::nlevel(const nstring& name_str): m_name(name_str),
		m_active_layer(ACTOR_DEFAULT_LAYER)
	{
		m_actor_iter = m_all_actor.end();
		m_nav_map = navigation_map::ptr(new navigation_map);
	}

	nlevel::~nlevel(void)
	{
		m_nav_map.reset();
	}

	void nlevel::init()
	{
		//-- create scene manager
		nscene_manager* psm = (nscene_manager*)nclass_factory::instance()->alloc_object(_T("nscene_manager"));
		if(psm == NULL)
			nthrow(_T("create scene manager failed."));
		m_scene_ptr.reset(psm);

		m_layer_set.insert(ACTOR_DEFAULT_LAYER);
		m_active_layer=ACTOR_DEFAULT_LAYER;
		nlevel_layer_desc desc;
		desc.is_locked=false;
		desc.is_visible=true;
		m_layer_desc[ACTOR_DEFAULT_LAYER]=desc;
	}

	nactor::ptr nlevel::create_actor(const nstring& actor_name, const nstring& class_name)
	{
		nstring layer_name;
		bool is_visible = true;
		nlayer_map::iterator layer_iter = (m_active_layer.length() > 0) ? m_layer_desc.find(m_active_layer) : m_layer_desc.end();
		if(layer_iter != m_layer_desc.end())
		{
			if(layer_iter->second.is_locked)
			{
				return nactor::ptr();
			}

			layer_name = m_active_layer;
			is_visible = layer_iter->second.is_visible;
		}
		nactor::ptr actor_ptr( nconstruct<nactor>(class_name, actor_name) );
		nASSERT( actor_ptr );		

		actor_ptr->_init(this);
		if(layer_name.length() > 0)
		{
			actor_ptr->set_layer(layer_name);
			actor_ptr->set_visible(is_visible);
		}
		m_all_actor.insert(make_pair(actor_ptr->get_name().name_crc, actor_ptr));
		return actor_ptr;
	}

	bool nlevel::destroy_actor(const nname& actor_name)
	{
		nactor_map::iterator iter = m_all_actor.find(actor_name.name_crc);			
		if( iter != m_all_actor.end() )
		{
			nactor::ptr actor_ptr = iter->second;
			//如果actor所在图层已经被锁定，则无法删除
			nlayer_map::iterator i=m_layer_desc.find(actor_ptr->get_layer_name());
			if(i!=m_layer_desc.end())
			{
				if(i->second.is_locked) return false;
			}
			actor_ptr->_remove_from_level();
			actor_ptr->_destroy();

			m_all_actor.erase(iter);
			return true;
		}
		return false;
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
		// start physics simulate
		simulate_level_phys( delta_time );

		for(nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			nactor::ptr& pa = iter->second;
			pa->update(delta_time, view);
		}

		// get physics simulate result
		get_level_phys_result();
	}

	void nlevel::render(const nviewport& view)
	{
		nrenderer_base* rnd = nengine::instance()->get_renderer();

		rnd->render_scene(view,m_scene_ptr,m_render_setting);
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
		release_level_phys();
		destroy_all_actors();
	}

	void nlevel::serialize_layer_name(narchive& ar, const nstring& layer_name,nlevel_serialize_callback* callback)
	{
		nstring layer_actors( _T("layer_actors_") );
		layer_actors += layer_name;

		m_active_layer=ACTOR_DEFAULT_LAYER;

		if( ar.is_loading() )
		{
			nlevel_layer_desc desc;
			desc.is_locked=!(layer_name==m_active_layer);
			desc.is_visible=true;
			m_layer_desc[layer_name]=desc;

			size_t s = 0;
			ar.array_begin(layer_actors.c_str(), s);

			for(size_t i=0; i<s; i++)
			{
				nactor::ptr new_obj;
				nserialize(ar, new_obj, ELEM_ArrayItem);
				ar.array_next();
				//将新建的actor移动到当前层
				new_obj->set_layer(layer_name);
				//如果当前层被锁定，则锁定actor
				if(is_layer_locked(layer_name))
				{
					new_obj->set_frozen(true);
				}
				m_all_actor.insert(make_pair(new_obj->get_name().name_crc, new_obj));
				if( callback )
					callback->actor_loaded(new_obj);
			}
			ar.array_end();
		}// end of if()
		else
		{
			//-- 计算需要存盘的对象数量
			size_t s = 0;
			for (nactor_map::iterator iter = m_all_actor.begin();
				iter != m_all_actor.end();
				++iter)
			{
				nactor::ptr obj = iter->second;
				if( obj->get_serializable()
					&& obj->get_layer_name()==layer_name)
					s++;
			}// end of for()

			//-- 存盘当前layer的所有actors
			ar.array_begin(layer_actors.c_str(), s);
			size_t i=0;
			for (nactor_map::iterator iter = m_all_actor.begin();
				iter != m_all_actor.end();
				++iter)
			{
				nactor::ptr obj = iter->second;
				if( obj->get_serializable()
					&& obj->get_layer_name()==layer_name)
				{
					nserialize(ar, obj, ELEM_ArrayItem);				
					ar.array_next();

					if( callback )
						callback->actor_saved(obj);
				}
				i++;
			}
			ar.array_end();
		}// end of else
	}

	void nlevel::serialize(narchive& ar)
	{
		nevent_target::serialize(ar);

		nSERIALIZE(ar, m_layer_set);
		nSERIALIZE(ar, m_name);
		
		nstring class_name=L"render_setting";
		ar.object_begin(L"m_render_setting",class_name);
		m_render_setting.serialize(ar);
		ar.object_end();
		
		if(ar.is_loading())
		{
			m_active_layer=ACTOR_DEFAULT_LAYER;
		}
	}

	void nlevel::serialize_loaded()
	{
		for (nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			nactor::ptr obj = iter->second;
			obj->_level_loaded(this);
		}
	}

	bool nlevel::line_check(ncheck_result& ret, const vector3& start, const vector3& end, ELineCheckType check_type)
	{
		//todo : 使用scene mamager加速

		bool hit = false;

		for (nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			nactor::ptr obj = iter->second;
			
			ncheck_result actor_hit;
			if(obj->line_check(actor_hit, start, end, check_type))
			{
				hit = true;
				if(actor_hit.dist < ret.dist)
					ret = actor_hit;
			}
		}

		return hit;
	}

	bool nlevel::rename_actor(const nstring& old_name, const nstring& new_name)
	{
		nname nm(old_name);
		nactor_map::iterator iter = m_all_actor.find(nm.name_crc);
		if(iter != m_all_actor.end())
		{
			nactor::ptr find_actor = iter->second;
			nlayer_map::iterator i=m_layer_desc.find(find_actor->get_layer_name());
			if(i!=m_layer_desc.end() && i->second.is_locked)
			{
				nLog_Warning(_T("Level layer locked, please unlock this layer first..."));
				return false;
			}

			m_all_actor.erase(iter);
			find_actor->_set_name(new_name);

			m_all_actor.insert(make_pair(find_actor->get_name().name_crc, find_actor));
			return true;
		}
		return false;
	}

	void nlevel::attach_actor(nactor::ptr actor)
	{
		actor->_init(this);
		m_all_actor.insert(make_pair(actor->get_name().name_crc, actor));
	}

	bool nlevel::deattach_actor(nactor::ptr actor)
	{
		nactor_map::iterator iter = m_all_actor.find(actor->get_name().name_crc);			
		if( iter != m_all_actor.end() )
		{
			nactor::ptr actor_ptr = iter->second;
			actor_ptr->_remove_from_level();
			m_all_actor.erase(iter);
			return true;
		}
		return false;
	}

	void nlevel::init_level_phys( const vector3& gravity )
	{
		if( m_phys_scene.get() != NULL )
		{
			// 已经创建
			nASSERT( false && _T("Recreate level phys scene...") );
			return;
		}

		nphysics_engine* phys_engine =  nengine::instance()->get_phys_engine();
		if( phys_engine )
		{
			m_phys_scene.reset( nconstruct<nphys_scene>( _T("nphys_scene") ) );
			nASSERT( m_phys_scene.get() );
			m_phys_scene->_init( phys_engine );
			m_phys_scene->set_gravity( gravity );
			
			// create actor physical
			for ( nactor_map::const_iterator actor_it = m_all_actor.begin();
				actor_it!=m_all_actor.end(); actor_it++ )
			{
				nactor* actor = actor_it->second.get();
				if( actor )
				{
					actor->init_phys();
				}
			}
		}
	}

	void nlevel::simulate_level_phys( float delta_time )
	{
		// physics scene 还没有创建
		if ( m_phys_scene.get() == NULL || !m_phys_scene->is_valid_scene() )
		{
			return;
		}

		// simulate level physics scene
		m_phys_scene->_simulate_phys_scene( delta_time );

		// for actor simulate
		for ( nactor_map::const_iterator actor_it = m_all_actor.begin();
			actor_it!=m_all_actor.end(); actor_it++ )
		{
			nactor* actor = actor_it->second.get();
			if( actor )
			{
				actor->simulate_phys(delta_time);
			}
		}
	}

	void nlevel::get_level_phys_result()
	{
		if( m_phys_scene.get() )
		{
			m_phys_scene->_get_phys_scene_result();
		}
	}

	void nlevel::release_level_phys()
	{
		// only for level phys destroy
	}

	void nlevel::do_serialize(const resource_location& folder, enum EFileMode fmode, nlevel_serialize_callback* callback)
	{		
		nfile_system* fs = nengine::instance()->get_file_sys();

		//-- 存储level对象全局数据
		narchive::ptr ap = narchive::open_xml_archive(fs,
			fmode, folder.pkg_name, folder.file_name+_T("/level_golbal.xml"));
		nstring dummy(_T("level_global_class"));
		ap->object_begin(_T("level_global"), dummy);
		this->serialize(*ap);
		ap->object_end();
		ap->close();

		// 存取导航图信息
		serialize_navigation_map(fs,folder,fmode);

		//-- 存储每个layer
		if( callback )
			callback->begin_actors();
		for(st_string_set::iterator iter = m_layer_set.begin();
			iter != m_layer_set.end();
			++iter)
		{
			nstring layer_name = *iter;
			resource_location file_loc = folder;
			file_loc.file_name += _T("/level_layer_");
			file_loc.file_name += layer_name;
			file_loc.file_name += _T(".xml");

			narchive::ptr layer_ap = narchive::open_xml_archive(fs,
				fmode, file_loc.pkg_name, file_loc.file_name);

			dummy = _T("level_layer_class");
			layer_ap->object_begin(layer_name.c_str(), dummy);
			this->serialize_layer_name(*layer_ap, layer_name, callback);
			layer_ap->object_end();
			layer_ap->close();
		}// end of for()

		// 顺便导出一下level资源
		if(fmode == EFileWrite)
		{
			export_level(folder.pkg_name, folder.file_name);
		}

		if( callback )
			callback->end_actors();

		if( fmode == EFileRead )
			serialize_loaded();
	}

	void nlevel::serialize_navigation_map( nfile_system* fs, const resource_location& level_folder, enum EFileMode mode )
	{
		nstring nav_file_name = level_folder.file_name + _T("/navigation_map_desc.xml");
		if((mode == EFileRead && !fs->file_exists(level_folder.pkg_name,nav_file_name)))
		{
			return;
		}
		m_nav_map->serialize(fs, level_folder.pkg_name, nav_file_name, mode);
	}

	void nlevel::_on_device_lost(int param)
	{
		//-- 处理所有actor
		for (nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			nactor::ptr obj = iter->second;
			obj->_on_device_lost(param);
		}

		//-- 处理所有scene capture
		for(nscene_capture_list::iterator iter = m_all_captures.begin();
			iter != m_all_captures.end();
			++iter)
		{
			(*iter)->_on_device_lost(param);
		}
	}

	bool nlevel::_on_device_reset(int param)
	{
		//-- 处理所有actor
		for (nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			nactor::ptr obj = iter->second;
			bool ret = obj->_on_device_reset(param);
			if( !ret )
				return false;
		}

		//-- 处理所有scene capture
		for(nscene_capture_list::iterator iter = m_all_captures.begin();
			iter != m_all_captures.end();
			++iter)
		{
			bool ret = (*iter)->_on_device_reset(param);
			if( !ret )
				return false;
		}

		return true;
	}

	//===============================================================================

	void nlevel::new_layer( const nstring& layer_name )
	{
		m_layer_set.insert(layer_name);
		nlevel_layer_desc desc;
		desc.is_locked=true;
		desc.is_visible=true;
		m_layer_desc[layer_name]=desc;
	}

	void nlevel::remove_layer(const nstring& layer_name)
	{
		//-- 查找这个图层的所有actor
		std::vector<nname> actor_remove_array;

		for ( nactor_map::const_iterator actor_it = m_all_actor.begin();
			actor_it!=m_all_actor.end(); actor_it++ )
		{
			nactor* actor = actor_it->second.get();
			if( actor->get_layer_name() == layer_name )
			{
				actor_remove_array.push_back(actor->get_name());
			}
		}

		//-- 移除这个图层的所有actor
		for (size_t i=0; i<actor_remove_array.size(); i++)
		{
			destroy_actor(actor_remove_array[i]);
		}

		//-- 从图层名称集合中移除
		m_layer_set.erase(layer_name);
		m_layer_desc.erase(layer_name);
		if (layer_name==m_active_layer)
		{
			m_active_layer=_T("");
		}
	}

	void nlevel::set_active_layer( const nstring& layer_name )
	{
		if(m_layer_set.find(layer_name)==m_layer_set.end())
		{
			new_layer(layer_name);
		}
		m_active_layer=layer_name;
	}

	nstring nlevel::get_active_layer()
	{
		return m_active_layer;
	}

	void nlevel::lock_layer( const nstring& layer_name,bool lock )
	{
		nlayer_map::iterator i=m_layer_desc.find(layer_name);
		if(i!=m_layer_desc.end())
		{
			i->second.is_locked=lock;
			//锁定所有关联actor
			for ( nactor_map::const_iterator actor_it = m_all_actor.begin();
				actor_it!=m_all_actor.end(); actor_it++ )
			{
				nactor* actor = actor_it->second.get();
				if( actor->get_layer_name() == layer_name )
				{
					actor->set_frozen(lock);
				}
			}
		}
	}

	void nlevel::rename_layer( const nstring& old_layer_name,const nstring& new_layer_name )
	{
		m_layer_set.insert(new_layer_name);
		m_layer_set.erase(old_layer_name);
		nlevel_layer_desc desc;
		nlayer_map::iterator i=m_layer_desc.find(old_layer_name);
		if(i!=m_layer_desc.end())
		{
			desc=i->second;
		}
		else
		{
			desc.is_locked=true;
			desc.is_visible=true;
		}
		
		m_layer_desc[new_layer_name]=desc;
		m_layer_desc.erase(old_layer_name);
		//更改所有关联的actor
		for ( nactor_map::const_iterator actor_it = m_all_actor.begin();
			actor_it!=m_all_actor.end(); actor_it++ )
		{
			nactor* actor = actor_it->second.get();
			if( actor->get_layer_name() == old_layer_name )
			{
				actor->set_layer(new_layer_name);
			}
		}
		//如果被修改的层是当前激活层，则需要修改激活层的信息
		if(old_layer_name==m_active_layer)
		{
			m_active_layer=new_layer_name;
		}
	}

	void nlevel::show_layer( const nstring& layer_name,bool show )
	{
		nlayer_map::iterator i=m_layer_desc.find(layer_name);
		if(i!=m_layer_desc.end())
		{
			i->second.is_visible=show;
			//设置所有关联actor的可见性
			for ( nactor_map::const_iterator actor_it = m_all_actor.begin();
				actor_it!=m_all_actor.end(); actor_it++ )
			{
				nactor* actor = actor_it->second.get();
				if( actor->get_layer_name() == layer_name )
				{
					actor->set_visible(show);
				}
			}
		}
	}

	bool nlevel::is_layer_visible( const nstring& layer_name )
	{
		nlayer_map::iterator i=m_layer_desc.find(layer_name);
		if(i!=m_layer_desc.end())
		{
			return i->second.is_visible;
		}
		return false;
	}

	bool nlevel::is_layer_locked( const nstring& layer_name )
	{
		nlayer_map::iterator i=m_layer_desc.find(layer_name);
		if(i!=m_layer_desc.end())
		{
			return i->second.is_locked;
		}
		return true;
	}

	bool nlevel::is_layer_exist( const nstring& layer_name )
	{
		nlayer_map::iterator i=m_layer_desc.find(layer_name);
		if(i!=m_layer_desc.end())
		{
			return true;
		}
		return false;
	}

	unsigned int nlevel::get_layer_count()
	{
		return m_layer_set.size();
	}

	nstring nlevel::get_layer_name(unsigned int index)
	{
		if(index>=m_layer_set.size()) return L"";
		st_string_set::iterator i=m_layer_set.begin();
		advance(i,index);
		return (*i);
	}

	void nlevel::move_actor_to_active_layer( const nstring& actor_name )
	{
		nname nm(actor_name);
		nactor_map::iterator iter = m_all_actor.find(nm.name_crc);
		if(iter != m_all_actor.end())
		{
			nactor::ptr find_actor = iter->second;
			find_actor->set_layer(m_active_layer);
		}
	}

	bool nlevel::line_of_sight( ncheck_result& ret, const vector3& start, const vector3& end, bool walkable_check )
	{
		return m_nav_map->line_of_sight(start, end, walkable_check, ret);
	}

	int nlevel::export_level( const nstring& pkg_name, const nstring& file_path )
	{
		nfile_system* fs = nengine::instance()->get_file_sys();
		int ret_count = 0;
		// 存取地图对象资源
		gameframework::nlevel_info_resource export_res;

		// 遍历所有的Actor
		for (nactor_map::iterator iter = m_all_actor.begin();
			iter != m_all_actor.end();
			++iter)
		{
			if(export_actor(&export_res, iter->second))
			{
				++ret_count;
			}
		}

		// 最后再存盘到指定的文件
		// _T("/level_info_resource.xml")
		export_res.serialize(fs, pkg_name, file_path + _T("/level_info_resource.xml"), EFileWrite, gameframework::ELIRT_ALL);
	
		return ret_count;
	}

	int nlevel::export_level( const resource_location& res_loc )
	{
		return export_level(res_loc.pkg_name, res_loc.file_name);
	}

	bool nlevel::export_actor( gameframework::nlevel_info_resource* res, nactor::ptr actor )
	{
		return actor->export_info_resource(res);
	}

	nactor::ptr nlevel::clone_actor( nactor::ptr actor )
	{
		nactor::ptr actor_ptr=actor->clone();
		if(actor_ptr==NULL) return actor_ptr;
		actor_ptr->_init(this);
		actor_ptr->_level_loaded(this);
		m_all_actor.insert(make_pair(actor_ptr->get_name().name_crc, actor_ptr));
		return actor_ptr;
	}
}//namespace nexus