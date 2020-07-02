#include "StdAfx.h"
#include "test_mtl_alpha_mask.h"

nDEFINE_CLASS(test_mtl_alpha_mask, nrender_test)

test_mtl_alpha_mask::test_mtl_alpha_mask(void)
{
}

test_mtl_alpha_mask::~test_mtl_alpha_mask(void)
{
}


nactor::ptr _load_sun_char_head(nlevel::ptr level_ptr, box_sphere_bounds& bounds)
{
	nresource_static_mesh::ptr mesh_ptr = nresource_manager::instance()->new_static_mesh(_T("sun_char_head_mesh"));

	mesh_ptr->import_lod(-1, _T("../game_data/sun/sun.obj"));
	bounds = mesh_ptr->get_bounds();

	// make material
	nmaterial_template::ptr mtl_temp = nresource_manager::instance()->load_material_template_script(
		resource_location(
		_T("engine_data"),
		_T("material/diffuse_masked.hlsl")));
	mtl_temp->set_trans_type(ETrans_AlphaMasked);

	nmaterial::ptr mtl_ptr1( nNew nmaterial(_T("test_mtl")) );
	mtl_ptr1->create(mtl_temp);
	mtl_ptr1->set_texture_parameter(_T("MTL_DiffuseMap"), 
		resource_location(_T("game_data:sun/face.tga"))
		);	
	mtl_ptr1->set_int_param(_T("MTL_AlphaMask")	, 250);

	mesh_ptr->import_set_material(0, 0, mtl_ptr1);

	nmaterial::ptr mtl_ptr2( nNew nmaterial(_T("test_mtl")) );
	mtl_ptr2->create(mtl_temp);
	mtl_ptr2->set_texture_parameter(_T("MTL_DiffuseMap"), 
		resource_location(_T("game_data:sun/hair.tga"))
		);	
	mtl_ptr2->set_int_param(_T("MTL_AlphaMask")	, 250);

	mesh_ptr->import_set_material(0, 1, mtl_ptr2);
	mesh_ptr->post_edit_change(true);

	//-- add to level 
	nactor::ptr actor_ptr = level_ptr->create_actor(_T("sun_girl_actor"));
	shared_ptr<nstatic_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);

	return actor_ptr;
}

void test_mtl_alpha_mask::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	add_point_light();

	//--
	box_sphere_bounds bounds;	
	_load_sun_char_head(m_level_ptr, bounds);

	cam->set_look_at(bounds.origin);
	m_light_anim_radius = bounds.sphere_radius*1.5f;
}