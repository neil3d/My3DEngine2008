#include "ntcp_session.h"
#include "ntcp_server.h"
#include "nguard.h"
#include "nlzo_wrap.h"
#include "nlog_mt.h"

namespace nexus {

	ntcp_session::ntcp_session() : m_session_id(0), m_server_ptr(NULL), m_socket(0), m_status(TSS_NONE), m_recv_msg_ptr(NULL), m_read_rd(0), m_read_wr(0), 
		m_write_buffer_ptr(NULL), m_write_rd(0), m_write_wr(0), m_wsa_send(0), m_wsa_recv(0),
		m_send_buffers_ptr(NULL), m_send_head_ptr(NULL), m_send_tail_ptr(NULL), m_empty_head_ptr(NULL), 
		m_max_recv_size(0), m_max_write_size(0), m_send_buffers(0), m_minilzo(false)
	{
	}

	ntcp_session::~ntcp_session()
	{
		_free_memory();
		m_session_id		= 0;
		m_server_ptr		= NULL;
		m_socket			= 0;
		m_status			= TSS_NONE;
		m_recv_msg_ptr		= NULL;
		m_read_rd			= 0;
		m_read_wr			= 0;
		m_write_buffer_ptr	= NULL;
		m_write_rd			= 0;
		m_write_wr			= 0;
		m_wsa_send			= 0;
		m_wsa_recv			= 0;
		m_send_buffers_ptr	= NULL;
		m_send_head_ptr		= NULL;
		m_send_tail_ptr		= NULL;
		m_empty_head_ptr	= NULL;
		m_max_recv_size		= 0;
		m_max_write_size	= 0;
		m_send_buffers		= 0;
		m_minilzo			= false;
	}

	// 内网到外网的send调用前，置好所有的引用计数，send函数只处理引用计数的减少
	bool ntcp_session::send(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// 检查参数
		if (!msg_ptr)
		{
			return false;
		}

		// 检查参数及连接的有效性,m_session_id和m_status多线程访问
		if (session_id != m_session_id || 0 == size || !m_empty_head_ptr || TSS_OPEN != m_status)
		{
			msg_ptr->dec_reference();
			return false;
		}

		{// 括号限制m_mutex保护的范围，避免递归
			// 获得锁
			nguard<nfast_mutex> scoped_lock(m_mutex);

			if (session_id != m_session_id || TSS_OPEN != m_status || !m_empty_head_ptr)
			{
				msg_ptr->dec_reference();
				return false;
			}

			m_empty_head_ptr->buffer_ptr	= msg_ptr->buffer;
			m_empty_head_ptr->size			= size;

			// 加入到发送队列
			if (!m_send_tail_ptr)
			{
				m_send_head_ptr = m_send_tail_ptr = m_empty_head_ptr;
			}
			else
			{
				m_send_tail_ptr->next_ptr = m_empty_head_ptr;
				m_send_tail_ptr = m_empty_head_ptr;
			}

			m_empty_head_ptr = m_empty_head_ptr->next_ptr;
			// 有数据链表和空数据链表之间是断开的，结点是通用的
			m_send_tail_ptr->next_ptr = NULL;	

			// 判断是否投递send请求, 注意不要死锁m_mutex, m_wsa_send
			if (::InterlockedCompareExchange((LPLONG)&m_wsa_send, 1, 0) != 0)
			{
				return true;
			}
			// 拼包
			m_write_rd = 0;
			m_write_wr += sizeof(DWORD) + _packeted_buffer(m_write_buffer_ptr, sizeof(DWORD));
			// 第一位标志是否压缩, 0表示不使用压缩，send 函数里不压缩, 统计压缩率
			*reinterpret_cast<DWORD*>(m_write_buffer_ptr) = ((m_write_wr - sizeof(DWORD)) << 1);
		}

		WSABUF wbuf;
		DWORD bytes = 0;

		wbuf.buf = m_write_buffer_ptr;
		wbuf.len = m_write_wr;

		m_write.status		= IOCS_WRITE;
		m_write.ov.hEvent	= NULL;

		int result = ::WSASend(m_socket, &wbuf, 1, &bytes, 0, &m_write.ov, NULL);
		DWORD last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_write(last_error, bytes, NULL, NULL);
			return false;
		}

