#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "narchive_xml.h"
#include "nexception.h"
#include "../util/string_util.h"


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

	//----------------------------------------------------------------------------------------
	narchive_xml_reader::narchive_xml_reader(void)
	{
		m_array_sibling_elem = NULL;
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

		m_xml_doc.LoadFile(m_std_file->get_fp());

		TiXmlElement* root = m_xml_doc.FirstChildElement(XML_RootName);
		m_element_stack.push(root);
	}

	void narchive_xml_reader::close()
	{}

	void narchive_xml_reader::array_begin(const TCHAR* array_name, size_t& array_size)
	{
		TiXmlElement* father = m_element_stack.top();

		std::string name_str = conv_string(array_name);
		TiXmlElement* array_elem = father->FirstChildElement( name_str.c_str() );
		
		array_elem->QueryIntAttribute(ELEM_ArraySize, (int*)&array_size);

		m_element_stack.push(array_elem);
		
		m_array_sibling_elem = array_elem->FirstChildElement(conv_string(ELEM_ArrayItem).c_str());
	}


	void narchive_xml_reader::object_begin(const TCHAR* object_name, nstring& class_name)
	{
		TiXmlElement* father = m_element_stack.top();
		TiXmlElement* obj_elem = NULL;

		if(_tcscmp(object_name, ELEM_ArrayItem) == 0)
			obj_elem = m_array_sibling_elem;
		else
			obj_elem = father->FirstChildElement( conv_string(object_name).c_str() );

		class_name = conv_string( obj_elem->Attribute(ELEM_ClassName) );
		m_element_stack.push(obj_elem);
	}

	template<typename T>
	void narchive_xml_reader::load_attribute(const TCHAR* att_name, T& val)
	{
		TiXmlElement* father = m_element_stack.top();
		TiXmlElement* att_elem = NULL;

		if(_tcscmp(att_name, ELEM_ArrayItem) == 0)
		{			
			nASSERT( m_array_sibling_elem!=NULL );
			att_elem = m_array_sibling_elem;
		}
		else
		{
			att_elem = father->FirstChildElement( conv_string(att_name).c_str() );
		}			

		TiXmlNode* data_node = att_elem->FirstChild();

		const char* data_str = data_node->Value();
		val = boost::lexical_cast<T>(data_str);
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
		m_array_sibling_elem = NULL;
	}

	void narchive_xml_reader::array_next()
	{
		nASSERT( m_array_sibling_elem!=NULL );
		m_array_sibling_elem = m_array_sibling_elem->NextSiblingElement();
	}
}//namespace nexus