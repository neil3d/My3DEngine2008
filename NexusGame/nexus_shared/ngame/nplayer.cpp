#include "nplayer.h"
#include "nclient_session.h"
#include "nenums.h"
#include "ntimer.h"
#include "nmap_mgr.h"
#include "nlog_mt.h"
#include "nutil.h"

#include "nevent_args.h"

#include "nmsg_map.h"
#include "nmsg_movement.h"

namespace nexus {

	nplayer::nplayer() : nunit(), m_session_ptr(NULL), m_load_flag(EPLF_All), m_is_teleporting(false), m_teleport_dest_yaw(0.0f), 
		m_teleport_dest_map_id(0), m_teleport_dest_instance_id(0), m_casted_spell_id(0), m_target_id(0), m_time_stamp(0.0f), m_cast_count(0), m_cast_index(0), 
		m_min_repeat_time(0), m_max_repeat_time(0), m_periodic(0), m_resurrect(0)
	{
		memset(m_nude_stats, 0, sizeof(m_nude_stats));
	}

	nplayer::~nplayer()
	{
	}

	bool nplayer::init(nclient_session* session_ptr, uint64 guid)
	{
		m_session_ptr = session_ptr;
		set_id(guid);
		m_data.init(this, EPF_PlayerEnd);
		register_event();
		return true;
	}

	void nplayer::destroy()
	{
		// TODO: D. Zhao add player destroy


		nunit::destroy();
	}

	void nplayer::update(uint32 elapse)
	{
		nunit::update(elapse);
		// TODO: D. Zhao add player update

		_update_repeat_spell(elapse);


		// test
		_resurrect(elapse);
	}

	void nplayer::make_enter_msg(void* data_ptr, uint32& size) const
	{
		tagS2C_PlayerEnterAoi* packet = static_cast<tagS2C_PlayerEnterAoi*>(data_ptr);
		packet->client_id	= 0;
		packet->msg_id		= S2C_PlayerEnterAoi;
		packet->player_id	= get_id();
		packet->position		= get_position();
		packet->yaw				= get_rotation().y;
		packet->acceleration	= get_acceleration();
		packet->time_stamp		= g_game_timer.get_time_stamp();
		packet->flag			= get_movement_flag();
		size				= sizeof(tagS2C_PlayerEnterAoi);
	}

	bool nplayer::teleport_to(float x, float y, float z, float yaw, uint32 map_id, uint32 instance_id)
	{
		if (m_is_teleporting)
		{
			return false;
		}

		// 校验参数的合法性
		if (!g_map_mgr.is_valid_coord(x, y, z, yaw, map_id))
		{
			NLOG_ERROR(_T("teleport_to invalid coord message, player id = %u"), get_id());
			return false;
		}

		// TODO: D. Zhao 各种限制判断

		if (get_map_id() != map_id)
		{
			// TODO: D. Zhao 各种限制判断
		}
		else
		{

		}


		// 保存传送目的地
		m_is_teleporting				= true;
		m_teleport_dest_pos.x			= x;
		m_teleport_dest_pos.y			= y;
		m_teleport_dest_pos.z			= z;
		m_teleport_dest_yaw				= yaw;
		m_teleport_dest_map_id			= map_id;
		m_teleport_dest_instance_id		= instance_id;

		tagS2C_TeleportTo request;
		request.map_id	= m_teleport_dest_map_id;
		request.pos		= m_teleport_dest_pos;
		request.yaw		= m_teleport_dest_yaw;
		send_message(&request, sizeof(request));
		return true;
	}

	bool nplayer::register_event()
	{
		nunit::register_event();

		return true;
	}

	void nplayer::on_move(const nevent_base_args* base_args)
	{
		nunit::on_move(base_args);

		const nevent_move_args* data_ptr = static_cast<const nevent_move_args*>(base_args);

		// 判断是否打断连续释放
		if (data_ptr->only_turn)
		{
			_repeat_spell_interrupt(ESIF_Turning);
		}
		else
		{
			_repeat_spell_interrupt(ESIF_Move);
		}
	}

	void nplayer::on_dead(const nevent_base_args* base_args)
	{
		nunit::on_dead(base_args);

		// 如果处于连续释放, 打断
		if (has_flag(EUS_CycleCasting))
		{
			cancel_spell();
		}
		// 5秒后复活
		m_resurrect = 1000 * 5;
	}

} // namespace nexus
