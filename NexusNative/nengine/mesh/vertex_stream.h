/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/


#ifndef _NEXUS_VERTEX_STREAM_H_
#define _NEXUS_VERTEX_STREAM_H_
#include "ncore.h"
#include "vertex_enum.h"

namespace nexus
{
	class nAPI vertex_stream
	{		

		unsigned char*	m_data;
		size_t			m_data_size;
		element_array	m_element_array;	// 一个vertex有几个elements
		size_t			m_vert_size;		// 一个vertex占用几个bytes

	private:// noncopyable
		vertex_stream(const vertex_stream&);
		vertex_stream& operator = (const vertex_stream&);
	public:
		typedef shared_ptr<vertex_stream> ptr;

		vertex_stream(void)
		{
			m_data = NULL;
			m_data_size = 0;
			m_vert_size = 0;
		}
		~vertex_stream(void)
		{
			destroy();
		}

		void do_serialize(narchive& ar);
		void destroy();
		void init(const element_array& elements, size_t num_vert);		

		unsigned char* get_data() const	{	return m_data;}
		size_t get_data_size() const	{	return m_data_size;}
		size_t get_num_vert() const		{	return m_data_size/m_vert_size;}
		size_t get_vertex_size() const	{	return m_vert_size;}

		const element_array& get_elemets_define() const	{	return m_element_array; }		

		template<typename T>
		T* get_element_ptr(size_t vert_index, size_t element_index)
		{
			nASSERT( vert_index < m_data_size/m_vert_size);
			nASSERT( element_index < m_element_array.size());
			nASSERT( sizeof(T) == get_element_size(m_element_array[element_index].type));

			unsigned char* vert_ptr = m_data+vert_index*m_vert_size;
			
			return (T*)(vert_ptr+m_element_array[element_index].offset);
		}

		template<typename T>
		void set_element(size_t vert_index, size_t element_index, const T& val)
		{
			T* ptr= get_element_ptr<T>(vert_index, element_index);

			*ptr = val;
		}
	};

	template<>
	inline narchive& nserialize(narchive& ar, vertex_stream& vs, const TCHAR* obj_name)
	{
		nstring class_name(_T("vertex_stream"));
		ar.object_begin(obj_name, class_name);
		vs.do_serialize(ar);
		ar.object_end();
		return ar;
	}
}//namespace nexus
#endif //_NEXUS_VERTEX_STREAM_H_