/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_MESH_INDEX_DATA_H_
#define _NEXUS_MESH_INDEX_DATA_H_

namespace nexus
{
	/**
	 *	引擎所使用的“内存中的index数据”
	*/
	template<typename T>
	struct index_buffer
	{
		typedef T element_type;

		vector<element_type>	data;

		void append_index(T i)
		{
			data.push_back(i);
		}

		const void* get_data_ptr() const 
		{
			return &(*data.begin());
		}

		size_t get_data_size() const
		{
			return data.size()*sizeof(element_type);
		}

		size_t get_index_count() const
		{
			return data.size();
		}
	};

	typedef index_buffer<unsigned short> index_buffer16;
}

#endif //_NEXUS_MESH_INDEX_DATA_H_