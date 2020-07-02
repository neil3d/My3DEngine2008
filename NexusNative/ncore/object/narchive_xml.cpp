#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "narchive_xml.h"
#include "nexception.h"
#include "../util/string_util.h"
#include "../util/base64.h"
#include "../rapidxml/rapidxml_print.hpp"

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

	narchive::ptr narchive::create_rapidxml_writer()
	{
		return narchive::ptr(nNew narchive_rapidxml_writer);
	}

	narchive::ptr narchive::create_rapidxml_reader()
	{
		return narchive::ptr(nNew narchive_rapidxml_reader);
	}

	narchive::ptr narchive::create_memory_xml_writer(nstring& buffer)
	{
		narchive_memory_xml_writer* _ar=nNew narchive_memory_xml_writer();
		_ar->open(buffer);
		narchive::ptr ar(_ar);
		return ar;
	}

	narchive::ptr narchive::create_memory_xml_reader(nstring& buffer)
	{
		narchive_memory_xml_reader* _ar=nNew narchive_memory_xml_reader();
		_ar->open(buffer);
		narchive::ptr ar(_ar);
		return ar;
	}

	narchive::ptr narchive::open_xml_archive(nfile_system* fs, enum EFileMode mode, const nstring& pkg, const nstring& filename)
	{
		nfile::ptr fp = fs->open_file(pkg, filename, mode);
		narchive::ptr ap;
		if (mode == EFileRead)
			ap = create_rapidxml_reader();		
		else if (mode == EFileWrite)
			ap = create_rapidxml_writer();
		else
		{
			nASSERT(0 && "bad file io mode");
		}

		ap->open(fp);
		return ap;
	}
	

	const char* const XML_RootName = "nexus_archive_root";	
	const char* const ELEM_ClassName = "class_name";	
	const char* const ELEM_ArraySize = "array_size";
	const char* const ELEM_BlobSize = "blob_size";

	const TCHAR* const T_XML_RootName = _T("nexus_archive_root");	
	const TCHAR* const T_ELEM_ClassName = _T("class_name");	
	const TCHAR* const T_ELEM_ArraySize = _T("array_size");
	const TCHAR* const T_ELEM_BlobSize = _T("blob_size");

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

		m_std_file->seek(0,EFileBegin);
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

	void narchive_xml_writer::object_description( const TCHAR* desc_name,nstring& description )
	{
		TiXmlElement* father = m_element_stack.top();
		if(father)
		{
			const char* elem_name = conv_string(desc_name).c_str();
			TiXmlElement* elem = nNew TiXmlElement(elem_name);
			elem->SetAttribute(elem_name, conv_string(description).c_str());

			father->LinkEndChild(elem);
		}
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
	{
	}

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
		nASSERT(array_elem);
		
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

	void narchive_xml_reader::object_description( const TCHAR* desc_name,nstring& description )
	{
		TiXmlElement* obj_elem = m_element_stack.top();
		description = obj_elem ? conv_string(obj_elem->Attribute(conv_string(desc_name).c_str())) : _T("NoneDesc");
	}
	//===============================================================================
	nDEFINE_CLASS(narchive_rapidxml_writer, narchive)

	narchive_rapidxml_writer::narchive_rapidxml_writer(void)
	{
	}

	narchive_rapidxml_writer::~narchive_rapidxml_writer(void)
	{
	}

	void narchive_rapidxml_writer::open(nfile::ptr file_ptr)
	{
		m_file=file_ptr;
		try
		{			
			m_xml_doc.clear();
			rapid_xml_node *root = m_xml_doc.allocate_node(rapidxml::node_element,T_XML_RootName);
			m_xml_doc.append_node(root);
			m_element_stack.push(root);
		}
		catch(...)
		{
			nthrow(_T("XML archive error: parse xml data failed."));
		}
	}

	void narchive_rapidxml_writer::close()
	{
		//保存到文件
		TCHAR ch=0xFEFF;	//unicode文件编码标志
		std::wstring s;
		s+=ch;s+=_T("<?xml version=\"1.0\" encoding=\"utf-16\"?>\n");
		rapidxml::print(std::back_inserter(s), m_xml_doc,0);
		m_file->write_buffer(const_cast<TCHAR*>(s.c_str()),sizeof(TCHAR)*(s.length()));
		m_file.reset();
	}

	void narchive_rapidxml_writer::array_begin(const TCHAR* array_name, size_t& array_size)
	{
		TCHAR* _array_name=m_xml_doc.allocate_string(array_name);
		rapid_xml_node *node = m_xml_doc.allocate_node(rapidxml::node_element,_array_name);
		rapid_xml_node* parent;
		if(m_element_stack.empty())
		{
			parent=&m_xml_doc;
		}
		else
		{
			parent=m_element_stack.top();
		}
		parent->append_node(node);

		TCHAR* _array_size=m_xml_doc.allocate_string((boost::lexical_cast<std::wstring>(array_size)).c_str());
		rapid_xml_attribute *attr = m_xml_doc.allocate_attribute(T_ELEM_ArraySize, _array_size);
		node->append_attribute(attr);

		m_element_stack.push(node);
	}

	void narchive_rapidxml_writer::object_begin(const TCHAR* object_name, nstring& class_name)
	{
		TCHAR* _object_name=m_xml_doc.allocate_string(object_name);
		TCHAR* _class_name=m_xml_doc.allocate_string(class_name.c_str());
		rapid_xml_node *node = m_xml_doc.allocate_node(rapidxml::node_element,_object_name);
		rapid_xml_node* parent;
		if(m_element_stack.empty())
		{
			parent=&m_xml_doc;
		}
		else
		{
			parent=m_element_stack.top();
		}
		parent->append_node(node);

		rapid_xml_attribute *attr = m_xml_doc.allocate_attribute(T_ELEM_ClassName, _class_name);
		node->append_attribute(attr);

		m_element_stack.push(node);
	}


	void narchive_rapidxml_writer::object_description( const TCHAR* desc_name, nstring& description )
	{
		rapid_xml_node *parent=m_element_stack.top();
		if(parent)
		{
			TCHAR* _description=m_xml_doc.allocate_string(description.c_str());
			rapid_xml_attribute *attr = m_xml_doc.allocate_attribute(desc_name, _description);
			parent->append_attribute(attr);
		}
	}

	template<typename T>
	void narchive_rapidxml_writer::save_attribute(const TCHAR* att_name, T& val)
	{
		TCHAR* _val=m_xml_doc.allocate_string((boost::lexical_cast<std::wstring>(val)).c_str());
		rapid_xml_node *node = m_xml_doc.allocate_node(rapidxml::node_element,att_name, _val);
		rapid_xml_node *parent=m_element_stack.top();
		parent->append_node(node);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, int& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, unsigned int& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, char& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, unsigned char& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, short& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, unsigned short& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, long& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, unsigned long& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, float& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, double& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, bool& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, nstring& val)
	{
		save_attribute(att_name, val);
	}

	void narchive_rapidxml_writer::serial(const TCHAR* att_name, std::string& val)
	{
		std::wstring _val = conv_string(val);
		save_attribute(att_name, _val);
	}

	void narchive_rapidxml_writer::object_end()
	{
		m_element_stack.pop();
	}

	void narchive_rapidxml_writer::array_end()
	{
		m_element_stack.pop();
	}

	size_t narchive_rapidxml_writer::serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size)
	{
		std::wstring encoded = base64_encode_unicode((unsigned char*)buffer, buffer_size);
		save_attribute(att_name, encoded);
		return buffer_size;
	}

	//-------------------------------------------------------------------------------
	
	narchive_rapidxml_reader::narchive_rapidxml_reader(void)
	{
		m_xml_buffer=0;
		m_buffer_size=0;
		//m_in_array = false;
	}

	narchive_rapidxml_reader::~narchive_rapidxml_reader(void)
	{
		if(m_xml_buffer!=NULL)
		{
			delete [] m_xml_buffer;
			m_xml_buffer=NULL;
		}
	}

	void narchive_rapidxml_reader::open(nfile::ptr file_ptr)
	{
		//m_in_array = false;
		m_file=file_ptr;
		m_buffer_size=m_file->get_file_size();

		m_buffer_size/=sizeof(TCHAR);	//计算字符数量
		if(m_xml_buffer!=NULL)
		{
			delete [] m_xml_buffer;
			m_xml_buffer=NULL;
		}

		m_xml_buffer=new TCHAR[m_buffer_size+1];
		
		m_file->seek(0,EFileBegin);

		if( m_xml_buffer == NULL )
			nthrow(_T("XML archive error: alloc buffer failed."));

		m_file->read_buffer(m_xml_buffer,sizeof(TCHAR)*m_buffer_size);

		m_xml_buffer[m_buffer_size]='\0';	
		
		try
		{
			if(m_xml_buffer[0]==0xFEFF)
			{
				//+1是为了跳过unicode文件的编码标志0xFEFF
				m_xml_doc.parse<0>(m_xml_buffer+1);
			}
			else
			{
				m_xml_doc.parse<0>(m_xml_buffer);
			}
			
		}
		catch(rapidxml::parse_error ex)
		{
			delete [] m_xml_buffer; m_xml_buffer = NULL;
			nthrow( conv_string(ex.what()).c_str() );
		}
		catch(...)
		{
			delete [] m_xml_buffer; m_xml_buffer = NULL;
			nthrow(_T("XML archive error: parse xml data failed."));
		}

		rapid_xml_node *root = m_xml_doc.first_node(T_XML_RootName);
		if(root)
		{
			m_element_stack.push(root);
		}
		else
		{
			nthrow(_T("XML archive error: can not find xml data root."));
		}
	}

	void narchive_rapidxml_reader::close()
	{
		while(!m_element_stack.empty())
		{
			m_element_stack.pop();
		}
		while(!m_array_sibling_stack.empty())
		{
			m_array_sibling_stack.pop();
		}
		m_xml_doc.clear();
		if(m_xml_buffer!=NULL)
		{
			delete [] m_xml_buffer;
			m_xml_buffer=NULL;
		}
	}

	void narchive_rapidxml_reader::array_begin(const TCHAR* array_name, size_t& array_size)
	{
		rapid_xml_node* father = m_element_stack.top();
		rapid_xml_node* array_elem = father ? father->first_node( array_name ) : NULL;

		rapid_xml_attribute* attr= array_elem ? array_elem->first_attribute(T_ELEM_ArraySize) : NULL;
		if(attr)
		{
			array_size=boost::lexical_cast<size_t>(attr->value());
		}
		else
		{
			 array_size=0;
		}

		//m_in_array = true;
		m_element_stack.push(array_elem);
		rapid_xml_node* array_child_elem = array_elem ? array_elem->first_node(ELEM_ArrayItem) : NULL;
		m_array_sibling_stack.push(array_child_elem);
	}


	void narchive_rapidxml_reader::object_begin(const TCHAR* object_name, nstring& class_name)
	{
		rapid_xml_node* father = m_element_stack.top();
		rapid_xml_node* obj_elem = NULL;

		if(_tcscmp(object_name, ELEM_ArrayItem) == 0)
			obj_elem = m_array_sibling_stack.top();
		else
		{
			if(father)
				obj_elem = father->first_node( object_name);
		}		

		//-- 允许这个节点不存在的情况,一般用于程序中新增了数据成员时,对已有文档读取的过程
		if(obj_elem)
		{
			rapid_xml_attribute* attr = obj_elem->first_attribute(T_ELEM_ClassName);
			if(attr)
			{
				class_name=attr->value();
			}
			else
			{
				class_name=_T("NoneClass");
			}
		}
		else
			class_name = _T("NoneClass");
		m_element_stack.push(obj_elem);
	}


	void narchive_rapidxml_reader::object_description( const TCHAR* desc_name,nstring& description )
	{
		rapid_xml_node* obj_elem = m_element_stack.top();
		rapid_xml_attribute* attr = obj_elem ? obj_elem->first_attribute(desc_name) : NULL;
		description = attr ? attr->value() : _T("NoneDesc");
	}

	template<typename T>
	void narchive_rapidxml_reader::load_attribute(const TCHAR* att_name, T& val)
	{
		rapid_xml_node* father = m_element_stack.top();
		rapid_xml_node* att_elem = NULL;

		if(_tcscmp(att_name, ELEM_ArrayItem) == 0)
		{			
			nASSERT( !m_array_sibling_stack.empty() );
			att_elem = m_array_sibling_stack.top();
		}
		else
		{
			if(father)
				att_elem = father->first_node( att_name );
		}			

		if(att_elem)
		{
			const TCHAR* str=att_elem->value();
			val = boost::lexical_cast<T>(str);
		}
		else
		{
			val = T();
		}
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, int& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, unsigned int& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, char& val)
	{
		int _val=val;
		load_attribute(att_name, _val);
		val=static_cast<char>(_val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, unsigned char& val)
	{
		unsigned int _val=val;
		load_attribute(att_name, _val);
		val=static_cast<unsigned char>(_val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, short& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, unsigned short& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, long& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, unsigned long& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, float& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, double& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, bool& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, nstring& val)
	{
		load_attribute(att_name, val);
	}

	void narchive_rapidxml_reader::serial(const TCHAR* att_name, std::string& val)
	{
		nstring _val;
		load_attribute(att_name, _val);
		val=conv_string(_val);
	}

	void narchive_rapidxml_reader::object_end()
	{
		m_element_stack.pop();
	}

	void narchive_rapidxml_reader::array_end()
	{
		m_element_stack.pop();		
		m_array_sibling_stack.pop();
		// NOTO(zewu):m_in_array意在解决读取文件时容错的问题，实际上如果array里面的数据如果还有array
		// 那么递归时会导致sibling node没有出栈,解决方案可以是在array_begin调用的push(NULL)
		//if(m_in_array)
		//{
		//	m_element_stack.pop();		
		//	m_array_sibling_stack.pop();
		//}
		//m_in_array = false;
	}

	void narchive_rapidxml_reader::array_next()
	{
		if(m_array_sibling_stack.empty()) return;
		rapid_xml_node*& array_sibling_elem = m_array_sibling_stack.top();
		if(array_sibling_elem!=NULL)
		{
			array_sibling_elem = array_sibling_elem->next_sibling();
		}		
	}

	size_t narchive_rapidxml_reader::serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size)
	{
		std::wstring _str_val;
		load_attribute(att_name, _str_val);
		std::vector<char> decode_buf;
		base64_decode_unicode(_str_val, decode_buf);

		if(buffer)
		{
			nASSERT(buffer_size >= decode_buf.size());
			memcpy(buffer, &decode_buf[0], decode_buf.size());
		}

		return decode_buf.size();
	}

	//===================================================================================
	void narchive_memory_xml_writer::open( nstring& buffer )
	{
		m_buffer=&buffer;
		m_file.reset();
		try
		{			
			m_xml_doc.clear();
			rapid_xml_node *root = m_xml_doc.allocate_node(rapidxml::node_element,T_XML_RootName);
			m_xml_doc.append_node(root);
			m_element_stack.push(root);
		}
		catch(...)
		{
			nthrow(_T("XML archive error: parse xml data failed."));
		}
	}

	void narchive_memory_xml_writer::close()
	{
		//保存到字符串
		*m_buffer=_T("<?xml version=\"1.0\" encoding=\"utf-16\"?>\n");
		rapidxml::print(std::back_inserter(*m_buffer), m_xml_doc,0);
	}

	//===================================================================================
	void narchive_memory_xml_reader::open( nstring& buffer )
	{
		m_file.reset();

		m_buffer_size=buffer.length();

		if(m_xml_buffer!=NULL)
		{
			delete [] m_xml_buffer;
			m_xml_buffer=NULL;
		}

		m_xml_buffer=new TCHAR[m_buffer_size+1];
		if( m_xml_buffer == NULL )
			nthrow(_T("XML archive error: alloc buffer failed."));

		wcscpy_s(m_xml_buffer,m_buffer_size+1,buffer.c_str());

		try
		{
			m_xml_doc.parse<0>(m_xml_buffer);
		}
		catch(...)
		{
			nthrow(_T("XML archive error: parse xml data failed."));
		}

		rapid_xml_node *root = m_xml_doc.first_node(T_XML_RootName);
		if(root)
		{
			m_element_stack.push(root);
		}
		else
		{
			nthrow(_T("XML archive error: can not find xml data root."));
		}
	}

	void narchive_memory_xml_reader::close()
	{
		while(!m_element_stack.empty())
		{
			m_element_stack.pop();
		}
		while(!m_array_sibling_stack.empty())
		{
			m_array_sibling_stack.pop();
		}
		m_xml_doc.clear();

		if(m_xml_buffer!=NULL)
		{
			delete [] m_xml_buffer;
			m_xml_buffer=NULL;
		}
	}
}//namespace nexus