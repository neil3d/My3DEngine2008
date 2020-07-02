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
	template<typename T>
	struct triangle_face
	{
		T index[3];
	};
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

		void append_face(const triangle_face<T>& face)
		{
			data.push_back(face.index[0]);
			data.push_back(face.index[1]);
			data.push_back(face.index[2]);
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

	typedef triangle_face<unsigned short> triangle_face16;
	typedef index_buffer<unsigned short> index_buffer16;
}

#endif //_NEXUS_MESH_INDEX_DATA_H_