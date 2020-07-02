/**
*	nexus core - Exception
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _NEXUS_EXCEPTION_H_
#define _NEXUS_EXCEPTION_H_
#include "../nlog.h"

namespace nexus
{
	/** nexus engine所使用的异常基类 */
	class nCoreAPI nexception
	{
	public:
		nexception(const TCHAR* info, const TCHAR* source_info);
		nexception(const TCHAR* info1, const TCHAR* info2, const TCHAR* source_info);
		
		virtual ~nexception()
		{}

		const nstring& what() const	{	return m_info; }

	private:
		nstring	m_info;
	};

#define nthrow(info) throw nexception(info, _T(__FUNCTION__));
#define nthrow2(info1, info2) throw nexception(info1, info2, _T(__FUNCTION__));

}// namespace nexus

#endif	// _NEXUS_EXCEPTION_H_