/**
*	Network for server
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#ifndef _NEXUS_TCP_CLIENT_H_
#define _NEXUS_TCP_CLIENT_H_
#include "lib_define.h"
#include <boost/shared_ptr.hpp>
#include "ntcp_client_connection.h"

namespace nexus
{
	class nasio_wrap;

	/**
	 *	服务器组内部使用的tcp client
	 *	@remarks 并不持有tcp client connection对象的引用
	*/
	class nNET_API ntcp_client
	{
	public:
		typedef boost::shared_ptr<ntcp_client> ptr;

		ntcp_client(boost::shared_ptr<nasio_wrap> asio_ptr);
		~ntcp_client(void);

		/** 新建一个连接对象，外部可以使用它去连接服务器 */
		ntcp_client_connection::ptr create_connection();

	private:
		struct impl;
		boost::shared_ptr<impl>	m_impl;
	};
}//namespace nexus

#endif //_NEXUS_TCP_CLIENT_H_