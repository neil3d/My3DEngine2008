#pragma once

#include "ncommon.h"
#include "game_unit.h"
#include "game_map/navigation_map.h"

class game_canvas;
class console_cmd;

/**
 *	游戏当前的地图
 *	@remarks 管理导航图+AOI范围内的所有NPC、怪物、远程玩家等游戏对象
*/
class game_map
{
public:
	game_map(void);
	~game_map(void);

	static game_map* instance();
	
	//!	初始化
	void init();
	
	//!	
	void load_map(uint32 map_id);

	//!	更新所有内部对象的游戏逻辑
	void update(float delta_time);

	//!	向画布上画出所有对象
	void draw(game_canvas* cvs);

	bool add_remote_unit(game_unit::ptr remote_unit_ptr);
	void remove_remote_unit(uint64 unit_id);
	uint32 get_remote_unit_num();

	game_unit::ptr get_game_unit(uint64 id);

	//!	取得内部导航图对象指针
	navigation_map::ptr get_nav_map() const	{ return m_nav_map_ptr; }
	gameframework::ngame_level* get_nav_level() const	{ return m_game_level_ptr; }

	//!	地图有多少格子宽
	int get_map_width() const	{	return m_nav_map_ptr->get_raw_width(); }

	//!	地图有多少格子高
	int get_map_height() const	{	return m_nav_map_ptr->get_raw_height(); }
protected:
	//!	控制台命令函数：打印地图信息
	void cmd_map_info(const console_cmd& cmd);
protected:
	typedef std::map<uint64, game_unit::ptr>	UNIT_MAP;
	
	UNIT_MAP									m_aoi_units;			// AOI范围内的游戏对象
	
	navigation_map::ptr							m_nav_map_ptr;
	gameframework::ngame_level*					m_game_level_ptr;
	gameframework::nlevel_info_resource::ptr	m_level_res_ptr;
};
