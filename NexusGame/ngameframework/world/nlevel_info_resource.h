#ifndef _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_RESOURCE_H_
#define _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_RESOURCE_H_
#include "ncore.h"
#include "nlevel_info_define.h"

namespace gameframework
{
	// 序列化时的过滤器
	enum ELevelInfoType
	{
		ELIRT_StartPoint	= 1<<1,
		ELIRT_Trigger		= 1<<2,

		ELIRT_ALL			= 0xFFFFFFFF
	};

	/**
	* 单个场景的资源
	*/ 
	class nGameFrameworkAPI nlevel_info_resource
	{
	public:
		typedef boost::shared_ptr<nlevel_info_resource> ptr;

		typedef std::vector<start_point_data>		start_point_data_vec;
		typedef std::vector<trigger_data>			trigger_data_vec;

		nlevel_info_resource();
		virtual ~nlevel_info_resource();

		//--  序列化
		void serialize( nfile_system* fs,const nstring& pkg_name, const nstring& file_name, enum EFileMode mode, ELevelInfoType filter);

		// create data (?todo:如何去避免ID重复的问题,目前做的是由外部去保证ID唯一，如果存在相同ID则返回false)
		bool add_info(const start_point_data& data);
		bool add_info(const trigger_data& data);

		// remove
		void remove_start_point_data(const nname& id);
		void remove_trigger_data(const nname& id);

		// find
		const start_point_data* find_start_point_data(const nname& id);
		const trigger_data* find_trigger_data(const nname& id);

		// get vector
		const start_point_data_vec& get_start_point_data() const { return m_vec_start_points; }
		const trigger_data_vec& get_trigger_data() const { return m_vec_triggers; }

		// clear
		void clear()
		{
			m_vec_start_points.clear();
			m_vec_triggers.clear();
		}

		// 提供一些助手函数帮助外部创建唯一标识
		nstring make_unique_id(ELevelInfoType info_type);

	private:
		start_point_data_vec			m_vec_start_points;
		trigger_data_vec				m_vec_triggers;
	};
} // end of namespace gameframework

#endif // end of _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_RESOURCE_H_