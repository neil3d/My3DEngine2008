#ifndef _NDEF_GAME_NDEF_BASE_H_
#define _NDEF_GAME_NDEF_BASE_H_

#include "ntype_def.h"

namespace nexus {

#pragma pack(push,1)

	struct att_info_base
	{
		// ID
		nname		id;

		// 名称
		nstring		name;

		// 描述
		nstring		description;

		att_info_base()
		{}

		explicit att_info_base(const nstring& id_str) 
		{
			id.reset(id_str);
		}

		void serialize(narchive& ar)
		{
			nSERIALIZE(ar, id);
			nSERIALIZE(ar, name);
			nSERIALIZE(ar, description);

			custom_serialize(ar);
		}

	protected:
		// 为了让未定义序列化的结构体在编译器抛出错误
		virtual void custom_serialize(narchive& ar) = 0;
	};


#define SERIALIZE_ATT_INFO(att_type)	\
	template<>\
	inline narchive& nserialize(narchive& ar, att_type& info, const TCHAR* obj_name)\
	{\
		nstring class_name(_T(#att_type));\
		ar.object_begin(obj_name, class_name);\
		info.serialize(ar);\
		ar.object_end();\
		return ar;\
	}\


#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NDEF_BASE_H_
