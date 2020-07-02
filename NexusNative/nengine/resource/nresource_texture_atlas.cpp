#include "StdAfx.h"
#include "nresource_texture_atlas.h"

namespace nexus
{
	nresource_texture_atlas::nresource_texture_atlas(const nstring& name_str) : nresource_texture_2d(name_str)
	{
	}

	nresource_texture_atlas::~nresource_texture_atlas(void)
	{
	}

	const tex_atlas_item& nresource_texture_atlas::get_item(const nstring& name)
	{
		st_item_map::iterator iter = m_items.find( nname(name) );
		if( iter!=m_items.end() )
			return iter->second;

		static tex_atlas_item temp;
		return temp;
	}

	void nresource_texture_atlas::load_from_file(const resource_location& loc)
	{		
		nresource::load_from_file(loc);
		nresource_texture_2d::load_from_file(m_tex_loc);
	}

	template<>
	inline narchive& nserialize(narchive& ar, tex_atlas_item& obj, const TCHAR* obj_name)
	{
		nstring class_name(_T("tex_atlas_item"));
		ar.object_begin(obj_name, class_name);

		nserialize(ar, obj.name, _T("name"));
		nserialize(ar, obj.uv_start, _T("uv_start"));
		nserialize(ar, obj.uv_end, _T("uv_end"));		

		ar.object_end();
		return ar;
	}


	void nresource_texture_atlas::serialize(narchive& ar)
	{
		nresource::serialize(ar);

		//-- 贴图文件
		nSERIALIZE(ar, m_tex_loc);
					
		
		//-- 内部对象列表
		const TCHAR* actor_map_name = _T("items");
		if( ar.is_loading() )
		{
			//-- 读入item map
			size_t s = 0;
			ar.array_begin(actor_map_name, s);

			for(size_t i=0; i<s; i++)
			{
				tex_atlas_item new_item;

				nserialize(ar, new_item, ELEM_ArrayItem);
				add_item(new_item);
				ar.array_next();				
			}

			ar.array_end();
		}
		else
		{
			//-- 将item map写入文档
			size_t s = m_items.size();

			ar.array_begin(actor_map_name, s);

			for(st_item_map::iterator iter = m_items.begin();
				iter != m_items.end();
				++iter)
			{
				nserialize(ar, iter->second, ELEM_ArrayItem);				
				ar.array_next();
			}

			ar.array_end();
		}
	}

	void nresource_texture_atlas::add_item(const nstring& name, vector2 uv_start, vector2 uv_end)
	{
		tex_atlas_item new_item = {name, uv_start, uv_end};
		m_items.insert( make_pair(nname(name),new_item) );
	}

	void nresource_texture_atlas::add_item(const tex_atlas_item& new_item)
	{
		m_items.insert( make_pair(nname(new_item.name),new_item) );
	}

	void nresource_texture_atlas::set_internal_texture(const resource_location& loc)
	{
		m_tex_loc = loc;
		nresource_texture_2d::load_from_file(loc);
	}

	bool nresource_texture_atlas::first_item(tex_atlas_item& out_item)
	{
		if( m_items.empty() )
			return false;

		m_iter = m_items.begin();
		out_item = m_iter->second;
		return true;
	}

	bool nresource_texture_atlas::next_item(tex_atlas_item& out_item)
	{
		if( m_iter == m_items.end() )
			return false;

		m_iter++;
		if( m_iter == m_items.end() )
			return false;

		out_item = m_iter->second;
		return true;
	}
}//namespace nexus