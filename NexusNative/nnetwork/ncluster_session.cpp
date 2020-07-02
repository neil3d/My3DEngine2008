#include "ncluster_session.h"
#include "ncluster_server.h"
#include "ncluster_client.h"
#include "nguard.h"
#include "nlog_mt.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace nexus {

	const int MSG_HEADER_SIZE	= sizeof(DWORD);		// 消息头的大小
	const int MAX_POOL_SIZE		= 12 * 1024 * 1024;		// 内存池的默认大小

	ncluster_session::ncluster_session() : m_session_id(0), m_server_ptr(NULL), m_client_ptr(NULL), m_socket(0), m_status(TSS_NONE),  
		m_send_index(0), m_max_send_buffer(0), m_send_event(NULL), m_terminate(0), m_pool(MAX_POOL_SIZE)
	{
		ZeroMemory(&m_address, sizeof(m_address));
		m_write_wr[0]	= 0;
		m_write_wr[1]	= 0;
		m_buffer[0]		= NULL;
		m_buffer[1]		= NULL;
	}

	ncluster_session::~ncluster_session()
	{
		_free_memory();
		m_session_id		= 0;
		m_server_ptr		= NULL;
		m_client_ptr		= NULL;
		m_socket			= 0;
		m_status			= TSS_NONE;
		m_send_index			= 0;
		m_max_send_buffer		= 0;
		m_write_wr[0]			= 0;
		m_write_wr[1]			= 0;
		m_buffer[0]				= NULL;
		m_buffer[1]				= NULL;
	}

	bool ncluster_session::send(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// 检查参数
		if (!msg_ptr)
		{
			return false;
		}

		bool ret = send_msg(session_id, &(msg_ptr->session_id), size);
			msg_ptr->dec_reference();
		return ret;
		}

	bool ncluster_session::send_msg(uint32 session_id, const void* msg_ptr, uint32 size)
		{
		if (session_id != m_session_id || !msg_ptr || 0 == size || TSS_OPEN != m_status)
			{
				return false;
			}

		// 判断数据是否益处
		uint32 index = 0;
		// 获得缓冲区
		nguard<nfast_mutex> scoped_lock(m_mutex);

		if (TSS_OPEN != m_status)
			{
			return false;
		}

		index = (m_send_index + 1) % 2;

		if (m_write_wr[index] + size >= m_max_send_buffer)
		{
			return false;
		}

		// 拷贝数据，1. 消息长度
		*reinterpret_cast<WORD*>(m_buffer[index] + MSG_HEADER_SIZE + m_write_wr[index]) = WORD(size);
		m_write_wr[index] += sizeof(WORD);
		// 拷贝数据，2. 消息内容
		memcpy(m_buffer[index] + MSG_HEADER_SIZE + m_write_wr[index], msg_ptr, size);
		m_write_wr[index] += size;

		// 激活send事件
		::SetEvent(m_send_event);
		return true;
	}

	bool ncluster_session::_init(uint16 index, ncluster_server* server_ptr, ncluster_client* client_ptr, uint32 max_send_buffer)
	{
		if (0XFFFF == index || 0 == max_send_buffer)
		{
			NLOG_NERROR(_T("ncluster_session init param error, index = %u, max_send_buffers = %u"), index, max_send_buffer);
			return false;
		}

		*reinterpret_cast<uint16*>(&m_session_id) = index;
		m_server_ptr			= server_ptr;
		m_client_ptr			= client_ptr;
		m_max_send_buffer		= max_send_buffer;

		// alloc memory
		m_buffer[0] = new char[m_max_send_buffer + MSG_HEADER_SIZE + 1];

		if (!m_buffer[0])
		{
			NLOG_NERROR(_T("alloc send buffer memory error!"));
			return false;
		}

		m_buffer[1] = new char[m_max_send_buffer + MSG_HEADER_SIZE + 1];

		if (!m_buffer[1])
		{
			NLOG_NERROR(_T("alloc send buffer memory error!"));
			_free_memory();
			return false;
		}

		m_status = TSS_INIT;

		return true;
	}

	void ncluster_session::_free_memory()
	{
		m_status = TSS_NONE;

		// 释放session new的内存数据
		if (m_buffer[0])
		{
			delete []m_buffer[0];
			m_buffer[0] = NULL;
		}

		if (m_buffer[1])
		{
			delete []m_buffer[1];
			m_buffer[1] = NULL;
		}

		// 关闭handle
		if (m_send_event)
		{
			::CloseHandle(m_send_event);
			m_send_event = NULL;
		}
	}

	void ncluster_session::_pre_accept()
	{
		m_write_wr[0]		= 0;
		m_write_wr[1]		= 0;
		m_send_index		= 0;
		m_status = TSS_ACCEPTEX;
	}

	void ncluster_session::_open()
	{
		// 设置状态
		m_status = TSS_OPEN;
		// 回调函数
		if (m_server_ptr)
		{
			m_server_ptr->on_connect(m_session_id, 0, 0);
		}
		else if (m_client_ptr)
		{
			m_client_ptr->on_connect(m_session_id, 0, 0);
		}
		// 自动激活
		m_send_event = ::CreateEvent(NULL, FALSE, TRUE, NULL);
		// 设置线程启动标志
		::InterlockedExchange((LONG*)&m_terminate, 0);
		// 启动读写线程
		m_threads.create_thread(boost::bind(&ncluster_session::_thread_recv, this));
		m_threads.create_thread(boost::bind(&ncluster_session::_thread_send, this));
	}

	void ncluster_session::_release()
	{
		// 尝试判断
		if (TSS_SHUT != m_status)
		{
			return;
		}
		// 注意线程安全
		{
			nguard<nfast_mutex> scoped_lock(m_mutex);

			if (TSS_SHUT != m_status)
			{
				return;
			}

		// 关闭handle
		if (m_send_event)
		{
			::CloseHandle(m_send_event);
			m_send_event = NULL;
		}
		// 设置session状态
		m_status = TSS_CLOSE;
		// 改变m_client_id
		++(*(reinterpret_cast<uint16*>(&m_session_id) + 1));
		}
		// 将session归还个连接管理池
		if (m_server_ptr)
		{
			m_server_ptr->_return_session(this);
		}
		else if (m_client_ptr)
		{
			m_client_ptr->_return_session(this);
		}
	}

	void ncluster_session::_closing()
	{
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSS_OPEN != m_status)
			{
				return;
			}

			m_status = TSS_SHUT;
		}

		if (m_server_ptr)
		{
			m_server_ptr->_closing_session(this);
		}
		else if (m_client_ptr)
		{
			m_client_ptr->_closing_session(this);
		}
	}

	void ncluster_session::_close()
	{
		if (TSS_SHUT != m_status)
		{
				return;
			}

		::shutdown(m_socket, SD_SEND);

		int retry = 0;
		int result = 0;
		fd_set fdread;
		timeval block_time;							// 设置阻塞时间
		block_time.tv_sec	= 0;
		block_time.tv_usec	= 150 * 1000;			// 150 ms 

		while (retry++ < 10)
		{
			FD_ZERO(&fdread);
			FD_SET(m_socket, &fdread);

			if (1 == select(0, &fdread, NULL, NULL, &block_time))
			{
				CHAR temp;
				result = ::recv(m_socket, &temp, 1, 0);

				// receive FIN, ok
				if (0 == result)
				{
					break;
				}

				// network down
				if (SOCKET_ERROR == result)
				{
					break;
				}
			}
		}

		::closesocket(m_socket);
		m_socket = 0;

		// 回调关闭函数
		if (m_server_ptr)
		{
			m_server_ptr->on_disconnect(m_session_id);
		}
		else if (m_client_ptr)
		{
			m_client_ptr->on_disconnect(m_session_id);
		}

		// 关闭读写线程
		::InterlockedExchange((LONG*)&m_terminate, 1);

		// 等待工作线程的退出
		m_threads.join_all();

		// 释放内存池资源，归还连接资源
		_release();
	}

	void ncluster_session::_thread_recv()
	{
		fd_set fdread;
		timeval block_time;					// 设置阻塞时间
		block_time.tv_sec	= 0;
		block_time.tv_usec	= 200 * 1000;	// 200 ms

		int result		= 0;
		DWORD msg_size	= 0;
		DWORD temp_size	= 0;
		DWORD ptr		= 0;
		char* temp_ptr	= NULL;
		nmessage* nmsg_ptr = NULL;

		char* buffer = new char[m_max_send_buffer + 1];

		if (!buffer)
		{
			return;
		}

		// 循环获得数据
		while (0 == m_terminate)
		{
			// 1. 接收DWORD的消息长度
			temp_size = sizeof(DWORD);
			temp_ptr = (char*)&msg_size;

			while (0 < temp_size)
			{
				FD_ZERO(&fdread);
				FD_SET(m_socket, &fdread);

				if (1 == select(0, &fdread, NULL, NULL, &block_time))
				{
					result = ::recv(m_socket, temp_ptr, temp_size, 0);

					if (0 == result || SOCKET_ERROR == result)
					{
						_closing();
						return;
					}

					temp_size -= result;
					temp_ptr += result;
				}

				if (1 == m_terminate)
				{
					_closing();
					return;
				}
			}

			if (SOCKET_ERROR != result)
			{
				temp_size = msg_size;
				temp_ptr = (char*)&(buffer[0]);

				while (0 < temp_size)
				{
					FD_ZERO(&fdread);
					FD_SET(m_socket, &fdread);

					if (1 == select(0, &fdread, NULL, NULL, &block_time))
					{
						result = ::recv(m_socket, temp_ptr, temp_size, 0);

						if (0 == result || SOCKET_ERROR == result)
						{
							_closing();
							return;
						}

						temp_size -= result;
						temp_ptr += result;
					}

					if (1 == m_terminate)
					{
						_closing();
						return;
					}
				}

				// 解析消息，回调函数
				temp_size = msg_size;
				temp_ptr = (char*)&(buffer[0]);

				// 解析为最小消息单位，回调函数
				ptr = 0;

				while (ptr < msg_size)
				{
					temp_size = DWORD(*(WORD*)(temp_ptr + ptr));
					ptr += sizeof(WORD);

					if (ptr + temp_size <= msg_size)
					{
						// 构造nmessage
						nmsg_ptr = reinterpret_cast<nmessage*>(m_pool.alloc(sizeof(nmessage) + temp_size));

						if (NULL == nmsg_ptr)
						{
							_closing();
							return;
						}

						nmsg_ptr->pool_ptr		= &m_pool;
						nmsg_ptr->next_ptr		= NULL;
						nmsg_ptr->reference		= 1;
						nmsg_ptr->size			= temp_size;
						nmsg_ptr->session_id	= 0;
						nmsg_ptr->server_id		= 0;

						// 拷贝数据
						memcpy(&(nmsg_ptr->session_id), temp_ptr + ptr, temp_size);

						if (m_server_ptr)
						{
							m_server_ptr->on_datarecv(m_session_id, nmsg_ptr, temp_size);
						}
						else if (m_client_ptr)
						{
							m_client_ptr->on_datarecv(m_session_id, nmsg_ptr, temp_size);
						}
					}
					else
					{
						break;
					}

					ptr += temp_size;
				}
			}
		}

		delete []buffer;
		buffer = NULL;
	}

	void ncluster_session::_thread_send()
	{
		fd_set fdwrite;
		timeval block_time;					// 设置发送阻塞时间
		block_time.tv_sec	= 0;
		block_time.tv_usec	= 200 * 1000;	// 200 ms

		int result		= 0;
		char*	msg_ptr = NULL;
		DWORD	msg_size = 0;

		while (0 == m_terminate)
		{
			// 注意内部的break
			for(;;)
			{
				// 获得要发送的数据缓冲区
				{
					nguard<nfast_mutex> scoped_lock(m_mutex);
					msg_ptr		= m_buffer[m_send_index];
					msg_size	= m_write_wr[m_send_index];

					if (0 == msg_size)
				{
						m_send_index = (m_send_index + 1) % 2;
						msg_ptr		= m_buffer[m_send_index];
						msg_size	= m_write_wr[m_send_index];
					}

					m_write_wr[m_send_index] = 0;
				}

				if (0 == msg_size)
				{
					break;
				}

				// 赋值打包消息长度
				*((DWORD*)m_buffer[m_send_index]) = msg_size;
				msg_size += sizeof(uint32);

				// send 可能不能一次将数据发完
				while (0 < msg_size && 0 == m_terminate)
				{
					result = ::send(m_socket, msg_ptr, msg_size, 0);

					if (SOCKET_ERROR == result)
					{
						DWORD last_error = ::WSAGetLastError();

						switch (last_error)
						{
						case WSAEWOULDBLOCK:
						case WSAENOBUFS:
							{
								// no buffer space is available within the transport 
								// system to hold the data to be transmitted
								// 等待数据可以发送的时机
								FD_ZERO(&fdwrite);
								FD_SET(m_socket, &fdwrite);
								select(0, NULL, &fdwrite, NULL, &block_time);
							}
							break;
						default:
							{
								// 故障
								_closing();
								return;
							}
							break;
						}
					}
					else
					{
						// 数据发送成功，计算缓冲区偏移
						msg_ptr += result;
						msg_size -= result;
					}
				}

				{
					nguard<nfast_mutex> scoped_lock(m_mutex);
					m_send_index = (m_send_index + 1) % 2;
			}
			}

			::WaitForSingleObject(m_send_event, 120);
		}
	}

} // namespace nexus
