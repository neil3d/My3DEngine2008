#pragma once

namespace NexusEngine
{
	/**
	* 对native engine全局方法的封装
	*/
	public ref class NativeEngineHelper
	{
	public:
		static float GetTimeSeconds();
	
		//! 计算指定字符串的CRC值
		static UInt32 CheckCRC(System::String^ nameStr);
	};
}
