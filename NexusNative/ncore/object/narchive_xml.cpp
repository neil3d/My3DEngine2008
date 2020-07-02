#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "narchive_xml.h"
#include "nexception.h"
#include "../util/string_util.h"
#include "../util/base64.h"

namespace nexus
{
	narchive::ptr narchive::create_xml_writer()
	{
		return narchive::ptr(nNew narchive_xml_writer);
	}

	narchive::ptr narchive::create_xml_reader()
	{
		return narchive::ptr(nNew narchive_xml_reader);
	}

	const char* const XML_RootName = "nexus_archive_root";	
	const char* const ELEM_ClassName = "class_name";	
	const char* const ELEM_ArraySize = "array_size";
	const char* const ELEM_BlobSize = "blob_size";

	nDEFINE_CLASS(narchive_xml_writer, narchive)

	narchive_xml_writer::narchive_xml_writer(void)
	{
	}

	narchive_xml_writer::~narchive_xml_writer(void)
	{
	}

	void narchive_xml_writer::open(nfile::ptr file_ptr)
	{
		if( !(file_ptr->reflection_get_class()->is_kind_of(nstd_file::reflection_get_static_class())) )
			nthrow(_T("XML archive support nstd_file only."));

		m_std_file = dynamic_pointer_cast<nstd_file>(file_ptr);
		
		
		if( m_std_file->get_fp() == NULL )
			nthrow(_T("XML archive error: bad file pointer."));

		TiXmlDeclaration * decl = nNew TiXmlDeclaration( "1.0", "", "" );
		m_xml_doc.LinkEndChild(decl);

		TiXmlElement* root_elem = nNew TiXmlElement(XML_RootName);
		m_element_stack.push(root_elem);
	}

	void narchive_xml_writer::close()
	{
		nASSERT(m_element_stack.size() == 1); // root
		m_xml_doc.LinkEndChild(m_element_stack.top());
		m_element_stack.pop();

		m_xml_doc.SaveFile(m_std_file->get_fp());
		m_std_file.reset();
	}

	void narchive_xml_writer::array_begin(const TCHAR* array_name, size_t& array_size)
	{
		std::string name_str = conv_string(array_name);
		TiXmlElement* elem = nNew TiXmlElement(name_str.c_str());		
		elem->SetAttribute(ELEM_ArraySize, (boost::lexical_cast<std::string>(array_size)).c_str());

		TiXmlElement* father = m_element_stack.top();
		father->LinkEndChild(elem);
		m_element_stack.push(elem);
	}

	void narchive_xml_writer::object_begin(const TCHAR* object_name, nstring& class_name)
	{
		TiXmlElement* elem = nNew TiXmlElement(conv_string(object_name).c_str());
		elem->SetAttribute(ELEM_ClassName, conv_string(class_name).c_str());

		TiXmlElement* father = m_element_stack.top();
		father->LinkEndChild(elem);
		m_element_stack.push(elem);
	}

	template<typename T>
	void narchive_xml_writer::save_attribute(const TCHAR* att_name, T& val)
	{
		TiXmlElement* elem = nNew TiXmlElement(conv_string(att_name).c_str());

		TiXmlText* elem_data = nNew TiXmlText((boost::lexical_cast<std::string>(val)).c_str());		
		elem->LinkEndChild(elem_data);

		TiXmlElement* father = m_element_stack.top();
		father->LinkEndChild(elem);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, int& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, unsigned int& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, char& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, unsigned char& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, short& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, unsigned short& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, long& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, unsigned long& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, float& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, double& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, bool& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, nstring& val)
	{
		std::string _val = conv_string(val);
		save_attribute(att_name, _val);
	}

	void narchive_xml_writer::serial(const TCHAR* att_name, std::string& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_xml_writer::object_end()
	{
		m_element_stack.pop();
	}

	void narchive_xml_writer::array_end()
	{
		m_element_stack.pop();
	}

	size_t narchive_xml_writer::serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size)
	{
		std::string encoded = base64_encode((unsigned char*)buffer, buffer_size);
		save_attribute(att_name, encoded);
		return buffer_size;
	}

	//----------------------------------------------------------------------------------------
	narchive_xml_reader::narchive_xml_reader(void)
	{		
	}

	narchive_xml_reader::~narchive_xml_reader(void)
	{}

