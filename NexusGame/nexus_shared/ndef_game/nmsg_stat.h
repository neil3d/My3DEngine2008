/**
 *	nexus ndef_game - nmsg_stat.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDEF_GAME_NMSG_STAT_H_
#define _NDEF_GAME_NMSG_STAT_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "nobject_fields.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * 属性相关消息 id区间[Msg_Client_Stat, Msg_Client_Stat + 20]
	 */
	enum
	{
		S2C_StatError			= Msg_Client_Stat,
		S2C_StatUpdate,
		S2C_RemoteStatUpdate,


	};

	// 错误枚举值
	enum EStatError
	{

	};

	// 玩家自己属性变化, 服务器发送时, 包大小外部计算不用get_packet_size函数
	struct tagS2C_StatUpdate : public nmsg_base
	{
		uint32			stat_data[(EPF_PlayerEnd + 31) / 32 + EPF_PlayerEnd];

		tagS2C_StatUpdate()
		{
			memset(this, 0, sizeof(*this));
			msg_id = S2C_StatUpdate;
		}

		uint32 get_packet_size() const
		{
			uint16 num = 0;

			for (uint16 index = 0; index < EPF_PlayerEnd; ++index)
			{
				if ((((uint8*)stat_data)[index >> 3] & (1 << (index & 0X7))) != 0)
				{
					++num;
				}
			}

			return sizeof(nmsg_base) + ((EPF_PlayerEnd + 31) / 32 + num) * sizeof(uint32);
		}
	};

	// 远程unit属性变化, 服务器发送时, 包大小外部计算不用get_packet_size函数
	// 该消息更新的属性主要有
	struct tagS2C_RemoteStatUpdate : public nmsg_base
	{
		uint64			unit_id;
		uint32			stat_data[(EUF_UnitEnd + 31) / 32 + EUF_UnitEnd];

		tagS2C_RemoteStatUpdate()
		{
			memset(this, 0, sizeof(*this));
			msg_id = S2C_RemoteStatUpdate;
		}

		uint32 get_packet_size() const
		{
			uint16 num = 0;

			for (uint16 index = 0; index < EUF_UnitEnd; ++index)
			{
				if ((((uint8*)stat_data)[index >> 3] & (1 << (index & 0X7))) != 0)
				{
					++num;
				}
			}

			return sizeof(nmsg_base) + sizeof(uint64) + ((EUF_UnitEnd + 31) / 32 + num) * sizeof(uint32);
		}
	};

	class nstat_update_send_helper
	{
	private:
		uint32* const 	mask_ptr;
		uint32* const 	data_ptr;
		uint16			max_values;
		uint16			value_index;

		void _set_update_bits(uint16 index) const
		{
			((uint8*)mask_ptr)[index >> 3] |= (1 << (index & 0X7));
		}

	public:
		explicit nstat_update_send_helper(uint32* stat_data_ptr, uint16 max_value_num) : mask_ptr(stat_data_ptr), 
			data_ptr(stat_data_ptr + (max_value_num + 31) / 32), max_values(max_value_num), value_index(0) {}
		~nstat_update_send_helper() {}

		void set_update_data(uint16 index, uint32 value)
		{
			_set_update_bits(index);
			data_ptr[value_index] = value;
			++value_index;
		}

		uint32 get_packet_size() const
		{
			if (EUF_UnitEnd == max_values)
			{
				return sizeof(nmsg_base) + sizeof(uint64) + ((max_values + 31) / 32 + value_index) * sizeof(uint32);
			}
			else
			{
				return sizeof(nmsg_base) + ((max_values + 31) / 32 + value_index) * sizeof(uint32);
			}
		}
	};

	class nstat_update_recv_helper
	{
	private:
		const uint32* 	mask_ptr;
		const uint32* 	data_ptr;
		uint16			cur_index;
		uint16			value_index;
		uint16			max_value;

		bool _get_update_bits(uint16 index) const
		{
			return (((uint8*)mask_ptr)[index >> 3] & (1 << (index & 0X7))) != 0;
		}

	public:
		explicit nstat_update_recv_helper(const uint32* stat_data_ptr, uint16 max_value_num) : mask_ptr(stat_data_ptr), 
			data_ptr(stat_data_ptr + (max_value_num + 31) / 32), cur_index(0), value_index(0), max_value(max_value_num) {}
		~nstat_update_recv_helper() {}

		bool get_update_data(uint16& index, uint32& value)
		{
			while (cur_index < max_value)
			{
				if (_get_update_bits(cur_index))
				{
					index = cur_index;
					value = data_ptr[value_index];
					++value_index;
					++cur_index;
					return true;
				}

				++cur_index;
			}

			return false;
		}
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_STAT_H_
