#include "StdAfx.h"
#include "nhit_proxy_hash.h"

namespace nexus
{
	nhit_proxy_hash::nhit_proxy_hash(void) : 
		m_width(0), m_height(0), m_pitch(0)
	{
	}

	nhit_proxy_hash::~nhit_proxy_hash(void)
	{
	}

	void nhit_proxy_hash::alloc_buffer(int width, int height, int pitch)
	{
		nASSERT(pitch >= width);
		int w = pitch/4;
		m_buffer.resize(w*height);

		m_width = width;
		m_height = height;
		m_pitch = pitch;
	}

	hit_id nhit_proxy_hash::get_hit_id(int x, int y)
	{
		if( x < 0 )
			x = 0;
		if( y < 0 )
			y = 0;
		if( x >= m_width )
			x = m_width-1;
		if( y >= m_height )
			y = m_height-1;

		int w = m_pitch/4;
		int index = y*w+x;
		if( index < (int)m_buffer.size() )
			return m_buffer[index];
		else
			return (hit_id)-1;
	}
}//namespace nexus