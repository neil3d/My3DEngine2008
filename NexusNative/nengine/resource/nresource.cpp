#include "StdAfx.h"
#include <boost/thread.hpp>
#include "nresource.h"
#include "../framework/nengine.h"
#include "nresource_manager.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#endif

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nresource, nobject)
		///////////////////////////////////////////////////////////////////////////////////////////
		//	class nresource
		///////////////////////////////////////////////////////////////////////////////////////////
		void nresource::add_ref()
	{
		::InterlockedIncrement(&m_ref_count);
	}

		void nresource::release()
	{
		::InterlockedDecrement(&m_ref_count);
		if(m_ref_count <= 0)
			nresource_manager::instance()->free_resource(this);
	}

	void nresource::serialize(narchive& ar)
	{
		(void)ar;
	}

	const resource_location& nresource::get_location() const	
	{	
		boost::mutex::scoped_lock lock(m_mutex);
		return m_location; 
	}

	void nresource::load_from_file(const resource_location& loc)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		m_location = loc;
		do_serialize_io(loc, EFileRead, false);
		
		::InterlockedExchange(&m_ready, 1L);
	}


	void nresource::post_edit_change(bool ready)
	{
		if(ready)
		{
			::InterlockedExchange(&m_ready, 1L);

			nresource_event evt;
			evt.type = EResEvt_Ready;
			evt.res_name = m_name;
			nresource_manager::instance()->post_event(evt);
		}
	}

	void nresource::save_to_file(const resource_location& loc, bool xml)
	{
		boost::mutex::scoped_lock lock(m_mutex);

		do_serialize_io(loc, EFileWrite, xml);
	}

	void nresource::do_serialize_io(const resource_location& loc, enum EFileMode fmode, bool xml)
	{
		nfile_system* fs = nengine::instance()->get_file_sys();
		nfile::ptr file_ptr = fs->open_file(loc.pkg_name, loc.file_name, fmode);

		narchive::ptr ar_ptr;
		if(fmode == EFileRead)
		{
			// 根据文件头, 自动判断是否XML
			std::string tag(ARCHIVE_TAG);
			if( file_ptr->get_file_size() > tag.length() )
			{
				char buf[64] = {0};
				file_ptr->read_buffer(buf, tag.length()*2);
				file_ptr->seek(0, EFileBegin);
				std::string read_tag(buf+4);//前面存了4字节的size
				if(tag == read_tag)
					xml = false;
				else
					xml = true;
			}
			else
			{
				xml = true;
			}
			
			if( xml )
				ar_ptr = narchive::create_rapidxml_reader();
			else
				ar_ptr = narchive::create_bin_reader();
		}
		else
		{
			nASSERT( fmode == EFileWrite );
			if(xml)
				ar_ptr = narchive::create_rapidxml_writer();
			else
				ar_ptr = narchive::create_bin_writer();
		}
		ar_ptr->open(file_ptr);

		serialize(*ar_ptr);

		ar_ptr->close();
	}
}