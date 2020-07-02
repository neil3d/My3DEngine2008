#ifndef _NEXUS_NSTART_POINT_ACTOR_H_
#define _NEXUS_NSTART_POINT_ACTOR_H_
#include "../framework/nactor.h"
#include "ngameframework.h"

namespace nexus
{
	using namespace gameframework;

	class gameframework::nlevel_info_resource;

	/**
	*	出生点
	*/
	class nAPI nstart_point_actor
		: public nactor
	{
	public:
		typedef shared_ptr<nstart_point_actor> ptr;

		nstart_point_actor(const nstring& name_str);
		virtual ~nstart_point_actor(void);

		// 重载该函数以同步start_point_data位置和朝向
		virtual void update_cmp_transform();
		virtual void _set_name(const nstring& new_name)
		{
			nactor::_set_name(new_name);
			m_data.name.reset(new_name);
		}

		// 重载序列化接口
		virtual void serialize(narchive& ar);

		// 外部获取数据
		const start_point_data& get_data() const { return m_data; }
		start_point_data& get_data() { return m_data; }

		//-- 导出资源接口
		virtual bool export_info_resource(gameframework::nlevel_info_resource* res);

	private:
		start_point_data  m_data;

		nDECLARE_NAMED_CLASS(nstart_point_actor)
	};
}//namespace nexus

#endif //_NEXUS_NSTART_POINT_ACTOR_H_