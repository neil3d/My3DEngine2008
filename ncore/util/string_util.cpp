#include "stdafx.h"
#include "string_util.h"
#include "../memory/nmemory.h"

namespace nexus
{
	struct local_helper
	{
		std::string cur_locale;

		local_helper()
		{
			cur_locale = setlocale(LC_ALL, NULL);        // curLocale = "C";
			setlocale(LC_ALL, "chs");
		}

		~local_helper()
		{
			setlocale(LC_ALL, cur_locale.c_str());
		}
	};

	nCoreAPI std::string conv_string(const std::wstring& ws)
	{
		local_helper set_loc;

		size_t dest_size = 2 * ws.size() + 1;
		boost::scoped_array<char> dest_buff( nNew char[dest_size] );
		memset(dest_buff.get(), 0, dest_size);

		wcstombs(dest_buff.get(), ws.c_str(), dest_size);

		return std::string(dest_buff.get());
	}

	nCoreAPI std::wstring conv_string(const std::string& s)
	{
		local_helper set_loc;

		size_t dest_size = s.size() + 1;

		boost::scoped_array<wchar_t> dest_buff(nNew wchar_t[dest_size]);

		wmemset(dest_buff.get(), 0, dest_size);

		mbstowcs(dest_buff.get(), s.c_str(), dest_size);

		return std::wstring(dest_buff.get());
	}
}//namespace nexus