#include "nevent_processor.h"
#include "nlog_mt.h"

namespace nexus {

	nsafe_mem_pool nevent_processor::m_pool;

	nevent_processor::nevent_processor()
	{
	}

	nevent_processor::~nevent_processor()
	{
		clear_events();
	}

	void nevent_processor::update(uint32 elapse)
	{
		nevent_list::iterator event_iter = m_events.begin();
		nevent* free_ptr = NULL;

		while (event_iter != m_events.end())
		{
			(*event_iter)->expire_time -= elapse;

			if ((*event_iter)->expire_time <= 0)
			{
				// 调用事件处理函数
				const nevent_base_args* args = reinterpret_cast<const nevent_base_args*>((*event_iter)->buffer);
				std::pair<nevent_handler_map::const_iterator, nevent_handler_map::const_iterator> range_pair = m_handers.equal_range(args->event_id);

				for (nevent_handler_map::const_iterator iter = range_pair.first; iter != range_pair.second && iter != m_handers.end(); ++iter)
				{
					iter->second(args);
				}

				free_ptr = (*event_iter);
				m_pool.free(free_ptr);
				event_iter = m_events.erase(event_iter);
			}
			else
			{
				++event_iter;
			}
		}
	}

	void nevent_processor::clear_events()
	{
		nevent_list::iterator iter = m_events.begin();
		nevent* free_ptr = NULL;

		while (iter != m_events.end())
		{
			free_ptr = (*iter);
			m_pool.free(free_ptr);
			iter = m_events.erase(iter);
		}
	}

	void nevent_processor::register_event_handler(uint16 event_id, nevent_handler handler)
	{
		m_handers.insert(std::make_pair(event_id, handler));
	}

	void nevent_processor::send_event(const void* event_ptr, uint32 size)
	{
		const nevent_base_args* args = reinterpret_cast<const nevent_base_args*>(event_ptr);
		std::pair<nevent_handler_map::const_iterator, nevent_handler_map::const_iterator> range_pair = m_handers.equal_range(args->event_id);

		for (nevent_handler_map::const_iterator iter = range_pair.first; iter != range_pair.second && iter != m_handers.end(); ++iter)
		{
			iter->second(args);
		}
	}

	void nevent_processor::post_event(uint32 expire_time, const void* data_ptr, uint32 size)
	{
		nevent* event_ptr = reinterpret_cast<nevent*>(m_pool.alloc(sizeof(nevent) + size));

		if (!event_ptr)
		{
			NLOG_ERROR(_T("alloc event error!"));
			return;
		}

		event_ptr->expire_time	= expire_time;
		event_ptr->size			= size;
		memcpy(event_ptr->buffer, data_ptr, size);

		m_events.push_back(event_ptr);
	}

	void nevent_processor::send_event_ts(const void* event_ptr, uint32 size)
	{
		nguard<nfast_mutex> guard(m_mutex);
		send_event(event_ptr, size);
	}

	void nevent_processor::post_event_ts(uint32 expire_time, const void* event_ptr, uint32 size)
	{
		nguard<nfast_mutex> guard(m_mutex);
		post_event(expire_time, event_ptr, size);
	}

} // namespace nexus
