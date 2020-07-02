#ifndef _NEXUS_MATERIAL_STATIC_H_
#define _NEXUS_MATERIAL_STATIC_H_
#include "nmtl_common.h"

namespace nexus
{
	/**
	 *	所有材质Shader参数使用一个定值的（还是可以由上层逻辑控制其改变）
	*/
	class nAPI nmtl_static : public nmtl_common
	{
	public:
		typedef boost::shared_ptr<nmtl_static> ptr;

		explicit nmtl_static(const nstring& name_str);
		virtual ~nmtl_static(void);

		//! 创建默认的tech；然后创建一个Tech，绑定到Lighting和Unlit渲染策略
		void default_create(const resource_location& shader_loc);

		void set_value_parameter(const std::string& param_name, const vector4& val);
		void set_value_parameter(const std::string& param_name, int val);
		virtual void setup_effect(nshading_effect* effect_ptr) const;
		size_t get_num_value_param() const								{	return m_value_param_array.size(); }
		const material_parameter& get_value_param(size_t i) const	{	return m_value_param_array[i]; }

		virtual void serialize(narchive& ar);
	protected:
		virtual void bind_shader_param(nmtl_tech_shader::ptr mtl_shader);
		virtual vector4 get_float_param(const std::string& param_name) const;
		virtual bool have_param(const std::string& param_name) const;

	protected:
		st_param_array	m_value_param_array;

		nDECLARE_NAMED_CLASS(nmtl_static)
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_STATIC_H_