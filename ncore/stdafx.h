// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma warning(disable: 4819)	// The file contains a character that cannot be represented in the current code page (936).

#define BOOST_LIB_DIAGNOSTIC

#include "base_define.h"
#include "base_depend.h"

#pragma warning(push)
#pragma warning(disable:4512)
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#define BOOST_FILESYSTEM_DYN_LINK 
#include <boost/filesystem.hpp>
#include <boost/crc.hpp>  // for boost::crc_32_type
#pragma warning(pop)

#include <sstream>