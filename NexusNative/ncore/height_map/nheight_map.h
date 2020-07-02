/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_CORE_HEIGHT_MAP_H_
#define _NEXUS_CORE_HEIGHT_MAP_H_
#include "../ncore.h"
#include <limits>

#ifdef min
#	undef min
#endif

#ifdef max
#	undef max
#endif

namespace nexus
{
	/**
	 * 高度图模板类
	 * @remarks 可以支持8bit和16bit无符号两种内部数据
	*/
	template<typename T>
	class nheight_map : boost::noncopyable
	{
	public:
		typedef T data_type;
		typedef shared_ptr<nheight_map> ptr;

		nheight_map(void)	
		{
			m_data = NULL;
			m_width = m_height = 0;
		}
		~nheight_map(void)	
		{
			destroy();
		}

		void create(int w, int h, T init_val)
		{
			nASSERT( w>0 && h>0);
			//-- free old data
			destroy();

			//-- create new data
			m_width = w;
			m_height = h;

			int data_size = sizeof(T)*w*h;
			m_data = (T*)nMalloc(data_size);
			for(int i=0; i<w*h; i++)
				m_data[i] = init_val;			
		}
		
		void clear(T val)
		{
			for(int i=0; i<m_width*m_height; i++)
				m_data[i] = val;
		}

		void destroy()
		{
			if( m_data )
			{
				nFree( m_data );
				m_data = NULL;
			}

			m_width = m_height = 0;
		}

		shared_ptr<nheight_map> create_sub_map(int x, int y, int width, int height)
		{
			nheight_map::ptr ret;

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
			
			ret.reset( new nheight_map );
			ret->create(width, height, T());
			data_type* dst = ret->get_raw_data();
			data_type* src = get_raw_data();			

			src += y*m_width;
			for (int j=0; j<height; j++)
			{
				memcpy(dst, src+x, sizeof(data_type)*width);
				dst += width;
				src += m_width;
			}

			return ret;
		}

		void blit(nheight_map* src_map, int x, int y)
		{
			if( src_map == NULL )
				return;
			if( m_data == NULL )
				return;

			int cpy_w = src_map->get_width();
			int cpy_h = src_map->get_height();

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

			data_type* dst = get_raw_data();
			data_type* src = src_map->get_raw_data();

			dst += y*m_width;
			for(int j=0; j<cpy_h; j++)
			{
				memcpy(dst+x, src, sizeof(data_type)*cpy_w);
				dst += m_width;
				src += src_map->get_width();
			}
		}

		int get_data_index(int x, int y) const
		{
			return m_width*y + x;
		}

		data_type get_value(int x, int y) const
		{
			nASSERT( x>=0 && x< m_width );
			nASSERT( y>=0 && y< m_height );

			return m_data[ get_data_index(x,y) ];
		}

		data_type get_value_safe(int x, int y) const
		{
			if( x<0 )
				x = 0;
			if( y<0 )
				y = 0;
			if( x >= m_width )
				x = m_width-1;
			if( y >= m_height )
				y = m_height-1;

			return m_data[ get_data_index(x,y) ];
		}

		void set_value(int x, int y, data_type val)
		{
			nASSERT( x>=0 && x< m_width );
			nASSERT( y>=0 && y< m_height );

			m_data[ get_data_index(x,y) ] = val;
		}

		void checked_add(int x, int y, int add_val)
		{
			if( !is_valid_index(x,y) )
				return;
			
			T& target = m_data[get_data_index(x,y)];
			long val = target;
			val += add_val;

			if( val < std::numeric_limits<T>::min() )
				val = std::numeric_limits<T>::min();
			if( val > std::numeric_limits<T>::max() )
				val = std::numeric_limits<T>::max();
			
			target = val;
		}

		void checked_set(int x, int y, long val)
		{
			if( val < std::numeric_limits<T>::min() )
				val = std::numeric_limits<T>::min();
			if( val > std::numeric_limits<T>::max() )
				val = std::numeric_limits<T>::max();
			set_value(x, y, (T)val);
		}

		int get_width() const	{	return m_width; }
		int get_height() const	{	return m_height; }
		bool is_valid() const	{	return m_data!=NULL; }
		bool is_valid_index(int x, int y) const
		{
			return x>=0 && y>=0 &&
				x<m_width && y<m_height;
		}

		size_t get_data_size() const	{	return m_width*m_height*sizeof(data_type); }
		data_type* get_raw_data() const	{	return m_data;}

	private:
		data_type*	m_data;
		int			m_width,
					m_height;
	};

	typedef nheight_map<unsigned short>	nheight_map16;
	typedef nheight_map<unsigned char>	nalpha_map;
}//namespace nexus

#endif