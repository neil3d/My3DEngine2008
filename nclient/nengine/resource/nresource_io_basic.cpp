#include "StdAfx.h"
#include "nresource_io_basic.h"

namespace nexus
{
	nDEFINE_CLASS(nresource_io_basic, nresource_io)

	nresource_io_basic::nresource_io_basic(void)
	{
	}

	nresource_io_basic::~nresource_io_basic(void)
	{
	}

	void nresource_io_basic::load_resource(nresource* res_ptr, nfile_system* fs, const nstring& pkg_name, const nstring& file_name, 
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		nASSERT( res_ptr );
		nASSERT( fs );

		(void)mode;
		(void)pri;

		res_ptr->load_from_file( resource_location(pkg_name, file_name) );
	}

	void nresource_io_basic::cancel_resource_load(nresource* res_ptr)
	{
		// do nothing
		(void)res_ptr;
	}
}//namespace nexus