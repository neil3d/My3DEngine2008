/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/
#ifndef _NEXUS_TCP_SVR_H_
#define _NEXUS_TCP_SVR_H_
#include "lib_define.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include "ntcp_connection.h"

namespace nexus
{
	class nasio_wrap;

	typedef boost::function<void (ntcp_connection::ptr)> new_connection_handler;

	/**
	 *	A tcp server
	 *	@remark 在指定的IP, port上启动listen，通过call back函数通知上层新建连接事件；
	 *			新建立的连接对象由上层管理，ntcp_svr并不持有connection的引用；
	*/
	class nNET_API ntcp_svr
	{
	public:
		typedef boost::shared_ptr<ntcp_svr> ptr;

		explicit ntcp_svr(boost::shared_ptr<nasio_wrap> asio_ptr);
		~ntcp_svr(void);

		/**	启动服务器	*/
		bool startup(const std::string& address, const std::string& port, size_t max_pkt_size);

		/** 关闭服务器 */
		void shutdown();

		/** 设置新建连接时的call back函数
		 *	@remark 事件函数将在io worker thread中被调用
		*/
		void set_new_connection_handler(new_connection_handler handler);

	private:
		struct impl;
		boost::shared_ptr<impl>	m_impl;
	};
}//namespace nexus

#endif //_NEXUS_TCP_SVR_H_