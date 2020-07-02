/**
*	Core of Nexus Engine -  Serialize template functions for common data type
*
*	Author: Neil
*	Date:	Dec, 2008
*/

#ifndef _NEXUS_CORE_SERIALIZE_H_
#define _NEXUS_CORE_SERIALIZE_H_
#include "../base_define.h"
#include "../base_depend.h"
#include <boost/type_traits.hpp>
#include "narchive.h"

namespace nexus
{
#define nSERIALIZE(ar, val) nserialize(ar, val, _T(#val))

	//-- ���л�һ��nobject���������
	template<typename T>
	inline narchive& nserialize(narchive& ar, T& obj, const TCHAR* obj_name)
	{
		nstring class_name = obj.reflection_get_class()->get_name();
		ar.object_begin(obj_name, class_name);
		obj.serialize(ar);
		ar.object_end();

		return ar;
	}

	//-- ���л�һ��vector��T��������������
	template<typename T>
	inline narchive& nserialize(narchive& ar, std::vector<T>& obj_array, const TCHAR* array_name)
	{	
		bool is_pod = boost::is_pod<T>::value; 
		if(ar.support_blob()
			&& is_pod)
		{
			if(ar.is_loading())
			{
				size_t s = ar.serial_blob(array_name, NULL, 0);
				size_t item_count = s/sizeof(T);
				obj_array.resize(item_count);
				if(item_count > 0)
				{
					ar.serial_blob(array_name,
						&(*obj_array.begin()), s);
				}
			}
			else
			{
				size_t s = obj_array.size()*sizeof(T);
				ar.serial_blob(array_name,
					&(*obj_array.begin()), s);
			}
		}
		else
		{
			size_t s = 0;
			if( !ar.is_loading() )
				s = obj_array.size();

			ar.array_begin(array_name, s);

			if( s > 0)
			{
				if( ar.is_loading() )
					obj_array.resize(s);


				for(std::vector<T>::iterator iter = obj_array.begin();
					iter != obj_array.end();
					++ iter)
				{
					T& obj = *iter;
					nserialize(ar, obj, ELEM_ArrayItem);			
					ar.array_next();
				}
			}// endof if

			ar.array_end();
		}
		return ar;
	}


	//-- ���л�һ��nobject�������share_ptr
	template<typename T>
	inline narchive& nserialize(narchive& ar, shared_ptr<T>& obj_ptr, const TCHAR* obj_name)
	{
		const TCHAR* const NONE_CLASS = _T("NoneClass");
		if(ar.is_loading())
		{
			nstring class_name;
			ar.object_begin(obj_name, class_name);

			if( class_name != NONE_CLASS )
			{
				if(nclass_factory::instance()->is_named_class(class_name))
					obj_ptr.reset(nconstruct<T>(class_name, obj_name));
				else
					obj_ptr.reset(nconstruct<T>(class_name));
				obj_ptr->serialize(ar);
			}// end of if

			ar.object_end();
		}
		else
		{
			nstring class_name;
			if( obj_ptr )
				class_name = obj_ptr->reflection_get_class()->get_name();
			else
				class_name = NONE_CLASS;

			ar.object_begin(obj_name, class_name);
			if( obj_ptr )
				obj_ptr->serialize(ar);
			ar.object_end();
		}

		return ar;
	}

	template<typename T>
	inline narchive& nserialize_enum_as_int(narchive& ar, T& enum_val, const TCHAR* obj_name)
	{
		int int_val = (int)enum_val;
		if(ar.is_loading())
		{
			ar.serial(obj_name, int_val);
			enum_val = (T)int_val;
		}
		else
		{
			ar.serial(obj_name, int_val);
		}
		return ar;
	}


	//-- ���primitive type���ػ�ģ�庯��
#define IMPL_PRIM_SERIAL(type) template<> inline narchive& nserialize(narchive& ar, type& prim, const TCHAR* obj_name) \
	{	ar.serial(obj_name, prim);	return ar;	}

	IMPL_PRIM_SERIAL(int)
	IMPL_PRIM_SERIAL(unsigned int)
	IMPL_PRIM_SERIAL(char)
	IMPL_PRIM_SERIAL(unsigned char)
	IMPL_PRIM_SERIAL(short)
	IMPL_PRIM_SERIAL(unsigned short)
	IMPL_PRIM_SERIAL(long)
	IMPL_PRIM_SERIAL(unsigned long)
	IMPL_PRIM_SERIAL(float)
	IMPL_PRIM_SERIAL(double)
	IMPL_PRIM_SERIAL(nstring)
	IMPL_PRIM_SERIAL(std::string)
}//namespace nexus

#endif //_NEXUS_CORE_SERIALIZE_H_