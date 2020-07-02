#include "StdAfx.h"
#include "nresource.h"
#include "../framework/nengine.h"
#include "nresource_manager.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nresource, nobject)
		///////////////////////////////////////////////////////////////////////////////////////////
		//	class nresource
		///////////////////////////////////////////////////////////////////////////////////////////
		void nresource::release()
	{
		m_ref_count--;
		if(m_ref_count <= 0)
			nresource_manager::instance()->free_resource(this);
	}

	void nresource::serialize(narchive& ar)
	{
		(void)ar;
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
				ar_ptr = narchive::create_xml_reader();
			else
				ar_ptr = narchive::create_bin_reader();
		}
		else
		{
			nASSERT( fmode == EFileWrite );
			if(xml)
				ar_ptr = narchive::create_xml_writer();
			else
				ar_ptr = narchive::create_bin_writer();
		}
		ar_ptr->open(file_ptr);

		serialize(*ar_ptr);

		ar_ptr->close();
	}
}