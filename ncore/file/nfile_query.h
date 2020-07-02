#ifndef _NEXUS_FILE_QUERY_H_
#define _NEXUS_FILE_QUERY_H_

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
		nstring			name;	// "abc.txt"
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
}//namespace nexus
#endif //_NEXUS_FILE_QUERY_H_