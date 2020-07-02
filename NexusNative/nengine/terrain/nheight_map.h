/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_HEIGHT_MAP_H_
#define _NEXUS_HEIGHT_MAP_H_
#include "ncore.h"
#include <limits>

namespace nexus
{
	/**
	 * 高度图模板类
	 * @remarks 可以支持8bit和16bit无符号两种内部数据
	*/
	template<typename T>
	class nheight_map
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
			memset(m_data, init_val, data_size);
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
			set_value(x, y, val);
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