/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	July, 2009
*/
#ifndef _NEXUS_SKY_BOX_H_
#define _NEXUS_SKY_BOX_H_
#include "sky_base.h"
#include "../resource/nresource.h"
#include "../mesh/mesh_vertex_data.h"
#include "../renderer/nrender_static_mesh.h"
#include "../renderer/nrender_texture.h"
#include "../material/nmaterial.h"

namespace nexus
{
	/**
	 *	只包含一个pos stream
	 *	@remakrs 适用于天空盒，使用pos作为cube map的纹理坐标
	*/
	class nsky_cube_vertex_data
		: public nmesh_vertex_data
	{
	public:
		nsky_cube_vertex_data(void);
		virtual ~nsky_cube_vertex_data(void);

		//!	初始化，填充内部stream
		void init();

		//--
		virtual size_t get_num_stream() const	{	return 1;}
		virtual vertex_stream* get_stream(size_t stream_index)	
		{
			(void)stream_index;
			return &m_pos_stream;
		}
		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;
	};

	/**
		使用一个Cube Map形成天空Diffuse
	*/
	class nsky_cube_material
		: public nmaterial_base
	{
	public:
		nsky_cube_material(const nstring& name_str);
		virtual ~nsky_cube_material(void);

		//!	创建材质
		void create(const resource_location& texture_loc);

		virtual void draw_effect_param(nshading_effect* effect_ptr) const;

		virtual void serialize(narchive& ar);
	private:		
		resource_location					m_texture_loc;
		render_res_ptr<nrender_cube_map>	m_texture;
	};

	/**
	 *	使用一个Skybox和一个Cube Map渲染简单的天空效果
	*/
	class nsky_box
		: public nsky_component
	{
	public:
		explicit nsky_box(const nstring& name_str);
		virtual ~nsky_box(void);

		//!	创建内部mesh和材质
		void create(const resource_location& texture_loc);
		virtual nmaterial_base* get_material(int lod, int mtl_id);		
		virtual nrender_mesh* get_render_mesh(int lod);		

		virtual void _destroy();
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nactor* owner);
	
	private:
		void create_render_mesh();

		render_res_ptr<nrender_static_mesh>	m_mesh;
		nsky_cube_material	m_mtl;

		nDECLARE_NAMED_CLASS(nsky_box)
	};
}//namespace nexus
#endif //_NEXUS_SKY_BOX_H_