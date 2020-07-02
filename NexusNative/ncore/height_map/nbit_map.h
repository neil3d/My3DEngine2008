/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_CORE_BIT_MAP_H_
#define _NEXUS_CORE_BIT_MAP_H_

namespace nexus
{
	class nfile_system;

	/**
	 *	类似于Height map, 但是每个"像素"只占用一个bit
	*/
	class nCoreAPI nbit_map
	{
		DISALLOW_COPY_AND_ASSIGN(nbit_map)
	public:
		typedef shared_ptr<nbit_map> ptr;

		nbit_map(void);
		~nbit_map(void);

		void create(size_t w, size_t h, bool init_val);
		void destroy();

		bool get_value(size_t x, size_t y) const;
		void set_value(size_t x, size_t y, bool val);

		size_t get_width() const { return m_width; }
		size_t get_height() const { return m_height; }

		shared_ptr<nbit_map> create_sub_map(size_t x, size_t y, size_t width, size_t height);

		void blit(nbit_map* src_map, int x, int y);

		/**	内部数据全部清0	*/
		void clear();

		void save_raw(nfile_system* fs, const nstring& pkg_name, const nstring& file_name);
		// 在调用load_raw函数之前需要先调用create创建好bitmap
		bool load_raw(nfile_system* fs, const nstring& pkg_name, const nstring& file_name);

	private:
		size_t			m_width,
						m_height;
		unsigned int	*m_data;	// 使用4字节指针,可以提高寻址速度
		int				m_num_bytes;	// buffer size
	};
}//namespace nexus

#endif //_NEXUS_CORE_BIT_MAP_H_