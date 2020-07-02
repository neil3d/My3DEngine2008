#pragma once
#include "game_player.h"

/**
 *	本地玩家对象
*/
class local_player : public game_player
{
public:
	local_player(void);
	~local_player(void);

	static local_player* instance();

	virtual void update(float delta_time);

	//void register_att_change(handler);

	void set_move_dst_pos(const vector3& dst);

	void request_load_att();
	void request_load_item();
	void request_load_Spell();
	void request_load_Aura();
	void request_load_CoolDown();
	void request_load_Skills();
	void request_load_Questing();
	void request_load_QuestDone();
	void request_load_SocialList();
	void request_load_Tile();
	void request_load_Pet();
	void request_load_Reputation();
	void request_load_Mail();
	void request_load_Guild();

	void handle_load_att();
	void handle_load_item();
	void handle_load_Spell();
	void handle_load_Aura();
	void handle_load_CoolDown();
	void handle_load_Skills();
	void handle_load_Questing();
	void handle_load_QuestDone();
	void handle_load_SocialList();
	void handle_load_Tile();
	void handle_load_Pet();
	void handle_load_Reputation();
	void handle_load_Mail();
	void handle_load_Guild();

private:
	bool				m_moving;
	uint32				m_time_stamp;
	float				m_stop_elapse;
	vector3				m_move_dst;

	float				m_last_chat;
};
