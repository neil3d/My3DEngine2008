#include "nobject_data.h"
#include "nplayer.h"
#include "nmap.h"
#include "nlog_mt.h"

#include "nmsg_stat.h"

namespace nexus {

	nobject_data::nobject_data() : m_int32_values(NULL), m_values_count(0), m_update_mask(NULL), m_mask_size(0), m_obj_ptr(NULL)
	{
	}

	nobject_data::~nobject_data()
	{
		destroy();
	}

	bool nobject_data::init(nworld_object* obj_ptr, uint16 values_count)
	{
		if (!obj_ptr)
		{
			return false;
		}

		m_obj_ptr		= obj_ptr;
		m_values_count	= values_count;

		m_mask_size		= (values_count + 31) / 32;
		m_update_mask	= new uint32[m_mask_size];

		if (!m_update_mask)
		{
			return false;
		}

		memset(m_update_mask, 0, m_mask_size * sizeof(uint32));
		m_uint32_values = new uint32[m_values_count];

		if (!m_uint32_values)
		{
			return false;
		}

		memset(m_uint32_values, 0, m_values_count * sizeof(uint32));
		return true;
	}

	void nobject_data::destroy()
	{
		if (m_uint32_values)
		{
			delete []m_uint32_values;
			m_uint32_values = NULL;
	}

		if (m_update_mask)
		{
			delete []m_update_mask;
			m_update_mask = NULL;
	}
	}

	void nobject_data::update(uint32 elapse)
	{
		// 注意上线时计算得到最终数据不是通过update传递到客户端的, 主要计算后clear_update_bits
		// note: tick 向客户单同步修改了的属性, (向数据库同步TODO: D. Zhao)

		tagS2C_StatUpdate reply1;
		tagS2C_RemoteStatUpdate reply2;

		nstat_update_send_helper helper1(reply1.stat_data, EPF_PlayerEnd);
		nstat_update_send_helper helper2(reply2.stat_data, EUF_UnitEnd);

		bool send1 = false;
		bool send2 = false;

		for (uint16 index = 0; index < m_values_count; ++index)
		{
			if (_get_update_bits(index))
			{
				send1 = true;
				helper1.set_update_data(index, get_uint32_value(index));

				if (EUF_UnitMaxHealth == index || EUF_UnitMaxEnergy == index || EUF_UnitMaxMana == index || 
					EUF_UnitSpeed == index || EUF_UnitAttackSpeed == index || EUF_UnitHealth == index || EUF_UnitEnergy == index ||
					EUF_UnitMana == index || EUF_UnitLevel == index || EUF_UnitFlags == index)
				{
					send2 = true;
					helper2.set_update_data(index, get_uint32_value(index));
	}
			}
		}

		if (send1 && EOT_PLAYER == m_obj_ptr->get_type())
		{
			((nplayer*)m_obj_ptr)->send_message(&reply1, helper1.get_packet_size());
		}

		if (send2)
		{
				reply2.unit_id = m_obj_ptr->get_id();
			m_obj_ptr->send_message_to_aoi(&reply2, helper2.get_packet_size(), false);
				}

		clear_update_bits();
	}

	void nobject_data::load_data(const uint32* data_ptr, uint16 off_set)
	{
		memcpy(m_uint32_values + off_set, data_ptr, (m_values_count - off_set) * sizeof(uint32));
	}

	void nobject_data::save_data(uint32* data_ptr, uint16 off_set)
	{
		memcpy(data_ptr, m_uint32_values + off_set, (m_values_count - off_set) * sizeof(uint32));
	}

	void nobject_data::set_int32_value(uint16 index, int32 value)
	{
		assert(index < m_values_count || _log_index_error(index, true));

		if (m_int32_values[index] != value)
		{
			m_int32_values[index] = value;
			_set_update_bits(index);
	}
	}

	void nobject_data::set_uint32_value(uint16 index, uint32 value)
	{
		assert(index < m_values_count || _log_index_error(index, true));

		if (m_uint32_values[index] != value)
		{
			m_uint32_values[index] = value;
			_set_update_bits(index);
		}
	}

