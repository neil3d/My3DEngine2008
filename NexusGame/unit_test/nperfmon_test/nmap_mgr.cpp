#include "nmap_mgr.h"
#include "nmap.h"
#include "nobject_define.h"
#include "nplayer.h"
#include "nlog_mt.h"
#include "ncfg.h"
#include "../../nexus_shared/ngame/nmap_define.h"

#include "file/nstd_file_system.h"
#include "object/narchive_xml.h"
#include "object/serialize.h"
#include "game_map/navigation_map.h"

namespace nexus {

	template<>
	inline narchive& nserialize(narchive& ar, nmap_desc& desc, const TCHAR* obj_name)
	{
		nstring class_name(_T("nmap_desc"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, desc.map_id, _T("map_id"));
		nserialize(ar, desc.instance_num, _T("instance_num"));
		ar.object_end();
		return ar;
	}

	template<> struct IS_POD<nmap_desc>
	{ 
		static const bool value = true; 
	};

	nmap_mgr::nmap_mgr()
	{
	}

	nmap_mgr::~nmap_mgr()
	{
	}

	bool nmap_mgr::init()
	{
		// 1. 读取该游戏世界都加载哪些地图
		// eg. map_id, instance_num(主世界地图 = 1, 新手村为 >= 1, 副本或战场 = 0(游戏中创建，服务器启动时不创建实例));
		// 2. 加载各个地图map_id的游戏逻辑信息, eg. tagMapInfo 地图的静态属性(刷怪信息, 导航图文件, )
		// 3. 根据总的地图信息，实例化地图实例

		tstring path = sncfg.get_tstring(ECI_ResPath);

		shared_ptr<nstd_file> file_ptr(new nstd_file);
		file_ptr->open(path + _T("map/map_config.xml"), EFileRead);

		narchive::ptr xml_read_ptr = narchive::create_xml_reader();

		if (!xml_read_ptr)
		{
			return false;
		}

		xml_read_ptr->enable_blob(false);
		xml_read_ptr->open(file_ptr);

		std::vector<nmap_desc> map_desc;
		nSERIALIZE(*xml_read_ptr, map_desc);

		nstd_file_system fs;
		fs.init(path);

		for (std::vector<nmap_desc>::const_iterator iter = map_desc.begin(); iter != map_desc.end(); ++iter)
		{
			if (!_load_map(&fs, (*iter).map_id, (*iter).instance_num))
			{
				NLOG_ERROR(_T("load map error, map_id = %u"), (*iter).map_id);
				return false;
			}
		}

		xml_read_ptr->close();
		return true;
	}

	void nmap_mgr::destroy()
	{

	}

	void nmap_mgr::update(uint32 elapse)
	{
		for (MAP_MAP_TYPE::const_iterator iter = m_maps.begin(); iter != m_maps.end(); ++iter)
		{
			iter->second->update(elapse);
		}
	}

	navigation_map::ptr nmap_mgr::create_nav_map(uint32 map_id)
	{
		return _get_nav_map(map_id);


		// 查找已经读取的nav map

		// 如果是主世界场景的话直接返回指针
		// 如果是非主世界的话直接克隆对象返回指针
	}

	void nmap_mgr::destroy_nav_map(navigation_map* nav_map_ptr)
	{
		// 如果是非主世界的话删除克隆对象
	}

	nmap* nmap_mgr::get_map(uint32 map_id, uint32 instance_id) const
	{
		MAP_MAP_TYPE::const_iterator iter = m_maps.find(MAKE_PAIR64(map_id, instance_id));

		if (iter == m_maps.end())
		{
			return NULL;
		}

		return iter->second;
	}

	bool nmap_mgr::_load_map(nstd_file_system* fs_ptr, uint32 map_id, uint32 instance_num)
	{
		nchar name[MAX_PATH];
		// load nav map
		_sntprintf_s(name, MAX_PATH, MAX_PATH, _T("%d\\navigation_map_desc.xml"), map_id);

		navigation_map::ptr nav_map_ptr(new navigation_map);

		if (!nav_map_ptr)
		{
			NLOG_ERROR(_T("create_nav_map error!"));
			return false;
		}

		const nstring file_name(name); 
		nav_map_ptr->serialize(fs_ptr, _T("map"), file_name, EFileRead);

		//int x = nav_map_ptr->get_raw_width();
		//int z = nav_map_ptr->get_raw_height();


		//for (int i = 0; i < x; ++i)
		//{
		//	for (int j = 0; j < z; ++j)
		//	{
		//		nav_map_ptr->set_raw_height(i, j, 0);
		//		nav_map_ptr->set_tile_walkable(i, j, true);
		//	}
		//}

		//nav_map_ptr->serialize(fs_ptr, _T("map"), _T("2\\navigation_map_desc.xml"), EFileWrite);

		// 加入到导航图管理器
		m_nav_maps.insert(std::make_pair(map_id, nav_map_ptr));

		gameframework::nlevel_info_resource::ptr level_res_ptr(new gameframework::nlevel_info_resource);

		if (!level_res_ptr)
		{
			NLOG_ERROR(_T("create_nlevel_info_resource error!"));
			return false;
		}

		_sntprintf_s(name, MAX_PATH, MAX_PATH, _T("%d\\level_res.xml"), map_id);

		const nstring file_name2(name); 
		level_res_ptr->serialize(fs_ptr, _T("map"), file_name2, EFileRead, gameframework::ELIRT_ALL);

		// TODO: D. Zhao, load... 


		// 创建地图实例
		nmap* map_ptr = NULL;

		for (uint32 i = 0; i < instance_num; ++i)
		{
			map_ptr = new nmap;

			if (!map_ptr)
			{
				NLOG_ERROR(_T("new nav_map error!"));
				return false;
			}

			if (false == map_ptr->init(map_id, level_res_ptr))
			{
				NLOG_ERROR(_T("map init error!"));
				return false;
			}

			if (false == _add_map(map_ptr))
			{
				NLOG_ERROR(_T("add map error!"));
				return false;
			}
		}

		return true;
	}

	bool nmap_mgr::_add_map(nmap* map_ptr)
	{
		return m_maps.insert(std::make_pair(MAKE_PAIR64(map_ptr->get_map_id(), map_ptr->get_instance_id()), map_ptr)).second;
	}

	void nmap_mgr::_remove_map(nmap* map_ptr)
	{
		m_maps.erase(MAKE_PAIR64(map_ptr->get_map_id(), map_ptr->get_instance_id()));
	}

	navigation_map::ptr nmap_mgr::_get_nav_map(uint32 map_id)
	{
		NAV_MAP_MAP_TYPE::const_iterator iter = m_nav_maps.find(map_id);

		if (iter == m_nav_maps.end())
		{
			return navigation_map::ptr();
		}

		return iter->second;
	}

	nmap_mgr g_map_mgr;

} // namespace nexus
