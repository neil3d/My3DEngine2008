#ifndef _NEXUS_MATERIAL_COMMON_H_
#define _NEXUS_MATERIAL_COMMON_H_
#include "nmtl_base.h"
#include "material_parameter.h"

namespace nexus
{
	class nrender_texture;
	/**
	 *	通用材质类
	 *	@remarks 自动提前shader需要的参数，管理他们的值
	*/
	class nAPI nmtl_common : public nmtl_base
	{
	public:
		explicit nmtl_common(const nstring& name_str);
		virtual ~nmtl_common(void);

		//!	设置2D贴图参数值
		void set_texture2d_parameter(const std::string& param_name, const resource_location& tex_loc);

		//! 设置Cube map贴图参数值
		void set_cube_map_parameter(const std::string& param_name, const resource_location& tex_loc);
		virtual void setup_effect(nshading_effect* effect_ptr) const = 0;
		virtual const nshader_modifier* get_shader_mod();
	
		virtual void serialize(narchive& ar);

		size_t get_num_texture_param() const								{	return m_texture_param_array.size(); }
		const material_parameter& get_texture_param(size_t i)	const	{	return m_texture_param_array[i]; }

		typedef std::vector<struct shader_define> shader_macro_array;

	protected:
		nrender_texture* get_texture_param(const std::string& param_name) const;
		virtual vector4 get_float_param(const std::string& param_name) const = 0;
		
		virtual bool have_param(const std::string& param_name) const;
		material_parameter* find_texture_param(const std::string& param_name);
	protected:
		typedef std::vector<material_parameter>	st_param_array;
		
		st_param_array	m_texture_param_array;	// 多个pass如果拥有参数名称相同的参数，则共享一个变量值

		nDECLARE_VIRTUAL_CLASS(nmtl_common)
	};
}//namespace nexus

#endif //_NEXUS_MATERIAL_COMMON_H_