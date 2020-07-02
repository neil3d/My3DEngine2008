#ifndef _NEXUS_RESOURCE_TEXTURE_ATLAS_H_
#define _NEXUS_RESOURCE_TEXTURE_ATLAS_H_
#include "nresource_texture.h"

namespace nexus
{
	struct tex_atlas_item
	{
		nstring name;
		vector2	uv_start;
		vector2 uv_end;
	};
	class nAPI nresource_texture_atlas
		: public nresource_texture_2d
	{
	public:
		typedef boost::intrusive_ptr<nresource_texture_atlas> ptr;

		nresource_texture_atlas(const nstring& name_str);
		virtual ~nresource_texture_atlas(void);

		const tex_atlas_item& get_item(const nstring& name);

		virtual void load_from_file(const resource_location& loc);
		virtual void serialize(narchive& ar);

		bool first_item(tex_atlas_item& out_item);
		bool next_item(tex_atlas_item& out_item);

		//-- 引擎内部接口
	public:
		void set_internal_texture(const resource_location& loc);
		void add_item(const tex_atlas_item& new_item);
		void add_item(const nstring& name, vector2 uv_start, vector2 uv_end);
		
	private:
		typedef std::map<nname, tex_atlas_item> st_item_map;
		
		resource_location	m_tex_loc;	// 实际的贴图文件
		st_item_map			m_items;
		st_item_map::iterator	m_iter;
	};
}//namespace nexus
#endif //_NEXUS_RESOURCE_TEXTURE_ATLAS_H_