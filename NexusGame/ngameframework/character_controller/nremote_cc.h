#ifndef _NEXUS_GAMEFRAMEWORK_NREMOTE_CC_H_
#define _NEXUS_GAMEFRAMEWORK_NREMOTE_CC_H_
#include "ncore.h"
#include "ncharacter_controller.h"

namespace gameframework
{
	/**
	* 远程角色控制器，只要职责是接受移动消息，处理移动消息控制的角色运动
	*/
	class nGameFrameworkAPI nremote_cc
		: public ncharacter_controller
	{
	public:
		typedef boost::shared_ptr<nremote_cc> ptr;

		nremote_cc();
		virtual ~nremote_cc();
	};

} // end of namespace gameframework

#endif // end of _NEXUS_GAMEFRAMEWORK_NREMOTE_CC_H_