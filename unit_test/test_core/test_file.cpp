#include "ncore.h"
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>

#include "file/nstd_file_system.h"

using namespace std;
using namespace nexus;

BOOST_AUTO_TEST_CASE(test_file_class)
{
	const nclass* class_ptr = nfile_system::reflection_get_static_class();
	
	for(size_t i = 0;
		i < class_ptr->get_num_children();
		i++)
	{
		const nclass* class_child = class_ptr->get_child(i);
		
		boost::scoped_ptr<nfile_system> fsys( static_cast<nfile_system*>(class_child->alloc_object()) );
		fsys->init( nfile_sys_default_root() );

		nstring pkg_name(_T("game_data"));
		nstring file_name(_T("test/hello.txt"));
		fsys->open_package(pkg_name);

		TCHAR buffer[]=_T("hello");
		//-- test write
		{
			nfile::ptr pfile = fsys->open_file(pkg_name, file_name, EFileWrite);
			
			size_t ret_write = pfile->write_buffer(buffer, sizeof(buffer));
			BOOST_CHECK_EQUAL(ret_write, sizeof(buffer));
		}

		//-- test read
		{
			nfile::ptr pfile = fsys->open_file(pkg_name, file_name, EFileRead);
			TCHAR read_buff[256] = {0};
			size_t ret_read = pfile->read_buffer(read_buff, sizeof(buffer));
			BOOST_CHECK_EQUAL(ret_read, sizeof(buffer));

			BOOST_CHECK(_tcscmp(buffer, read_buff) == 0);
		}

		//-- 删除文件
		{
			bool bremove = fsys->remove_file(pkg_name, file_name);
			BOOST_CHECK( bremove);
		}
		

		fsys->close();
	}
}

BOOST_AUTO_TEST_CASE(test_conv_string)
{
	std::wstring wstr(_T("中国"));
	std::string str = conv_string(wstr);

	std::wstring wstr_bk = conv_string(str);

	BOOST_CHECK(wstr == wstr_bk);
}

struct my_file_query : public nfile_query
{
	virtual void on_file_entity(const nfile_entity& item)
	{
		if( item.type == EFile_Folder)
			wcout << _T("<dir>");
		wcout << item.name 			
			<< endl;
	}
};

BOOST_AUTO_TEST_CASE(test_package_query)
{
	nstring root_dir = nfile_sys_default_root();
	nstd_file_system file_sys;
	file_sys.init(root_dir);

	my_file_query fq;
	wcout << _T("query package root...") << endl;
	file_sys.query_package(_T("game_data"), _T("/"), fq);

	wcout << _T("query package folder...") << endl;
	file_sys.query_package(_T("game_data"), _T("terrain"), fq);

}