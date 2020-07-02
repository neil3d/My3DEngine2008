/**
 *	nexus gameframework - nobj_data
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NEXUS_GAMEFRAMEWORK_NOBJ_DATA_H_
#define _NEXUS_GAMEFRAMEWORK_NOBJ_DATA_H_

#include "ntype_def.h"

#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4100) // anonymous unions warning


using namespace nexus;

namespace gameframework {

	class nGameFrameworkAPI nobj_data
	{
	public:
		typedef boost::shared_ptr<nobj_data> ptr;

		nobj_data();
		virtual ~nobj_data();

		bool init(uint16 values_count);
		void destroy();

		void on_recv_init_stat(const uint32* data_ptr);
		void on_recv_stat_update(const uint32* data_ptr);
		void on_recv_remote_stat_update(const uint32* data_ptr);

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

		bool get_update_bits(uint16 index) const
		{
			return (((uint8*)m_update_mask)[index >> 3] & (1 << (index & 0X7))) != 0;
		}

	private:
		virtual bool _log_index_error(uint16 index, bool set_value) const { return false; }

		void _set_update_bits(uint16 index) const
		{
			((uint8*)m_update_mask)[index >> 3] |= (1 << (index & 0X7));
		}

	private:
		union
		{
			int32*				m_int32_values;
			uint32*				m_uint32_values;
			float*				m_float_values;
		};

		uint16					m_values_count;
		uint32*					m_update_mask;
		uint16					m_mask_size;
	};

} // namespace gameframework

#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4100)
#endif

#endif // _NEXUS_GAMEFRAMEWORK_NOBJ_DATA_H_
