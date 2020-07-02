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

		//-- shader interface
		virtual enum ETransparentType get_trans_type() const	{	return m_template->get_trans_type(); }
		virtual nstring get_template_name() const	{	return m_template->get_name_str(); }
		virtual const resource_location& get_template_loc() const	{	return m_template->get_location(); }
		virtual const std::string& get_shader_code() const	{	return m_template->get_shader_code(); }
		bool is_two_sided() const	{	return m_template->is_two_sided();}

		virtual void draw_effect_param(nshading_effect* effect_ptr) const = 0;

		virtual void serialize(narchive& ar);

		nstring get_name() const	{	return m_name; }

		virtual void _destroy()	
		{
			m_template.reset();
		}
	protected:
		nstring					m_name;
		nmaterial_template::ptr	m_template;

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
		virtual void draw_effect_param(nshading_effect* effect_ptr) const;

		//--
		void create(nmaterial_template::ptr mtl_tmp);		
		void create(const resource_location& tmp_loc);

		//-- parameter interface
		void set_texture_parameter(const nstring& param_name, const resource_location& tex_loc);
		void set_vector_parameter(const nstring& param_name, const vector4& vec);
		void set_int_param(const nstring& param_name, int val);

		size_t get_num_parameter() const						{	return m_parameter_array.size();}
		const material_parameter_value& get_parameter(size_t i) const	{	return m_parameter_array[i];}
		
		virtual void serialize(narchive& ar);

		virtual void _destroy()
		{
			nmaterial_base::_destroy();
			m_parameter_array.clear();
		}
	private:		

		template<typename T>
		void set_parameter(const nstring& param_name, T val);
	private:
		typedef std::vector<material_parameter_value>	parameter_array;
		parameter_array			m_parameter_array;

		nDECLARE_NAMED_CLASS(nmaterial)

	};
}//namespace nexus
#endif //_NMATERIAL_INSTANCE_H_