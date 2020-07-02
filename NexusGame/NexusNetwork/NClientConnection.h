// NexusNetwork.h

#pragma once
#include "native/client_connection.h"
#include "GameMessageListener.h"

using namespace System;

namespace NexusEngine {

	public enum class ClientSessionStatus : System::Int32
	{
		None		=	nexus::ECSS_None,
		Connected	=	nexus::ECSS_Connected,
		Disconnect	=	nexus::ECSS_Disconnect,
	};


	/**
	* 封装native class client_connection
	* 使用流程 
	* NClientConnection connection = new NClientConnection
	* connection->AddListener(...);
	* connection->Startup(...);
	* connection->Connect(...);
	* while(game)
	* {
	*	connection->SendMessage(...);
	*	connection->Update(...);
	* }
	* connection->Close();
	* connection->ClearListeners();
	* connection->Shutdown();
	*/
	public ref class NClientConnection sealed
	{
	public:
		NClientConnection();
		virtual ~NClientConnection();

		// -- native pointer
		property nexus::client_connection*  NativePtr
		{
			nexus::client_connection*  get()
			{
				return m_NativeObj;
			}
		}

		// 当前Session状态
		property ClientSessionStatus SessionStatus
		{
			ClientSessionStatus get()
			{
				return static_cast<enum class ClientSessionStatus>(m_NativeObj->get_status());
			}
		}

		//-- 封装接口
		void Update(float DeltaTime)
		{
			m_NativeObj->update(DeltaTime);
		}

		// 游戏层监听网络消息
		void AddListener(INetMessageListener^ listener)
		{
			m_NativeObj->add_listener( 
				nexus::msg_listener_ptr(new GameMessageListener(listener))
				);
		}

		void ClearListeners()
		{
			m_NativeObj->clear_listeners();
		}

		// 启动客户端，初始化资源
		bool Startup(UInt32 timeout, bool enableNagle)
		{
			return m_NativeObj->startup(timeout,enableNagle);
		}

		// 关闭客户端，该函数所在执行线程不被强制关闭，建议在主线程中关闭
		void Shutdown()
		{
			m_NativeObj->shutdown();
		}
		
		// 异步连接服务器，连接状况通过回调函数on_connect返回
		bool Connect(System::String^ ipAddress, UINT16 port)
		{
			pin_ptr<const wchar_t> szIP = PtrToStringChars(ipAddress);		
			return m_NativeObj->connect_to(nexus::conv_string(szIP), port);
		}
	
		// 关闭与服务器的链接，关闭成功后回调on_disconnect函数
		void Close()
		{
			m_NativeObj->close();
		}

		// 网络消息结构体如何转换
		bool SendMessage(const void* message, UINT32 size)
		{
			return m_NativeObj->send(message, size);
		}

	private:
		// 自己管理Native对象的生存期
		nexus::client_connection*  m_NativeObj;

	};
}
