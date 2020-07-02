#include "stdafx.h"
#include "../base_depend.h"
#include "nclass.h"

namespace nexus
{
	//-------------------------------------------------------------------------------------------------------
	//	class nclass
	//-------------------------------------------------------------------------------------------------------
	nclass::nclass(const TCHAR* class_name, nclass* super_class, alloc_object_func* func_ptr)		
	{
		m_class_name	= class_name;
		m_super			= super_class;
		m_alloc_func	= func_ptr;

		nclass_factory::instance()->link_class(this);
	}

	nclass::nclass(const TCHAR* class_name, nclass* super_class, alloc_named_object_func* func_ptr, bool tag)
	{
		(void)tag; // 只用来帮助函数重载, func_ptr==NULL的情况
		m_class_name	= class_name;
		m_super			= super_class;
		m_named_alloc_func	= func_ptr;

		nclass_factory::instance()->link_class(this);
	}

	nclass::~nclass(void)	{
		
	}

	size_t nclass::get_num_children() const
	{
		return m_children.size();
	}

	const nclass* nclass::get_child(size_t index) const
	{
		nASSERT(index < get_num_children());
		return m_children[index];
	}

	bool nclass::is_kind_of(const nclass* class_ptr) const
	{
		if(class_ptr == NULL)
			return false;

		const nclass* class_iter = this;
		while(class_iter != NULL)
		{
			if(class_iter == class_ptr)
				return true;
			else
				class_iter = class_iter->m_super;
		}

		return false;
	}

	bool nclass::is_kind_of(const TCHAR* class_name) const
	{
		if(class_name == NULL)
			return false;

		const nclass* class_iter = this;
		while(class_iter != NULL)
		{
			if(_tcscmp(class_name, class_iter->m_class_name) == 0)				
				return true;
			else
				class_iter = class_iter->m_super;
		}

		return false;
	}

	bool nclass::is_named_class() const
	{
		return m_named_alloc_func!=NULL;
	}

	const TCHAR* nclass::get_name() const
	{	
		return m_class_name; 
	}

	nobject* nclass::alloc_object() const	
	{	
		return (m_alloc_func)(); 
	}

	nobject* nclass::alloc_named_object(const nstring& obj_name) const
	{
		return (m_named_alloc_func)(obj_name); 
	}

	void nclass::_clear_sub_class_array()
	{
		m_children.clear();
	}

	void nclass::_link_parent_class()
	{
		if(m_super)
			m_super->_add_child(this);
	}

	void nclass::_add_child(nclass* class_ptr)
	{
		m_children.push_back(class_ptr);
	}

	//-------------------------------------------------------------------------------------------------------
	//	class nclass_factory
	//-------------------------------------------------------------------------------------------------------
	nclass_factory::nclass_factory(void)
	{
	}

	nclass_factory::~nclass_factory(void)
	{
	}

	nclass_factory* nclass_factory::instance()
	{
		// todo : 为什么singleton模板会失败?
		static nclass_factory g_class_factory;
		return &g_class_factory;
	}

	void nclass_factory::link_class(nclass* class_ptr)
	{
		if(class_ptr == NULL)
			return;

		nASSERT(m_classes.end() == m_classes.find(class_ptr->get_name()) );
		
		m_classes.insert(make_pair(class_ptr->get_name(), class_ptr));
	}

	nobject* nclass_factory::alloc_object(const nstring& class_name)
	{
		class_map::iterator iter =  m_classes.find(class_name);
		if(iter == m_classes.end())
			return NULL;

		const nclass* class_ptr = iter->second;
		return class_ptr->alloc_object();
	}

	nobject* nclass_factory::alloc_named_object(const nstring& class_name, const nstring& obj_name)
	{
		class_map::iterator iter =  m_classes.find(class_name);
		if(iter == m_classes.end())
			return NULL;

		const nclass* class_ptr = iter->second;
		return class_ptr->alloc_named_object(obj_name);
	}

	const nclass* nclass_factory::find_class(const nstring& class_name)
	{
		class_map::iterator iter =  m_classes.find(class_name);
		if(iter == m_classes.end())
			return NULL;

		return iter->second;
	}

	void nclass_factory::build_sub_classes()
	{
		for( class_map::iterator iter = m_classes.begin();
			iter != m_classes.end();
			++iter)
		{
			nclass* class_ptr = iter->second;
			class_ptr->_clear_sub_class_array();
		}

		for( class_map::iterator iter = m_classes.begin();
			iter != m_classes.end();
			++iter)
		{
			nclass* class_ptr = iter->second;
			class_ptr->_link_parent_class();
		}
	}

	size_t nclass_factory::get_num_classes() const
	{
		return m_classes.size();
	}

	bool nclass_factory::is_named_class(const nstring& class_name)
	{
		return find_class(class_name)->is_named_class();
	}
}//namespace nexus