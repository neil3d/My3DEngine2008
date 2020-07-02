#ifndef _NEXUS_GAMEFRAMEWORK_NATT_INFO_DATA_H_
#define _NEXUS_GAMEFRAMEWORK_NATT_INFO_DATA_H_

#include <boost/unordered_map.hpp>
#include "ncore.h"
#include "ndef_levelup.h"
#include "nspell_define.h"
//#include "nspell_serialize.h"

namespace gameframework
{
	/**
	* 策划模板数据
	*/ 
	template<typename info_type>
	class natt_info_data
	{	
	public:
		typedef boost::shared_ptr<info_type> info_type_ptr;
		typedef boost::unordered_map<uint32,info_type_ptr>	info_container;
	private:
		info_container	m_infos;

	public:
		~natt_info_data()
		{
			clear_info();
		}

	private:
		// 序列化模板共享指针
		narchive& serialize_info_ptr(narchive& ar, info_type_ptr* obj_ptr, const TCHAR* obj_name)
		{
			if(ar.is_loading())
			{
				nstring class_name;
				ar.object_begin(obj_name, class_name);

				obj_ptr->reset(new info_type());
				(*obj_ptr)->serialize(ar);

				ar.object_end();
			}
			else
			{
				nstring class_name(_T("info_type"));

				ar.object_begin(obj_name, class_name);
				if( obj_ptr )
					(*obj_ptr)->serialize(ar);
				ar.object_end();
			}

			return ar;
		}

	public:
		/** 序列化 */	
		bool serialize( nfile_system* fs,const nstring& pkg_name, const nstring& file_name, enum EFileMode mode)
		{
			if((mode == EFileRead && !fs->file_exists(pkg_name,file_name)))
			{
				nLog_Error(_T("file not exist when load att_info_data [%s:%s]..."), pkg_name.c_str(), file_name.c_str());
				return false;
			}

			narchive::ptr resource_ap = narchive::open_xml_archive(fs, mode, pkg_name, file_name);
			nstring dummy( _T("att_info_data"));
			resource_ap->object_begin(_T("att_info_data"),dummy);

			// 序列化m_infos容器
			const TCHAR* array_name = _T("m_infos");
			if( resource_ap->is_loading() )
			{
				m_infos.clear();

				size_t s = 0;
				resource_ap->array_begin(array_name, s);
				for (size_t i=0; i<s; i++)
				{
					info_type_ptr info;
					serialize_info_ptr(*resource_ap, &info, ELEM_ArrayItem);
					resource_ap->array_next();

					m_infos[info->id.name_crc] = info;
				}
				resource_ap->array_end();
			}
			else
			{
				size_t s = m_infos.size();
				resource_ap->array_begin(array_name, s);
				for(info_container::iterator iter = m_infos.begin();
					iter != m_infos.end();
					++iter)
				{
					serialize_info_ptr(*resource_ap, &(iter->second), ELEM_ArrayItem);			
					resource_ap->array_next();
				}
				resource_ap->array_end();
			}

			resource_ap->object_end();
			resource_ap->close();
			return true;
		};

		/** param id : 需要查找的模板ID */ 
		info_type_ptr find_info(const nname& id) const
		{

			return find_info(id.name_crc);
		}

		info_type_ptr find_info(uint32 crc_id) const
		{
			info_container::const_iterator iter = m_infos.find(crc_id);
			if(iter != m_infos.end())
			{
				return iter->second;
			}
			return info_type_ptr();
		}

		/** 编辑器需要的接口 */ 
		void add_new_info(info_type_ptr info)
		{
			nASSERT(info);
			if(find_info(info->id) != NULL)
			{
				nLog_Error(_T("add existed att_info_data [%s : %u]!"), info->id.name_str.c_str(), info->id.name_crc);
			}
			m_infos[info->id] = info;
		}

		bool remove_info(const nname& id)
		{
			return remove_info(id.name_crc);
		}

		bool remove_info(uint32 crc_id)
		{
			return (m_infos.erase(crc_id) != 0);
		}

		void clear_info()
		{
			m_infos.clear();
		}

		const info_container& get_infos() const
		{	
			return m_infos;
		}
	};

#define DECLARE_NATIVE_ATT_INFO_DATA_CLASS(CLASS, INFOTYPE)	\
	class nGameFrameworkAPI CLASS : public natt_info_data<INFOTYPE>, public nexus::singleton<CLASS>	\
	{	};	\

	// 声明角色升级模板表
	DECLARE_NATIVE_ATT_INFO_DATA_CLASS(nlevelup_info_dictionary,levelup_info);
	// 技能树模板表, 技能学习
	DECLARE_NATIVE_ATT_INFO_DATA_CLASS(nspell_talent_table_dictionary,nspell_talent_table);
	// 技能模板表
	DECLARE_NATIVE_ATT_INFO_DATA_CLASS(nspell_info_dictionary,nspell_info);
	// 技能效果模板表
	DECLARE_NATIVE_ATT_INFO_DATA_CLASS(nspell_effect_info_dictionary,nspell_effect_info);
	// 技能效果模板表
	DECLARE_NATIVE_ATT_INFO_DATA_CLASS(nspell_aura_info_dictionary,nspell_aura_info);

} // end of namespace gameframework

#endif // end of _NEXUS_GAMEFRAMEWORK_NATT_INFO_DATA_H_
