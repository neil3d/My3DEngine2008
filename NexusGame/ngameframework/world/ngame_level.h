#ifndef _NEXUS_GAMEFRAMEWORK_NGAME_LEVEL_H_
#define _NEXUS_GAMEFRAMEWORK_NGAME_LEVEL_H_
#include "ncore.h"
#include "math/ncheck_result.h"
#include "game_map/navigation_map.h"

namespace gameframework
{
	class ncharacter_controller;

	/**
	* 游戏关卡
	*/
	class nGameFrameworkAPI ngame_level
		: public nexus::nobject
	{
	public:
		typedef boost::shared_ptr<ngame_level> ptr;	

		ngame_level(){}
		virtual ~ngame_level(){}

		// 指定关卡使用的导航图
		virtual void attach_nav_map(navigation_map::ptr nav_map);

		// 尝试移动角色
		virtual bool try_move_character(ncharacter_controller* cc, const vector3& start, const vector3& disp,float setp_height, ncheck_result& hit);
		
		// 检测目标点是否传送可以达到
		virtual bool try_transport(vector3& dest, bool place_nav_above);

	private:
		// level最重要的组成部分导航图
		navigation_map::ptr m_nav_map;

	};

} // end of namespace gameframework

#endif // end of _NEXUS_GAMEFRAMEWORK_NGAME_LEVEL_H_