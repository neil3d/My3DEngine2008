#include "StdAfx.h"
#include "nbit_map.h"
#include <boost/dynamic_bitset.hpp>

namespace nexus
{
	struct nbit_map::impl
	{
		size_t	m_width,
				m_height;

		boost::dynamic_bitset<>	m_data;
	};

	nbit_map::nbit_map(void)
		: m_impl(nNew impl)
	{
	}

	nbit_map::~nbit_map(void)
	{
		delete m_impl;
	}

	void nbit_map::create(size_t w, size_t h, bool init_val)
	{
		m_impl->m_width = w;
		m_impl->m_height = h;

		m_impl->m_data.resize(w*h, init_val);
	}

	void nbit_map::destroy()
	{
		m_impl->m_data.reset();
	}

	bool nbit_map::get_value(size_t x, size_t y) const
	{
		nASSERT(x < m_impl->m_width);
		nASSERT(y < m_impl->m_height);
		
		return m_impl->m_data[y*m_impl->m_width + x];
	}

	void nbit_map::set_value(size_t x, size_t y, bool val)
	{
		nASSERT(x < m_impl->m_width);
		nASSERT(y < m_impl->m_height);

		m_impl->m_data[y*m_impl->m_width + x] = val;
	}

	void nbit_map::clear()
	{
		m_impl->m_data.clear();
		m_impl->m_data.resize(m_impl->m_width *m_impl->m_height, false);
	}

}//namespace nexus