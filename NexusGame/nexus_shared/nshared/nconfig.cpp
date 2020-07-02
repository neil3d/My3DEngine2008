#include "nconfig.h"
#include "nlog_mt.h"

namespace nexus {

	nconfig::nconfig() : m_int32_values(NULL), m_values_count(0)
	{
	}

	nconfig::~nconfig()
	{
		destroy();
	}

	bool nconfig::init(const tstring& file_name, uint16 values_count)
	{
		m_values_count = values_count;
		return true;
	}

	void nconfig::destroy()
	{
		if (m_int32_values)
		{
			delete []m_int32_values;
			m_int32_values = NULL;
	}
	}

	void nconfig::set_tstring(uint16 index, const tstring& str)
	{
		assert (index < m_values_count || _log_index_error(index));

		if (false == m_tstr_map.insert(std::make_pair(index, str)).second)
		{
			NLOG_ERROR(_T("config insert string error!"));
		}
	}

	void nconfig::set_string(uint16 index, const std::string& str)
	{
		assert (index < m_values_count || _log_index_error(index));

		if (false == m_str_map.insert(std::make_pair(index, str)).second)
		{
			NLOG_ERROR(_T("config insert string error!"));
		}
	}

	tstring nconfig::get_tstring(uint16 index) const
	{
		assert (index < m_values_count || _log_index_error(index));

		TSTR_MAP::const_iterator iter = m_tstr_map.find(index);

		if (iter == m_tstr_map.end())
		{
			return tstring(_T(""));
		}

		return iter->second;
	}

	std::string nconfig::get_string(uint16 index) const
	{
		assert (index < m_values_count || _log_index_error(index));

		STR_MAP::const_iterator iter = m_str_map.find(index);

		if (iter == m_str_map.end())
		{
			return std::string("");
		}

		return iter->second;
	}

	bool nconfig::_set_values_count(uint16 count)
	{
		m_values_count = count;

		m_int32_values = new int32[m_values_count];

		if (!m_int32_values)
		{
			return false;
		}

		return true;
	}

	bool nconfig::_log_index_error(uint16 index) const
	{
		NLOG_ERROR(_T("index error, access config, %u > %u!"), index, m_values_count);
		return false;
	}

	nconfig_file::nconfig_file() : m_file(NULL)
	{
	}

	nconfig_file::~nconfig_file()
	{
		if (NULL != m_file)
		{
			fclose(m_file);
		}
	}

	tstring nconfig_file::_trim_ws(const tstring& s)
	{
		tstring::size_type n, n2;
		n = s.find_first_not_of(_T(" \t\r\n"));

		if (n == tstring::npos)
		{
			return tstring();
		}
		else
		{
			n2 = s.find_last_not_of(_T(" \t\r\n"));
			return s.substr(n, n2 - n + 1);
		}
	}

	bool nconfig_file::open(const tstring& file_name)
	{
		FILE* m_file = NULL;
		_tfopen_s(&m_file, file_name.c_str(), _T("rb"));

		if (NULL == m_file)
		{
			return false;
		}

		m_file_name = file_name;
		long cur = ftell(m_file);
		fseek(m_file, 0, SEEK_END);

		long end = ftell(m_file);
		fseek(m_file, cur, SEEK_SET);

		char* data_ptr = new char[end + 2];

		if (!data_ptr)
		{
			NLOG_ERROR(_T("alloc memory error, init config"));
			return false;
		}

		// 加入换行符
		fread(data_ptr, 1, end, m_file);

		nchar* ndate_ptr = (nchar*)data_ptr;
		ndate_ptr[end / sizeof(nchar)] = 0x0a;

		tstring str_line;
		tstring str;
		tstring::size_type n;
		tstring name;
		tstring value;

		for (uint32 i = 1; i <= end / sizeof(nchar); ++i)
		{
			if (ndate_ptr[i] == 0x0a)
			{
				// strip '#' comments and whitespace
				if ((n = str_line.find(_T('#'))) != tstring::npos)
				{
					str_line = str_line.substr(0, n);
				}

				str = _trim_ws(str_line);

				if ((n = str.find('=')) != tstring::npos)
				{
					name = _trim_ws(str.substr(0, n));
					value = _trim_ws(str.substr(n + 1));

					// 加入到player map
					if (!m_name_values.insert(std::make_pair(name, value)).second)
					{
						NLOG_ERROR(_T("insert name value error, init config"));
						return false;
					}
				}

				str_line = _T("");
			}
			else
			{
				str_line.append(1, ndate_ptr[i]);
			}
		}
		
		return true;
	}

	void nconfig_file::close()
	{
		m_name_values.clear();

		if (NULL != m_file)
		{
			fclose(m_file);
		}
	}

	tstring nconfig_file::get_tstring_default(const tstring& name, const tstring& _default)
	{
		NAME_VALUE_MAP::const_iterator iter = m_name_values.find(name);

		if (iter == m_name_values.end())
		{
			return _default;
		}

		if (iter->second.at(0) == _T('"'))
		{
			size_t len = iter->second.length();

			return iter->second.substr(1, len - 2);
		}
		else
		{
			return iter->second;
		}
	}

	std::string nconfig_file::get_string_default(const tstring& name, const std::string& _default)
	{
		NAME_VALUE_MAP::const_iterator iter = m_name_values.find(name);

		if (iter == m_name_values.end())
		{
			return _default;
		}

		tstring temp;

		if (iter->second.at(0) == _T('"'))
		{
			size_t len = iter->second.length();
			temp = iter->second.substr(1, len - 2);
		}
		else
		{
			temp = iter->second;
		}

		std::string cur_locale = setlocale(LC_ALL, NULL);        // curLocale = "C";
		setlocale(LC_ALL, "chs");

		char buff[1024] = {0};

		size_t dest_size = 2 * temp.size() + 1;

		if (dest_size > 1024)
		{
			return _default;
		}

		size_t covert = 0;
		wcstombs_s(&covert, buff, temp.c_str(), dest_size);
		setlocale(LC_ALL, cur_locale.c_str());

		return std::string(buff);
	}

	bool nconfig_file::get_bool_default(const tstring& name, const bool _default)
	{
		NAME_VALUE_MAP::const_iterator iter = m_name_values.find(name);

		if (iter == m_name_values.end())
		{
			return _default;
		}

		if (iter->second == _T("true") || iter->second == _T("TRUE") || iter->second == _T("yes") || iter->second == _T("YES") || iter->second == _T("1"))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int32 nconfig_file::get_int32_default(const tstring& name, const int32 _default)
	{
		NAME_VALUE_MAP::const_iterator iter = m_name_values.find(name);

		if (iter == m_name_values.end())
		{
			return _default;
		}

		return _tstoi(iter->second.c_str());
	}

	uint32 nconfig_file::get_uint32_default(const tstring& name, const uint32 _default)
	{
		NAME_VALUE_MAP::const_iterator iter = m_name_values.find(name);

		if (iter == m_name_values.end())
		{
			return _default;
		}

		return uint32(_tstoi(iter->second.c_str()));
	}

	int64 nconfig_file::get_int64_default(const tstring& name, const int64 _default)
	{
		NAME_VALUE_MAP::const_iterator iter = m_name_values.find(name);

		if (iter == m_name_values.end())
		{
			return _default;
		}

		return _tstoi64(iter->second.c_str());
	}

	float nconfig_file::get_float_default(const tstring& name, const float _default)
	{
		NAME_VALUE_MAP::const_iterator iter = m_name_values.find(name);

		if (iter == m_name_values.end())
		{
			return _default;
		}

		return (float)_tstof(iter->second.c_str());
	}

} // namespace nexus
