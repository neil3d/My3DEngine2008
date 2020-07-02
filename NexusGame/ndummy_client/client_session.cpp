#include "StdAfx.h"
#include "client_session.h"
#include "client.h"
#include "client_define.h"
#include "msg_queue.h"
#include "local_player.h"
#include "game_map.h"
#include "game_app.h"
#include "timer/ntimestamp.h"

#include "nmsg_login.h"
#include "nmsg_char.h"
#include "nmsg_loading.h"
#include "nmsg_map.h"
#include "nmsg_stat.h"
#include "nmsg_movement.h"
#include "nmsg_chat.h"
#include "nmsg_spell.h"

#include "ConsoleDlg.h"
#include "nobject_define.h"

#define MSG_STATIC_CAST(DATA_PTR, MSG_PTR, MSG_TYPE) \
	const tag##MSG_TYPE* DATA_PTR = static_cast<const tag##MSG_TYPE*>(MSG_PTR);

#define	OFFSET		(sizeof(nsafe_mem_pool*) + sizeof(client_msg*) + sizeof(uint32))

// 定长消息长度检查
#define CHECK_MSG_SIZE(MSG_PTR, MSG_TYPE) \
	if ((reinterpret_cast<const client_msg*>(reinterpret_cast<const char*>(msg_ptr) - OFFSET))->size != sizeof(tag##MSG_TYPE)) \
	{ nLog_Info(_T("msg size error!")); return; }

client_session::client_session() : m_status(ECSS_None), m_lag(0), m_avg_lag(0), m_lag_count(0)
{
}

client_session::~client_session()
{
}

client_session* client_session::instance()
{
	static client_session s_inst;
	return &s_inst;
}

void client_session::handle_login_proof_result(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_LoginProofResult message, size = %u"), sizeof(tagS2C_LoginProofResult));

	CHECK_MSG_SIZE(msg_ptr, S2C_LoginProofResult);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_LoginProofResult);

	if (EError_Success == data_ptr->error)
	{
		m_status = ECSS_Logined;
	nLog_Info(_T("login sucess!!!"));

		tagC2S_CharEnum request;
		client::instance()->send(&request, sizeof(request));
}
	else
	{
		nLog_Error(_T("login error!!!"));
	}
}

void client_session::handle_char_enum(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_CharEnum message, size = %u"), sizeof(tagS2C_CharEnum));

	CHECK_MSG_SIZE(msg_ptr, S2C_CharEnum);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_CharEnum);

	nLog_Info(_T("Char data"));
}

void client_session::handle_char_create(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_CharCreate message, size = %u"), sizeof(tagS2C_CharCreate));

	CHECK_MSG_SIZE(msg_ptr, S2C_CharCreate);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_CharCreate);

	nLog_Info(_T("new char data"));
}

void client_session::handle_char_delete(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_CharDelete message, size = %u"), sizeof(tagS2C_CharDelete));

	CHECK_MSG_SIZE(msg_ptr, S2C_CharDelete);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_CharDelete);

	nLog_Info(_T("delete char data"));
}

void client_session::handle_char_enter_game(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_CharEnterGame message, size = %u"), sizeof(tagS2C_CharEnterGame));

	CHECK_MSG_SIZE(msg_ptr, S2C_CharEnterGame);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_CharEnterGame);

	nLog_Info(_T("enter game sucess!"));

	m_status = ECSS_Loading;

	local_player::instance()->set_id(data_ptr->player_id);
	local_player::instance()->set_pos(data_ptr->pos);
	local_player::instance()->set_rotation(vector3(0.0,data_ptr->yaw,0.0));

	// 向服务器loading数据
	tagC2S_GetInitStat request;
	client::instance()->send(&request, sizeof(request));
}

void client_session::handle_get_init_stat(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_GetInitStat message, size = %u"), sizeof(tagS2C_GetInitStat));

	CHECK_MSG_SIZE(msg_ptr, S2C_GetInitStat);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_GetInitStat);

	local_player::instance()->get_data().on_recv_init_stat(data_ptr->stat_data);

	//for (uint16 i = EUF_UnitNull + 1; i < EUF_UnitMeleeDamage; ++i)
	//{
	//	nLog_Info(_T("player stat, (%u, %u)"), i, local_player::instance()->get_data().get_uint32_value(i));
	//}

	//for (uint16 i = EUF_UnitMeleeDamage; i < EUF_UnitTotalWeight; ++i)
	//{
	//	nLog_Info(_T("player stat, (%u, %f)"), i, local_player::instance()->get_data().get_float_value(i));
	//}

	//for (uint16 i = EUF_UnitTotalWeight; i < EPF_PlayerExp; ++i)
	//{
	//	nLog_Info(_T("player stat, (%u, %u)"), i, local_player::instance()->get_data().get_uint32_value(i));
	//}

	tagC2S_GetInitSpell request;
	client::instance()->send(&request, sizeof(request));
}

