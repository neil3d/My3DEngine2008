/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once

namespace NexusEngine
{
	/**
	 * native log对象包装
	*/
	public ref class NNativeLog
	{
	private:
		NNativeLog(void);

		static NNativeLog^ s_instance;
	public:
		static NNativeLog(void)
		{
			s_instance = gcnew NNativeLog;
		}
		static property NNativeLog^ Instance
		{
			NNativeLog^ get()
			{
				return s_instance;
			}
		}

		String^ GetLastException();
	};
}//namespace NexusEngine