#pragma once
#include "game_unit.h"

/**
 * 游戏地图中的玩家
*/
class game_player :	public game_unit
{
public:
	typedef boost::shared_ptr<game_player> ptr;

	game_player(void);
	virtual ~game_player(void);

private:

};
