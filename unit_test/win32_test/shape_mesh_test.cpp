#include "StdAfx.h"
#include "shape_mesh_test.h"

nDEFINE_CLASS(shape_mesh_test, nrender_test)

shape_mesh_test::shape_mesh_test(void)
{
}

shape_mesh_test::~shape_mesh_test(void)
{
}

void shape_mesh_test::open(editor_camera_controller* cam)
{
	nrender_test::open(cam);
	add_point_light();

	//--
	nactor::ptr actor_ptr = m_level_ptr->create_actor(_T("my_shape_actor"));
	shared_ptr<nshape_component> comp_ptr 
		= actor_ptr->create_component_t<nshape_component>(_T("shape_comp"));
	comp_ptr->create_box(100,100,100);

	//-- material
	nmaterial_template::ptr mtl_temp = nresource_manager::instance()->load_material_template_script(
		resource_location(
		_T("engine_data"),
		_T("material/diffuse_basic.hlsl")));

	nmaterial::ptr mtl_ptr( nNew nmaterial(_T("test_mtl")) );
	mtl_ptr->create(mtl_temp);
	mtl_ptr->set_texture_parameter(_T("MTL_DiffuseMap"), 
		resource_location(_T("game_data:terrain/cell.jpg"))
		);
	comp_ptr->set_material(mtl_ptr);


	m_light_anim_radius = 100*1.5f;
}