	void narchive_xml_reader::open(nfile::ptr file_ptr)
	{
		if( !(file_ptr->reflection_get_class()->is_kind_of(nstd_file::reflection_get_static_class())) )
			nthrow(_T("XML archive support nstd_file only."));

		m_std_file = dynamic_pointer_cast<nstd_file>(file_ptr);


		if( m_std_file->get_fp() == NULL )
			nthrow(_T("XML archive error: bad file pointer."));

		if( !m_xml_doc.LoadFile(m_std_file->get_fp()) )
		{
			nstring xml_error = conv_string(m_xml_doc.ErrorDesc());
			nthrow2(_T("XML Document"), xml_error.c_str());
		}

		TiXmlElement* root = m_xml_doc.FirstChildElement(XML_RootName);
		m_element_stack.push(root);
	}

	void narchive_xml_reader::close()
	{}

	void narchive_xml_reader::array_begin(const TCHAR* array_name, size_t& array_size)
	{
		TiXmlElement* father = m_element_stack.top();
		if(!father)
		{
			array_size = 0;
			return;
		}

		std::string name_str = conv_string(array_name);
		TiXmlElement* array_elem = father->FirstChildElement( name_str.c_str() );
		
		array_elem->QueryIntAttribute(ELEM_ArraySize, (int*)&array_size);

		m_element_stack.push(array_elem);
		
		TiXmlElement* array_sibling_elem = array_elem->FirstChildElement(conv_string(ELEM_ArrayItem).c_str());
		nASSERT(array_sibling_elem!=NULL);
		m_array_sibling_stack.push(array_sibling_elem);
	}


	void narchive_xml_reader::object_begin(const TCHAR* object_name, nstring& class_name)
	{
		TiXmlElement* father = m_element_stack.top();
		TiXmlElement* obj_elem = NULL;

		if(_tcscmp(object_name, ELEM_ArrayItem) == 0)
			obj_elem = m_array_sibling_stack.top();
		else
		{
			if(father)
				obj_elem = father->FirstChildElement( conv_string(object_name).c_str() );
		}

		//-- 允许这个节点不存在的情况,一般用于程序中新增了数据成员时,对已有文档读取的过程
		if(obj_elem)
			class_name = conv_string( obj_elem->Attribute(ELEM_ClassName) );
		else
			class_name = _T("NoneClass");
		m_element_stack.push(obj_elem);
	}

	template<typename T>
	void narchive_xml_reader::load_attribute(const TCHAR* att_name, T& val)
	{
		TiXmlElement* father = m_element_stack.top();
		TiXmlElement* att_elem = NULL;

		if(_tcscmp(att_name, ELEM_ArrayItem) == 0)
		{			
			nASSERT( !m_array_sibling_stack.empty() );
			att_elem = m_array_sibling_stack.top();
		}
		else
		{
			if(father)
				att_elem = father->FirstChildElement( conv_string(att_name).c_str() );
		}			

		TiXmlNode* data_node = NULL;
		if(att_elem)
			data_node = att_elem->FirstChild();

		if( data_node )
		{
			const char* data_str = data_node->Value();
			val = boost::lexical_cast<T>(data_str);
		}
		else
		{
			val = T();
		}
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, int& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, unsigned int& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, char& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, unsigned char& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, short& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, unsigned short& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, long& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, unsigned long& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, float& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, double& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, bool& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, nstring& val)
	{
		std::string _val;
		load_attribute(att_name, _val);
		val = conv_string(_val);
	}

	void narchive_xml_reader::serial(const TCHAR* att_name, std::string& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_xml_reader::object_end()
	{
		m_element_stack.pop();
	}

	void narchive_xml_reader::array_end()
	{
		m_element_stack.pop();		
		m_array_sibling_stack.pop();
	}

	void narchive_xml_reader::array_next()
	{
		nASSERT( !m_array_sibling_stack.empty() );
		TiXmlElement*& array_sibling_elem = m_array_sibling_stack.top();
		nASSERT(array_sibling_elem!=NULL);
		array_sibling_elem = array_sibling_elem->NextSiblingElement();		
	}

	size_t narchive_xml_reader::serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size)
	{
		std::string str_val;
		load_attribute(att_name, str_val);

		std::vector<char> decode_buf;
		base64_decode(str_val, decode_buf);

		if(buffer)
		{
			nASSERT(buffer_size >= decode_buf.size());
			memcpy(buffer, &decode_buf[0], decode_buf.size());
		}

		return decode_buf.size();;
	}
}//namespace nexus