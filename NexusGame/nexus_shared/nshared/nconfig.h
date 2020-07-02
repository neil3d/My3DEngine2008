/**
 *	nexus nshared - nconfig
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NSHARED_NCONFIG_H_
#define _NSHARED_NCONFIG_H_

#include "ncommon.h"

namespace nexus {

	/**
	 *	nconfig
	 */
	class nconfig
	{
	public:
		nconfig();
		virtual ~nconfig();

		virtual bool init(const tstring& file_name, uint16 values_count);
		virtual void destroy();

		void set_int32(uint16 index, int32 value)
		{
			assert (index < m_values_count || _log_index_error(index));
			m_int32_values[index] = value;
		}

		void set_uint32(uint16 index, uint32 value)
		{
			assert (index < m_values_count || _log_index_error(index));
			m_uint32_values[index] = value;
		}

		void set_float(uint16 index, float value)
		{
			assert (index < m_values_count || _log_index_error(index));
			m_float_values[index] = value;
		}

		void set_tstring(uint16 index, const tstring& str);
		void set_string(uint16 index, const std::string& str);

		int32 get_int32(uint16 index) const
		{
			assert (index < m_values_count || _log_index_error(index));
			return m_int32_values[index];
		}

		uint32 get_uint32(uint16 index) const
		{
			assert (index < m_values_count || _log_index_error(index));
			return m_uint32_values[index];
		}

		float get_float(uint16 index) const
		{
			assert (index < m_values_count || _log_index_error(index));
			return m_float_values[index];
		}

		tstring get_tstring(uint16 index) const;
		std::string get_string(uint16 index) const;

	protected:
		bool _set_values_count(uint16 count);

	private:
		bool _log_index_error(uint16 index) const;

	private:
		union
		{
			int32*		m_int32_values;
			uint32*		m_uint32_values;
			float*		m_float_values;
		};

		uint16			m_values_count;

		typedef std::map<uint16, tstring> TSTR_MAP;
		TSTR_MAP		m_tstr_map;

		typedef std::map<uint16, std::string> STR_MAP;
		STR_MAP			m_str_map;
	};

	class nconfig_file
	{
	public:
		nconfig_file();
		~nconfig_file();

		bool open(const tstring& file_name);
		void close();

		tstring get_tstring_default(const tstring& name, const tstring& _default);
		std::string get_string_default(const tstring& name, const std::string& _default);
		bool get_bool_default(const tstring& name, const bool _default = false);
		int32 get_int32_default(const tstring& name, const int32 _default);
		uint32 get_uint32_default(const tstring& name, const uint32 _default);
		int64 get_int64_default(const tstring& name, const int64 _default);
		float get_float_default(const tstring& name, const float _default);

	private:
		tstring nconfig_file::_trim_ws(const tstring& s);

	private:
		typedef boost::unordered_map<tstring, tstring>	NAME_VALUE_MAP;

		NAME_VALUE_MAP			m_name_values;
		tstring					m_file_name;
		FILE*					m_file;
	};

} // namespace nexus

#endif _NSHARED_NCONFIG_H_
