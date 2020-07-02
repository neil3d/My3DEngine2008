/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NMATERIAL_INSTANCE_H_
#define _NMATERIAL_INSTANCE_H_
#include "../resource/nresource.h"
#include "nmaterial_template.h"
#include "material_parameter.h"

namespace nexus
{
	class nshading_effect;

	class nAPI nmaterial_base
		: public nobject
	{
	public:
		typedef shared_ptr<nmaterial_base> ptr;

		explicit nmaterial_base(const nstring& name_str):m_name(name_str)
		{}
		virtual ~nmaterial_base(void)	{}

		virtual enum ETransparentType get_trans_type() const = 0;
		//-- shader interface
		virtual nstring get_template_name() const = 0;
		virtual const resource_location& get_template_loc() const = 0;
		virtual const std::string& get_shader_code() const = 0;
		virtual void draw_effect_param(nshading_effect* effect_ptr) const = 0;

		virtual void serialize(narchive& ar);

		nstring get_name() const	{	return m_name; }
	private:
		nstring	m_name;

		nDECLARE_VIRTUAL_CLASS(nmaterial_base)
	};

	/**
	 *	材质
	 *	@remark 组合一个“材质模版的引用”和“材质参数值数据”
	 *	@see class nmaterial_template, struct material_parameter_value
	*/
	class nAPI nmaterial
		: public nmaterial_base
	{
	public:
		typedef shared_ptr<nmaterial> ptr;

		explicit nmaterial(const nstring& name_str);
		virtual ~nmaterial(void);		

		//-- nmaterial_base interface
		virtual enum ETransparentType get_trans_type() const	{	return m_template->get_trans_type();}
		virtual nstring get_template_name() const;
		virtual const resource_location& get_template_loc() const;
		virtual const std::string& get_shader_code() const;
		virtual void draw_effect_param(nshading_effect* effect_ptr) const;

		//--
		void create(nmaterial_template::ptr mtl_tmp);		

		//-- parameter interface
		void set_texture_parameter(const nstring& param_name, const resource_location& tex_loc);
		void set_vector_parameter(const nstring& param_name, const vector4& vec);

		size_t get_num_parameter() const						{	return m_parameter_array.size();}
		const material_parameter& get_parameter(size_t i) const	{	return m_parameter_array[i];}
		
		virtual void serialize(narchive& ar);
	private:		

		template<typename T>
		void set_parameter(const nstring& param_name, T val);
	private:
		typedef std::vector<material_parameter_value>	parameter_array;
		
		nmaterial_template::ptr	m_template;
		parameter_array			m_parameter_array;

		nDECLARE_NAMED_CLASS(nmaterial)

	};
}//namespace nexus
#endif //_NMATERIAL_INSTANCE_H_