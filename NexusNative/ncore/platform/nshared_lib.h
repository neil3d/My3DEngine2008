/**
*	nexus core - Platform
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _NEXUS_SHARED_LIB_H_
#define _NEXUS_SHARED_LIB_H_

#include "../base_define.h"
#include "../base_depend.h"

namespace nexus
{
	/**
	 *	封装Win32 DLL或者Linux so
	 *	@remark 由于插件的class接口都使用nobject的reflection机制，所以无需提供dll export相关接口
	*/
	class nCoreAPI nshared_lib
	{
	public:
		typedef shared_ptr<nshared_lib> ptr;

		nshared_lib(void);
		virtual ~nshared_lib(void);

		static nshared_lib::ptr load_lib(const nstring& file_name);

	private:
		virtual void load(const nstring& file_name) = 0;
	};
}//namespace nexus
#endif //_NEXUS_SHARED_LIB_H_