void client_session::handle_get_init_spell(const nmsg_base* msg_ptr)
{
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_GetInitSpell);

	// 变长消息，检查消息长度
	if ((reinterpret_cast<const client_msg*>(reinterpret_cast<const char*>(msg_ptr) - OFFSET))->size != data_ptr->get_packet_size())
	{
		nLog_Info(_T("msg size error!"));
		return;
	}

	nLog_Info(_T("recv S2C_GetInitSpell message, size = %u"), data_ptr->get_packet_size());

	tagC2S_GetInitAura request;
	client::instance()->send(&request, sizeof(request));
}

void client_session::handle_get_init_aura(const nmsg_base* msg_ptr)
{
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_GetInitAura);

	// 变长消息，检查消息长度
	if ((reinterpret_cast<const client_msg*>(reinterpret_cast<const char*>(msg_ptr) - OFFSET))->size != data_ptr->get_packet_size())
	{
		nLog_Info(_T("msg size error!"));
		return;
	}

	nLog_Info(_T("recv S2C_GetInitAura message, size = %u"), data_ptr->get_packet_size());

	m_status = ECSS_Loaded;

	tagC2S_MapEnter request;
	client::instance()->send(&request, sizeof(request));
}

void client_session::handle_map_enter(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_MapEnter message, size = %u"), sizeof(tagS2C_MapEnter));

	CHECK_MSG_SIZE(msg_ptr, S2C_MapEnter);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_MapEnter);

	m_status = ECSS_Gaming;

	nLog_Info(_T("enter map sucess!"));
}

void client_session::handle_map_teleport_to(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_TeleportTo message, size = %u"), sizeof(tagS2C_TeleportTo));

	CHECK_MSG_SIZE(msg_ptr, S2C_TeleportTo);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_TeleportTo);

	local_player::instance()->move(data_ptr->pos, vector3(0.0f, data_ptr->yaw, 0.0f), vector3::zero, 0, EMove_Ground);

	tagC2S_TeleportAck request;
	client::instance()->send(&request, sizeof(request));
}

void client_session::handle_stat_update_stat(const nmsg_base* msg_ptr)
{
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_StatUpdate);

	// 变长消息，检查消息长度
	if ((reinterpret_cast<const client_msg*>(reinterpret_cast<const char*>(msg_ptr) - OFFSET))->size != data_ptr->get_packet_size())
	{
		nLog_Info(_T("msg size error!"));
		return;
	}

	nLog_Info(_T("recv S2C_StatUpdate message, size = %u"), data_ptr->get_packet_size());

	bool flag = local_player::instance()->has_flag(EUS_Dead);

	local_player::instance()->get_data().on_recv_stat_update(data_ptr->stat_data);

	bool new_flag = local_player::instance()->has_flag(EUS_Dead);

	if (flag != new_flag)
	{
		if (new_flag)
		{
			nLog_Info(_T("you are dead!"));
}
		else
		{
			nLog_Info(_T("you are resurrect!"));
		}
	}
}

void client_session::handle_remote_stat_update_stat(const nmsg_base* msg_ptr)
{
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_RemoteStatUpdate);

	// 变长消息，检查消息长度
	if ((reinterpret_cast<const client_msg*>(reinterpret_cast<const char*>(msg_ptr) - OFFSET))->size != data_ptr->get_packet_size())
	{
		nLog_Info(_T("msg size error!"));
		return;
	}

	nLog_Info(_T("recv S2C_RemoteStatUpdate message, size = %u"), data_ptr->get_packet_size());

	game_unit::ptr unit_ptr = game_map::instance()->get_game_unit(data_ptr->unit_id);

	if (unit_ptr)
	{
		unit_ptr->get_data().on_recv_remote_stat_update(data_ptr->stat_data);
	}
	else
	{
		nLog_Info(_T("get unit error"));
	}
}

