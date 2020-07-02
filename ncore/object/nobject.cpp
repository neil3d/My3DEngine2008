#include "stdafx.h"
#include "nobject.h"
#include "narchive.h"
#include "../nlog.h"

namespace nexus
{
	//-- reflection
	nclass	nobject::m_class_nobject(_T("nobject"), NULL, nobject::reflection_alloc_object);
	nclass* nobject::reflection_get_class() const
	{	return &m_class_nobject; }
	nclass*	nobject::reflection_get_static_class()
	{	return &m_class_nobject; }
	nobject* nobject::reflection_alloc_object()
	{	return nNew nobject;}

	nDEFINE_VIRTUAL_CLASS(narchive, nobject)

	nobject::nobject(void)
	{
	}

	nobject::~nobject(void)
	{
	}

	void nobject::serialize(narchive& ar)
	{
		(void)ar;
	}

	//
	nexception::nexception(const TCHAR* info, const TCHAR* source_info)
	{
		m_info = source_info;
		m_info += _T("() : ");
		m_info += info;

		nLog_Exception(m_info.c_str());
	}

	nexception::nexception(const TCHAR* info1, const TCHAR* info2, const TCHAR* source_info)
	{
		m_info = source_info;
		m_info += _T("() : ");
		m_info += info1;
		m_info += _T(" , ");
		m_info += info2;
		
		nLog_Exception(m_info.c_str());
	}
}//namespace nexus