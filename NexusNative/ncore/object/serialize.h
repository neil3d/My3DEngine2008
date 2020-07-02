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
#include "../is_pod.h"

namespace nexus
{
#define nSERIALIZE(ar, val) nserialize(ar, val, _T(#val))

	//-- 序列化一个nobject派生类对象
	template<typename T>
	inline narchive& nserialize(narchive& ar, T& obj, const TCHAR* obj_name)
	{
		nstring class_name = obj.reflection_get_class()->get_name();
		ar.object_begin(obj_name, class_name);
		obj.serialize(ar);
		ar.object_end();

		return ar;
	}

	template<typename T>
	inline narchive& nserialize(narchive& ar, std::set<T>& obj_set, const TCHAR* array_name)
	{
		if( ar.is_loading() )
		{
			size_t s = 0;
			ar.array_begin(array_name, s);
			for (size_t i=0; i<s; i++)
			{
				T obj;
				nserialize(ar, obj, ELEM_ArrayItem);
				ar.array_next();
				obj_set.insert(obj);
			}
			ar.array_end();
		}
		else
		{
			size_t s = obj_set.size();
			ar.array_begin(array_name, s);
			for(std::set<T>::iterator iter = obj_set.begin();
				iter != obj_set.end();
				++iter)
			{
				T& obj = *iter;
				nserialize(ar, obj, ELEM_ArrayItem);			
				ar.array_next();
			}
			ar.array_end();
		}
		return ar;
	}

	template<typename T>
	inline narchive& nserialize(narchive& ar, std::list<T>& obj_list, const TCHAR* array_name)
	{
		if( ar.is_loading() )
		{
			size_t s = 0;
			ar.array_begin(array_name, s);
			for (size_t i=0; i<s; i++)
			{
				T obj;
				nserialize(ar, obj, ELEM_ArrayItem);
				ar.array_next();
				obj_list.push_back(obj);
			}
			ar.array_end();
		}
		else
		{
			size_t s = obj_list.size();
			ar.array_begin(array_name, s);
			for(std::list<T>::iterator iter = obj_list.begin();
				iter != obj_list.end();
				++iter)
			{
				T& obj = *iter;
				nserialize(ar, obj, ELEM_ArrayItem);			
				ar.array_next();
			}
			ar.array_end();
		}
		return ar;
	}

	//-- 序列化一个vector，T可以是任意类型
	template<typename T>
	inline narchive& nserialize(narchive& ar, std::vector<T>& obj_array, const TCHAR* array_name)
	{	
		bool is_pod = IS_POD<T>::value; 
		bool blob_mode = is_pod && ar.blob_enabled();

		if(blob_mode)
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
			}// end of if

			ar.array_end();
		}
		return ar;
	}


	//-- 序列化一个nobject派生类的share_ptr
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


	//-- 针对primitive type的特化模板函数
#define IMPL_PRIM_SERIAL(type) template<> inline narchive& nserialize(narchive& ar, type& prim, const TCHAR* obj_name) \
	{	ar.serial(obj_name, prim);	return ar;	}\
	template<> struct IS_POD<type>\
	{ static const bool value = true; };

	//IMPL_PRIM_SERIAL(bool)
	IMPL_PRIM_SERIAL(int)
	IMPL_PRIM_SERIAL(unsigned int)
	IMPL_PRIM_SERIAL(char)	
	IMPL_PRIM_SERIAL(short)
	IMPL_PRIM_SERIAL(unsigned short)
	IMPL_PRIM_SERIAL(long)
	IMPL_PRIM_SERIAL(unsigned long)
	IMPL_PRIM_SERIAL(float)
	IMPL_PRIM_SERIAL(double)
	IMPL_PRIM_SERIAL(nstring)
	IMPL_PRIM_SERIAL(std::string)

	template<> inline narchive& nserialize(narchive& ar, unsigned char& prim, const TCHAR* obj_name)
	{	
		unsigned short t = prim;
		ar.serial(obj_name, t);	
		prim = (unsigned char)t;
		return ar;	
	}

	// 明确的将bool类型序列化成 “1” or “0”
	template<> inline narchive& nserialize(narchive& ar, bool& prim, const TCHAR* obj_name)
	{	
		unsigned short t = prim ? 1 : 0;
		ar.serial(obj_name, t);	
		prim = (t == 1) ? true : false;
		return ar;	
	}

}//namespace nexus

#endif //_NEXUS_CORE_SERIALIZE_H_