void client_session::handle_player_enter_aoi(const nmsg_base* msg_ptr)
{
	// nLog_Info(_T("recv S2C_PlayerEnterAoi message, size = %u"), sizeof(tagS2C_PlayerEnterAoi));

	CHECK_MSG_SIZE(msg_ptr, S2C_PlayerEnterAoi);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_PlayerEnterAoi);

	if (local_player::instance()->get_id() != data_ptr->player_id)
	{
		game_unit::ptr unit_ptr = game_map::instance()->get_game_unit(data_ptr->player_id);

		if (!unit_ptr)
		{
			unit_ptr.reset(new game_player);

			unit_ptr->set_id(data_ptr->player_id);

			game_map::instance()->add_remote_unit(unit_ptr);

			unit_ptr->move(data_ptr->position, vector3(0.0f, data_ptr->yaw, 0.0f), data_ptr->acceleration, data_ptr->time_stamp, data_ptr->flag);
		}
		else
		{
			nLog_Info(_T("enter player exit"));
	}
	}
	else
	{
		local_player::instance()->move(data_ptr->position, vector3(0.0f, data_ptr->yaw, 0.0f), data_ptr->acceleration, data_ptr->time_stamp, data_ptr->flag);
	}

	//nchar buff2[300] = {0};

	//_stprintf_s(buff2, _T("aoi unit num = %u"), game_map::instance()->get_remote_unit_num());
	//CConsoleDlg::instance()->print_line(buff2);
	// nLog_Info(_T("aoi unit num = %u"), game_map::instance()->get_remote_unit_num());
}

void client_session::handle_player_move(const nmsg_base* msg_ptr)
{
	// nLog_Info(_T("recv S2C_Move message, size = %u"), sizeof(tagS2C_Move));

	CHECK_MSG_SIZE(msg_ptr, S2C_Move);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_Move);

	game_unit::ptr unit_ptr = game_map::instance()->get_game_unit(data_ptr->unit_id);

	if (unit_ptr)
	{
		// 计算位置误差
		float dis = vec_distance(unit_ptr->get_pos(), data_ptr->position);
		unit_ptr->move(data_ptr->position, vector3(0.0f, data_ptr->yaw, 0.0f), data_ptr->acceleration, data_ptr->time_stamp, data_ptr->flag);

		float cur = game_app::instance()->get_game_time();
		float lag = cur > data_ptr->time_stamp ? cur - data_ptr->time_stamp : 0.0f;

		// if (lag > 0.5f || dis > 100.0f)
		//{
		//	nchar buff2[100] = {0};
		//	_stprintf_s(buff2, _T("aoi lag time = %d, aoi dis = %f"), uint32(lag * 1000), dis);
			// CConsoleDlg::instance()->print_line(buff2);
		//}

	}
	else if (local_player::instance()->get_id() == data_ptr->unit_id)
	{
		// 计算位置误差
		float dis = vec_distance(local_player::instance()->get_pos(), data_ptr->position);

		// local_player::instance()->move(data_ptr->position, vector3(0.0f, data_ptr->yaw, 0.0f), data_ptr->acceleration, data_ptr->time_stamp, data_ptr->flag);

		float cur = game_app::instance()->get_game_time();
		float lag = cur > data_ptr->time_stamp ? cur - data_ptr->time_stamp : 0.0f;

		//if (lag > 0.5f || dis > 100.0f)
		//{
		//	nchar buff2[100] = {0};
		//	_stprintf_s(buff2, _T("aoi lag time = %d, aoi dis = %f"), uint32(lag * 1000), dis);
			// CConsoleDlg::instance()->print_line(buff2);
		//}
	}
	else
	{
		nLog_Info(_T("player move error can't find the unit"));
	}
}

void client_session::handle_object_leave_aoi(const nmsg_base* msg_ptr)
{
	// nLog_Info(_T("recv S2C_ObjectLeaveAoi message, size = %u"), sizeof(tagS2C_ObjectLeaveAoi));

	CHECK_MSG_SIZE(msg_ptr, S2C_ObjectLeaveAoi);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_ObjectLeaveAoi);

	if (local_player::instance()->get_id() != data_ptr->object_id)
	{
	game_unit::ptr unit_ptr = game_map::instance()->get_game_unit(data_ptr->object_id);

	if (!unit_ptr)
	{
		nLog_Info(_T("can't find the object leave aoi"));
		return;
	}

	game_map::instance()->remove_remote_unit(data_ptr->object_id);
}

	//nchar buff2[300] = {0};
	//_stprintf_s(buff2, _T("aoi unit num = %u"), game_map::instance()->get_remote_unit_num());
	//CConsoleDlg::instance()->print_line(buff2);
	 ///nLog_Info(_T("aoi unit num = %u"), game_map::instance()->get_remote_unit_num());
	
}

