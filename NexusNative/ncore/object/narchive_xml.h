/**
*	nexus core - Serialization
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _NEXUS_ARCHIVE_XML_H_
#define _NEXUS_ARCHIVE_XML_H_

#include <stack>
#include "narchive.h"
#include "../file/nstd_file_system.h"

#include "../tinyxml/tinyxml.h"
#include "../rapidxml/rapidxml.hpp"

namespace nexus
{
	/**
	 *	使用xml序列化对象数据
	*/
	class narchive_xml_writer :
		public narchive
	{
	public:
		narchive_xml_writer(void);
		virtual ~narchive_xml_writer(void);

		virtual void open(nfile::ptr file_ptr);
		virtual void close();

		virtual bool is_loading() const		{	return false; }
		
		virtual void array_begin(const TCHAR* array_name, size_t& array_size);		
		virtual void object_begin(const TCHAR* object_name, nstring& class_name);
		virtual void object_description(const TCHAR* desc_name,nstring& description);

		virtual void serial(const TCHAR* att_name, int& val);
		virtual void serial(const TCHAR* att_name, unsigned int& val);
		virtual void serial(const TCHAR* att_name, char& val);
		virtual void serial(const TCHAR* att_name, unsigned char& val);
		virtual void serial(const TCHAR* att_name, short& val);
		virtual void serial(const TCHAR* att_name, unsigned short& val);
		virtual void serial(const TCHAR* att_name, long& val);
		virtual void serial(const TCHAR* att_name, unsigned long& val);
		virtual void serial(const TCHAR* att_name, float& val);
		virtual void serial(const TCHAR* att_name, double& val);
		virtual void serial(const TCHAR* att_name, bool& val);
		virtual void serial(const TCHAR* att_name, nstring& val);
		virtual void serial(const TCHAR* att_name, std::string& val);	

		virtual size_t serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size);

		virtual void object_end();
		virtual void array_end();

		virtual nfile::ptr get_file() const
		{
			return m_std_file;
		}

	private:
		template<typename T>
		void save_attribute(const TCHAR* att_name, T& val);		

		TiXmlDocument			m_xml_doc;
		shared_ptr<nstd_file>	m_std_file;
		std::stack<TiXmlElement*>	m_element_stack;

		nDECLARE_CLASS(narchive_xml_writer)
	};

	/** 读取xml对象文档 */
	class narchive_xml_reader :
		public narchive
	{
	public:
		narchive_xml_reader(void);
		virtual ~narchive_xml_reader(void);

		virtual void open(nfile::ptr file_ptr);
		virtual void close();

		virtual bool is_loading() const	{	return true; }
		
		virtual void array_begin(const TCHAR* array_name, size_t& array_size);		
		virtual void array_next();
		virtual void object_begin(const TCHAR* object_name, nstring& class_name);
		virtual void object_description(const TCHAR* desc_name,nstring& description);

		virtual void serial(const TCHAR* att_name, int& val);
		virtual void serial(const TCHAR* att_name, unsigned int& val);
		virtual void serial(const TCHAR* att_name, char& val);
		virtual void serial(const TCHAR* att_name, unsigned char& val);
		virtual void serial(const TCHAR* att_name, short& val);
		virtual void serial(const TCHAR* att_name, unsigned short& val);
		virtual void serial(const TCHAR* att_name, long& val);
		virtual void serial(const TCHAR* att_name, unsigned long& val);
		virtual void serial(const TCHAR* att_name, float& val);
		virtual void serial(const TCHAR* att_name, double& val);
		virtual void serial(const TCHAR* att_name, bool& val);
		virtual void serial(const TCHAR* att_name, nstring& val);
		virtual void serial(const TCHAR* att_name, std::string& val);		
		virtual size_t serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size);

		virtual void object_end();
		virtual void array_end();

		virtual nfile::ptr get_file() const
		{
			return m_std_file;
		}
	private:
		template<typename T>
		void load_attribute(const TCHAR* att_name, T& val);		

		TiXmlDocument				m_xml_doc;
		shared_ptr<nstd_file>		m_std_file;
		std::stack<TiXmlElement*>	m_element_stack;
		std::stack<TiXmlElement*>	m_array_sibling_stack;
	};

	//===============================================================================
	class narchive_rapidxml_writer :
		public narchive
	{
	public:
		typedef rapidxml::xml_node<TCHAR>		rapid_xml_node;
		typedef rapidxml::xml_attribute<TCHAR>	rapid_xml_attribute;
		typedef rapidxml::xml_document<TCHAR>	rapid_xml_docment;
		typedef rapidxml::memory_pool<TCHAR>	rapid_xml_memory_pool;

		narchive_rapidxml_writer(void);
		virtual ~narchive_rapidxml_writer(void);

		virtual void open(nfile::ptr file_ptr);
		virtual void close();

		virtual bool is_loading() const		{	return false; }

		virtual void array_begin(const TCHAR* array_name, size_t& array_size);		
		virtual void object_begin(const TCHAR* object_name, nstring& class_name);

		virtual void object_description(const TCHAR* desc_name,nstring& description);

		virtual void serial(const TCHAR* att_name, int& val);
		virtual void serial(const TCHAR* att_name, unsigned int& val);
		virtual void serial(const TCHAR* att_name, char& val);
		virtual void serial(const TCHAR* att_name, unsigned char& val);
		virtual void serial(const TCHAR* att_name, short& val);
		virtual void serial(const TCHAR* att_name, unsigned short& val);
		virtual void serial(const TCHAR* att_name, long& val);
		virtual void serial(const TCHAR* att_name, unsigned long& val);
		virtual void serial(const TCHAR* att_name, float& val);
		virtual void serial(const TCHAR* att_name, double& val);
		virtual void serial(const TCHAR* att_name, bool& val);
		virtual void serial(const TCHAR* att_name, nstring& val);
		virtual void serial(const TCHAR* att_name, std::string& val);	

		virtual size_t serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size);

		virtual void object_end();
		virtual void array_end();

		virtual nfile::ptr get_file() const
		{
			return m_file;
		}

	protected:
		template<typename T>
		void save_attribute(const TCHAR* att_name, T& val);		

		rapid_xml_docment			m_xml_doc;
		std::stack<rapid_xml_node*>	m_element_stack;
		nfile::ptr					m_file;
		nDECLARE_CLASS(narchive_rapidxml_writer)
	};

	/** 读取xml对象文档 */
	class narchive_rapidxml_reader :
		public narchive
	{
	public:
		typedef rapidxml::xml_node<TCHAR>		rapid_xml_node;
		typedef rapidxml::xml_attribute<TCHAR>	rapid_xml_attribute;
		typedef rapidxml::xml_document<TCHAR>	rapid_xml_docment;
		typedef rapidxml::memory_pool<TCHAR>	rapid_xml_memory_pool;

		narchive_rapidxml_reader(void);
		virtual ~narchive_rapidxml_reader(void);

		virtual void open(nfile::ptr file_ptr);
		virtual void close();

		virtual bool is_loading() const	{	return true; }

		virtual void array_begin(const TCHAR* array_name, size_t& array_size);		
		virtual void array_next();
		virtual void object_begin(const TCHAR* object_name, nstring& class_name);

		virtual void object_description(const TCHAR* desc_name,nstring& description);

		virtual void serial(const TCHAR* att_name, int& val);
		virtual void serial(const TCHAR* att_name, unsigned int& val);
		virtual void serial(const TCHAR* att_name, char& val);
		virtual void serial(const TCHAR* att_name, unsigned char& val);
		virtual void serial(const TCHAR* att_name, short& val);
		virtual void serial(const TCHAR* att_name, unsigned short& val);
		virtual void serial(const TCHAR* att_name, long& val);
		virtual void serial(const TCHAR* att_name, unsigned long& val);
		virtual void serial(const TCHAR* att_name, float& val);
		virtual void serial(const TCHAR* att_name, double& val);
		virtual void serial(const TCHAR* att_name, bool& val);
		virtual void serial(const TCHAR* att_name, nstring& val);
		virtual void serial(const TCHAR* att_name, std::string& val);		
		virtual size_t serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size);

		virtual void object_end();
		virtual void array_end();

		virtual nfile::ptr get_file() const
		{
			return m_file;
		}
	protected:
		template<typename T>
		void load_attribute(const TCHAR* att_name, T& val);		

		rapid_xml_docment			m_xml_doc;
		TCHAR*						m_xml_buffer;
		size_t						m_buffer_size;
		nfile::ptr					m_file;
		std::stack<rapid_xml_node*>	m_element_stack;
		std::stack<rapid_xml_node*>	m_array_sibling_stack;
		//bool	m_in_array;
	};

	//===================================================================================
	class narchive_memory_xml_writer:public narchive_rapidxml_writer
	{
	public:
		narchive_memory_xml_writer(){}
		virtual ~narchive_memory_xml_writer(){}
		virtual void open(nfile::ptr file_ptr){};
		virtual void open(nstring& buffer);
		virtual void close();
	protected:
		nstring*	m_buffer;
	};

	class narchive_memory_xml_reader:public narchive_rapidxml_reader
	{
	public:
		narchive_memory_xml_reader(){}
		virtual ~narchive_memory_xml_reader(){}
		virtual void open(nfile::ptr file_ptr){};
		virtual void open(nstring& buffer);
		virtual void close();
	};
}//namespace nexus

#endif //_NEXUS_ARCHIVE_XML_H_