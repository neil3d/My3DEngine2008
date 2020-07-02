/**
 *	Core of Nexus Engine - Visual file system
 *	使用ANSI C库函数来读取磁盘中的"散文件", 此时,一个package对应磁盘的一个目录
 *
 *	Author: Neil
 *	Date:	Sept, 2008
*/
#ifndef _NEXUS_STD_FILE_SYSTEM_H_
#define _NEXUS_STD_FILE_SYSTEM_H_
#include <stdio.h>
#include "nfile_system.h"

namespace nexus
{
	/*
	 *	一个标准文件的封装
	*/
	class nstd_file : public nfile
	{
	public:
		nstd_file(void);
		virtual ~nstd_file(void);

		void open(const nstring& path_str, enum EFileMode mode);
		void set_file_loc(const nstring& pkg, const nstring& file_name)
		{
			m_pkg = pkg;
			m_file_name = file_name;
		}

		virtual size_t read_buffer(void* buffer, size_t read_size);
		virtual size_t write_buffer(void* buffer, size_t write_size);
		
		virtual void seek(int offset, EFileSeek origin);
		virtual size_t get_file_size() const;

		FILE*	get_fp() const		{	return m_fp; }
		nstring get_path() const	{	return m_path;}

	private:
		mutable mutex	m_mutex;

		FILE*	m_fp;
		nstring	m_path;

		nDECLARE_CLASS(nstd_file)
	};

	/**
	 *	访问标准文件系统的虚拟文件系统
	 *	@remark In this class, we treat a floder as a package
	*/
	class nCoreAPI nstd_file_system : public nfile_system
	{
	public:
		nstd_file_system(void);
		virtual ~nstd_file_system(void);

		virtual void init(const nstring& root);
		virtual void close();

		virtual size_t get_num_package() const;
		virtual nstring get_package_name(size_t i);
		virtual void query_package(const nstring& pkg_name, const nstring& path, nfile_query& out_info);

		virtual void open_package(const nstring& pkg_name);
		virtual void close_package(const nstring& pkg_name);
		virtual nfile::ptr open_file(const nstring& pkg_name, const nstring& file_name, enum EFileMode mode);
		virtual bool remove_file(const nstring& pkg_name, const nstring& file_name);
		virtual int get_file_size(const nstring& pkg_name, const nstring& file_name);
		virtual time_t get_file_time(const nstring& pkg_name, const nstring& file_name);
		virtual bool file_exists(const nstring& pkg_name, const nstring& file_name);

	private:
		nstring get_full_path(const nstring& pkg_name, const nstring& file_name) const;
		void query_path_R(const nstring& path_str, nfile_query* out_info, int parent_id, int& count);

		nstring	m_base_path;
		std::vector<nstring>	m_packages;

		nDECLARE_CLASS(nstd_file_system)
	};
}//namespace nexus
#endif //_NEXUS_STD_FILE_SYSTEM_H_