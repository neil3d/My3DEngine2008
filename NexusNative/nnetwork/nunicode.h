/**
 *	nexus network - nunicode.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: L
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NUNICODE_H_
#define _NNETWORK_NUNICODE_H_

#include <string>

namespace nexus {

#ifdef _UNICODE
	typedef std::wstring		tstring;
	typedef std::wstringstream	tstringstream;
	typedef std::wstreambuf		tstreambuf;
	typedef std::wifstream      tifstream;
	typedef std::wofstream      tofstream;
	typedef std::wfstream       tfstream;
#else
	typedef std::string			tstring;
	typedef std::stringstream	tstringstream;
	typedef std::streambuf		tstreambuf;
	typedef std::ifstream       tifstream;
	typedef std::ofstream       tofstream;
	typedef std::fstream        tfstream;
#endif

#ifndef __TFILE__
#ifdef _UNICODE
#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)
#define __TFILE__ WIDEN(__FILE__)
#else
#define __TFILE__ __FILE__
#endif
#endif

#ifndef __TFUNCTION__
#ifdef _UNICODE
#define __TFUNCTION__ WIDEN(__FUNCTION__)
#else
#define __TFUNCTION__ __FUNCTION__
#endif
#endif

#ifndef __TDATE__
#ifdef _UNICODE
#define __TDATE__ WIDEN(__DATE__)
#else
#define __TDATE__ __DATE__
#endif
#endif

} // namespace nexus

#endif _NNETWORK_NUNICODE_H_
