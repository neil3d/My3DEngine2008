#include "nclient_session.h"
#include "nworld_session.h"
#include "nclient.h"
#include "nlocal_player.h"
#include "nlog_mt.h"
#include "ndefine.h"
#include "nmap_mgr.h"
#include "timer/ntimestamp.h"
#include "nperfmon_test.h"
#include "nutil.h"

#include "nmsg_login.h"
#include "nmsg_char.h"
#include "nmsg_loading.h"
#include "nmsg_map.h"
#include "nmsg_stat.h"
#include "nmsg_movement.h"
#include "nmsg_chat.h"
#include "nmsg_spell.h"

#include "nobject_define.h"

namespace nexus {

#define MSG_STATIC_CAST(DATA_PTR, MSG_PTR, MSG_TYPE) \
	const tag##MSG_TYPE* DATA_PTR = static_cast<const tag##MSG_TYPE*>(MSG_PTR);

	// 登陆的开始账号
	uint32	nclient_session::m_index_login_account	= 0;

	nclient_session::nclient_session() : m_session_id(0XFFFFFFFF), m_status(ECSS_None), m_player_ptr(NULL)
	{
	}

	nclient_session::~nclient_session()
	{
	}

	bool nclient_session::init(uint32 session_id)
	{
		m_session_id = session_id;
		m_status = ECSS_None;
		return true;
	}

	void nclient_session::destroy()
	{
		m_session_id = 0XFFFFFFFF;
		m_status = ECSS_None;

		if (m_player_ptr)
		{
			delete m_player_ptr;
		}
	}

	void nclient_session::update(uint32 elapse)
	{
		if (m_player_ptr && ECSS_Gaming == m_status)
		{
			m_player_ptr->update(elapse);
		}
	}

	bool nclient_session::send_message(const void* msg_ptr, uint32 size)
	{
		if (!msg_ptr || 0 == size)
		{
			return false;
		}

		if (0 == (m_status & (ECSS_Connected | ECSS_Logined | ECSS_Loading | ECSS_Loaded | ECSS_Gaming)))
		{
			NLOG_ERROR(_T("send msg error, not connect!"));
			return false;
		}

		return g_client.send_msg(m_session_id, msg_ptr, size);
	}

	void nclient_session::handle_connect(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, CI_Connect);

