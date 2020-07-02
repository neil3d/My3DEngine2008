/**
*	Core of Nexus Engine - Visual file system interface
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _NEXUS_FILE_SYSTEM_H_
#define _NEXUS_FILE_SYSTEM_H_
#include "../object/nobject.h"
#include "../object/nexception.h"
#include "../base_depend.h"
#include "nfile_query.h"

namespace nexus
{

	enum EFileMode
	{
		EFileRead,
		EFileWrite,
	};

	enum EFileSeek
	{
		EFileBegin,
		EFileCurrent,
		EFileEnd,
	};

	/**
	 *	文件读写接口
	 * @remark 【线程安全】
	*/
	class nCoreAPI nfile : public nobject
	{
	public:
		typedef shared_ptr<nfile> ptr;

		nfile(void)	{}
		virtual ~nfile(void)	{}

		virtual size_t read_buffer(void* buffer, size_t read_size) = 0;
		virtual size_t write_buffer(void* buffer, size_t write_size) = 0;
		virtual void seek(int offset, EFileSeek origin) = 0;
		virtual size_t get_file_size() const = 0;

		nstring get_package() const		{	return m_pkg;}
		nstring get_file_name() const	{	return m_file_name;}
		
	protected:
		nstring	m_pkg;
		nstring	m_file_name;

		nDECLARE_VIRTUAL_CLASS(nfile)
	};

	/**
	 *	虚拟文件系统
	 *	@remark	提供对于"文件夹散文件"和"文件打包"一致的访问接口;
	 *	@remark 【线程安全】
	*/
	class nCoreAPI nfile_system : public nobject
	{
	public:
		typedef shared_ptr<nfile_system> ptr;
		nfile_system(void)	{}
		virtual ~nfile_system(void)	{}

		//-- 文件系统操作-----------------------------------------------------
		virtual void init(const nstring& root)	= 0;
		virtual void close() = 0;

		//-- 文件系统查询接口
		virtual size_t get_num_package() const = 0;
		virtual nstring get_package_name(size_t i) = 0;
		virtual void query_package(const nstring& pkg_name, const nstring& path, nfile_query& out_info) = 0;

		//-- "文件包/文件夹"操作-----------------------------------------------------
		virtual void open_package(const nstring& pkg_name) = 0;
		virtual void close_package(const nstring& pkg_name) = 0;
		virtual void create_directory(const nstring& pkg_name,const nstring& dir)=0;
		virtual void remove_directory(const nstring& pkg_name,const nstring& dir)=0;
		virtual bool rename_directory(const nstring& pkg_name,const nstring& old_name,const nstring& new_name)=0;
		//-- 文件操作 -----------------------------------------------------
		/**
		 *	打开某个package中的某个文件
		*/
		virtual nfile::ptr open_file(const nstring& pkg_name, const nstring& file_name, enum EFileMode mode) = 0;
		
		/**
		 *	从包中删除一个文件
		*/
		virtual bool remove_file(const nstring& pkg_name, const nstring& file_name) = 0;

		/**
		 *	取得文件大小
		 *	@return 文件大小(Bytes), 如果文件不存在, 则返回-1
		*/
		virtual int get_file_size(const nstring& pkg_name, const nstring& file_name) = 0;
		virtual time_t get_file_time(const nstring& pkg_name, const nstring& file_name) = 0;

		virtual bool file_exists(const nstring& pkg_name, const nstring& file_name) = 0;

		virtual nstring get_full_path(const nstring& pkg_name, const nstring& file_name) const = 0;

		nDECLARE_VIRTUAL_CLASS(nfile_system)
	};

	class nCoreAPI nfile_exception : public nexception
	{
	public:
		nfile_exception(const TCHAR* info1, const TCHAR* info2, const TCHAR* source_info)
			: nexception(info1, info2, source_info)
		{}
		virtual ~nfile_exception(void)	{}
	};

#define nTHROW_FILE(info, file_name) throw nfile_exception(info, file_name, _T(__FUNCTION__))

	nCoreAPI nstring nfile_sys_default_root();

}//namespace nexus
#endif //_NEXUS_FILE_SYSTEM_H_