#include <process.h>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "ncore.h"
#include "object/serialize.h"
#include "object/narchive_xml.h"
#include "file/nstd_file_system.h"
#include "math/math_serialize.h"
#include <boost/timer.hpp>
#include "timer/nprofile.h"

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
	nfile::ptr file_ptr = file_sys.open_file(_T("demo_data"), _T("test/test.xml"), EFileWrite);
	ar_ptr = narchive::create_xml_writer();
#else
	nfile::ptr file_ptr = file_sys.open_file(_T("demo_data"), _T("test/test.xml"), EFileRead);
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

	float f = 99.9f;
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

BOOST_AUTO_TEST_CASE(test_nserial_with_rapidxml_for_debug)
{
	nstring root_dir = nfile_sys_default_root();
	nstd_file_system file_sys;
	file_sys.init(root_dir);

	narchive::ptr ar_ptr;
#if 0
	nfile::ptr file_ptr = file_sys.open_file(_T("demo_data"), _T("test/test2.xml"), EFileWrite);
	ar_ptr = narchive::create_rapidxml_writer();

	ar_ptr->open(file_ptr);

	int a = 99;
	nSERIALIZE(*ar_ptr, a);

	std::vector<int> vec_a;
	vec_a.push_back(123);
	vec_a.push_back(345);
	nSERIALIZE(*ar_ptr, vec_a);

	my_object obj;
	nSERIALIZE(*ar_ptr, obj);

	float f = 99.9f;
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
#else
	nfile::ptr file_ptr = file_sys.open_file(_T("demo_data"), _T("test/test2.xml"), EFileRead);
	ar_ptr = narchive::create_rapidxml_reader();

	ar_ptr->open(file_ptr);

	int a = 0;
	nSERIALIZE(*ar_ptr, a);
	cout<<"a="<<a<<'\n';

	std::vector<int> vec_a;
	vec_a.push_back(0);
	vec_a.push_back(0);
	nSERIALIZE(*ar_ptr, vec_a);
	for(size_t i=0;i<vec_a.size();++i)
	{
		cout<<"vec_a["<<i<<"]="<<vec_a[i]<<"\n";
	}

	my_object obj;
	nSERIALIZE(*ar_ptr, obj);

	float f = 0;
	nSERIALIZE(*ar_ptr, f);
	cout<<"f="<<a<<'\n';

	vector2 v2(0,0);
	nSERIALIZE(*ar_ptr, v2);
	cout<<"v2=("<<v2.x<<','<<v2.y<<");\n";

	std::vector<vector2> vv;
	vv.push_back(vector2(0,0));
	vv.push_back(vector2(0,0));
	nSERIALIZE(*ar_ptr, vv);

	matrix44 m;
	m._11 = 0;
	m._13 = 0;
	nSERIALIZE(*ar_ptr, m);

	ar_ptr->close();
#endif
}

//比较rapidXML和tinyXML的效率
//1)输出文件一定次数，计算平均时间
//2)读取文件一定次数，计算平均时间

void _xml_round_for_performence_test(narchive::ptr ar_ptr,bool read=false)
{
	nstring root_dir = nfile_sys_default_root();
	nstd_file_system file_sys;
	file_sys.init(root_dir);


	nfile::ptr file_ptr;
	if(read)
	{
		file_ptr= file_sys.open_file(_T("demo_data"), _T("test/test.xml"), EFileRead);
	}
	else
	{
		file_ptr= file_sys.open_file(_T("demo_data"), _T("test/test.xml"), EFileWrite);
	}	

	ar_ptr->open(file_ptr);
	ar_ptr->enable_blob(false);

	int a = 99;
	nSERIALIZE(*ar_ptr, a);

	std::vector<int> vec_a;
	for(int i=0; i<100; i++)
	{
		vec_a.push_back(123);
		vec_a.push_back(345);
	}
	nSERIALIZE(*ar_ptr, vec_a);

	my_object obj;
	nSERIALIZE(*ar_ptr, obj);

	float f = 99.9f;
	nSERIALIZE(*ar_ptr, f);

	vector2 v2(88,99);
	nSERIALIZE(*ar_ptr, v2);

	std::vector<vector2> vv;
	for( int i=0; i<1000; i++)
	{
		vv.push_back(vector2(1,2));
		vv.push_back(vector2(3,4));
	}
	nSERIALIZE(*ar_ptr, vv);

	nstring text=L"退 出中文字符测试";
	nSERIALIZE(*ar_ptr, text);

	std::vector<matrix44> vm;
	vm.resize(1000);	
	nSERIALIZE(*ar_ptr, vm);

	ar_ptr->close();
	file_ptr.reset();
}


