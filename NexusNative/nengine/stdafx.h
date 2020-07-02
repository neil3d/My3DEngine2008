
#pragma once

#pragma warning(disable: 4251)	// class  needs to have dll-interface to
#pragma warning(disable: 4819)	// The file contains a character that cannot be represented in the current code page (936).
#pragma warning(disable: 4127)

#pragma warning(push)
#pragma warning(disable:4512)
#pragma warning(disable:4244)
#include "base_depend.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#define BOOST_FILESYSTEM_DYN_LINK 
#include <boost/filesystem.hpp>
#pragma warning(pop)

#include "engine_define.h"