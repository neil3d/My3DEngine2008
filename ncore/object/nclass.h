/**
*	nexus core - Reflection
*
*	Author: Neil
*	Date:	Sept, 2008
*/

#ifndef _NEXUS_CLASS_H_
#define _NEXUS_CLASS_H_

#include "../base_define.h"
#include "../base_depend.h"
#include "singleton.h"

namespace nexus
{
	class nobject;
	class nclass;

	typedef vector<const nclass*>	nclass_array;
	/**
	 * a class info
	*/
	class nCoreAPI nclass
	{
	public:
		typedef nobject*(alloc_object_func)();		
		typedef nobject*(alloc_named_object_func)(const nstring&);		

		nclass(const TCHAR* class_name, nclass* super_class, alloc_object_func* func_ptr);
		nclass(const TCHAR* class_name, nclass* super_class, alloc_named_object_func* func_ptr, bool tag);
		~nclass(void);

		bool is_kind_of(const nclass* class_ptr) const;
		bool is_kind_of(const TCHAR* class_name) const;
		bool is_named_class() const;

		const TCHAR* get_name() const;
		nobject* alloc_object() const;
		nobject* alloc_named_object(const nstring& obj_name) const;

		size_t get_num_children() const;
		const nclass* get_child(size_t index) const;
	
		void _clear_sub_class_array();
		void _link_parent_class();
	private:
		void _add_child(nclass* class_ptr);

		const TCHAR*		m_class_name;
		nclass*				m_super;		
		nclass_array		m_children;

		alloc_object_func*			m_alloc_func;
		alloc_named_object_func*	m_named_alloc_func;
	};


	typedef std::map<nstring, nclass*>	class_map;
	/**
	 * class factory
	*/
	class nCoreAPI nclass_factory
	{
	public:
		nclass_factory(void);
		~nclass_factory(void);

		static nclass_factory* instance();

		void build_sub_classes();

		void link_class(nclass* class_ptr);		
		nobject* alloc_object(const nstring& class_name);
		nobject* alloc_named_object(const nstring& class_name, const nstring& obj_name);
		const nclass* find_class(const nstring& class_name);
		bool is_named_class(const nstring& class_name);
		size_t get_num_classes() const;

	private:
		class_map	m_classes;
	};

	template<typename NClass>
	inline NClass* nconstruct(const nstring& class_name)
	{
		return static_cast<NClass*>(nclass_factory::instance()->alloc_object(class_name));
	}
	
	template<typename NClass>
	inline NClass* nconstruct(const nstring& class_name, const nstring& obj_name)
	{
		return static_cast<NClass*>(nclass_factory::instance()->alloc_named_object(class_name, obj_name));
	}

	template<typename T>
	T* ndynamic_cast(nobject* obj_ptr)
	{
		if(obj_ptr == NULL)
			return NULL;

		const nclass* src_class = obj_ptr->reflection_get_class();
		const nclass* dst_class = T::reflection_get_static_class();

		if( src_class->is_kind_of(dst_class) )
			return (T*)obj_ptr;
		else
			return NULL;
	}
}//namespace nexus
#endif // _NEXUS_CLASS_H_