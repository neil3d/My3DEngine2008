#ifndef _NEXUS_MATERIAL_BASE_H_
#define _NEXUS_MATERIAL_BASE_H_
#include "ncore.h"
#include "nmtl_tech_shader.h"
#include "../renderer/render_define.h"

namespace nexus
{
	class nshading_effect;

	const TCHAR* const MTL_DEFAULT_TECH_CONST_ALL = _T("MtlDftTechConstAll");
	const TCHAR* const MTL_DEFAULT_TECH_CONST_COLOR = _T("MtlDftTechConstColor");

	/**
	 *	材质基类
	 *	@remarks 管理多个pass的shader，以及所有shader需要的参数
	*/
	class nAPI nmtl_base : public nobject
	{
	public:
		typedef boost::shared_ptr<nmtl_base> ptr;

		explicit nmtl_base(const nstring& name_str);
		virtual ~nmtl_base(void);

		//!	从hlsl文件创建材质数据
		virtual void create_from_hlsl(const nstring& name_str, const struct resource_location& shader_loc);		

		//! 从材质文件中创建一个材质
		static nmtl_base::ptr create_from_file( const resource_location& loc );

		//! 保存材质到文件
		void save_to_file( const resource_location& loc );

		/**	取得材质的透明类型
		 *	这个值需由上层维护，由任何一个Pass需要透明处理，则按照最高透明需求设置
		 */
		virtual enum EBlendType get_blend_type() const	{	return m_blend_type; }

		//!	设置透明类型
		virtual void set_blend_type(enum EBlendType val)	{	m_blend_type = val;}

		virtual void set_render_state(nshading_effect* effect_ptr);

		virtual void set_two_side( bool btrue) { is_two_side = btrue; }
		virtual bool get_two_side( ) const	{ return is_two_side; }

		virtual bool is_translucent()
		{
			return m_blend_type == ETranslucent || m_blend_type == EAdditive;
		}

		//!	取得某个tech所使用的shader
		nmtl_tech_shader::ptr get_shader();

		resource_location	get_shader_loc()	{return m_shader_loc;}

		//!	开始绘制某个pass，将它需要的参数设置到渲染用的effect中，并设置相应的渲染状态
		virtual void setup_effect(nshading_effect* effect_ptr) const 
		{
			(void)effect_ptr;
		};

		//!	是否内部用到的所有资源已经加载完成
		virtual bool resource_ready() const	{	return true;}

		//!	释放内部所有使用到的资源
		virtual void _destroy()	{}

		const nstring& get_name() const	{	return m_name; }
		void set_name(const nstring& nm){	m_name = nm; }

		virtual void serialize(narchive& ar);

		virtual const nshader_modifier* get_shader_mod()	
		{	
			return &s_shader_modifier; 
		}
	protected:
		virtual void bind_shader_param(nmtl_tech_shader::ptr mtl_shader)	{}

	protected:
		nstring			m_name;
		nmtl_tech_shader::ptr	m_shader;
		resource_location		m_shader_loc;
		EBlendType		m_blend_type;
		bool			is_two_side;
		nshader_modifier s_shader_modifier;
		nDECLARE_VIRTUAL_CLASS(nmtl_base)
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_BASE_H_