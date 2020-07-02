#include "StdAfx.h"
#include "nresource_io_basic.h"
#include "nresource_manager.h"

namespace nexus
{
	nDEFINE_CLASS(nresource_io_basic, nresource_io)

	nresource_io_basic::nresource_io_basic(void)
	{
	}

	nresource_io_basic::~nresource_io_basic(void)
	{
	}

	void nresource_io_basic::load_resource(nresource::ptr res_ptr, const nstring& pkg_name, const nstring& file_name, 
		enum EResourceIOMode mode, enum EResourceIOPriority pri)
	{
		nASSERT( res_ptr );		

		(void)mode;
		(void)pri;

		try
		{
			res_ptr->load_from_file( resource_location(pkg_name, file_name) );
		}
		catch (const nexception& ne)
		{
			st_res_type2loc::iterator iter = m_default_res.find( res_ptr->reflection_get_class()->get_name() );
			if( iter != m_default_res.end() )
			{
				resource_location loc = iter->second;
				res_ptr->load_from_file( loc );
			}
			else
			{
				throw;
			}
		}		

		nresource_event evt;
		evt.type = EResEvt_Ready;
		evt.res_name = res_ptr->get_name();
		nresource_manager::instance()->post_event(evt);
	}

	void nresource_io::start()
	{
		m_default_res[_T("nresource_texture_2d")] = resource_location(_T("engine_data:resource_default/white.bmp"));
		m_default_res[_T("nresource_static_mesh")] = resource_location(_T("engine_data:resource_default/ball.nmdl"));
		m_default_res[_T("nspeed_base_tree")] = resource_location(_T("engine_data:resource_default/Sycamore_RT.spt"));
	}
}//namespace nexus