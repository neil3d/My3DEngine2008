// 这是主 DLL 文件。

#include "stdafx.h"

#include "NClientConnection.h"

namespace NexusEngine
{


	NClientConnection::NClientConnection()
	{
		m_NativeObj = new nexus::client_connection();
	}

	NClientConnection::~NClientConnection()
	{
		if(m_NativeObj!=NULL)
		{
			delete m_NativeObj; 
			m_NativeObj = NULL;
		}
	}
}