	void nobject_data::set_uint64_value(uint16 index, uint64 value)
	{
		assert(index + 1 < m_values_count || _log_index_error(index, true));

		if (*((uint64*)&(m_uint32_values[index])) != value)
		{
			m_uint32_values[index] = *((uint32*)&value);
			m_uint32_values[ index + 1] = *(((uint32*)&value) + 1);
			_set_update_bits(index);
			_set_update_bits(index + 1);
		}
	}

	void nobject_data::set_float_value(uint16 index, float value)
	{
		assert(index < m_values_count || _log_index_error(index, true));

		if (m_float_values[index] != value)
		{
			m_float_values[index] = value;
			_set_update_bits(index);
		}
	}

	void nobject_data::set_uint8_value(uint16 index, uint8 offset, uint8 value)
	{
		assert(index < m_values_count || _log_index_error(index, true));
		assert(offset < sizeof(uint32) / sizeof(uint8));

		if (uint8(m_uint32_values[index] >> (offset * 8)) != value)
		{
			m_uint32_values[index] &= ~uint32(uint32(0XFF) << (offset * 8));
			m_uint32_values[index] |= uint32(uint32(value) << (offset * 8));
			_set_update_bits(index);
		}
	}

	void nobject_data::set_uint16_value(uint16 index, uint8 offset, uint16 value)
	{
		assert(index < m_values_count || _log_index_error(index, true));
		assert(offset < sizeof(uint32) / sizeof(uint16));

		if (uint16(m_uint32_values[index] >> (offset * 16)) != value)
		{
			m_uint32_values[index] &= ~uint32(uint32(0XFFFF) << (offset * 16));
			m_uint32_values[index] |= uint32(uint32(value) << (offset * 16));
			_set_update_bits(index);
		}
	}

	void nobject_data::set_stat_float_value(uint16 index, float value)
	{
		if (value < 0)
		{
			value = 0.0f;
		}

		set_float_value(index, value);
	}

	void nobject_data::set_stat_int32_value(uint16 index, int32 value)
	{
		if (value < 0)
		{
			value = 0;
		}

		set_int32_value(index, value);
	}

	void nobject_data::set_stat_uint32_value(uint16 index, uint32 value)
	{
		int32 val = int32(value);

		if (val < 0)
		{
			value = 0;
		}

		set_uint32_value(index, value);
	}

	void nobject_data::apply_mod_uint32_value(uint16 index, int32 value, bool apply)
	{
		int32 cur = get_uint32_value(index);
		cur += (apply ? value : -value);

		if (cur < 0)
		{
			cur = 0;
		}

		set_uint32_value(index, cur);
	}

	void nobject_data::apply_mod_int32_value(uint16 index, int32 value, bool apply)
	{
		int32 cur = get_int32_value(index);
		cur += (apply ? value : -value);
		set_uint32_value(index, cur);
	}

	void nobject_data::apply_mod_uint64_value(uint16 index, int64 value, bool apply)
	{
		int64 cur = get_uint64_value(index);
		cur += (apply ? value : -value);

		if (cur < 0)
		{
			cur = 0;
		}

		set_uint64_value(index, cur);
	}

	void nobject_data::apply_mod_positive_float_value( uint16 index, float value, bool apply)
	{
		float cur = get_float_value(index);
		cur += (apply ? value : -value);

		if (value < 0)
		{
			value = 0.0f;
		}

		set_float_value(index, cur);
	}

	void nobject_data::apply_mod_signed_float_value( uint16 index, float value, bool apply)
	{
		float cur = get_float_value(index);
		cur += (apply ? value : -value);
		set_float_value(index, cur);
	}

	void nobject_data::set_flag(uint16 index, uint32 new_flag)
	{
		assert(index < m_values_count || _log_index_error(index, true));
		uint32 old_value = m_uint32_values[index];
		uint32 new_value = old_value | new_flag;

		if (old_value != new_value)
		{
			m_uint32_values[index] = new_value;
			_set_update_bits(index);
		}
	}

	void nobject_data::remove_flag(uint16 index, uint32 old_flag)
	{
		assert(index < m_values_count || _log_index_error(index, true));
		uint32 old_value = m_uint32_values[index];
		uint32 new_value = old_value & ~old_flag;

		if (old_value != new_value)
		{
			m_uint32_values[index] = new_value;
			_set_update_bits(index);
		}
	}

