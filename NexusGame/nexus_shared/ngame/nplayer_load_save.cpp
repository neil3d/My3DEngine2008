#include "nplayer.h"
#include "nenums.h"
#include "nlog_mt.h"
#include "nutil.h"
#include "ninfo_data.h"
#include "nspell_aura.h"

#include "nmsg_world_db.h"

#include "nmsg_loading.h"
#include "nmsg_char.h"

namespace nexus {

	void nplayer::request_load_data_from_db()
	{
		m_session_ptr->set_status(ESS_DBLoading);
		// 向db发送load玩家数据消息
		tagMsg_W2D_LoadPlayerData request;
		request.player_id = get_id();
		// send_message_to_db(&request, sizeof(request));


		tagMsg_D2W_LoadPlayerStat data;
		load_stat_from_db(&data);


		// TODO: D. Zhao, 加入一个功能去掉一个flag
		// set_load_flag(EPLF_Att);
		set_load_flag(EPLF_Item);
		set_load_flag(EPLF_Spell);
		set_load_flag(EPLF_Aura);
		set_load_flag(EPLF_CoolDown);
		set_load_flag(EPLF_Skills);
		set_load_flag(EPLF_Questing);
		set_load_flag(EPLF_QuestDone);
		set_load_flag(EPLF_SocialList);
		set_load_flag(EPLF_Tile);
		set_load_flag(EPLF_Pet);
		set_load_flag(EPLF_Reputation);
		set_load_flag(EPLF_Mail);
		set_load_flag(EPLF_Guild);
	}

	void nplayer::load_stat_from_db(const tagMsg_D2W_LoadPlayerStat* data_ptr)
	{
		assert(data_ptr);
		// TODO: D. Zhao 目前生成玩家的临时数据
		static uint64 base_id = 0;
		++base_id;

		static uint32 base_map_id = 0;
		++base_map_id;

		vector3 pos[5];

		pos[0].x = 0.0f;
		pos[0].y = 0.0f;
		pos[0].z = 0.0f;

		pos[1].x = 32768.0f;
		pos[1].y = 0.0f;
		pos[1].z = 32768.0f;

		pos[2].x = 32768.0f;
		pos[2].y = 0.0f;
		pos[2].z = -32768.0f;

		pos[3].x = -32768.0f;
		pos[3].y = 0.0f;
		pos[3].z = -32768.0f;

		pos[4].x = -32768.0f;
		pos[4].y = 0.0f;
		pos[4].z = 32768.0f;

		int32 x = s_util.irand(-5000, 5000);
		int32 z = s_util.irand(-5000, 5000);

		vector3 born;

		born.x = pos[base_id % 5].x + float(x);
		born.y = pos[base_id % 5].y;
		born.z = pos[base_id % 5].z + float(z);

		born.x = float(s_util.rand_norm() - 0.5) * 500.0f * 120.0f;
		born.y = 0;
		born.z = float(s_util.rand_norm() - 0.5) * 500.0f * 120.0f;

		int32 map_id = base_map_id % 4 + 1;

		uint64 guid = MAKE_OBJECT_GUID(base_id, 0, EOT_PLAYER);

		set_id(guid);
		// nworld_object::init(map_id, 0, born, vector3(0.0f,0.0f,0.0f));
		// nworld_object::init(1, 0, born, vector3(0.0f,0.0f,0.0f));
		nworld_object::init(1, 0, vector3(0.0f, 0.0f, 0.0f), vector3(0.0f,0.0f,0.0f));
		// nworld_object::init(1, 0, vector3(26944.0f, 3.0f, -19264.0f), vector3(0.0f,0.0f,0.0f));
		set_visible(true);

		uint32 stat_data[EPF_PlayerEnd - EUF_UnitSeparator] = {};
		stat_data[EUF_UnitLevel - EUF_UnitSeparator] = 5;
		stat_data[EPF_PlayerBaseStamina - EUF_UnitSeparator] = 4;
		stat_data[EUF_PlayerBaseVitality - EUF_UnitSeparator] = 3;
		stat_data[EUF_PlayerBaseSpirit - EUF_UnitSeparator] = 2;
		stat_data[EUF_PlayerBaseGenGu - EUF_UnitSeparator] = 1;
		stat_data[EPF_PlayerReserveStat - EUF_UnitSeparator] = 10;

		m_data.load_data(stat_data, EUF_UnitSeparator);

		m_data.set_uint64_value(EPF_PlayerExp, 100000);
		m_data.set_uint64_value(EPF_PlayerMoney, 200000);
		m_data.set_uint64_value(EPF_PlayerMoneyBind, 300000);

		m_nude_stats[EUF_UnitStamina] = 25.0f;
		m_nude_stats[EUF_UnitVitality] = 26.0f;
		m_nude_stats[EUF_UnitSpirit] = 28.0f;
		m_nude_stats[EUF_UnitGenGu] = 20.0f;

		// 设置其他数据

		set_load_flag(EPLF_Att);
	}

