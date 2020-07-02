#ifndef _NEXUS_CORE_EVENT_ARG_H_
#define _NEXUS_CORE_EVENT_ARG_H_

namespace nexus
{
	/**
	* 参数基类,如果需要传递更多参数内容可以从该类派生
	*/
	class nevent_arg
	{
	public:
		// 是否被处理
		bool handled;
	};
}

#endif //_NEXUS_CORE_EVENT_ARG_H_
