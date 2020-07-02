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
		virtual bool support_blob() const	{	return false;}

		virtual void array_begin(const TCHAR* array_name, size_t& array_size);		
		virtual void object_begin(const TCHAR* object_name, nstring& class_name);
		
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

		virtual void object_end();
		virtual void array_end();

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
		virtual bool support_blob() const	{	return false;}

		virtual void array_begin(const TCHAR* array_name, size_t& array_size);		
		virtual void array_next();
		virtual void object_begin(const TCHAR* object_name, nstring& class_name);

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

		virtual void object_end();
		virtual void array_end();

	private:
		template<typename T>
		void load_attribute(const TCHAR* att_name, T& val);		

		TiXmlDocument				m_xml_doc;
		shared_ptr<nstd_file>		m_std_file;
		std::stack<TiXmlElement*>	m_element_stack;
		TiXmlElement*				m_array_sibling_elem;
	};
}//namespace nexus

#endif //_NEXUS_ARCHIVE_XML_H_