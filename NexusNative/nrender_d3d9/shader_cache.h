#pragma once

namespace nexus
{	
	class shader_compile_environment;

	typedef std::vector<unsigned char> code_buf;
	class shader_cache
	{		
	public:
		shader_cache(void);
		~shader_cache(void);

		void load_from_file();
		void save_to_file();

		const code_buf& acquire_shader_code(shader_compile_environment* env);

	private:		
		typedef std::map<nstring, code_buf> st_code_map;

		st_code_map	m_code_map;
	};
}//namespace nexus