BOOST_AUTO_TEST_CASE(xml_performence_test)
{
	narchive::ptr tinyxml_writer=narchive::create_xml_writer();
	narchive::ptr tinyxml_reader=narchive::create_xml_reader();
	narchive::ptr rapidxml_writer=narchive::create_rapidxml_writer();
	narchive::ptr rapidxml_reader=narchive::create_rapidxml_reader();
	
	boost::timer	m_timer;
	double result[4];
	const int max_test_rount=1;	

	{
		m_timer.restart();
		for(int i=0;i<max_test_rount;++i)
		{
			START_PROFILE_COUNTER( _T("tinyxml_writer") );
			_xml_round_for_performence_test(tinyxml_writer);
			END_PROFILE_COUNTER( _T("tinyxml_writer") );
		}		
		result[0]=m_timer.elapsed()/max_test_rount;
	}

	{
		m_timer.restart();
		for(int i=0;i<max_test_rount;++i)
		{
			START_PROFILE_COUNTER( _T("tinyxml_reader") );
			_xml_round_for_performence_test(tinyxml_reader, true);
			END_PROFILE_COUNTER( _T("tinyxml_reader") );
		}		
		result[2]=m_timer.elapsed()/max_test_rount;
	}

	
	{
		m_timer.restart();
		for(int i=0;i<max_test_rount;++i)
		{
			START_PROFILE_COUNTER( _T("rapidxml_writer") );
			_xml_round_for_performence_test(rapidxml_writer);
			END_PROFILE_COUNTER( _T("rapidxml_writer") );
		}		
		result[1]=m_timer.elapsed()/max_test_rount;
	}

	
	{
		m_timer.restart();
		for(int i=0;i<max_test_rount;++i)
		{
			START_PROFILE_COUNTER( _T("rapidxml_reader") );
			_xml_round_for_performence_test(rapidxml_reader, true);
			END_PROFILE_COUNTER( _T("rapidxml_reader") );
		}
		result[3]=m_timer.elapsed()/max_test_rount;
	}

	profile_manager* manager = profile_manager::instance();
	//profile_value* value = manager->first_profile();
	//while (value != NULL)
	//{
	//	nstring info = value->to_string();
	//	std::wcout << info;

	//	value = manager->next_profile();
	//}

	profile_value* tw_value = manager->find_profile(nname(_T("tinyxml_writer")));
	profile_value* tr_value = manager->find_profile(nname(_T("tinyxml_reader")));
	profile_value* rw_value = manager->find_profile(nname(_T("rapidxml_writer")));
	profile_value* rr_value = manager->find_profile(nname(_T("rapidxml_reader")));

	std::wcout << _T("tinyxml_writer: ") << tw_value->to_string() << endl;
	std::wcout << _T("tinyxml_reader: ") << tr_value->to_string() << endl;
	std::wcout << _T("rapidxml_writer: ") << rw_value->to_string() << endl;
	std::wcout << _T("rapidxml_reader: ") << rr_value->to_string() << endl;

	std::wcout<<"tinyxml_writer:rapidxml_writer=" << tw_value->get_average()/rw_value->get_average() << endl;
	std::wcout<<"tinyxml_reader:rapidxml_reader="<<tr_value->get_average()/rr_value->get_average() << endl;


	cout<< "--------------------------------------------"<<endl;
	cout<<"TinyXML写入数据的平均开销:"<<result[0]<< endl;
	cout<<"RapidXML写入数据的平均开销:"<<result[1]<<endl;
	cout<<"RapidXML:TinyXML="<<result[0]/result[1]<<endl;
	cout<< endl;
	cout<<"TinyXML读取数据的平均开销:"<<result[2]<<endl;
	cout<<"RapidXML读取数据的平均开销:"<<result[3]<<endl;
	cout<<"RapidXML:TinyXML="<<result[2]/result[3]<<endl;
	cout<< "--------------------------------------------"<<endl;
	::system("pause");
}