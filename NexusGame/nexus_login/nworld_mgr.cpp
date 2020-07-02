#include "nworld_mgr.h"
#include "nworld.h"
#include "nlog_mt.h"

namespace nexus {

	nworld_mgr::nworld_mgr()
	{
	}

	nworld_mgr::~nworld_mgr()
	{
	}

	bool nworld_mgr::init()
	{
		return true;
	}

	void nworld_mgr::destroy()
	{
	}

	void nworld_mgr::update(uint32 elapse)
	{
		for (WORLD_MAP::const_iterator iter = m_worlds.begin(); iter != m_worlds.end(); ++iter)
		{
			iter->second->update(elapse);
		}
	}

	bool nworld_mgr::add_world(nworld* world_ptr)
	{
		if (false == m_worlds.insert(std::make_pair(world_ptr->get_world_id(), world_ptr)).second)
		{
			return false;
		}

		if (false == m_crc_to_id.insert(std::make_pair(world_ptr->get_world_name_crc(), world_ptr->get_world_id())).second)
		{
			m_worlds.erase(world_ptr->get_world_id());
			return false;
		}

		return true;
	}

	void nworld_mgr::remove_world(nworld* world_ptr)
	{
		m_worlds.erase(world_ptr->get_world_id());
		m_crc_to_id.erase(world_ptr->get_world_name_crc());
	}

	nworld* nworld_mgr::get_world(uint32 world_id) const
	{
		WORLD_MAP::const_iterator iter = m_worlds.find(world_id);

		if (iter == m_worlds.end())
		{
			return NULL;
		}

		return iter->second;
	}

	nworld* nworld_mgr::get_world_by_name_crc(uint32 name_crc) const
	{
		CRC2ID_MAP::const_iterator iter = m_crc_to_id.find(name_crc);

		if (iter == m_crc_to_id.end())
		{
			return NULL;
		}

		return get_world(iter->second);
	}

	nworld_mgr g_world_mgr;

} // namespace nexus
