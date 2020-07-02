/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_BIT_MAP_H_
#define _NEXUS_BIT_MAP_H_

namespace nexus
{
	/**
	 *	类似于Height map, 但是每个"像素"只占用一个bit
	*/
	class nbit_map
	{
	public:
		nbit_map(void);
		~nbit_map(void);

		void create(size_t w, size_t h, bool init_val);
		void destroy();

		bool get_value(size_t x, size_t y) const;
		void set_value(size_t x, size_t y, bool val);

		/**	内部数据全部清0	*/
		void clear();

	private:
		struct	impl;
		impl*	m_impl;
	};
}//namespace nexus

#endif //_NEXUS_BIT_MAP_H_