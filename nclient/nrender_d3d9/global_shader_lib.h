#pragma once
#include "d3d_ptr.h"

namespace nexus
{
	class global_shader_lib
	{
	public:
		global_shader_lib(void);
		~global_shader_lib(void);

		static global_shader_lib* instance();

		void load(const nstring& folder_name);
		d3d_effect_ptr find_shader(const nstring& file_name);
		
		void close()
		{
			m_effect_map.clear();
		}

	private:
		void create_effect_from_file(const nstring& key_name, const nstring& file_name);
	private:
		typedef std::map<nstring, d3d_effect_ptr> effect_map;
		effect_map	m_effect_map;
	};
}//namespace nexus