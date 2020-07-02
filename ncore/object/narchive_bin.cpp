#include "stdafx.h"
#include "narchive_bin.h"

namespace nexus
{
	const char* const ARCHIVE_TAG = "nexus_archive_bin";
	narchive::ptr narchive::create_bin_writer()
	{
		return narchive::ptr(nNew narchive_bin_writer);
	}

	narchive::ptr narchive::create_bin_reader()
	{
		return narchive::ptr(nNew narchive_bin_reader);
	}

	narchive_bin_writer::narchive_bin_writer(void)
	{
	}

	narchive_bin_writer::~narchive_bin_writer(void)
	{
	}

	void narchive_bin_writer::open(nfile::ptr file_ptr)
	{
		m_file_ptr = file_ptr;

		//--write tag
		serial(_T("tag"), std::string(ARCHIVE_TAG));
	}

	void narchive_bin_writer::close()
	{
		m_file_ptr.reset();
	}

	template<typename T>
	void narchive_bin_writer::save_value_type(const T& val)
	{
		nASSERT( m_file_ptr );
		m_file_ptr->write_buffer((void*)&val, sizeof(T));
	}

	void narchive_bin_writer::array_begin(const TCHAR* array_name, size_t& array_size)
	{
		save_value_type(array_size);
	}

	void narchive_bin_writer::object_begin(const TCHAR* object_name, nstring& class_name)
	{
		serial(object_name, class_name);
	}
	
	void narchive_bin_writer::serial(const TCHAR* att_name, int& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, unsigned int& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, char& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, unsigned char& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, short& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, unsigned short& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, long& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, unsigned long& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, float& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, double& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, bool& val)
	{
		save_value_type(val);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, nstring& val)
	{
		size_t s = (val.length()+1)*sizeof(nstring::value_type);
		serial_blob(att_name, (void*)val.c_str(), s);
	}

	void narchive_bin_writer::serial(const TCHAR* att_name, std::string& val)
	{
		size_t s = (val.length()+1)*sizeof(std::string::value_type);
		serial_blob(att_name, (void*)val.c_str(), s);
	}

	size_t narchive_bin_writer::serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size)
	{
		nASSERT(buffer);
		nASSERT(m_file_ptr);

		save_value_type(buffer_size);
		m_file_ptr->write_buffer(buffer, buffer_size);

		return buffer_size;
	}

	narchive_bin_reader::narchive_bin_reader(void)
	{
	}

	narchive_bin_reader::~narchive_bin_reader(void)
	{
	}

	void narchive_bin_reader::open(nfile::ptr file_ptr)
	{
		m_file_ptr = file_ptr;

		//--read tag
		std::string sz_tag;
		serial(_T("tag"), sz_tag);		
		nASSERT(sz_tag == ARCHIVE_TAG);
	}

	void narchive_bin_reader::close()
	{
		m_file_ptr.reset();
	}

	template<typename T>
	void narchive_bin_reader::load_value_type(const T& val)
	{
		nASSERT( m_file_ptr );
		m_file_ptr->read_buffer((void*)&val, sizeof(T));
	}

	void narchive_bin_reader::array_begin(const TCHAR* array_name, size_t& array_size)
	{
		load_value_type(array_size);
	}

	void narchive_bin_reader::object_begin(const TCHAR* object_name, nstring& class_name)
	{
		serial(object_name, class_name);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, int& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, unsigned int& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, char& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, unsigned char& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, short& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, unsigned short& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, long& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, unsigned long& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, float& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, double& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, bool& val)
	{
		load_value_type(val);
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, nstring& val)
	{
		size_t s = serial_blob(att_name, NULL, 0);
		scoped_array<char> buffer(nNew char[s]);		
		serial_blob(att_name, (void*)buffer.get(), s);
		val = (TCHAR*)buffer.get();
	}

	void narchive_bin_reader::serial(const TCHAR* att_name, std::string& val)
	{
		// todo :
		size_t s = serial_blob(att_name, NULL, 0);
		scoped_array<char> buffer(nNew char[s]);		
		serial_blob(att_name, (void*)buffer.get(), s);
		val = buffer.get();
	}

	size_t narchive_bin_reader::serial_blob(const TCHAR* att_name, void* buffer, size_t buffer_size)
	{
		nASSERT(m_file_ptr);
		if( buffer == NULL )
		{
			load_value_type(buffer_size);
			return buffer_size;
		}
		else
		{			
			return m_file_ptr->read_buffer(buffer, buffer_size);
		}		
	}
	
}//namespace nexus