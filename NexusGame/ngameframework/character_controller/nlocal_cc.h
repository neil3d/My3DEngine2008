#ifndef _NEXUS_GAMEFRAMEWORK_NLOCAL_CC_H_
#define _NEXUS_GAMEFRAMEWORK_NLOCAL_CC_H_
#include "ncore.h"
#include "ncharacter_controller.h"

namespace gameframework
{
	/**
	* 本地角色控制器，只要职责通过输入或者AI控制角色运动，并按照一定的策略发出移动消息
	*/
	class nGameFrameworkAPI nlocal_cc 
		: public ncharacter_controller
	{
	public:
		typedef boost::shared_ptr<nlocal_cc> ptr;

		nlocal_cc();
		virtual ~nlocal_cc();
	};

} // end of namespace gameframework

#endif // end of _NEXUS_GAMEFRAMEWORK_NLOCAL_CC_H_