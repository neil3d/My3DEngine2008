#include "nutil.h"

namespace nexus {

	nutil::nutil() : m_max_generators(0), m_counter(0)
	{
		memset(m_generators, 0, sizeof(m_generators));
		memset(m_mutexs, 0, sizeof(m_mutexs));
	}

	nutil::~nutil()
	{
		destroy();
	}

	bool nutil::init()
	{
		srand(::timeGetTime());
		m_max_generators =  get_cpu_num() * 2 + 2;

		if (m_max_generators > MAX_NUMBER_OF_GENERATORS)
		{
			m_max_generators = MAX_NUMBER_OF_GENERATORS;
		}

		for (uint32 i = 0; i < m_max_generators; ++i)
		{
			m_mutexs[i] = new nmutex;

			if (!m_mutexs[i])
			{
				return false;
			}

			m_generators[i] = new CRandomMersenne(_generate_seed());

			if (!m_generators[i])
			{
				return false;
			}
		}

		return true;
	}

	uint32 nutil::_generate_seed()
	{
		uint32 mstime = ::timeGetTime();
		uint32 stime = (uint32)time(NULL);
		uint32 rnd[2];
		rnd[0] = rand()*rand()*rand();
		rnd[1] = rand()*rand()*rand();

		uint32 val = mstime ^ rnd[0];
		val += stime ^ rnd[1];
		return val;
	}

	void nutil::destroy()
	{
		for (uint32 i = 0; i < m_max_generators; ++i)
		{
			if (m_mutexs[i])
			{
				delete m_mutexs[i];
				m_mutexs[i] = NULL;
			}

			if (m_generators[i])
			{
				delete m_generators[i];
				m_generators[i] = NULL;
			}
		}
	}

	uint32 nutil::get_cpu_num()
	{
		SYSTEM_INFO sys_info;
		::GetSystemInfo(&sys_info);
		return sys_info.dwNumberOfProcessors;
	}


	int32 nutil::irand(int32 min, int32 max)
	{
		uint32 ret = 0;
		uint32 index = 0;

		for (;;)
		{
			index = m_counter % m_max_generators;

			if (m_mutexs[index]->try_lock())
			{
				ret = m_generators[index]->IRandom(min, max);
				m_mutexs[index]->unlock();
				return ret;
			}

			++m_counter;
		}
	}

	uint32 nutil::urand(uint32 min, uint32 max)
	{
		return (uint32)irand(int32(min), int32(max));
	}

	int32 nutil::rand32()
	{
		return irand(0, RAND_MAX);
	}

	double nutil::rand_norm()
	{
		double ret = 0;
		uint32 index = 0;

		for (;;)
		{
			index = m_counter % m_max_generators;

			if (m_mutexs[index]->try_lock())
			{
				ret = m_generators[index]->Random();
				m_mutexs[index]->unlock();
				return ret;
			}

			++m_counter;
		}
	}

	double nutil::rand_chance(void)
	{
		return rand_norm() * 100.0;
	}

} // namespace nexus
