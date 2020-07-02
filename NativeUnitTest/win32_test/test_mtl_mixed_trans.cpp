#include "StdAfx.h"
#include "test_mtl_mixed_trans.h"

nDEFINE_CLASS(test_mtl_mixed_trans, nrender_test)

test_mtl_mixed_trans::test_mtl_mixed_trans(void)
{
}

test_mtl_mixed_trans::~test_mtl_mixed_trans(void)
{
}

static void _load_box(nlevel::ptr level_ptr, box_sphere_bounds& bounds)
{
	nresource_static_mesh::ptr mesh_ptr = nresource_manager::instance()->new_static_mesh(_T("trans_box"));

	mesh_ptr->import_lod(-1, _T("../game_data/box/box.obj"));
	bounds = mesh_ptr->get_bounds();


	//-- material 
	nmaterial_template::ptr mtl_temp = nresource_manager::instance()->load_material_template_script(
		resource_location(
		_T("engine_data"),
		_T("material/diffuse_alpha.hlsl")
		)
		);

	// 指定半透明类型
	mtl_temp->set_trans_type(ETrans_Mixed);

	nmaterial::ptr mtl_ptr1( nNew nmaterial(_T("test_mtl")) );
	mtl_ptr1->create(mtl_temp);
	mtl_ptr1->set_texture_parameter(_T("MTL_DiffuseMap"), 
		resource_location(_T("game_data:box/box.tga"))
		);	

	mesh_ptr->import_set_material(0, 0, mtl_ptr1);

	//-- add to level 
	nactor::ptr actor_ptr = level_ptr->create_actor(_T("trans_box_actor"));
	shared_ptr<nstatic_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);
}

void test_mtl_mixed_trans::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	add_point_light();

	//--
	box_sphere_bounds bounds;	
	_load_box(m_level_ptr, bounds);

	cam->set_look_at(bounds.origin);
	m_light_anim_radius = bounds.sphere_radius*1.5f;
}