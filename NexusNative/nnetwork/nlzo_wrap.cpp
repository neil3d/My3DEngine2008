#include "nlzo_wrap.h"

namespace nexus {

	nlzo_wrap::nlzo_wrap() : m_work_mem(NULL)
	{
	}

	nlzo_wrap::~nlzo_wrap()
	{
		destroy();
	}

	bool nlzo_wrap::init()
	{
		m_work_mem = new char[LZO1X_1_MEM_COMPRESS + sizeof(lzo_align_t)];

		if (!m_work_mem)
		{
			return false;
		}

		if (LZO_E_OK != lzo_init())
		{
			return false;
		}

		return true;
	}

	void nlzo_wrap::destroy()
	{
		if (m_work_mem)
		{
			delete []m_work_mem;
			m_work_mem = NULL;
		}
	}

	bool nlzo_wrap::check_out_buff(uint32 in_len, uint32 out_len)
	{
		if (out_len < in_len + (in_len / 64) + 16 + 3)
		{
			return false;
		}

		return true;
	}

	bool nlzo_wrap::compress(const void* in, uint32 in_len, void* out, uint32& out_len)
	{
		lzo_uint dst_len = 0;
		int ret = lzo1x_1_compress((const lzo_bytep)in, (lzo_uint)in_len, (lzo_bytep)out, (lzo_uintp)(&dst_len), (lzo_voidp)m_work_mem);
		out_len = (uint32)dst_len;

		if (LZO_E_OK != ret)
		{
			return false;
		}

		return true;
	}

	bool nlzo_wrap::decompress(const void* in, uint32 in_len, void* out, uint32& out_len)
	{
		lzo_uint dst_len = 0;
		int ret = lzo1x_decompress((const lzo_bytep)in, (lzo_uint)in_len, (lzo_bytep)out, (lzo_uintp)(&dst_len), NULL);
		out_len = (uint32)dst_len;

		if (LZO_E_OK != ret)
		{
			return false;
		}

		return true;
	}

	bool nlzo_wrap::compress_ts(const void* in, uint32 in_len, void* out, uint32& out_len)
	{
		lzo_align_t __LZO_MMODEL work_mem[((LZO1X_1_MEM_COMPRESS) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t)];
		lzo_uint dst_len = 0;
		int ret = lzo1x_1_compress((const lzo_bytep)in, (lzo_uint)in_len, (lzo_bytep)out, (lzo_uintp)(&dst_len), (lzo_voidp)work_mem);
		out_len = (uint32)dst_len;

		if (LZO_E_OK != ret)
		{
			return false;
		}

		return true;
	}

} // namespace nexus
