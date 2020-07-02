#ifndef _NEXUS_RESOURCE_SPECIAL_EFFECT_H_
#define _NEXUS_RESOURCE_SPECIAL_EFFECT_H_
#include "../resource/nresource.h"
#include "neffect_element.h"

namespace nexus
{
	/**
	 *	特效资源对象
	 *	@remarks 管理一组特效元素的属性描述数据
	*/
	class nAPI nresource_special_effect
		: public nresource
	{
	public:
		typedef boost::intrusive_ptr<nresource_special_effect> ptr;

		nresource_special_effect(const nstring& name_str);
		virtual ~nresource_special_effect(void);

		size_t get_num_element() const	{	return m_elements.size();}
		neffect_element::ptr get_element(size_t index)
		{
			nASSERT( index < m_elements.size() );
			return m_elements[index];
		}

		//-- for editor
		void add_element(neffect_element::ptr new_elem);
		void remove_element(const nstring& elem_name);
		void clear();

		virtual void serialize(narchive& ar);

	private:
		typedef std::vector<neffect_element::ptr> st_elem_array;
		
		st_elem_array	m_elements;
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_SPECIAL_EFFECT_H_