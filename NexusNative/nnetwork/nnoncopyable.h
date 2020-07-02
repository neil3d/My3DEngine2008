/**
 *	nexus network - nnoncopyable
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NNONCOPYABLE_H_
#define _NNETWORK_NNONCOPYABLE_H_

#include "ncommon.h"

namespace nexus {

	class nNET_API nnoncopyable
	{
	protected:
		nnoncopyable() {}
		~nnoncopyable() {}

	private:
		// emphasize the following members are private
		nnoncopyable(const nnoncopyable&);
		const nnoncopyable& operator=(const nnoncopyable&);
	};

} // namespace nexus

#endif _NNETWORK_NNONCOPYABLE_H_
