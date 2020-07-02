/**
 *	nexus ngame - nobject_data
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NOBJECT_DATA_H_
#define _NGAME_NOBJECT_DATA_H_

#include "ncommon.h"

namespace nexus {

	class nworld_object;

	class nobject_data : private nnoncopyable
	{
	public:
		nobject_data();
		virtual ~nobject_data();

		bool init(nworld_object* obj_ptr, uint16 values_count);
		void destroy();
		void update(uint32 elapse);

		// 数据加载与保存
		void load_data(const uint32* data_ptr, uint16 off_set);
		void save_data(uint32* data_ptr, uint16 off_set);

		int32 get_int32_value(uint16 index) const
		{
			assert(index < m_values_count || _log_index_error(index, false));
			return m_int32_values[index];
		}

		uint32 get_uint32_value(uint16 index) const
		{
			assert(index < m_values_count || _log_index_error(index, false));
			return m_uint32_values[index];
		}

		uint64 get_uint64_value(uint16 index) const
		{
			assert(index < m_values_count || _log_index_error(index, false));
			return *((uint64*)&(m_uint32_values[index]));
		}

		float& get_float_value(uint16 index) const
		{
			assert(index < m_values_count || _log_index_error(index, false));
			return m_float_values[index];
		}

		uint8 get_uint8_value(uint16 index, uint8 offset) const
		{
			assert(index < m_values_count || _log_index_error(index, false));
			assert(offset < sizeof(uint32) / sizeof(uint8));
			return *(((uint8*)&m_uint32_values[index]) + offset);
		}

		uint16 get_uint16_value(uint16 index, uint8 offset) const
		{
			assert(index < m_values_count || _log_index_error(index, false));
			assert(offset < sizeof(uint32) / sizeof(uint16));
			return *(((uint16*)&m_uint32_values[index]) + offset);
		}

		void set_int32_value(uint16 index, int32 value);
		void set_uint32_value(uint16 index, uint32 value);
		void set_uint64_value(uint16 index, uint64 value);
		void set_float_value(uint16 index, float value);
		void set_uint8_value(uint16 index, uint8 offset, uint8 value);
		void set_uint16_value(uint16 index, uint8 offset, uint16 value);
		void set_int16_value(uint16 index, uint8 offset, int16 value ) { set_uint16_value(index, offset,(uint16)value); }
		void set_stat_float_value(uint16 index, float value);
		void set_stat_int32_value(uint16 index, int32 value);
		void set_stat_uint32_value(uint16 index, uint32 value);

		void apply_mod_uint32_value(uint16 index, int32 value, bool apply);
		void apply_mod_int32_value(uint16 index, int32 value, bool apply);
		void apply_mod_uint64_value(uint16 index, int64 value, bool apply);
		void apply_mod_positive_float_value( uint16 index, float value, bool apply);
		void apply_mod_signed_float_value( uint16 index, float value, bool apply);

		void apply_percent_mod_float_value(uint16 index, float value, bool apply)
		{
			value = value > -100.0f ? value : -99.9f;
			set_float_value(index, get_float_value(index) * (apply ? (100.0f + value) / 100.0f : 100.0f / (100.0f + value)));
		}

		void set_flag(uint16 index, uint32 new_flag);
		void remove_flag(uint16 index, uint32 old_flag);

		void toggle_flag( uint16 index, uint32 flag)
		{
			if (has_flag(index, flag))
			{
				remove_flag(index, flag);
			}
			else
			{
				set_flag(index, flag);
			}
		}

		bool has_flag( uint16 index, uint32 flag ) const
		{
			assert(index < m_values_count || _log_index_error(index, false));
			return (m_uint32_values[index] & flag) != 0;
		}

		void set_uint8_flag(uint16 index, uint8 offset, uint8 new_flag);
		void remove_uint8_flag(uint16 index, uint8 offset, uint8 old_flag);

		void togglef_lag(uint16 index, uint8 offset, uint8 flag)
		{
			if (has_uint8_flag(index, offset, flag))
			{
				remove_uint8_flag(index, offset, flag);
			}
			else
			{
				set_uint8_flag(index, offset, flag);
			}
		}

		bool has_uint8_flag( uint16 index, uint8 offset, uint8 flag ) const
		{
			assert(index < m_values_count || _log_index_error(index, false));
			assert(offset < sizeof(uint32) / sizeof(uint8));
			return (((uint8*)&m_uint32_values[index])[offset] & flag) != 0;
		}

		void apply_mod_flag( uint16 index, uint32 flag, bool apply)
		{
			apply ? set_flag(index, flag) : remove_flag(index, flag);
		}

		void set_uint64_flag(uint16 index, uint64 new_flag)
		{
			uint64 old_value = get_uint64_value(index);
			uint64 new_value = old_value | new_flag;
			set_uint64_value(index, new_value);
		}

		void remove_uint64_flag(uint16 index, uint64 old_flag)
		{
			uint64 old_value = get_uint64_value(index);
			uint64 new_value = old_value & ~old_flag;
			set_uint64_value(index, new_value);
		}

		void toggle_uint64_flag(uint16 index, uint64 flag)
		{
			if (has_uint64_flag(index, flag))
			{
				remove_uint64_flag(index, flag);
			}
			else
			{			
				set_uint64_flag(index, flag);
			}
		}

		bool has_uint64_flag(uint16 index, uint64 flag) const
		{
			assert(index < m_values_count || _log_index_error(index, false));
			return (get_uint64_value(index) & flag) != 0;
		}

		void apply_mod_uint64_flag(uint16 index, uint64 flag, bool apply)
		{
			apply ? set_uint64_flag(index, flag) : remove_uint64_flag(index, flag);
		}

		void clear_update_bits()
		{
			if (m_update_mask)
			{
				memset(m_update_mask, 0, m_mask_size * sizeof(uint32));
			}
		}

		void clear_update_bits(uint16 index)
		{
			((uint8*)m_update_mask)[index >> 3] &= (0XFF ^ (1 << (index & 0X7)));
		}

		// 立即同步到客户端数据库, 不会涉及到广播的数据
		void send_update(uint16 index);
		void send_update(const std::vector<uint16> indexs);
		void test_send_recv();

	private:
		bool _log_index_error(uint16 index, bool set_value) const;

		void _set_update_bits(uint16 index) const
		{
			((uint8*)m_update_mask)[index >> 3] |= (1 << (index & 0X7));
		}

		bool _get_update_bits(uint16 index) const
		{
			return (((uint8*)m_update_mask)[index >> 3] & (1 << (index & 0X7))) != 0;
		}

	private:
		union
		{
			int32*				m_int32_values;
			uint32*				m_uint32_values;
			float*				m_float_values;
	};

		uint16					m_values_count;
		uint32*					m_update_mask;					// 根据guid代表的类型解释mask的长度
		uint16					m_mask_size;

		nworld_object*			m_obj_ptr;
	};

} // namespace nexus

#endif // _NGAME_NOBJECT_DATA_H_
