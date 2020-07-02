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

	void nresource::do_serialize_io(const resource_location& loc, enum EFileMode fmode)
	{
		nfile_system* fs = nengine::instance()->get_file_sys();
		nfile::ptr file_ptr = fs->open_file(loc.pkg_name, loc.file_name, fmode);

		narchive::ptr ar_ptr;
		if(fmode == EFileRead)
			ar_ptr = narchive::create_bin_reader();
		else
		{
			nASSERT( fmode == EFileWrite );
			ar_ptr = narchive::create_bin_writer();
		}
		ar_ptr->open(file_ptr);

		serialize(*ar_ptr);

		ar_ptr->close();
	}
}