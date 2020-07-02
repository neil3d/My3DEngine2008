#include "nwan_server.h"
#include "nwan_session.h"
#include "nmsg_dispatch.h"
#include "nlog_mt.h"

#include <vector>

namespace nexus {

	nwan_server::nwan_server() : m_session_ptrs(NULL), m_max_session(0)
	{
	}

	nwan_server::~nwan_server()
	{
		destroy();
	}

	bool nwan_server::init(uint32 max_session)
	{
		if (0 == max_session)
		{
			NLOG_ERROR(_T("invaild max session num, max_session = %u"), max_session);
			return false;
		}

		m_max_session = max_session;

		m_session_ptrs = new nwan_session[m_max_session];

		if (!m_session_ptrs)
		{
			NLOG_ERROR(_T("allocate sesssion memory error!"));
			return false;
		}

		return true;
	}

	void nwan_server::destroy()
	{
		if (m_session_ptrs)
		{
			delete []m_session_ptrs;
			m_session_ptrs = NULL;
		}
	}

	void nwan_server::update(uint32 elapse)
	{

	}

	void nwan_server::on_connect(uint32 session_id, uint32 address, uint32 port)
	{
		// 注册连接
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return;
		}

		// 判断是否正在使用, 同一session的on_connect和on_disconnect底层保证串行
		if (m_session_ptrs[index].is_in_used())
		{
			NLOG_ERROR(_T("this session id is in use!, index = %u, session_id = %u"), index, session_id);
			return;
		}

		m_session_ptrs[index].init(session_id, address, port);
	}

	void nwan_server::on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// 消息转发, 所有调用函数尽量操作不费时
		g_msg_dispatch.do_wan2lan_dispatch(session_id, msg_ptr, size);
	}

	void nwan_server::on_disconnect(uint32 session_id)
	{
		// 注销连接
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return;
		}

		g_msg_dispatch.handle_disconnect(session_id, NULL, 0);
		m_session_ptrs[index].destroy();
	}

	nwan_session* nwan_server::get_session(uint32 session_id)
	{
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return NULL;
		}

		// 校验id, 不输出log，正常运行时会出现
		if (session_id != m_session_ptrs[index].get_sesssion_id())
		{
			return NULL;
		}

		return &(m_session_ptrs[index]);
	}

	// 注意多线程安全
	void nwan_server::broadcast(nmessage* msg_ptr, uint32 size)
	{
		std::vector<uint32> session_ids;

		for (uint32 i = 0; i <m_max_session; ++i)
		{
			if (m_session_ptrs[i].is_in_used())
			{
				session_ids.push_back(m_session_ptrs[i].get_sesssion_id());
			}
		}

		msg_ptr->reference = static_cast<LONG>(session_ids.size());

		for (uint32 i = 0; i < session_ids.size(); ++i)
		{
			if (false == send(session_ids[i], msg_ptr, size))
			{
				// 正常运行时会出现
				NLOG_DEBUG(_T("send client error, may be buffer overflow or connect close!"));
				// 踢掉缓冲区溢出的客户端
				close(session_ids[i]);
			}
		}

		session_ids.clear();
	}

	nwan_server g_wan_server;

} // namespace nexus