	void nplayer::load_item_from_db(const tagMsg_D2W_LoadPlayerItem* data_ptr)
	{
		assert(data_ptr);

	}

	void nplayer::load_spell_from_db(const tagMsg_D2W_LoadPlayerSpell* data_ptr)
	{
		assert(data_ptr);

		nplayer_spell_set::iterator iter;
		nspell_info const* spell_info_ptr = NULL;

		for (uint16 i = 0; i < data_ptr->num; ++i)
		{
			iter = m_spells.find(data_ptr->spell_data[i]);

			if (iter != m_spells.end())
			{
				NLOG_DEBUG(_T("has the same player spell, spell_id = %u"), data_ptr->spell_data[i]);
				continue;
	}

			if (false == m_spells.insert(data_ptr->spell_data[i]).second)
	{
				NLOG_DEBUG(_T("player spell insert error, spell_id = %u"), data_ptr->spell_data[i]);
				continue;
			}

			// 释放被动天赋技能, 被动技能, 注意释放顺序, 原则上先释放所有天赋技能(人物的物品的), 在被动天赋技能
			// 因为天赋技能可以对被动技能的释放操作影响, 天赋对被动技能的影响, 可以用叠加的方式不用修改
			// eg. 被动技能使你的MaxHP增加5%, 一个天赋技能又使这个效果增加100%, 最后配一个再加5%的技能, 使天赋不影响被动技能
		
			spell_info_ptr = s_spell_info.get_info(data_ptr->spell_data[i]);

			if (!spell_info_ptr)
			{
				NLOG_DEBUG(_T("s_spell_info.get_info error, spell_id = %u"), data_ptr->spell_data[i]);
				continue;
	}

			if (!spell_info_ptr->active)
	{
				this->cast_spell(data_ptr->spell_data[i], get_id(), vector3::zero, 0, 0, NULL, NULL);
			}
		}
	}

