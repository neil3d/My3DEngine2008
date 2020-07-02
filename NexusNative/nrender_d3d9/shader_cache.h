#pragma once
#include "d3d_ptr.h"

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

		void clear()
		{
			m_code_map.clear();
			m_dummy_compiler.reset();
		}
	private:		
		typedef std::map<nstring, code_buf> st_code_map;

		st_code_map	m_code_map;

		/**
		 *	持有一个Compiler的引用避免频繁的出现
		 'xxxx.exe': Loaded 'C:\Windows\System32\D3DCompiler_42.dll'
		 'xxxx.exe': Unloaded 'C:\Windows\System32\D3DCompiler_42.dll'
		*/
		d3d_ptr<ID3DXEffectCompiler>	m_dummy_compiler;
	};
}//namespace nexus