	void nobject_data::set_uint8_flag(uint16 index, uint8 offset, uint8 new_flag)
	{
		assert(index < m_values_count || _log_index_error(index, true));
		assert(offset < sizeof(uint32) / sizeof(uint8));

		if (!(uint8(m_uint32_values[index] >> (offset * 8)) & new_flag))
		{
			m_uint32_values[index] |= uint32(uint32(new_flag) << (offset * 8));
			_set_update_bits(index);
		}
	}

	void nobject_data::remove_uint8_flag(uint16 index, uint8 offset, uint8 old_flag)
	{
		assert(index < m_values_count || _log_index_error(index, true));
		assert(offset < sizeof(uint32) / sizeof(uint8));

		if (!(uint8(m_uint32_values[index] >> (offset * 8)) & old_flag))
		{
			m_uint32_values[index] &= ~uint32(uint32(old_flag) << (offset * 8));
			_set_update_bits(index);
		}
	}

	void nobject_data::send_update(uint16 index)
	{
		if (EOT_PLAYER != m_obj_ptr->get_type())
		{
			return;
		}

		tagS2C_StatUpdate reply;
		nstat_update_send_helper helper(reply.stat_data, EPF_PlayerEnd);
		helper.set_update_data(index, get_uint32_value(index));

		// 向数据库同步TODO: D. Zhao

		((nplayer*)m_obj_ptr)->send_message(&reply, helper.get_packet_size());
		clear_update_bits(index);
	}

	void nobject_data::send_update(const std::vector<uint16> indexs)
	{
		if (EOT_PLAYER != m_obj_ptr->get_type())
		{
			return;
		}

		tagS2C_StatUpdate reply;
		nstat_update_send_helper helper(reply.stat_data, EPF_PlayerEnd);

		for (std::vector<uint16>::const_iterator iter = indexs.begin(); iter != indexs.end(); ++iter)
		{
			helper.set_update_data(*iter, get_uint32_value(*iter));
			clear_update_bits(*iter);
		}

		// 向数据库同步TODO: D. Zhao

		((nplayer*)m_obj_ptr)->send_message(&reply, helper.get_packet_size());
	}

	void nobject_data::test_send_recv()
	{
		// send 1
		tagS2C_StatUpdate reply1;

		nstat_update_send_helper helper1(reply1.stat_data, EPF_PlayerEnd);

		for (uint16 index = 0; index < m_values_count; ++index)
		{
			if (_get_update_bits(index))
			{
				helper1.set_update_data(index, get_uint32_value(index));
			}
		}

		uint32 size = helper1.get_packet_size();
		// recv 1
		const tagS2C_StatUpdate* data_ptr = static_cast<const tagS2C_StatUpdate*>(&reply1);

		assert(size == data_ptr->get_packet_size());

		nstat_update_recv_helper recv_helper((uint32*)(data_ptr->stat_data), EPF_PlayerEnd);

		uint16 _index = 0;
		uint32 _value = 0;

		while (recv_helper.get_update_data(_index, _value))
		{
			assert(_value == get_uint32_value(_index));
		}

		// clear_update_bits();

		//////////////////////////////////////////////////////////////////////////
		// send 2
		tagS2C_RemoteStatUpdate reply2;

		nstat_update_send_helper helper2(reply2.stat_data, EUF_UnitEnd);

		for (uint16 index = 0; index < m_values_count; ++index)
		{
			if (_get_update_bits(index))
			{
				helper2.set_update_data(index, get_uint32_value(index));
			}
		}

		uint32 size2 = helper2.get_packet_size();
		// recv 2
		const tagS2C_RemoteStatUpdate* data2_ptr = static_cast<const tagS2C_RemoteStatUpdate*>(&reply2);

		assert(size2 == data2_ptr->get_packet_size());

		nstat_update_recv_helper recv_helper2((uint32*)(data2_ptr->stat_data), EUF_UnitEnd);

		_index = 0;
		_value = 0;

		while (recv_helper2.get_update_data(_index, _value))
		{
			assert(_value == get_uint32_value(_index));
		}

		clear_update_bits();
	}

	bool nobject_data::_log_index_error(uint16 index, bool set_value) const
	{
		NLOG_ERROR(_T("Attempt %s non-existed value field: %u (count: %u)"), (set_value ? _T("set value to") : _T("get value from")), index, m_values_count);
		// assert must fail after function call
		return false;
	}

} // namespace nexus
