#include "StdAfx.h"
#include "vertex_stream.h"

namespace nexus
{
	void vertex_stream::destroy()
	{
		if(m_data)
		{
			nFree(m_data);
			m_data = NULL;
			m_data_size = 0;
		}
		m_element_array.clear();
	}

	void vertex_stream::init(const element_array& elements, size_t num_vert)
	{
		destroy();

		m_element_array = elements;

		//-- 计算一个vertex有几个bytes
		m_vert_size = 0;
		for(size_t i=0; i<m_element_array.size(); i++)
		{
			vertex_element_define& elem = m_element_array[i];
			elem.offset = (unsigned short)m_vert_size;
			m_vert_size += get_element_size(elem.type);
		}

		//--
		m_data_size = num_vert*m_vert_size;
		nASSERT(m_data_size > 0);
		m_data = (unsigned char*)nMalloc(m_data_size);
	}

	void vertex_stream::do_serialize(narchive& ar)
	{
		if(ar.is_loading())
		{
			size_t s = 0;
			s = ar.serial_blob(_T("m_data"), NULL, 0);
			m_data = (unsigned char*)nMalloc(s);

			ar.serial_blob(_T("m_data"), m_data, s);
		}
		else
		{
			ar.serial_blob(_T("m_data"), m_data, get_data_size());
		}
		nSERIALIZE(ar, m_data_size);
		nSERIALIZE(ar, m_element_array);
		nSERIALIZE(ar, m_vert_size);
	}
}//namespace nexus