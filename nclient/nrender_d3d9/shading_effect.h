/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_SHADING_EFFECT_H_
#define _NEXUS_SHADING_EFFECT_H_
#include "d3d_ptr.h"
#include "drawing_policy_type.h"

namespace nexus
{
	struct d3d_view_info;
	class vertex_factory_type;
	class shader_compile_environment;

	enum EVertexShaderParameter
	{
		EVSP_WorldViewProject = 0,
		EVSP_LocalToWorld,
		EVSP_WorldToLocal,
		EVSP_EyePos,

		EVSP_Max
	};

	const char* const vertex_shader_parameter_semantic[]=
	{
		"SYS_WorldViewProject",
		"SYS_LocalToWorld",
		"SYS_WorldToLocal",
		"SYS_EyePos",
	};

	/**
	 *	一个创建好的hardware friendly的effect对象
	*/
	class d3d9_shading_effect :
		public nshading_effect
	{
	public:
		typedef shared_ptr<d3d9_shading_effect> ptr;

		d3d9_shading_effect(void);
		virtual ~d3d9_shading_effect(void);

		virtual void set_float(const std::string& name, float val)
		{
			D3DXHANDLE param_handle = m_d3d_effect->GetParameterBySemantic(NULL, name.c_str());
			if( param_handle )
			{
				HRESULT hr = m_d3d_effect->SetFloat(param_handle, val);
			}
		}

		virtual void set_vector(const std::string& name, const vector2& val)
		{
			HRESULT hr = set_float_array_by_semantic(name, val);
		}

		virtual void set_vector(const std::string& name, const vector3& val)
		{
			HRESULT hr = set_float_array_by_semantic(name, val);
		}

		virtual void set_vector(const std::string& name, const vector4& val)
		{
			HRESULT hr = set_float_array_by_semantic(name, val);
		}

		virtual void set_texture(const std::string& name, const nrender_texture* tex)
		{
			D3DXHANDLE param_handle = m_d3d_effect->GetParameterBySemantic(NULL, name.c_str());
			if( param_handle )
			{
				HRESULT hr = m_d3d_effect->SetTexture(param_handle,
					static_cast<IDirect3DBaseTexture9*>(tex->get_handle())
					);
			}
		}

		virtual void set_matrix_array(const std::string& name, const matrix44* mat, size_t count)
		{
			D3DXHANDLE param_handle = m_d3d_effect->GetParameterBySemantic(NULL, name.c_str());
			if( param_handle )
			{
				HRESULT hr = m_d3d_effect->SetMatrixArray(param_handle, (const D3DXMATRIX*)mat, count);
			}
		}

		void set_vertex_factory_parameter(const d3d_view_info* view, const nrender_proxy* obj);		

		void create_effect(shader_compile_environment* env);
		ID3DXEffect* get_d3d_effect()	{	return m_d3d_effect.get();}

		//-- ID3DXEffect接口的简单封装
		template<typename T>
		HRESULT set_float_array_by_semantic(const std::string& semantic_str, const T& vec)
		{
			D3DXHANDLE param_handle = m_d3d_effect->GetParameterBySemantic(NULL, semantic_str.c_str());
			if( !param_handle )
				return D3DERR_NOTFOUND;

			return m_d3d_effect->SetFloatArray( param_handle,
				(const float*)&vec, sizeof(vec)/sizeof(float) );
		}
	protected:
		d3d_effect_ptr	m_d3d_effect;
		D3DXHANDLE		m_vs_parameter[EVSP_Max];
	};


	struct shading_effect_name
	{
		nstring	drawing_policy_name;
		nstring	vertex_factory_name;
		nstring	material_template_name;
		nstring mod_name;
		unsigned int name_crc;

		void make_name_crc();
	};

	inline bool operator < (const shading_effect_name& v1, const shading_effect_name& v2)
	{
		return v1.name_crc < v2.name_crc;
	}


	/**
	 *	所有编译好的effect对象管理在这里
	*/
	class shading_effect_lib
	{
	public:
		shading_effect_lib(void):m_default_mtl(NULL)
		{
		}
		virtual ~shading_effect_lib(void)	{}

		static shading_effect_lib* instance();
		
		void init_drawing_policy_type()
		{
			return m_dp_type_list.create_all_type();
		}
		drawing_policy_type::ptr get_draw_policy_type(const nstring& dp_name)
		{
			return m_dp_type_list.find_drawing_policy_type(dp_name);
		}
		
		d3d9_shading_effect* acquire_shader(drawing_policy_type* dp_type, vertex_factory_type* vf_type, 
			const nmaterial_base* mtl, const nshader_modifier* mod);

		void destroy()
		{
			m_dp_type_list.clear();
			m_shader_map.clear();
		}

		virtual void set_default_material(nmaterial_base* mtl)	{	m_default_mtl = mtl;}

	private:
		d3d9_shading_effect::ptr create_shader(drawing_policy_type* dp_type, vertex_factory_type* vf_type, 
			const nmaterial_base* mtl, const nshader_modifier* mod);
	private:
		typedef std::map<shading_effect_name, d3d9_shading_effect::ptr> shader_map;		
		shader_map		m_shader_map;
		nmaterial_base*		m_default_mtl;

		dp_type_list	m_dp_type_list;
	};
}//namespace nexus

#endif //_NEXUS_SHADING_EFFECT_H_