#include "StdAfx.h"
#include "NCharacterStatData.h"

namespace NexusEngine
{
	

	NCharacterStatData::NCharacterStatData( void )
	{
		m_nativeData = new nobj_data();
	}

	NCharacterStatData::~NCharacterStatData( void )
	{
		delete m_nativeData;
		m_nativeData = NULL;
	}

	bool NCharacterStatData::Init( UInt16 valueCount )
	{
		return NativePtr->init(valueCount);
	}

	void NCharacterStatData::OnRecvInitStat( IntPtr data )
	{
		NativePtr->on_recv_init_stat(reinterpret_cast<const uint32*>((void*)data));

		//  The CPP compiler automatically provides threadsafe access,
		//  which allows you to raise the event directly:
		StatInited(this, gcnew EventArgs());
	}

	void NCharacterStatData::OnRecvStatUpdate( IntPtr data )
	{
		NativePtr->on_recv_stat_update(reinterpret_cast<const uint32*>((void*)data));

		//  The CPP compiler automatically provides threadsafe access,
		//  which allows you to raise the event directly:
		StatUpdated(this, gcnew EventArgs());
	}

	void NCharacterStatData::OnRecvRemoteStatUpdate( IntPtr data )
	{
		NativePtr->on_recv_remote_stat_update(reinterpret_cast<const uint32*>((void*)data));

		//  The CPP compiler automatically provides threadsafe access,
		//  which allows you to raise the event directly:
		StatUpdated(this, gcnew EventArgs());
	}

	System::Int32 NCharacterStatData::GetInt32Value( UInt16 index )
	{
		return NativePtr->get_int32_value(index);
	}

	System::UInt32 NCharacterStatData::GetUInt32Value( UInt16 index )
	{
		return NativePtr->get_uint32_value(index);
	}

	System::UInt64 NCharacterStatData::GetUInt64Value( UInt16 index )
	{
		return NativePtr->get_uint64_value(index);
	}

	float NCharacterStatData::GetFloatValue( UInt16 index )
	{
		return NativePtr->get_float_value(index);
	}

	System::Byte NCharacterStatData::GetByteValue( UInt16 index, Byte offest )
	{
		return NativePtr->get_uint8_value(index, offest);
	}

	System::UInt16 NCharacterStatData::GetInt16Value( UInt16 index, Byte offest )
	{
		return NativePtr->get_uint16_value(index, offest);
	}

	bool NCharacterStatData::HasFlag( UInt16 index, UInt32 flag )
	{
		return NativePtr->has_flag(index, flag);
	}
}
