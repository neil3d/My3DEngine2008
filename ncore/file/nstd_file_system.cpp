#include "stdafx.h"
#include "nstd_file_system.h"

namespace nexus
{
	//-----------------------------------------------------------------------------------------------
	// class nstd_file
	//-----------------------------------------------------------------------------------------------
	nDEFINE_CLASS(nstd_file, nfile);

	nstd_file::nstd_file(void) : m_fp(NULL)
	{}

	nstd_file::~nstd_file(void)
	{
		mutex::scoped_lock lock(m_mutex);
		if(m_fp)
			fclose(m_fp);
	}

	void nstd_file::open(const nstring& path_str, enum EFileMode mode)
	{
		mutex::scoped_lock lock(m_mutex);

		const TCHAR* mode_str = NULL;
		if(mode == EFileRead)
			mode_str = _T("rb");
		else if(mode == EFileWrite)
			mode_str = _T("wb");
		else
		{
			nTHROW_FILE(_T("bad file mode"), path_str.c_str());
		}

		FILE* fp = _tfopen(path_str.c_str(), mode_str);

		if(fp == NULL)
		{
			nTHROW_FILE(_T("file open failed"), path_str.c_str());
		}

		//--
		m_fp = fp;
		m_path = path_str;
	}

	size_t nstd_file::read_buffer(void* buffer, size_t read_size)
	{
		mutex::scoped_lock lock(m_mutex);
		return fread(buffer, 1, read_size, m_fp);
	}

	size_t nstd_file::write_buffer(void* buffer, size_t write_size)
	{
		mutex::scoped_lock lock(m_mutex);
		return fwrite(buffer, 1, write_size, m_fp);
	}

	//-----------------------------------------------------------------------------------------------
	// class nstd_file_system
	//-----------------------------------------------------------------------------------------------
	nDEFINE_CLASS(nstd_file_system, nfile_system)

	nstd_file_system::nstd_file_system(void)
	{
	}

	nstd_file_system::~nstd_file_system(void)
	{
	}

	nCoreAPI nstring nfile_sys_default_root()
	{
		boost::filesystem::wpath base_path(
		boost::filesystem::initial_path<boost::filesystem::wpath>()
		);

		nstring ret = base_path.string();
		
		nstring::size_type f = ret.rfind(_T('/'));
		if(f != nstring::npos)
		{
			ret = ret.substr(0, f);
		}
		return ret;
	}

	void nstd_file_system::init(const nstring& root)
	{
		m_base_path = root;

		//-- 跟目录下每个子目录作为一个package(目录中必须包含一个标志性的空文件)
		m_packages.clear();
		boost::filesystem::wpath base_path(m_base_path);

		const TCHAR* const RES_TAG = _T("/__nexus_pkg__");

		boost::filesystem::wdirectory_iterator end_iter;
		for(boost::filesystem::wdirectory_iterator iter(base_path);
			iter != end_iter;
			++iter)
		{
			if(boost::filesystem::is_directory( iter->status() ) )
			{
				boost::filesystem::wpath path = *iter;

				//-- 测试是否有标志文件
				nstring test_tag_file = path.string()+RES_TAG;
				boost::filesystem::wpath test_path( test_tag_file );
				if( boost::filesystem::exists(test_path) )
				{
					nstring path_str = path.leaf();
					m_packages.push_back(path_str);
				}
			}
		}// end of for
	}

	void nstd_file_system::close()
	{}

	void nstd_file_system::open_package(const nstring& pkg_name)
	{}

	void nstd_file_system::close_package(const nstring& pkg_name)
	{}

	nfile::ptr nstd_file_system::open_file(const nstring& pkg_name, const nstring& file_name, enum EFileMode mode)
	{
		shared_ptr<nstd_file> new_file(nNew nstd_file);
		new_file->open(get_full_path(pkg_name, file_name), mode);

		return boost::dynamic_pointer_cast<nfile>(new_file);
	}

	bool nstd_file_system::remove_file(const nstring& pkg_name, const nstring& file_name)
	{
		boost::filesystem::wpath fpath( get_full_path(pkg_name, file_name) );
		return boost::filesystem::remove(fpath);
	}

	nstring nstd_file_system::get_full_path(const nstring& pkg_name, const nstring& file_name) const
	{
		wostringstream ostr;
		ostr << m_base_path
			<< _T('/')
			<< pkg_name
			<< _T('/')
			<< file_name;

		return ostr.str();
	}

	int nstd_file_system::get_file_size(const nstring& pkg_name, const nstring& file_name)
	{
		try
		{
			nstring full_path_str = get_full_path(pkg_name, file_name);
			boost::filesystem::wpath fpath( full_path_str );
			return (int)boost::filesystem::file_size(fpath);
		}
		catch(std::exception& e)
		{
			nLog_Exception(e.what());
			nLog_Error(_T("[%s : %s] file not exsit\r\n"), pkg_name.c_str(), file_name.c_str());
		}

		return -1;
	}

	bool nstd_file_system::file_exists(const nstring& pkg_name, const nstring& file_name)
	{
		nstring full_path_str = get_full_path(pkg_name, file_name);
		boost::filesystem::wpath fpath( full_path_str );

		return boost::filesystem::exists(fpath);
	}

	size_t nstd_file_system::get_num_package() const
	{
		return m_packages.size();
	}

	nstring nstd_file_system::get_package_name(size_t i)
	{
		nASSERT( i<m_packages.size() );
		return m_packages[i];
	}

	void nstd_file_system::query_package(const nstring& pkg_name, const nstring& path, nfile_query& out_info)
	{
		nASSERT( !pkg_name.empty() );

		//-- make full path
		wostringstream oss;
		oss << m_base_path
			<< _T('/')
			<< pkg_name;
		if( path != _T("/") ) // pkg root
		{
			oss << _T('/')
			<< path;
		}

		nstring path_str = oss.str();
		boost::filesystem::wpath base_path(path_str);

		boost::filesystem::wdirectory_iterator end_iter;
		for(boost::filesystem::wdirectory_iterator iter(base_path);
			iter != end_iter;
			++iter)
		{
			boost::filesystem::wpath cur_path = *iter;

			nfile_entity new_item;
			new_item.type = EFile_Invalid;			
			new_item.name = cur_path.leaf();
			new_item.path = path;
			if( path != _T("/") ) // pkg root
				new_item.path += _T('/');
			new_item.path += new_item.name;
			

			if(new_item.name.length() > 0
				&& new_item.name[0]!=_T('.')//过滤掉“.svn”这样的文件和目录
				)
			{
				if(boost::filesystem::is_directory( iter->status() ) )
				{				
					new_item.file_size = -1;
					new_item.type = EFile_Folder;				
				}
				else if(boost::filesystem::is_regular( iter->status() ) )
				{
					new_item.file_size = (size_t)boost::filesystem::file_size(cur_path);
					new_item.type = EFile_Regular;				
				}
			}

			if( new_item.type != EFile_Invalid )
			{
				out_info.on_file_entity(new_item);				
			}
		}// end of for	
	}
}//namespace nexus