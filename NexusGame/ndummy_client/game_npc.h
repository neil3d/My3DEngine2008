#pragma once
#include "game_unit.h"

/**
 *	游戏地图中的NPC、怪物对象
*/
class game_npc : public game_unit
{
public:
	typedef boost::shared_ptr<game_npc> ptr;

	game_npc(void);
	virtual ~game_npc(void);
};
