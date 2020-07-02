/**
 *	nexus network - nlzo_wrap
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NNETWORK_NLZO_WRAP_H_
#define _NNETWORK_NLZO_WRAP_H_

#include "ncommon.h"
#include "minilzo.h"

namespace nexus {

	/**
	 *	nlzo_wrap
	 */
	class nNET_API nlzo_wrap : private nnoncopyable
	{
	public:
		nlzo_wrap();
		~nlzo_wrap();

		bool init();
		void destroy();

		static bool check_out_buff(uint32 in_len, uint32 out_len);

		bool compress(const void* in, uint32 in_len, void* out, uint32& out_len);
		bool decompress(const void* in, uint32 in_len, void* out, uint32& out_len);

		// 线程安全
		bool compress_ts(const void* in, uint32 in_len, void* out, uint32& out_len);

	private:
		char*			m_work_mem;
	};

} // namespace nexus

#endif // _NNETWORK_NLZO_WRAP_H_

