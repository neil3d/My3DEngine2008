#include "ngm_commands.h"
#include "nclient_session.h"
#include "nlog_mt.h"
#include "nplayer.h"
#include "nmap.h"
#include "nregion.h"

namespace nexus {

	void ngm_commands::_handle_debug_test(const ncommand_param& param, nclient_session* session_ptr)
	{
		uint32 info_id = 0, num = 0;
		float dot = 5.0f;
		tstring str;
		param.get_param(0, info_id);
		param.get_param(1, num);
		param.get_param(2, dot);
		param.get_param(3, str);
		return;
	}

	void ngm_commands::_handle_map_get_objects_test(const ncommand_param& param, nclient_session* session_ptr)
	{
		if (!session_ptr)
		{
			return;
		}

		enum ERegionType
		{
			ERT_Circle			= 0,
			ERT_FanShaped		= 1,
			ERT_Rectangle		= 2,
		};

		uint32 type = 0;
		uint32 include_slef = 0;
		param.get_param(0, type);
		param.get_param(1, include_slef);

		NLOG_DEBUG(_T("recv map_get_objects command, type = %u, include_slef = %s"), type, include_slef != 0 ? _T("true") : _T("false"));

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			NLOG_DEBUG(_T("session get_player error"));
			return;
		}

		nmap* map_ptr = player_ptr->get_map();

		if (!map_ptr)
		{
			NLOG_DEBUG(_T("not in map, cant test .map_get_objects"));
			return;
		}

		vector3 pos = player_ptr->get_position();
		float yaw = player_ptr->get_rotation().y;
		float radius = 1000.0f;
		float angle = 2.09f;
		bool in_self = (include_slef != 0);
		std::list<nworld_object*> objs;

		switch (type)
		{
		case ERT_Circle:
			{
				ncircle_region region;
				region.set_param(pos, radius);
				map_ptr->get_objects(player_ptr, &region, objs, EOT_PLAYER | EOT_CREATURE, in_self);
			}
			break;
		case ERT_FanShaped:
			{
				nfan_shaped_region region;
				region.set_param(pos, radius, yaw, angle);
				map_ptr->get_objects(player_ptr, &region, objs, EOT_PLAYER | EOT_CREATURE, in_self);
			}
			break;
		case ERT_Rectangle:
			{
				nrectangle_region region;
				region.set_param(pos, radius, 500.0f, 1000.0f, yaw);
				map_ptr->get_objects(player_ptr, &region, objs, EOT_PLAYER | EOT_CREATURE, in_self);
			}
			break;
		}

		for (std::list<nworld_object*>::const_iterator iter = objs.begin(); iter != objs.end(); ++iter)
		{
			NLOG_DEBUG(_T("object id = %lu"), (*iter)->get_id());
		}
	}

} // namespace nexus