		return true;
	}

	bool ntcp_session::_init(uint16 index, ntcp_server* server_ptr, uint32 max_recv_size, uint32 max_write_size, uint32 send_buffers, bool minilzo)
	{
		if (0XFFFF == index || !server_ptr || 0 == max_recv_size || 0 == max_write_size || 0 == send_buffers)
		{
			NLOG_NERROR(_T("ntcp_session init error!, index = %u, server_ptr = %u, max_recv_size = %u, max_write_size = %u, send_buffers = %u"), 
				index, server_ptr, max_recv_size, max_write_size, send_buffers);
			return false;
		}

		*reinterpret_cast<uint16*>(&m_session_id) = index;
		m_server_ptr			= server_ptr;
		m_max_recv_size			= max_recv_size;
		m_max_write_size		= max_write_size;
		m_send_buffers			= send_buffers;
		m_minilzo				= minilzo;

		// alloc memory
		m_write_buffer_ptr = new char[m_max_write_size];

		if (!m_write_buffer_ptr)
		{
			NLOG_NERROR(_T("alloc write_buffer memory error!"));
			return false;
		}

		m_send_buffers_ptr = new nsend_buffer[m_send_buffers];

		if (!m_send_buffers_ptr)
		{
			NLOG_NERROR(_T("alloc nsend_buffer memory error!"));
			_free_memory();
			return false;
		}

		for (DWORD i = 0; i < m_send_buffers - 1; ++i)
		{
			m_send_buffers_ptr[i].next_ptr = &(m_send_buffers_ptr[i + 1]);
		}

		m_send_buffers_ptr[m_send_buffers - 1].next_ptr = NULL;
		m_empty_head_ptr = &(m_send_buffers_ptr[0]);

		m_status = TSS_INIT;

		return true;
	}

	void ntcp_session::_free_memory()
	{
		m_status = TSS_NONE;

		// 释放session new的内存数据，从内存池申请的数据在close时，已经释放给内存池
		if (m_send_buffers_ptr)
		{
			delete []m_send_buffers_ptr;
			m_send_buffers_ptr = NULL;
		}

		if (m_write_buffer_ptr)
		{
			delete []m_write_buffer_ptr;
			m_write_buffer_ptr = NULL;
		}
	}

	void ntcp_session::_pre_accept()
	{
		m_socket			= ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		m_accept.status		= IOCS_ACCEPT;
		m_accept.ov.hEvent	= NULL;
		m_accept.session_ptr= this;

		// pool数据在close时统一释放
		m_recv_msg_ptr		= NULL;
		m_read_rd			= 0;
		m_read_wr			= 0;
		m_write_rd			= 0;
		m_write_wr			= 0;

		m_wsa_send			= 0;
		m_wsa_recv			= 0;

		m_send_head_ptr		= NULL;
		m_send_tail_ptr		= NULL;

		for (DWORD i = 0; i < m_send_buffers - 1; ++i)
		{
			m_send_buffers_ptr[i].buffer_ptr = NULL;
			m_send_buffers_ptr[i].size = 0;
			m_send_buffers_ptr[i].next_ptr = &(m_send_buffers_ptr[i + 1]);
		}

		m_send_buffers_ptr[m_send_buffers - 1].buffer_ptr = NULL;
		m_send_buffers_ptr[m_send_buffers - 1].size = 0;
		m_send_buffers_ptr[m_send_buffers - 1].next_ptr = NULL;

		m_empty_head_ptr = &(m_send_buffers_ptr[0]);

		m_status = TSS_ACCEPTEX;
	}

	void ntcp_session::_open(HANDLE completion_port, nsafe_mem_pool* pool_ptr, DWORD address, DWORD port)
	{
		// Disable blocking send/recv calls
		u_long arg = 1;
		::ioctlsocket(m_socket, FIONBIO, &arg);
		// Disables the NAGLE algorithm for send coalescing
		BOOL nodelay = TRUE;
		::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay));
		// Assigns the socket to his completion port
		::CreateIoCompletionPort((HANDLE)m_socket, completion_port, (ULONG_PTR)this, 0);
		// 设置状态
		m_status = TSS_OPEN;
		// 在回调函数之间先设置标志位
		::InterlockedIncrement((LPLONG)&m_wsa_recv);
		// 回调函数
		m_server_ptr->on_connect(m_session_id, (uint32)address, (uint32)port);

		// 投递读请求
		m_recv_msg_ptr = reinterpret_cast<nmessage*>(pool_ptr->alloc(RECV_SIZE(m_max_recv_size)));

		if (!m_recv_msg_ptr)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_recv);
			_close();
			NLOG_NERROR(_T("pool alloc error!"));
			return;
		}

		m_recv_msg_ptr->pool_ptr	= pool_ptr;
		m_recv_msg_ptr->next_ptr	= NULL;
		m_recv_msg_ptr->reference	= 1;
		m_recv_msg_ptr->size		= 0;
		m_recv_msg_ptr->session_id	= m_session_id;
		m_recv_msg_ptr->server_id	= 0;
		m_read_rd					= 0;
		m_read_wr					= sizeof(WORD);

		// 投递recv length请求
		WSABUF wbuf;
		DWORD flags = 0;
		DWORD bytes = 0;

		wbuf.buf = m_recv_msg_ptr->buffer;
		wbuf.len = m_read_wr;

		m_read.status		= IOCS_READ;
		m_read.ov.hEvent	= NULL;

		// 可能回调函数调用了close，上面申请的pool资源会采用统一的close_helper释放
		if (TSS_OPEN != m_status)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_recv);
			return;
		}

		int result = ::WSARecv(m_socket, &wbuf, 1, &bytes, &flags, &m_read.ov, NULL);
		DWORD last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_read(last_error, bytes, NULL);
			return;
		}
	}

	// 数据打包
	DWORD ntcp_session::_packeted_buffer(char* temp_ptr, DWORD off_size)
	{
		DWORD _off_size = off_size;
		WORD head_size = 0;

		nsend_buffer* empty_buffer_ptr = NULL;
		nmessage* temp_msg_ptr = NULL;

		while (m_send_head_ptr)
		{
			head_size = WORD(m_send_head_ptr->size);

			if (_off_size + head_size + sizeof(WORD) > m_max_write_size)
			{
				break;
			}

			memcpy(temp_ptr + _off_size, &head_size, sizeof(WORD));
			_off_size += sizeof(WORD);
			memcpy(temp_ptr + _off_size, m_send_head_ptr->buffer_ptr, m_send_head_ptr->size);
			_off_size += m_send_head_ptr->size;

			// 修改buffer的引用计数，释放资源
			temp_msg_ptr = NC2MSG(m_send_head_ptr->buffer_ptr);
			temp_msg_ptr->dec_reference();

			empty_buffer_ptr = m_send_head_ptr;
			m_send_head_ptr = m_send_head_ptr->next_ptr;

			// 归还空buffer
			empty_buffer_ptr->buffer_ptr = NULL;
			empty_buffer_ptr->size = 0;
			empty_buffer_ptr->next_ptr = m_empty_head_ptr;
			m_empty_head_ptr = empty_buffer_ptr;
		}

		// 链表中数据可能没有完全拷贝到buffer中
		if (!m_send_head_ptr)
		{
			m_send_tail_ptr = m_send_head_ptr;
		}

		return _off_size - off_size;
	}

	void ntcp_session::_release()
	{
		// 尝试判断
		if (TSS_SHUT != m_status || 0 != m_wsa_recv && 0 != m_wsa_send)
		{
			return;
		}
		// 释放该session的内存池引用
		nmessage* msg_ptr = NULL;

		// 保证pool内存消息释放一次
		{// _release在helper的析构函数里调用，避免递归
			nguard<nfast_mutex> guard(m_mutex);

			if (TSS_SHUT != m_status || 0 != m_wsa_recv && 0 != m_wsa_send)
			{
				return;
			}

			while (m_send_head_ptr)
			{
				// 修改buffer的引用计算，释放资源, 没有整理有数据队列和没有数据队列_pre_accept会统一整理
				msg_ptr = NC2MSG(m_send_head_ptr->buffer_ptr);
				msg_ptr->dec_reference();
				m_send_head_ptr = m_send_head_ptr->next_ptr;
			}

			if (NULL != m_recv_msg_ptr)
			{
				m_recv_msg_ptr->pool_ptr->free(m_recv_msg_ptr);
			}

			// 设置session状态
			m_status = TSS_CLOSE;
			uint32 session_id = m_session_id;
			// 改变m_client_id
			++(*(reinterpret_cast<uint16*>(&m_session_id) + 1));

			// 投递回调close call back函数请求
			m_server_ptr->_post_close_call_back(session_id);
		}
	}

	void ntcp_session::_close()
	{
		{// 括号保护m_mutex访问避免递归，helper析构里可能访问m_mutex
			nguard<nfast_mutex> guard(m_mutex);

			if (TSS_OPEN != m_status)
			{
				return;
			}

			m_status = TSS_SHUT;
		}

		::shutdown(m_socket, SD_BOTH);
		::closesocket(m_socket);
		m_socket = 0;

		// 尝试释放资源
		_release();
	}

	void ntcp_session::_handle_read(DWORD last_error, DWORD bytes_transferred, nsafe_mem_pool* pool_ptr)
	{
		nclose_helper helper(this);

		if (0 != last_error || 0 == bytes_transferred)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_recv);
			_close();
			return;
		}

		m_read_rd += bytes_transferred;

		if (m_read_rd == m_read_wr)
		{
			if (0 == m_recv_msg_ptr->size)
			{
				m_recv_msg_ptr->size = static_cast<DWORD>(*reinterpret_cast<WORD*>(m_recv_msg_ptr->buffer));
				m_read_rd = 0;
				m_read_wr = m_recv_msg_ptr->size;

				// 检查错误的消息头
				if (0 == m_recv_msg_ptr->size || m_recv_msg_ptr->size > m_max_recv_size)
				{
					::InterlockedDecrement((LPLONG)&m_wsa_recv);
					_close();
					return;
				}
			}
			else
			{
				// $$$ 回调消息函数, 注意调用时机保证消息获得串行性，在wsarecv之间调用
				m_server_ptr->on_datarecv(m_session_id, m_recv_msg_ptr, m_recv_msg_ptr->size);

				// 重新获得消息
				m_recv_msg_ptr = reinterpret_cast<nmessage*>(pool_ptr->alloc(RECV_SIZE(m_max_recv_size)));

				if (!m_recv_msg_ptr)
				{
					::InterlockedDecrement((LPLONG)&m_wsa_recv);
					_close();
					NLOG_NERROR(_T("pool alloc error!"));
					return;
				}

				m_recv_msg_ptr->pool_ptr	= pool_ptr;
				m_recv_msg_ptr->next_ptr	= NULL;
				m_recv_msg_ptr->reference	= 1;
				m_recv_msg_ptr->size		= 0;
				m_recv_msg_ptr->session_id	= m_session_id;
				m_recv_msg_ptr->server_id	= 0;
				m_read_rd					= 0;
				m_read_wr					= sizeof(WORD);
			}
		}

		WSABUF wbuf;
		DWORD flags = 0;
		DWORD bytes = 0;

		// 投递recv length请求
		wbuf.buf = m_recv_msg_ptr->buffer + m_read_rd;
		wbuf.len = m_read_wr - m_read_rd;

		m_read.status		= IOCS_READ;
		m_read.ov.hEvent	= NULL;

		// 可能外部调用了close，pool资源会采用统一的close_helper释放
		if (TSS_OPEN != m_status)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_recv);
			return;
		}

		int result = ::WSARecv(m_socket, &wbuf, 1, &bytes, &flags, &m_read.ov, NULL);
		last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_read(last_error, bytes, NULL);
			return;
		}
		}

	void ntcp_session::_handle_write(DWORD last_error, DWORD bytes_transferred, char* temp_ptr, nlzo_wrap* minilzo_ptr)
	{
		nclose_helper helper(this);

		if (0 != last_error || 0 == bytes_transferred)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_send);
			_close();
			return;
		}

		m_write_rd += bytes_transferred;

		if (m_write_rd == m_write_wr)
		{
			m_write_rd = 0;
			m_write_wr = 0;
		}
		else
		{
			// 该情况不会发生(有争议), 加上该处理更加安全
			uint32 const len = m_write_wr - m_write_rd;
			::memmove(m_write_buffer_ptr, m_write_buffer_ptr + m_write_rd, len);
			m_write_rd = 0;
			m_write_wr = len;
		}

		DWORD temp_size = 0;

		{// 括号保护m_mutex访问避免递归，helper析构里可能访问m_mutex
			nguard<nfast_mutex> scoped_lock(m_mutex);
			temp_size = _packeted_buffer(temp_ptr, 0);					// 拼包
		}

		// 如果是TSS_OPEN != m_status，有close请求，pool资源会采用统一的close_helper释放
		if (0 == temp_size || TSS_OPEN != m_status)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_send);
			return;
		}

		// 判断是否进行压缩
		DWORD head = 0;

		if (!m_minilzo || temp_size <= 100 || !minilzo_ptr)
		{
		// 最低一位标志是否压缩, 1表示使用压缩, 0表示不使用压缩
			head = (temp_size << 1);

		memcpy(m_write_buffer_ptr + m_write_wr, &head, sizeof(DWORD));
		m_write_wr += sizeof(DWORD);
			memcpy(m_write_buffer_ptr + m_write_wr, temp_ptr, temp_size);
			m_write_wr += temp_size;
		}
		else
		{
			// 检查压缩数据buff的大小, !!!服务器控制发送给客户端的原始数据不要超过客户端的缓冲区MAX_RECV_SIZE
			if (!nlzo_wrap::check_out_buff(temp_size, m_max_write_size))
			{
				::InterlockedDecrement((LPLONG)&m_wsa_send);
				_close();
				return;
			}

			uint32 new_size = 0;
			// 压缩
			if (!minilzo_ptr->compress(temp_ptr, temp_size, m_write_buffer_ptr + m_write_wr + sizeof(DWORD), new_size))
			{
				::InterlockedDecrement((LPLONG)&m_wsa_send);
				_close();
				return;
			}
			// 最低一位标志是否压缩, 1表示使用压缩, 0表示不使用压缩
			head = (new_size << 1) + 1;

			memcpy(m_write_buffer_ptr + m_write_wr, &head, sizeof(DWORD));
			m_write_wr += sizeof(DWORD) + new_size;
		}

		WSABUF wbuf;
		DWORD bytes = 0;

		wbuf.buf = m_write_buffer_ptr;
		wbuf.len = m_write_wr;

		m_write.status		= IOCS_WRITE;
		m_write.ov.hEvent	= NULL;

		int result = ::WSASend(m_socket, &wbuf, 1, &bytes, 0, &m_write.ov, NULL);
		last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_write(last_error, bytes, NULL, NULL);
			return;
		}
	}

	ntcp_session::nclose_helper::~nclose_helper()
	{
		m_session_ptr->_release();
	}

} // namespace nexus
