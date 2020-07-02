/**
*	nexus core - Serialization
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _NEXUS_ARCHIVE_H_
#define _NEXUS_ARCHIVE_H_
#include "../base_depend.h"
#include "nobject.h"
#include "../file/nfile_system.h"

namespace nexus
{
	/**
	*	序列化文档基类
	*	@remark nexus searilization机制：
	*		.通过narchive类提供基本的IO操作接口；
	*		.通过nserilize<>()模板函数实现各种数据的narchive操作封装，并通过模板函数特化来实现非插入式的对象序列化；
	*		.nobject派生内通过serilize()虚函数来实现对象成员数据的序列化封装；
	*		.凡是需要序列化指针或智能指针的类，需要使用nobject模式，以实现动态创建对象；否则可选择非插入式序列化；
	*/
	class nCoreAPI narchive : public nobject
	{
	public:
		typedef shared_ptr<narchive> ptr;

		narchive(void)	{	}
		virtual ~narchive(void)	{	}

		virtual void open(nfile::ptr file_ptr) = 0;
		virtual void close() = 0;

		virtual bool is_loading() const = 0;
		virtual bool support_blob() const = 0;
		
		virtual void array_begin(const TCHAR* array_name, size_t& array_size) = 0;
		virtual void array_next() {}
		virtual void object_begin(const TCHAR* object_name, nstring& class_name) = 0;
		
		virtual void serial(const TCHAR* att_name, int& val) = 0;
		virtual void serial(const TCHAR* att_name, unsigned int& val) = 0;
		virtual void serial(const TCHAR* att_name, char& val) = 0;
		virtual void serial(const TCHAR* att_name, unsigned char& val) = 0;
		virtual void serial(const TCHAR* att_name, short& val) = 0;
		virtual void serial(const TCHAR* att_name, unsigned short& val) = 0;
		virtual void serial(const TCHAR* att_name, long& val) = 0;
		virtual void serial(const TCHAR* att_name, unsigned long& val) = 0;
		virtual void serial(const TCHAR* att_name, float& val) = 0;
		virtual void serial(const TCHAR* att_name, double& val) = 0;
		virtual void serial(const TCHAR* att_name, bool& val) = 0;

		virtual void serial(const TCHAR* att_name, nstring& val) = 0;
		virtual void serial(const TCHAR* att_name, std::string& val) = 0;

		/**
		*	@return 在读取模式下，如果buffer为NULL，则只读出buffer_size并返回；
		*/
		virtual size_t serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size)	
		{
			(void)att_name;	(void)buffer;	(void)buffer_size;
			return 0;
		}

		virtual void object_end() = 0;
		virtual void array_end() = 0;

		//-- factory method
		static narchive::ptr create_xml_writer();
		static narchive::ptr create_xml_reader();
		static narchive::ptr create_bin_writer();
		static narchive::ptr create_bin_reader();


		nDECLARE_VIRTUAL_CLASS(narchive)
	};

	const TCHAR* const ELEM_ArrayItem = _T("array_item");
}//namespace nexus
#endif //_NEXUS_ARCHIVE_H_