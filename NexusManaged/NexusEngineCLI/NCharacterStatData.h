#pragma once
#include "stat/nobj_data.h"

namespace NexusEngine
{
	// gameframework::nobj_data Interop封装
	// 使用需要注意:
	// 1.必须在适当的实际初始化适当的大小Init
	// 2.访问数据成员是使用Get*()函数 同时注意数据类型
	// 3.设置数据一律使用OnRecv*()函数
	public ref class NCharacterStatData
	{
	public:
		//-- 事件处理
		event EventHandler<EventArgs^>^ StatInited;
		event EventHandler<EventArgs^>^ StatUpdated;

		NCharacterStatData(void);
		~NCharacterStatData(void);

		bool Init(UInt16 valueCount);

		void OnRecvInitStat(IntPtr data);
		void OnRecvStatUpdate(IntPtr data);
		void OnRecvRemoteStatUpdate(IntPtr data);

		Int32 GetInt32Value(UInt16 index);
		UInt32 GetUInt32Value(UInt16 index);
		UInt64 GetUInt64Value(UInt16 index);
		float GetFloatValue(UInt16 index);
		Byte GetByteValue(UInt16 index, Byte offest);
		UInt16 GetInt16Value(UInt16 index, Byte offest);

		bool HasFlag(UInt16 index, UInt32 flag);

		property nobj_data* NativePtr
		{
			nobj_data* get()
			{
				return m_nativeData;
			}
		}

	private:
		nobj_data* m_nativeData;

	};
}
