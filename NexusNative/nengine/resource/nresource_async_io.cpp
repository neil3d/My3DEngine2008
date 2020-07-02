#include "StdAfx.h"
#include <boost/bind.hpp>
#include "nresource_async_io.h"
#include "nresource.h"
#include "../framework/nengine.h"
#include "nresource_manager.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#endif

namespace nexus
{
	nDEFINE_CLASS(nresource_async_io, nresource_io)

	//-- class io_request_queue --------------------------------------------------------------------------------------
	void io_request_queue::wait_for_request()
	{
		boost::mutex::scoped_lock lock( m_mutex );
		m_con.wait( lock );
	}

	void io_request_queue::push_request(const io_request& req)
	{
		boost::mutex::scoped_lock lock( m_mutex );		
		
		m_queue.push(req);

		if(m_queue.size() == 1)//第一个元素，原来队列是空的
			m_con.notify_one();
	}

	void io_request_queue::pop_request(io_request& out_req)
	{
		boost::mutex::scoped_lock lock( m_mutex );
		out_req = m_queue.top();
		m_queue.pop();
	}

	bool io_request_queue::empty() const
	{
		boost::mutex::scoped_lock lock( m_mutex );
		return m_queue.empty();
	}

	void io_request_queue::cancel_wait()
	{
		if( empty() )
		{
			m_con.notify_all();
		}
	}

	//-- class nresource_async_io --------------------------------------------------------------------------------------
	nresource_async_io::nresource_async_io(void):m_shutdown(0)
	{
	}

	nresource_async_io::~nresource_async_io(void)
	{
	}

	void nresource_async_io::start()
	{
		m_threads.create_thread(
			boost::bind(&nresource_async_io::io_thread_func, this)
			);
	}

	void nresource_async_io::shutdown()
	{
		::InterlockedExchange(&m_shutdown, 1);		
		m_requests.cancel_wait();
		m_threads.join_all();
	}

	void nresource_async_io::io_thread_func()
	{
		io_request req;
		while( !m_shutdown )
		{
			while( !m_requests.empty() )
			{
				//-- 因为大量的资源都涉及到render resource，所以当render device lost的时候统一停止资源加载
				bool device_lost = nengine::instance()->get_renderer()->is_device_lost();
				if(  device_lost )
				{
					::Sleep(1000);
				}
				else
				{
					//-- 执行请求
					m_requests.pop_request(req);

					if( req.req_type == EIOReq_LoadResource )
					{
						resource_location loc(req.pkg_name, req.file_name);
						req.res_ptr->load_from_file( loc );

						nresource_event evt;
						evt.type = EResEvt_Ready;
						evt.res_name = req.res_ptr->get_name();
						nresource_manager::instance()->post_event(evt);
					}
				}
			}// end of while
			
			m_requests.wait_for_request();
		}// end of while
	}

	void nresource_async_io::load_resource(nresource::ptr res_ptr,
		const nstring& pkg_name, const nstring& file_name, enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		if(mode == EIO_Block)
		{
			res_ptr->load_from_file( resource_location(pkg_name, file_name) );

			nresource_event evt;
			evt.type = EResEvt_Ready;
			evt.res_name = res_ptr->get_name();
			nresource_manager::instance()->post_event(evt);
		}
		else
		{
			io_request req;
			req.req_type	= EIOReq_LoadResource;
			req.priority	= pri;
			req.res_ptr		= res_ptr;
			req.pkg_name	= pkg_name;
			req.file_name	= file_name;

			m_requests.push_request(req);
		}
	}
}//namespace nexus