		switch (data_ptr->param1)
		{
		case ECS_Connected:
			{
				set_status(ECSS_Connected);

				tagC2S_LoginProof request;
				// add by leo
				make_proof_msg(&request, m_session_id);

				send_message(&request, sizeof(request));
			}
			break;
		case ECS_ConnectTimeout:
			{
				set_status(ECSS_ConnectTimeout);
				NLOG_ERROR(_T("ECSS_ConnectTimeout error!"));
			}
			break;
		case ECS_ConnectAborted:
			{
				set_status(ECSS_ConnectError);
				NLOG_ERROR(_T("ECSS_ConnectError error!"));
			}
			break;
		case ECS_Error:
			{
				set_status(ECSS_ConnectError);
				NLOG_ERROR(_T("ECSS_ConnectError error!"));
			}
			break;
		default:
			{
				set_status(ECSS_ConnectError);
				NLOG_ERROR(_T("ECSS_ConnectError error!"));
			}
			break;
		}
	}

	void nclient_session::handle_disconnect(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, CI_DisConnect);
		set_status(ECSS_Disconnect);

		g_world_session.remove_session(m_session_id);
		this->destroy();
		delete this;

		NLOG_ERROR(_T("nclient_session disconnect!"));
	}

	void nclient_session::handle_login_proof_result(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_LoginProofResult);

		if (EError_Success == data_ptr->error)
		{
			set_status(ECSS_Logined);

			tagC2S_CharEnum request;
			send_message(&request, sizeof(request));
		}
		else
		{
			NLOG_ERROR(_T("S2C_LoginProofResult error!"));
		}
	}

	void nclient_session::handle_char_enum(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_CharEnum);

		// 创建nlocal_player
		m_player_ptr = new nlocal_player;

		if (!m_player_ptr)
		{
			NLOG_ERROR(_T("alloc nlocal_player error!"));
			return;
		}

		m_player_ptr->init(this);

		nmap* map_ptr = g_map_mgr.get_map(1, 0);

		if (!map_ptr)
		{
			NLOG_ERROR(_T("get map error!"));
			return;
		}

		m_player_ptr->set_map(map_ptr);

		set_status(ECSS_Loading);

		tagC2S_CharEnterGame request;
		request.player_id = 125;
		send_message(&request, sizeof(request));
	}

	void nclient_session::handle_char_create(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_char_delete(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_char_enter_game(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_CharEnterGame);

		m_status = ECSS_Loading;

		m_player_ptr->set_id(data_ptr->player_id);
		m_player_ptr->set_position(data_ptr->pos);
		m_player_ptr->set_rotation(vector3(0.0f, data_ptr->yaw, 0.0f));
		m_player_ptr->set_acceleration(vector3::zero);

		// 1. 请求加载属性数据
		tagC2S_GetInitStat request;
		send_message(&request, sizeof(request));
	}

	void nclient_session::handle_get_init_stat(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_GetInitStat);

		m_player_ptr->get_data().on_recv_init_stat(data_ptr->stat_data);

		tagC2S_GetInitSpell request;
		send_message(&request, sizeof(request));
	}

	void nclient_session::handle_get_init_spell(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_GetInitSpell);

		tagC2S_GetInitAura request;
		send_message(&request, sizeof(request));
	}

	void nclient_session::handle_get_init_aura(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_GetInitAura);

		m_status = ECSS_Loaded;

		tagC2S_MapEnter request;
		send_message(&request, sizeof(request));
	}

	void nclient_session::handle_map_enter(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_MapEnter);
		m_status = ECSS_Gaming;
	}

	void nclient_session::handle_map_teleport_to(const nmsg_base* msg_ptr)
	{

	}

	// stat update
	void nclient_session::handle_stat_update_stat(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_StatUpdate);

		m_player_ptr->get_data().on_recv_stat_update(data_ptr->stat_data);
	}

	void nclient_session::handle_remote_stat_update_stat(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_player_enter_aoi(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_player_move(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_object_leave_aoi(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_check_time(const nmsg_base* msg_ptr)
	{
		MSG_STATIC_CAST(data_ptr, msg_ptr, S2C_CheckTime);

		nperfmon_test::instance().set_time_stamp(data_ptr->server_time_stamp + (get_time() - data_ptr->client_time_stamp) / 2.0f);
	}

	// chat
	void nclient_session::handle_player_chat(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_cast_spell_result(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_spell_prepare(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_spell_casting(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_spell_failed(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_spell_hit(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_add_aura(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_remove_aura(const nmsg_base* msg_ptr)
	{

	}

	void nclient_session::handle_update_aura(const nmsg_base* msg_ptr)
	{

	}

	// 生成登陆验证消息
	void nclient_session::make_proof_msg(void* tag_c2s_login_proof, uint32 index)
	{
		if(!tag_c2s_login_proof) return;

		//登陆验证消息
		tagC2S_LoginProof* proof_msg = (tagC2S_LoginProof*)tag_c2s_login_proof;

		index += m_index_login_account;

		//版本
		proof_msg->cur_version	= 1;

		//生成账号ID
		char name[MAX_ACCOUNT_LEN];
		memset(name, 0, sizeof(name));
		sprintf_s(name, MAX_ACCOUNT_LEN, "nexus_%d", index);
		proof_msg->account_id	= s_util.crc32(name, (uint32)strlen(name));

		//生成密码
		char psd[MAX_MD5_SIZE];
		memset(psd, 0, sizeof(psd));
		sprintf_s(psd, MAX_MD5_SIZE, "nexus_%d", index);
		memcpy(proof_msg->psd, psd, strlen(psd));

		//登陆IP
		proof_msg->ip			= 1;

		//mac地址
		memset(proof_msg->mac, 1, sizeof(proof_msg->mac));
	}
} // namespace nexus