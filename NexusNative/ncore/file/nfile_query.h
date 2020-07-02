#ifndef _NEXUS_FILE_QUERY_H_
#define _NEXUS_FILE_QUERY_H_
#include <boost/function.hpp>

namespace nexus
{
	enum EFileEntityType
	{
		EFile_Invalid = 0,
		EFile_Regular,	// 普通文件
		EFile_Folder		// 文件夹
	};

	struct nfile_entity
	{
		EFileEntityType	type;
		nstring			pkg;	// package name
		nstring			path;	// "sub_folder/abc.txt"
		size_t			file_size;	// 普通文件有效
	};

	class nfile_query
	{
	public:
		nfile_query()	{}
		virtual ~nfile_query()	{}

		virtual void on_file_entity(const nfile_entity& file) = 0;
	};

	typedef boost::function<void(const nfile_entity&)> file_query_handler;

	class nfile_query_bind : public nfile_query
	{
		file_query_handler	m_handler;
	public:
		explicit nfile_query_bind(const file_query_handler& hdl):m_handler(hdl)
		{}
		virtual ~nfile_query_bind()
		{}

		virtual void on_file_entity(const nfile_entity& file)
		{
			m_handler(file);
		}
	};
}//namespace nexus
#endif //_NEXUS_FILE_QUERY_H_