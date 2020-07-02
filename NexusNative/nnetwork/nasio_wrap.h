/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#ifndef _NEXUS_SVR_NET_IO_IMPL_H_
#define _NEXUS_SVR_NET_IO_IMPL_H_
#include <vector>
#include "lib_define.h"
#include <boost/asio.hpp>

#define BOOST_THREAD_DYN_DLL
#include <boost/thread.hpp>

namespace nexus
{
	/**
	 *	nsvr_net_io的实现
	 *	@remark 供ntcp_svr个内部类使用，避免对外包括asio
	*/
	class nasio_wrap
	{
	public:
		nasio_wrap(void);
		~nasio_wrap(void);

		void startup(size_t thread_pool_size=-1);
		void shutdown();

		boost::asio::io_service& get_asio()	{	return m_asio; }

	private:
		void _thread_run(void);
	private:
		typedef std::vector<boost::shared_ptr<boost::thread> > thread_pool;

		boost::recursive_mutex	m_mutex;
		boost::asio::io_service	m_asio;
		thread_pool				m_thread_pool;
		bool					m_runing;
	};
}//namespace nexus
#endif //_NEXUS_SVR_NET_IO_IMPL_H_