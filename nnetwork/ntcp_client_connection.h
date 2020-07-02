/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/


#ifndef _NEXUS_TCP_CLIENT_CONNECTION_H_
#define _NEXUS_TCP_CLIENT_CONNECTION_H_
#include "lib_define.h"
#include <boost/shared_ptr.hpp>
#include "ntcp_connection.h"

namespace nexus
{
	/**
	 *	tcp client的一个连接对象
	*/
	class nNET_API ntcp_client_connection
		: public ntcp_connection
	{
	public:
		typedef boost::shared_ptr<ntcp_client_connection> ptr;

		ntcp_client_connection(boost::shared_ptr<nasio_wrap> asio_ptr, int id, size_t max_pkt_size);
		virtual ~ntcp_client_connection(void);

		/** 以阻塞模式连接指定的服务器 */
		bool connect_to(const std::string& host_name, const std::string& port);
	};
}//namespace nexus
#endif //_NEXUS_TCP_CLIENT_CONNECTION_H_