void client_session::handle_check_time(const nmsg_base* msg_ptr)
{
	CHECK_MSG_SIZE(msg_ptr, S2C_CheckTime);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_CheckTime);

	game_app::instance()->set_game_time(data_ptr->server_time_stamp + (get_time() - data_ptr->client_time_stamp) / 2.0f);
}

// chat
void client_session::handle_player_chat(const nmsg_base* msg_ptr)
{
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_ChatMessage);

	// 变长消息，检查消息长度
	if ((reinterpret_cast<const client_msg*>(reinterpret_cast<const char*>(msg_ptr) - OFFSET))->size != data_ptr->get_packet_size())
	{
		nLog_Info(_T("msg size error!"));
		return;
	}

	// nLog_Info(_T("recv S2C_ChatMessage message, size = %u"), data_ptr->get_packet_size());

	nchar buff[MAX_MSG_LEN + 1] = {0};
	nchar buff2[300] = {0};

	if (ECT_Channel == data_ptr->type)
	{
		m_lag = ::timeGetTime() - data_ptr->msg2.channel_id;

		const tagTimeStamp* data_ptr2 = reinterpret_cast<const tagTimeStamp*>(data_ptr->msg2.msg);

		m_avg_lag = data_ptr2->time_stamp2 - data_ptr2->time_stamp1;
		m_lag_count = (m_lag - m_avg_lag) / 2;

		if (m_avg_lag > 1000)
		{
			nLog_Info(_T("g2w send time = %u, w2g send time = %u"), data_ptr2->time_stamp3 - data_ptr2->time_stamp1, data_ptr2->time_stamp2 - data_ptr2->time_stamp3);
		}
	}
	else
	{
		memcpy(buff, data_ptr->msg1.msg, data_ptr->msg_size);
	_stprintf_s(buff2, _T("player_id = %u, type = %u, flag = %u, chat : %s"), PAIR64_LOPART(data_ptr->player_id), data_ptr->type, data_ptr->flag, buff);
}

//	nLog_Info(_T("type = %u, flag = %u, chat : %s"), data_ptr->type, data_ptr->flag, buff);

	// CConsoleDlg::instance()->print_line(buff2);
}

void client_session::handle_cast_spell_result(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_CastSpellResult message, size = %u"), sizeof(tagS2C_CastSpellResult));

	CHECK_MSG_SIZE(msg_ptr, S2C_CastSpellResult);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_CastSpellResult);

	return;
}

void client_session::handle_spell_prepare(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_SpellPrepare message, size = %u"), sizeof(tagS2C_SpellPrepare));

	CHECK_MSG_SIZE(msg_ptr, S2C_SpellPrepare);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_SpellPrepare);

	return;
}

void client_session::handle_spell_casting(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_SpellCasting message, size = %u"), sizeof(tagS2C_SpellCasting));

	CHECK_MSG_SIZE(msg_ptr, S2C_SpellCasting);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_SpellCasting);

	return;
}

void client_session::handle_spell_failed(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_SpellFailed message, size = %u"), sizeof(tagS2C_SpellFailed));

	CHECK_MSG_SIZE(msg_ptr, S2C_SpellFailed);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_SpellFailed);

	return;
}

void client_session::handle_spell_hit(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_SpellHit message, size = %u"), sizeof(tagS2C_SpellHit));

	CHECK_MSG_SIZE(msg_ptr, S2C_SpellHit);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_SpellHit);

	return;
}

void client_session::handle_add_aura(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_AddAura message, size = %u"), sizeof(tagS2C_AddAura));

	CHECK_MSG_SIZE(msg_ptr, S2C_AddAura);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_AddAura);

	return;
}

void client_session::handle_remove_aura(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_RemoveAura message, size = %u"), sizeof(tagS2C_RemoveAura));

	CHECK_MSG_SIZE(msg_ptr, S2C_RemoveAura);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_RemoveAura);

	return;
}

void client_session::handle_update_aura(const nmsg_base* msg_ptr)
{
	nLog_Info(_T("recv S2C_UpdateAura message, size = %u"), sizeof(tagS2C_UpdateAura));

	CHECK_MSG_SIZE(msg_ptr, S2C_UpdateAura);
	MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_UpdateAura);

	return;
}
