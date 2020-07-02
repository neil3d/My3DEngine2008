/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NRENDER_RESOURCE_RENDERABLE_H_
#define _NRENDER_RESOURCE_RENDERABLE_H_
#include "nrender_resource.h"
#include "render_define.h"
#include "../material/nrender_state.h"
#include "../mesh/mesh_vertex_data.h"

namespace nexus
{
	class nrender_texture;

	/**
	*	just a interface holder
	*	@remark index buffer + material id
	*/
	class nAPI nrender_mesh_section :
		public nrender_resource
	{
	public:
		nrender_mesh_section(void):m_material_id(-1),m_draw_type(EDraw_None),m_primitive_count(0),
			m_draw_limit(-1)
		{	}
		virtual ~nrender_mesh_section(void)	{	}

		int get_material_id() const							{	return m_material_id;}
		void set_material_id(int mtl_id)					{	m_material_id=mtl_id;}
		enum EPrimitiveType get_primitive_type() const		{	return m_draw_type;	}	
		size_t get_primitive_count() const	{	return m_primitive_count;}

		void set_draw_limit(int primitive_count)
		{
			nASSERT(primitive_count <= m_primitive_count);
			m_draw_limit = primitive_count;
		}
	protected:
		int					m_material_id;
		enum EPrimitiveType	m_draw_type;
		int					m_primitive_count;
		int					m_base_vertex_index;
		int					m_draw_limit;

		nDECLARE_VIRTUAL_CLASS(nrender_mesh_section)
	};

	/**
	*	just a interface holder
	*/
	class nAPI nvertex_factory :
		public nrender_resource
	{
	public:
		nvertex_factory(void)	{}
		virtual ~nvertex_factory(void)	{}

		nDECLARE_VIRTUAL_CLASS(nvertex_factory)
	};

	class nAPI nshading_effect:
		public nrender_resource
	{
	public:
		nshading_effect(void)	{}
		virtual ~nshading_effect(void)	{}

		//! 渲染状态设置
		virtual void set_render_state(enum ERenderState rs, int val) = 0;

		//!	Shader/Effect参数设置
		virtual void set_int(const std::string& name, int val) = 0;
		virtual void set_float(const std::string& name, float val) = 0;
		virtual void set_vector(const std::string& name, const vector2& val) = 0;
		virtual void set_vector(const std::string& name, const vector3& val) = 0;
		virtual void set_vector(const std::string& name, const vector4& val) = 0;
		virtual void set_matrix(const std::string& name, const matrix44& mat) = 0;
		virtual void set_texture(const std::string& name, const nrender_texture* tex) = 0;
		virtual void set_matrix_array(const std::string& name, const matrix44* mat, size_t count) = 0;
		virtual void set_float_array(const std::string& semantic_str, const float* val, size_t count) = 0;		
		
		nDECLARE_VIRTUAL_CLASS(nshading_effect)
	};

	/**
	 *	可渲染的资源
	 *	每个对象只对应mesh的一个lod
	*/
	class nAPI nrender_mesh :
		public nrender_resource
	{
	public:
		nrender_mesh(void)	{}
		virtual ~nrender_mesh(void)	{}

		virtual nshader_modifier* get_shader_mod()	{	return NULL; }
		virtual nvertex_factory* get_vertex_factory() = 0;
		virtual size_t get_num_section() = 0;		
		virtual nrender_mesh_section* get_section(size_t section_index) = 0;

		//--用于dynamic mesh来做每一帧的渲染前准备和渲染后清理工作
		/**
		 *	@return 如果返回true，则需要post render处理；返回false则不需要
		*/
		virtual bool pre_render()	{	return false; }
		virtual void post_render()	{}

		virtual void draw_setup_effect(nshading_effect* effect_ptr)	
		{
			(void)effect_ptr;
		}

		nDECLARE_VIRTUAL_CLASS(nrender_mesh)
	};
}//namespace nexus
#endif //_NRENDER_RESOURCE_RENDERABLE_H_