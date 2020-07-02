#include "StdAfx.h"
#include <boost/crc.hpp>  // for boost::crc_32_type
#include "nname.h"

namespace nexus
{
	nname::nname(const nstring& szname) : name_str(szname)
	{
		boost::crc_32_type  crc_result;
		crc_result.process_bytes(name_str.data(), 
			name_str.size()*sizeof(nstring::value_type));

		name_crc = crc_result.checksum();
	}

	void nname::reset(const nstring& szname)
	{
		name_str = szname;

		boost::crc_32_type  crc_result;
		crc_result.process_bytes(name_str.data(), 
			name_str.size()*sizeof(nstring::value_type));

		name_crc = crc_result.checksum();
	}
}//namespace nexus