#include "StdAfx.h"
#include "nresource_special_effect.h"

namespace nexus
{
	nresource_special_effect::nresource_special_effect(const nstring& name_str):nresource(name_str)
	{
	}

	nresource_special_effect::~nresource_special_effect(void)
	{
	}

	void nresource_special_effect::add_element(neffect_element::ptr new_elem)
	{
		m_elements.push_back( new_elem );
	}

	void nresource_special_effect::remove_element(const nstring& elem_name)
	{
		for( st_elem_array::iterator iter = m_elements.begin();
			iter != m_elements.end();
			++iter )
		{
			neffect_element::ptr elem = *iter;
			if( elem->get_name() == elem_name )
			{
				m_elements.erase(iter);
				break;
			}
		}// end of for()
	}

	void nresource_special_effect::clear()
	{
		m_elements.clear();
	}

	void nresource_special_effect::serialize(narchive& ar)
	{
		nresource::serialize( ar );
		nSERIALIZE(ar, m_elements);
	}
}//namespace nexus