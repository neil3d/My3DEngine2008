/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#ifndef _NEXUS_SVR_NET_IO_H_
#define _NEXUS_SVR_NET_IO_H_
#include "lib_define.h"
#include <boost/shared_ptr.hpp>
#include "ntcp_svr.h"
#include "ntcp_client.h"

namespace nexus
{
	class nasio_wrap;

	/**
	 *	server端网络层的facade类
	 *	@remarks 本类并不持有ntcp_svr和ntcp_client对象的引用
	*/
	class nNET_API nsvr_net_io
	{
	public:
		nsvr_net_io(void);
		~nsvr_net_io(void);

		/** 启动：初始化asio service，开启io线程池	*/
		void startup();

		/** 关闭asio service，等待所有io线程结束 */
		void shutdown();

		/** 创建一个tcp server对象 */
		ntcp_svr::ptr create_tcp_svr();		

		/** 创建一个tcp client对象 */
		ntcp_client::ptr create_tcp_client();

	protected:
		boost::shared_ptr<nasio_wrap> m_impl;
	};
}//namespace nexus
#endif //_NEXUS_SVR_NET_IO_H_