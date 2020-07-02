#include <iostream>
#include <boost/test/unit_test.hpp>
#include "ncore.h"
#include "object/serialize.h"
#include "object/narchive_xml.h"
#include "file/nstd_file_system.h"
#include "math/math_serialize.h"

using namespace nexus;
using namespace std;

class null_archive : public nexus::narchive
{
public:

	null_archive(void)	{ m_current_level=0;	}
	virtual ~null_archive(void)	{	}

	virtual void open(nfile::ptr file_ptr) {}
	virtual void close() {}

	virtual bool is_loading() const { return false;}

	virtual void array_begin(const TCHAR* array_name) 
	{
		wcout << "serial array, name = " << array_name << endl;
		m_current_level++;
	}
	virtual void object_begin(const TCHAR* class_name, const TCHAR* object_name) 
	{
		wcout << "serial object, name = " << object_name << endl;
		m_current_level++;
	}

	template<typename T>
	void print(const TCHAR* att_name, T& val)
	{
		for(int i=0; i< m_current_level; i++)
			wcout << '\t';
		wcout << "serial primitive type, name = " << att_name
			<< ", value = " << val << endl;
	}

	virtual void serial(const TCHAR* att_name, int& val){ print(att_name, val); }
	virtual void serial(const TCHAR* att_name, unsigned int& val){ print(att_name, val); }
	virtual void serial(const TCHAR* att_name, char& val){ print(att_name, val); }
	virtual void serial(const TCHAR* att_name, unsigned char& val){ print(att_name, val); }
	virtual void serial(const TCHAR* att_name, float& val){ print(att_name, val); }
	virtual void serial(const TCHAR* att_name, double& val){ print(att_name, val); }
	virtual void serial(const TCHAR* att_name, bool& val){ print(att_name, val); }
	virtual void serial(const TCHAR* att_name, short& val) { print(att_name, val); }
	virtual void serial(const TCHAR* att_name, unsigned short& val) { print(att_name, val); }
	virtual void serial(const TCHAR* att_name, long& val) { print(att_name, val); }
	virtual void serial(const TCHAR* att_name, unsigned long& val) { print(att_name, val); }

	virtual void object_end() 
	{
		m_current_level--;
	}
	virtual void array_end() 
	{
		m_current_level--;
	}

private:
	int	m_current_level;
};

class my_object : public nexus::nobject
{
	double	m_d;
public:
	my_object()	{	m_d = 123.455;}
	virtual ~my_object()	{}
	virtual void serialize(narchive& ar)
	{
		nSERIALIZE(ar, m_d);
	}
};

BOOST_AUTO_TEST_CASE(test_nserial_for_debug)
{
	nstring root_dir = nfile_sys_default_root();
	nstd_file_system file_sys;
	file_sys.init(root_dir);

	narchive::ptr ar_ptr;
#if 1
	nfile::ptr file_ptr = file_sys.open_file(_T("game_data"), _T("test/test.xml"), EFileWrite);
	ar_ptr = narchive::create_xml_writer();
#else
	nfile::ptr file_ptr = file_sys.open_file(_T("game_data"), _T("test/test.xml"), EFileRead);
	ar_ptr = narchive::create_xml_reader();
#endif

	ar_ptr->open(file_ptr);

	int a = 99;
	nSERIALIZE(*ar_ptr, a);

	std::vector<int> vec_a;
	vec_a.push_back(123);
	vec_a.push_back(345);
	nSERIALIZE(*ar_ptr, vec_a);

	my_object obj;
	nSERIALIZE(*ar_ptr, obj);

	float f = 99.9;
	nSERIALIZE(*ar_ptr, f);

	vector2 v2(88,99);
	nSERIALIZE(*ar_ptr, v2);

	std::vector<vector2> vv;
	vv.push_back(vector2(1,2));
	vv.push_back(vector2(3,4));
	nSERIALIZE(*ar_ptr, vv);

	matrix44 m;
	m._11 = 11;
	m._13 = 13;
	nSERIALIZE(*ar_ptr, m);

	ar_ptr->close();
}