/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_D3D_EXCEPTION_H_
#define _NEXUS_D3D_EXCEPTION_H_
#include "ncore.h"

#include <DXErr.h>
#pragma comment(lib,"DXErr.lib")

namespace nexus
{
	class d3d_exception : public nexception
	{
	public:
		d3d_exception(const TCHAR* info, const TCHAR* source_info):nexception(info, source_info)
		{}

		d3d_exception(HRESULT hr, const TCHAR* info, const TCHAR* source_info)
			: nexception(info, DXGetErrorString(hr), source_info)
		{}

	};

#define THROW_D3D_EXCEPTION(info) throw d3d_exception(info, _T(__FUNCTION__))
#define THROW_D3D_HRESULT(hr, info) throw d3d_exception(hr, info, _T(__FUNCTION__))
}//namespace nexus

#endif //_NEXUS_D3D_EXCEPTION_H_