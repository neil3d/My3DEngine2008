#include "StdAfx.h"
#include "nsky_box.h"
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nsky_box, nsky_component)
	////////////////////////////////////////////////////////////////////////////
	//	class nsky_cube_vertex_data
	////////////////////////////////////////////////////////////////////////////
	nsky_cube_vertex_data::nsky_cube_vertex_data(void)
	{}

	nsky_cube_vertex_data::~nsky_cube_vertex_data(void)
	{}

	nstring nsky_cube_vertex_data::get_vertex_type_name() const
	{
		return _T("sky_cube_vertex");
	}

	nstring nsky_cube_vertex_data::get_vertex_factory_name() const
	{
		return _T("vfactory_sky_cube");
	}

	void nsky_cube_vertex_data::init()
	{
		//-- create stream data
		size_t num_vert = 6*2*3;
		init_pos_stream( num_vert);

		//-- fill stream
		vector3 p[8];
		p[0] = vector3( 1, 1,-1 );
		p[1] = vector3( -1, 1,-1 );
		p[2] = vector3( -1,-1,-1 );
		p[3] = vector3(  1,-1,-1 );

		p[4] = vector3(  1, 1, 1 );
		p[5] = vector3( -1, 1, 1 );
		p[6] = vector3( -1,-1, 1 );
		p[7] = vector3(  1,-1, 1 );

		size_t i=0;
		m_pos_stream.set_element(i++, 0, p[0]);
		m_pos_stream.set_element(i++, 0, p[1]);
		m_pos_stream.set_element(i++, 0, p[3]);

		m_pos_stream.set_element(i++, 0, p[1]);
		m_pos_stream.set_element(i++, 0, p[2]);
		m_pos_stream.set_element(i++, 0, p[3]);

		m_pos_stream.set_element(i++, 0, p[0]);
		m_pos_stream.set_element(i++, 0, p[4]);
		m_pos_stream.set_element(i++, 0, p[1]);

		m_pos_stream.set_element(i++, 0, p[1]);
		m_pos_stream.set_element(i++, 0, p[4]);
		m_pos_stream.set_element(i++, 0, p[5]);

		m_pos_stream.set_element(i++, 0, p[5]);
		m_pos_stream.set_element(i++, 0, p[4]);
		m_pos_stream.set_element(i++, 0, p[7]);

		m_pos_stream.set_element(i++, 0, p[5]);
		m_pos_stream.set_element(i++, 0, p[7]);
		m_pos_stream.set_element(i++, 0, p[6]);

		m_pos_stream.set_element(i++, 0, p[1]);
		m_pos_stream.set_element(i++, 0, p[6]);
		m_pos_stream.set_element(i++, 0, p[5]);

		m_pos_stream.set_element(i++, 0, p[1]);
		m_pos_stream.set_element(i++, 0, p[2]);
		m_pos_stream.set_element(i++, 0, p[6]);

		m_pos_stream.set_element(i++, 0, p[0]);
		m_pos_stream.set_element(i++, 0, p[3]);
		m_pos_stream.set_element(i++, 0, p[7]);

		m_pos_stream.set_element(i++, 0, p[0]);
		m_pos_stream.set_element(i++, 0, p[7]);
		m_pos_stream.set_element(i++, 0, p[4]);

		m_pos_stream.set_element(i++, 0, p[2]);
		m_pos_stream.set_element(i++, 0, p[6]);
		m_pos_stream.set_element(i++, 0, p[7]);

		m_pos_stream.set_element(i++, 0, p[2]);
		m_pos_stream.set_element(i++, 0, p[7]);
		m_pos_stream.set_element(i++, 0, p[3]);

		
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nsky_cube_material
	////////////////////////////////////////////////////////////////////////////
	nsky_cube_material::nsky_cube_material(const nstring& name_str):nmaterial_base(name_str)
	{}

	nsky_cube_material::~nsky_cube_material(void)
	{}

	void nsky_cube_material::create(const resource_location& texture_loc)
	{
		m_template = nresource_manager::instance()->load_material_template_script(
			resource_location(_T("engine_data:material/SKY_cube.hlsl"))
			);

		//-- load texture
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		m_texture.reset( rres_mgr->alloc_cube_map() );
		m_texture->load_from_file( nengine::instance()->get_file_sys(),
			texture_loc.pkg_name, texture_loc.file_name );

		m_texture_loc = texture_loc;
	}

	void nsky_cube_material::draw_effect_param(nshading_effect* effect_ptr) const
	{
		effect_ptr->set_texture("MTL_DiffuseMap", m_texture.get());
	}

	void nsky_cube_material::serialize(narchive& ar)
	{
		nmaterial_base::serialize(ar);

		nSERIALIZE(ar, m_texture_loc);

		if( ar.is_loading() )
			create( m_texture_loc );
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nsky_box
	////////////////////////////////////////////////////////////////////////////
	nsky_box::nsky_box(const nstring& name_str):nsky_component(name_str),
		m_mtl(_T("SKY_BOX_CUBE_MTL"))
	{
	}

	nsky_box::~nsky_box(void)
	{
	}

	void nsky_box::create_render_mesh()
	{
		nsky_cube_vertex_data	vb;
		vb.init();

		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		m_mesh.reset( rres_mgr->alloc_static_mesh() );
		m_mesh->create(EDraw_TriangleList, &vb);
	}

	void nsky_box::create(const resource_location& texture_loc)
	{
		//-- mesh
		create_render_mesh();

		//-- material
		m_mtl.create(texture_loc);
	}

	nmaterial_base* nsky_box::get_material(int lod, int mtl_id)
	{
		return &m_mtl;
	}

	nrender_mesh* nsky_box::get_render_mesh(int lod)
	{
		return m_mesh.get();
	}

	void nsky_box::serialize(narchive& ar)
	{
		nsky_component::serialize(ar);

		nSERIALIZE(ar, m_mtl);
	}
	
	void nsky_box::_level_loaded(nactor* owner)
	{
		nsky_component::_level_loaded(owner);

		create_render_mesh();
	}

	void nsky_box::_destroy()
	{
		m_mesh.get();
	}
}//namespace nexus