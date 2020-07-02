#include "nmap.h"
#include "nmap_mgr.h"
#include "nunit.h"
#include "nplayer.h"
#include "ncreature.h"
#include "nobject_define.h"
#include "nlog_mt.h"

namespace nexus {


	nmap::nmap() : m_map_id(0), m_instance_id(0), m_game_level_ptr(NULL)
	{
	}

	nmap::~nmap()
	{
	}

	bool nmap::init(uint32 map_id, gameframework::nlevel_info_resource::ptr level_res_ptr)
	{
		m_map_id		= map_id;
		m_instance_id	= 0;
		m_level_res_ptr	= level_res_ptr;
		m_nav_map_ptr	= g_map_mgr.create_nav_map(m_map_id);

		if (!m_nav_map_ptr)
		{
			NLOG_ERROR(_T("create_nav_map error!"));
			return false;
		}

		m_game_level_ptr = new gameframework::ngame_level;

		if (!m_game_level_ptr)
		{
			NLOG_ERROR(_T("create ngame_level error!"));
			return false;
		}

		m_game_level_ptr->attach_nav_map(m_nav_map_ptr);
		return true;
	}

	void nmap::destroy()
	{
		if (m_game_level_ptr)
		{
			delete []m_game_level_ptr;
			m_game_level_ptr = NULL;
		}
	}

	void nmap::update(uint32 elapse)
	{
		for (REMOTE_PLAYER_MAP::const_iterator iter = m_player_map.begin(); iter != m_player_map.end(); ++iter)
		{
			iter->second->update(elapse);
		}

		for (CREATURE_MAP::const_iterator iter = m_creature_map.begin(); iter != m_creature_map.end(); ++iter)
		{
			iter->second->update(elapse);
		}
	}

	bool nmap::add_player(nplayer* player_ptr)
	{
		if (!player_ptr)
		{
			return false;
		}

		// 加入到player map
		if (!m_player_map.insert(std::make_pair(player_ptr->get_id(), player_ptr)).second)
		{
			return false;
		}

		// 设置玩家的地图指针
		player_ptr->set_map(this);
		return true;
	}

	void nmap::remove_player(nplayer* player_ptr)
	{
		// 从player map删除
		m_player_map.erase(player_ptr->get_id());
		player_ptr->set_map(NULL);
	}

	bool nmap::add_creature(ncreature* creature_ptr)
	{
		return true;
	}

	void nmap::remove_creature(ncreature* creature_ptr)
	{

	}

	nunit* nmap::get_unit(uint64 guid) const
	{
		switch (GET_TYPE(guid))
		{
		case EOT_PLAYER:
			{
				return (nunit*)get_player(guid);
			}
			break;
		case EOT_CREATURE:
			{
				return (nunit*)get_creature(guid);
			}
			break;
		default:
			{
				return NULL;
			}
			break;
		}

		return NULL;
	}

	nplayer* nmap::get_player(uint64 guid) const
	{
		REMOTE_PLAYER_MAP::const_iterator iter = m_player_map.find(guid);

		if (iter == m_player_map.end())
		{
			return NULL;
		}

		return iter->second;
	}

	ncreature* nmap::get_creature(uint64 guid) const
	{
		CREATURE_MAP::const_iterator iter = m_creature_map.find(PAIR64_LOPART(guid));

		if (iter == m_creature_map.end())
		{
			return NULL;
		}

		return iter->second;
	}

} // namespace nexus
