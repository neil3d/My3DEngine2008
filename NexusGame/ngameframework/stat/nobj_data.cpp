#include "stdafx.h"
#include "nobj_data.h"

#include "nmsg_stat.h"

namespace gameframework {

	nobj_data::nobj_data() : m_int32_values(NULL), m_values_count(0), m_update_mask(NULL), m_mask_size(0)
	{
	}

	nobj_data::~nobj_data()
	{
		destroy();
	}

	bool nobj_data::init(uint16 values_count)
	{
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

	void nobj_data::destroy()
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

	void nobj_data::on_recv_init_stat(const uint32* data_ptr)
	{
		memcpy(m_uint32_values, data_ptr, m_values_count * sizeof(uint32));

		// 回调
	}

	void nobj_data::on_recv_stat_update(const uint32* data_ptr)
	{
		nstat_update_recv_helper recv_helper(data_ptr, m_values_count);

		uint16 _index = 0;
		uint32 _value = 0;

		while (recv_helper.get_update_data(_index, _value))
		{
			set_uint32_value(_index, _value);
		}

		// 回调
		clear_update_bits();
	}

	void nobj_data::on_recv_remote_stat_update(const uint32* data_ptr)
	{
		nstat_update_recv_helper recv_helper(data_ptr, EUF_UnitEnd);

		uint16 _index = 0;
		uint32 _value = 0;

		while (recv_helper.get_update_data(_index, _value))
		{
			set_uint32_value(_index, _value);
		}

		// 回调
		clear_update_bits();
	}

	void nobj_data::set_int32_value(uint16 index, int32 value)
	{
		assert(index < m_values_count || _log_index_error(index, true));

		if (m_int32_values[index] != value)
		{
			m_int32_values[index] = value;
			_set_update_bits(index);
		}
	}

	void nobj_data::set_uint32_value(uint16 index, uint32 value)
	{
		assert(index < m_values_count || _log_index_error(index, true));

		if (m_uint32_values[index] != value)
		{
			m_uint32_values[index] = value;
			_set_update_bits(index);
		}
	}

	void nobj_data::set_uint64_value(uint16 index, uint64 value)
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

	void nobj_data::set_float_value(uint16 index, float value)
	{
		assert(index < m_values_count || _log_index_error(index, true));

		if (m_float_values[index] != value)
		{
			m_float_values[index] = value;
			_set_update_bits(index);
		}
	}

	void nobj_data::set_uint8_value(uint16 index, uint8 offset, uint8 value)
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

	void nobj_data::set_uint16_value(uint16 index, uint8 offset, uint16 value)
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

	void nobj_data::set_stat_float_value(uint16 index, float value)
	{
		if (value < 0)
		{
			value = 0.0f;
		}

		set_float_value(index, value);
	}

	void nobj_data::set_stat_int32_value(uint16 index, int32 value)
	{
		if (value < 0)
		{
			value = 0;
		}

		set_int32_value(index, value);
	}

	void nobj_data::set_stat_uint32_value(uint16 index, uint32 value)
	{
		int32 val = int32(value);

		if (val < 0)
		{
			value = 0;
		}

		set_uint32_value(index, value);
	}

	void nobj_data::set_flag(uint16 index, uint32 new_flag)
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

	void nobj_data::remove_flag(uint16 index, uint32 old_flag)
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

} // namespace gameframework
