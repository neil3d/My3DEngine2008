#include "StdAfx.h"
#include "nbit_map.h"
#include "../file/nfile_system.h"

namespace nexus
{
	const int US = 32;	//unit size in bits
	const unsigned int HEX1 = ((unsigned) 0x00000001);

	nbit_map::nbit_map(void):m_width(0), m_height(0),
		m_data(NULL), m_num_bytes(0)
	{
	}

	nbit_map::~nbit_map(void)
	{
		destroy();
	}

	void nbit_map::create(size_t w, size_t h, bool init_val)
	{
		destroy();

		//--
		int num_dwords = ((w*h)/US)+1;
		m_num_bytes = num_dwords*US/8;
		
		m_data = (unsigned int*)malloc(m_num_bytes);

		if( init_val )
			memset(m_data,0xFF,m_num_bytes);
		else
			memset(m_data,0,m_num_bytes);

		m_width=w;
		m_height=h;
	}

	void nbit_map::destroy()
	{
		if( m_data )
		{
			::free(m_data);
			m_data = NULL;
		}
		m_width = m_height = 0;
		m_num_bytes = 0;
	}

	bool nbit_map::get_value(size_t x, size_t y) const
	{
		nASSERT(x < m_width);
		nASSERT(y < m_height);
		
		int bit = m_width*y + x;		
		return (*(m_data+bit/US) & (HEX1 << (US-1-(bit % US))))!=0;
	}

	void nbit_map::set_value(size_t x, size_t y, bool val)
	{
		nASSERT(x < m_width);
		nASSERT(y < m_height);

		int bit = m_width*y + x;

		if(val)
			*(m_data+bit/US) |= (HEX1 << (US-1-(bit % US)));//set bit to 1
		else
			*(m_data+bit/US) &= (~(HEX1 << (US-1-(bit % US))));//set bit to 0
	}

	void nbit_map::clear()
	{
		if( m_data )
			memset(m_data,0,m_num_bytes);
	}

	void nbit_map::save_raw(nfile_system* fs, const nstring& pkg_name, const nstring& file_name)
	{
		//nASSERT( m_data );
		//需要快速测试“最近使用的文件”功能，所以该处屏蔽
		if(m_data==0) return;

		nfile::ptr fp = fs->open_file(pkg_name, file_name, EFileWrite);
		fp->write_buffer(m_data, m_num_bytes);
	}

	bool nbit_map::load_raw(nfile_system* fs, const nstring& pkg_name, const nstring& file_name)
	{
		if(!fs->file_exists(pkg_name,file_name))
		{
			return false;
		}

		nASSERT( m_data );

		nfile::ptr fp = fs->open_file(pkg_name, file_name, EFileRead);
		fp->read_buffer(m_data, m_num_bytes);
		
		return true;
	}

	shared_ptr<nbit_map> nbit_map::create_sub_map( size_t x, size_t y, size_t width, size_t height )
	{
		nbit_map::ptr ret;

		if( x < 0 )
			x = 0;
		if( y < 0 )
			y = 0;
		if(x+width > m_width)
			width = m_width-x;
		if(y+height > m_height)
			height = m_height-y;

		if (width <= 0
			|| height <= 0)
			return ret;

		ret.reset( new nbit_map );
		ret->create(width, height, 0);

		for (size_t j=0; j<height; j++)
		{
			for (size_t i=0; i<width; i++)
			{
				ret->set_value(i, j, this->get_value(x + i, y + j));
			}
		}

		return ret;
	}

	void nbit_map::blit( nbit_map* src_map, int x, int y )
	{
		if( src_map == NULL )
			return;
		if( m_data == NULL )
			return;

		size_t cpy_w = src_map->get_width();
		size_t cpy_h = src_map->get_height();

		if( x < 0 )
			x = 0;
		if( y < 0 )
			y = 0;
		if(x+cpy_w > m_width)
			cpy_w = m_width-x;
		if(y+cpy_h > m_height)
			cpy_h = m_height-y;

		if( cpy_w <= 0
			|| cpy_h <= 0)
			return;

		for (size_t j=0; j<cpy_h; j++)
		{
			for (size_t i=0; i<cpy_w; i++)
			{
				this->set_value(x + i,y + j, src_map->get_value(i,j));
			}
		}
	}
}//namespace nexus