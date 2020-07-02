/**
*	nexus core - Serialization
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _NEXUS_ARCHIVE_BIN_H_
#define _NEXUS_ARCHIVE_BIN_H_
#include "narchive.h"
#include "../file/nstd_file_system.h"

namespace nexus
{
	/**
	 *	使用二进制来保存对象
	*/
	class narchive_bin_writer
		: public narchive
	{
	public:
		narchive_bin_writer(void);
		virtual ~narchive_bin_writer(void);

		virtual void open(nfile::ptr file_ptr);
		virtual void close();

		virtual bool is_loading() const		{	return false;}
		
		virtual void array_begin(const TCHAR* array_name, size_t& array_size);
		virtual void array_next() {}
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
		virtual size_t serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size);
		virtual void object_end()	{}
		virtual void array_end()	{}

		virtual nfile::ptr get_file() const
		{
			return m_file_ptr;
		}
	private:
		template<typename T>
		void save_value_type(const T& val);
	private:
		nfile::ptr	m_file_ptr;
	};

	/** 读取二进制对象文档 */
	class narchive_bin_reader
		: public narchive
	{
	public:
		narchive_bin_reader(void);
		virtual ~narchive_bin_reader(void);

		virtual void open(nfile::ptr file_ptr);
		virtual void close();

		virtual bool is_loading() const		{	return true;}
		
		virtual void array_begin(const TCHAR* array_name, size_t& array_size);
		virtual void object_begin(const TCHAR* object_name, nstring& class_name);
		virtual void array_next() {}
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
		virtual void object_end()	{}
		virtual void array_end()	{}

		virtual nfile::ptr get_file() const
		{
			return m_file_ptr;
		}
	private:
		template<typename T>
		void load_value_type(const T& val);
	private:
		nfile::ptr	m_file_ptr;
	};
}//namespace nexus

#endif //_NEXUS_ARCHIVE_BIN_H_