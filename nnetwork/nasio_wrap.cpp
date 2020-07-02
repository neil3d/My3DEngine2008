#include "nasio_wrap.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "../ncore/ncore.h"

namespace nexus
{
	nasio_wrap::nasio_wrap(void):m_runing(false)
	{
	}

	nasio_wrap::~nasio_wrap(void)
	{
	}

	void nasio_wrap::startup(size_t thread_pool_size)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		m_runing = true;

		//-- 配置线程个数
		if (thread_pool_size == -1)
		{
			thread_pool_size = boost::thread::hardware_concurrency() *2;
		}
		if (thread_pool_size == 0)
		{
			thread_pool_size = 1;
		}

		//-- 启动线程池
		m_thread_pool.clear();
		for (size_t i=0; i<thread_pool_size; i++)
		{
			boost::shared_ptr<boost::thread> thd(new boost::thread(
				boost::bind(&nasio_wrap::_thread_run, this) ));
			m_thread_pool.push_back(thd);
		}
	}

	void nasio_wrap::shutdown()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		m_runing = false;

		// 关闭asio service
		m_asio.stop();

		// 等待线程全部结束
		for(thread_pool::iterator iter = m_thread_pool.begin();
			iter != m_thread_pool.end();
			++iter)
		{
			(*iter)->join();
		}
		m_thread_pool.clear();
	}

	void nasio_wrap::_thread_run(void)
	{
		do
		{
			try
			{
				m_asio.run();
			}
			catch (std::exception& e)
			{
				nLog_Exception(e.what());
			}
			boost::this_thread::sleep( boost::posix_time::milliseconds(1) );
		}
		while(m_runing);
	}
}//namespace nexus