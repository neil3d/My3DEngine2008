#include "nlog_mt.h"
#include "ncmd_queue.h"


namespace nexus{

ncmd_queue::ncmd_queue()
{
}

ncmd_queue::~ncmd_queue()
{
}

void ncmd_queue::enqueue(const nchar* cmd)
{
	nguard<nmutex> guard(m_mutex);

	m_cmd_list.push_back(tstring(cmd));
}

tstring ncmd_queue::dequeue(void)
{
	nguard<nmutex> guard(m_mutex);

	if( m_cmd_list.size() )
	{
		tstring cmd = m_cmd_list.front();
		m_cmd_list.pop_front();
		return cmd;
	}

	return tstring(_T("\0"));
}

bool ncmd_queue::empty(void)
{
	nguard<nmutex> guard(m_mutex);
	return (0 == m_cmd_list.size());
}

}
