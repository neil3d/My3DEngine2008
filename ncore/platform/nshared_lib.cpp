#include "stdafx.h"
#include "nshared_lib.h"
#include "../object/nexception.h"

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
	#define NOMINMAX
	#include <Windows.h>
#endif

namespace nexus
{
#ifdef WIN32
	class nwin32_dll : public nshared_lib
	{
	public:
		nwin32_dll()	
		{
			m_handle = NULL;
		}
		virtual ~nwin32_dll()
		{
			if(m_handle)
			{
				::FreeLibrary(m_handle);
				m_handle = NULL;
			}
		}

		virtual void load(const nstring& file_name)
		{
			nASSERT( m_handle==NULL );
			m_handle = ::LoadLibrary(file_name.c_str());
			if(m_handle == NULL)
				nthrow2(_T("dll load failed!"), file_name.c_str());
		}

	private:
		HMODULE	m_handle;
	};

#endif // WIN32

	nshared_lib::nshared_lib(void)
	{
	}

	nshared_lib::~nshared_lib(void)
	{
	}

	nshared_lib::ptr nshared_lib::load_lib(const nstring& file_name)
	{
#ifdef WIN32
		shared_ptr<nwin32_dll> dll(nNew nwin32_dll);
		dll->load(file_name);
		return dynamic_pointer_cast<nshared_lib>(dll);
#endif
	}
}//namespace nexus