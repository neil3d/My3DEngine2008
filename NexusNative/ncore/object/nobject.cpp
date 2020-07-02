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
		wostringstream os;
		os << _T("native source info : [ ")
			<< source_info << _T("() ]\r\n")
			<< info;
		m_info = os.str();

		nLog_Exception(m_info.c_str());
	}

	nexception::nexception(const TCHAR* info1, const TCHAR* info2, const TCHAR* source_info)
	{
		wostringstream os;
		os << _T("native source info : [ ")
			<< source_info << _T("() ]\r\n")
			<< info1 << " , "
			<< info2;
		m_info = os.str();
		
		nLog_Exception(m_info.c_str());
	}
}//namespace nexus