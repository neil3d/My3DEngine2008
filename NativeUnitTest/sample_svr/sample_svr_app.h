#ifndef _SAMPLE_SVR_APP_H_
#define _SAMPLE_SVR_APP_H_
#include <map>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#define BOOST_THREAD_DYN_DLL
#include <boost/thread.hpp>

#include "sample_protocol.h"
#include "../../NexusNative/nnetwork/net_pkt_util.h"
#include "../../NexusNative/nnetwork/nsvr_net_io.h"
#include "../../NexusNative/nnetwork/ntcp_svr.h"

using namespace std;

class sample_svr_app;

/**
 *	管理一个与客户端的会话
 *	@remarks 持有tcp connection对象引用，负责网络消息解析
*/
struct sample_session
	: public boost::enable_shared_from_this<sample_session>
{
	typedef boost::shared_ptr<sample_session> ptr;

	ntcp_connection::ptr	m_conn;
	sample_svr_app*			m_owner;

	sample_session(ntcp_connection::ptr conn,
		sample_svr_app* owner)
		: m_conn(conn),
		m_owner(owner)
	{
		// 不要bind到shared_from_this()否则会与connection对象产生循环引用
		m_conn->set_recv_handler(boost::bind(&sample_session::on_recv_net_pkt, this, _1));
		m_conn->set_state_change_handler(boost::bind(&sample_session::on_conn_state_change, this, _1));
	}
	~sample_session()
	{
		cout << "svr event : session dtor()" << endl;
	}

	void shutdown()
	{	
		
	}

	/** 
	 *	处理连接状态改变
	 *	@remarks 当连接关闭是，需要从svr_app中移除自己的引用，以释放session对象
	*/
	void on_conn_state_change(enum EConnectionState st);

	/** 
	 *	处理接收到的网络包
	 *	@remarks 对于反应式的服务器，可以在这里处理网络消息，这并不会阻塞底层IO线程；
	 *		对于游戏循环式服务器，在这里可以将消息保存至接收队列，待svr tick时由游戏主线程去处理；
	*/
	void on_recv_net_pkt(nnet_packet_ptr pkt_ptr)
	{
		nnet_pkt_reader pkt_reader(pkt_ptr);
		while( !pkt_reader.empty() )
		{
			nnet_msg_header* msg_header = pkt_reader.front_msg();
			if( msg_header->msg_id == EMSG_Echo )
			{
				MSG_Echo* emsg = (MSG_Echo*)msg_header;
				cout << "session id : " << m_conn->get_id()
					<< ", recv echo msg : " << emsg->msg_txt << endl;
			}
			else
			{
				cout << "svr event : BAD msg id." << endl;
			}

			m_conn->send_push_msg(msg_header);
			pkt_reader.pop_msg();
		}

		m_conn->send_packet();
	}
};

/**
 *	管理当前所有session，处理tcp_svr事件
*/
class sample_svr_app
{
public:
	boost::recursive_mutex		m_mutex;
	nsvr_net_io		m_svr_net;
	ntcp_svr::ptr	m_tcp_svr;
	std::map<int, sample_session::ptr>	m_session_map;

public:
	sample_svr_app(){}
	~sample_svr_app(){}

	bool startup()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		m_svr_net.startup();
		
		m_tcp_svr = m_svr_net.create_tcp_svr();
		m_tcp_svr->set_new_connection_handler(boost::bind(&sample_svr_app::on_new_connection, this, _1));
		
		return m_tcp_svr->startup("localhost", "9000", NNET_MAX_WAN_PACKET_SIZE);
	}

	void shutdown()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		//-- 关闭所有session
		std::map<int, sample_session::ptr>::iterator iter;

		for(iter = m_session_map.begin();
			iter != m_session_map.end();
			++iter)
		{
			iter->second->shutdown();
		}		

		//-- 关闭底层
		m_tcp_svr->shutdown();
		m_tcp_svr.reset();

		m_svr_net.shutdown();
	}

	
	/** 完成一个新连接，建立session对象 */
	void on_new_connection(ntcp_connection::ptr new_conn)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		sample_session::ptr new_session( new sample_session(new_conn, this) );		
		m_session_map.insert(std::make_pair(new_conn->get_id(), new_session));

		cout << "svr event : new client connected, num clients = " << m_session_map.size()
			<< endl;
	}

	/** 当连接断开后，session做完自己必要的处理后向svr app注销自己 */
	void on_close_session(sample_session::ptr session)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		std::map<int, sample_session::ptr>::iterator iter = m_session_map.find( session->m_conn->get_id() );

		BOOST_ASSERT( iter != m_session_map.end() );
		BOOST_ASSERT( iter->second == session );

		m_session_map.erase( iter );

		cout << "svr event : client disconnected, num clients = " << m_session_map.size()
			<< endl;
	}
};

inline void sample_session::on_conn_state_change(enum EConnectionState st)
{
	switch(st)
	{
	case ECS_Disconnected:
	case ECS_Error:
		m_owner->on_close_session(shared_from_this());
		break;
	}
}

#endif //_SAMPLE_SVR_APP_H_