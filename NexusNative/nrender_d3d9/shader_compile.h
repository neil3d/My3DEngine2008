/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_SHADER_COMPILE_H_
#define _NEXUS_SHADER_COMPILE_H_
#include "drawing_policy.h"
#include "vertex_factory_type.h"

namespace nexus
{
	struct drawing_policy_type;
	class vertex_factory_type;	

	/**
	 *	处理effect编译中的include callback
	*/
	class shader_include 
		: public ID3DXInclude
	{
	public:
		shader_include()
			: m_vf_type(NULL), m_mtl_shader(NULL)
		{}
		virtual ~shader_include()	{}

		void set_data(drawing_policy_type*	dp, vertex_factory_type* vf_type, const nmtl_tech_shader* mtl_shader)
		{
			m_dp = dp;
			m_vf_type = vf_type;
			m_mtl_shader = mtl_shader;
		}
		STDMETHOD(Open)(THIS_ D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
		{
			//-- drawing policy
			if(m_dp->get_include_code(pFileName, m_include_code))
			{
				*ppData = m_include_code.c_str();
				*pBytes = (UINT)m_include_code.size();
				return D3D_OK;
			}

			//-- fixed memory include file
			if(_stricmp(pFileName, "vertex_factory.hlsl") == 0)
			{
				if(m_vf_type == NULL)
					return D3DERR_NOTFOUND;
				*ppData = m_vf_type->get_shader_source();
				*pBytes = (UINT)m_vf_type->get_shader_size();
			}
			else if(_stricmp(pFileName, "material.hlsl") == 0)
			{
				if(m_mtl_shader == NULL)
					return D3DERR_NOTFOUND;
				
				const std::string& code = m_mtl_shader->get_shader_code();

				*ppData = (void*)code.c_str();
				*pBytes = (UINT)code.size();
			}
			else
			{
				if( search_include_file(conv_string(pFileName)) )
				{
					*ppData = m_include_code.c_str();
					*pBytes = (UINT)m_include_code.size();
					return D3D_OK;
				}
				else
				{
					return D3DERR_NOTFOUND;
				}
			}

			return D3D_OK;
		}

		STDMETHOD(Close)(THIS_ LPCVOID pData)
		{
			return D3D_OK;
		}

		bool search_include_file(const nstring& file_name);
		void load_shader_source_file()
		{
			m_dp->load_shader_file();
			m_vf_type->load_shader_file();
		}
	private:
		vertex_factory_type*		m_vf_type;
		const nmtl_tech_shader*		m_mtl_shader;
		drawing_policy_type*		m_dp;
		
		std::string	m_include_code;
	};

	/**
	 *	effect编译的相关数据，包括include，macro等
	*/
	class shader_compile_environment
	{
	public:
		shader_compile_environment(void);
		~shader_compile_environment(void);

		void init(drawing_policy_type* dp_type, 
			vertex_factory_type* vf_type, 
			const nmtl_tech_shader* mtl_shader,
			const nshader_modifier* mesh_mod,
			const nshader_modifier* mtl_mod);
		
		const char* get_shader_source()		{	return m_dp_type->shader_source.c_str();}
		size_t get_shader_source_len()	{	return m_dp_type->shader_source.size();}
		D3DXMACRO* get_macros()			
		{	
			if(m_macro_array.empty())
				return NULL;
			else
				return &(*m_macro_array.begin());
		}
		ID3DXInclude* get_include()		{	return &m_include;}
		DWORD get_compile_flags()		{	return 0;}

		const nstring& get_name() const	{	return m_name;}

		void load_shader_source()
		{
			m_include.load_shader_source_file();
		}
	private:
		drawing_policy_type*	m_dp_type;
		shader_include			m_include;
		vector<D3DXMACRO>		m_macro_array;

		nstring	m_name;
	};
}//namespace nexus

#endif //_NEXUS_SHADER_COMPILE_H_