	void nplayer::load_aura_from_db(const tagMsg_D2W_LoadPlayerAura* data_ptr)
	{
		assert(data_ptr);

		naura* aura_ptr = NULL;
		nspell_aura_info const* aura_info_ptr = NULL;
		nspell_info const* spell_info_ptr = NULL;

		for (uint16 i = 0; i < data_ptr->num; ++i)
		{
			aura_info_ptr = s_spell_aura_info.get_info(data_ptr->aura_data[i].info_id);

			if (!aura_info_ptr)
			{
				NLOG_DEBUG(_T("s_spell_aura_info.get_info error, aura_id = %u"), data_ptr->aura_data[i].info_id);
				continue;
	}

			spell_info_ptr = NULL;

			if (0 != data_ptr->aura_data[i].spell_id)
			{
				spell_info_ptr = s_spell_info.get_info(data_ptr->aura_data[i].spell_id);

				if (!spell_info_ptr)
				{
					NLOG_DEBUG(_T("s_spell_info.get_info error, spell_id = %u"), data_ptr->aura_data[i].spell_id);
					continue;
				}
			}

			aura_ptr = naura::construct(this, data_ptr->aura_data[i].caster_id, aura_info_ptr, spell_info_ptr, data_ptr->aura_data[i].index);

			if (!aura_ptr)
			{
				NLOG_DEBUG(_T("allocate aura error!"));
				continue;
			}

			aura_ptr->set_max_stack(data_ptr->aura_data[i].stack_count);
			aura_ptr->set_max_duration(data_ptr->aura_data[i].max_duration);
			aura_ptr->set_duration(data_ptr->aura_data[i].remain_time);
			aura_ptr->set_amount(data_ptr->aura_data[i].amount);

			this->add_aura(aura_ptr);
		}
	}

	void nplayer::load_cool_down_from_db(const tagMsg_D2W_LoadPlayerCoolDown* data_ptr)
	{
		assert(data_ptr);

		for (uint16 i = 0; i < data_ptr->num; ++i)
		{
		}
	}

	void nplayer::send_init_stat()
	{
		tagS2C_GetInitStat reply;

		for (uint16 i = 0; i < EPF_PlayerEnd; ++i)
		{
			reply.stat_data[i] = m_data.get_uint32_value(i);
		}

		// 构造消息中其他数据
		send_message(&reply, sizeof(reply));
	}

	void nplayer::send_init_item()
	{

	}

	void nplayer::send_init_spell()
	{
		tagS2C_GetInitSpell reply;
		reply.num = 0;
		send_message(&reply, reply.get_packet_size());
	}

	void nplayer::send_init_aura()
	{
		tagS2C_GetInitAura reply;
		reply.num = 0;
		send_message(&reply, reply.get_packet_size());

		m_session_ptr->set_status(ESS_Loaded);
	}

	void nplayer::send_init_cool_down()
	{

	}

	bool nplayer::is_stat_ok() const
	{
		return EPLF_UpdateStat == m_load_flag;
	}

	void nplayer::set_load_flag(uint32 flag)
	{
		m_load_flag &= ~flag;

		if (true == is_stat_ok())
		{
			enable_modify_stats(true);
			_update_all_stats();
			set_health(m_data.get_uint32_value(EUF_UnitMaxHealth));
			set_energy(m_data.get_uint32_value(EUF_UnitMaxEnergy));
			set_mana(m_data.get_uint32_value(EUF_UnitMaxMana));

			m_data.clear_update_bits();

			//for (uint16 i = EUF_UnitNull + 1; i < EUF_UnitMeleeDamage; ++i)
			//{
			//	NLOG_DEBUG(_T("player stat, (%u, %u)"), i, m_data.get_uint32_value(i));
			//}

			//for (uint16 i = EUF_UnitMeleeDamage; i < EUF_UnitTotalWeight; ++i)
			//{
			//	NLOG_DEBUG(_T("player stat, (%u, %f)"), i, m_data.get_float_value(i));
			//}

			//for (uint16 i = EUF_UnitTotalWeight; i < EPF_PlayerExp; ++i)
			//{
			//	NLOG_DEBUG(_T("player stat, (%u, %u)"), i, m_data.get_uint32_value(i));
			//}
		}

		if (true == is_load_ok())
		{
			_on_load_data_completed();
		}
	}

	void nplayer::_on_load_data_completed()
	{
		m_session_ptr->set_status(ESS_Loading);

		tagS2C_CharEnterGame reply;
		reply.player_id = get_id();
		reply.map_id	= get_map_id();
		reply.pos		= get_position();
		reply.yaw		= get_rotation().y;
		send_message(&reply, sizeof(reply));
	}

	void nplayer::save_data_to_db()
	{

	}

} // namespace nexus
