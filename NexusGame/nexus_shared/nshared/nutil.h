/**
*	nexus nshared - nutil
*
*	Copyright (C) 2010 NexusDev
*
*	Author: D. Zhao
*	Date:	May, 2010
*/

#ifndef _NSHARED_NUTIL_H_
#define _NSHARED_NUTIL_H_

#include "ncommon.h"
#include "nmutex.h"
#include "MersenneTwister.h"
#include "nsingleton.h"

#include <boost/crc.hpp>

namespace nexus {

	// fast int abs
	static inline int int32abs(const int value)
	{
		return (value ^ (value >> 31)) - (value >> 31);
	}

	// fast int abs and recast to unsigned
	static inline uint32 int32abs2uint32(const int value)
	{
		return (uint32)(value ^ (value >> 31)) - (value >> 31);
	}

	// fastest method of float2int32
	static inline int float2int32(const float value)
	{
		union
		{
			int as_int[2];
			double as_double;
		} n;

		n.as_double = value + 6755399441055744.0;
		return n.as_int[0];
	}

	// fastest method of long2int32
	static inline int long2int32(const double value)
	{
		union
		{
			int as_int[2];
			double as_double;
		} n;

		n.as_double = value + 6755399441055744.0;
		return n.as_int[0];
	}


	inline void ntolower(tstring& str)
	{
		for (size_t i = 0; i < str.length(); ++i)
		{
			str[i] = (nchar)_totlower(str[i]);
		}
	}

	inline void ntoupper(tstring& str)
	{
		for (size_t i = 0; i < str.length(); ++i)
		{
			str[i] = (nchar)_totupper(str[i]);
		}
	}

	const uint32 MAX_NUMBER_OF_GENERATORS = 20;

	class nutil : private nnoncopyable
	{
	public:
		nutil();
		~nutil();

		bool init();
		void destroy();

		uint32 get_cpu_num();

		/* Return a random number in the range min..max; (max-min) must be smaller than 32768. */
		int32 irand(int32 min, int32 max);

		/* Return a random number in the range min..max (inclusive). For reliable results, the difference
		* between max and min should be less than RAND32_MAX. */
		uint32 urand(uint32 min, uint32 max);

		/* Return a random number in the range 0 .. RAND32_MAX. */
		int32 rand32();

		/* Return a random double from 0.0 to 1.0 (exclusive). Floats support only 7 valid decimal digits.
		* A double supports up to 15 valid decimal digits and is used internally (RAND32_MAX has 10 digits).
		* With an FPU, there is usually no difference in performance between float and double. */
		double rand_norm();

		/* Return a random double from 0.0 to 99.9999999999999. Floats support only 7 valid decimal digits.
		* A double supports up to 15 valid decimal digits and is used internaly (RAND32_MAX has 10 digits).
		* With an FPU, there is usually no difference in performance between float and double. */
		double rand_chance(void);

		/* Return true if a random roll fits in the specified chance (range 0-100). */
		inline bool roll_chance_f(float chance)
		{
			return chance > rand_chance();
		}

		/* Return true if a random roll fits in the specified chance (range 0-100). */
		inline bool roll_chance_i(int chance)
		{
			return chance > irand(0, 99);
		}

		uint32 crc32(const void* data_ptr, uint32 size)
		{
			boost::crc_32_type  crc_result;
			crc_result.process_bytes(data_ptr, size);
			return (uint32)crc_result.checksum();
		}

	private:
		uint32 _generate_seed();

	private:
		nmutex*				m_mutexs[MAX_NUMBER_OF_GENERATORS];
		CRandomMersenne*	m_generators[MAX_NUMBER_OF_GENERATORS];
		uint32				m_max_generators;
		uint32				m_counter;
	};

	#define s_util	nsingleton<nutil>::instance()

} // namespace nexus

#endif _// NSHARED_NUTIL_H_
