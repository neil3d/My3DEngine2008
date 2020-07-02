#ifndef _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_DEFINE_H_
#define _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_DEFINE_H_
#include "ncore.h"

using namespace nexus;

namespace gameframework
{
	/**
	* client/server公用的地图对象资源结构体定义
	*/
	struct nGameFrameworkAPI info_data_base
	{
		// 以字符串最为ID
		nname		name;
		// 友好名字，方便记忆
		nstring		friend_name;
		// 描述
		nstring		description;

		info_data_base()
		{}

		explicit info_data_base(const nstring& id) 
			: name(id)
		{
		}

		virtual void serialize(narchive& ar)
		{
			nSERIALIZE(ar, name);
			nSERIALIZE(ar, friend_name);
			nSERIALIZE(ar, description);
		}
	};

	/**
	* 场景出生点
	*/
	struct nGameFrameworkAPI start_point_data
		: public info_data_base
	{
		// 出生点位置
		vector3		location;

		// 朝向
		vector3		rotation;

		// 允许在一定范围内随机
		float		radius;

		start_point_data() : radius(0)
		{}
		explicit start_point_data(const nstring& id) 
			: info_data_base(id)
		{
		}

		virtual void serialize(narchive& ar)
		{
			info_data_base::serialize(ar);
			nSERIALIZE(ar, location);
			nSERIALIZE(ar, radius);
		}
	};

	/** 
	* 触发器结构体定义
	*/
	struct nGameFrameworkAPI trigger_data 
		: public info_data_base
	{
		// 位置
		vector3		location;

		// 在圆形范围内触发
		float		radius;

		// 切换到新的地图，不填则不需要切换地图
		nname		transfer_map;

		// 切换到新的地图对应的出生点
		nname		start_point;

		// 调用脚本
		nname		invoke_script;

		// 延迟多长时间触发
		float		delay_time;

		// 是否已经被锁定
		bool		locked;

		trigger_data() : radius(0),delay_time(0),locked(false)
		{}
		explicit trigger_data(const nstring& id) 
			: info_data_base(id)
		{
		}

		virtual void serialize(narchive& ar)
		{
			info_data_base::serialize(ar);
			nSERIALIZE(ar, location);
			nSERIALIZE(ar, radius);
			nSERIALIZE(ar, transfer_map);
			nSERIALIZE(ar, start_point);
			nSERIALIZE(ar, invoke_script);
			nSERIALIZE(ar, delay_time);
			nSERIALIZE(ar, locked);
		}

	};

} // end of namespace gameframework

namespace nexus
{
	template<>
	inline narchive& nserialize(narchive& ar, gameframework::info_data_base& info, const TCHAR* obj_name)
	{
		nstring class_name(_T("info_data_base"));
		ar.object_begin(obj_name, class_name);
		info.serialize(ar);
		ar.object_end();
		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, gameframework::start_point_data& info, const TCHAR* obj_name)
	{
		nstring class_name(_T("start_point_data"));
		ar.object_begin(obj_name, class_name);
		info.serialize(ar);
		ar.object_end();
		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, gameframework::trigger_data& info, const TCHAR* obj_name)
	{
		nstring class_name(_T("trigger_data"));
		ar.object_begin(obj_name, class_name);
		info.serialize(ar);
		ar.object_end();
		return ar;
	}
}

#endif // end of _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_DEFINE_H_