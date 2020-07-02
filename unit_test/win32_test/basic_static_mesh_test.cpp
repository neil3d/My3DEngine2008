#include "StdAfx.h"
#include "basic_static_mesh_test.h"

nDEFINE_CLASS(basic_static_mesh_test, nrender_test)

basic_static_mesh_test::basic_static_mesh_test(void)
{
}

basic_static_mesh_test::~basic_static_mesh_test(void)
{
}

nresource_static_mesh::ptr _load_man_mesh()
{
	nresource_static_mesh::ptr mesh_ptr(
		nresource_manager::instance()->new_static_mesh(_T("gow_man_mesh"))
		);

	mesh_ptr->import_lod(-1, _T("../game_data/gow/man.fsm"));

	nstring pkg_name(_T("game_data:"));

	// make material
	nmaterial_template::ptr mtl_temp = nresource_manager::instance()->load_material_template_script(
		resource_location(
		_T("engine_data"),
		_T("material/normal_maped.hlsl")));

	nmaterial::ptr mtl_ptr1( nNew nmaterial(_T("test_mtl")) );
	mtl_ptr1->create(mtl_temp);
	mtl_ptr1->set_texture_parameter(_T("MTL_DiffuseMap"), resource_location(pkg_name+_T("gow/TEX_ManDiffuse.dds")) );
	mtl_ptr1->set_texture_parameter(_T("MTL_NormalMap"), resource_location(pkg_name+_T("gow/TEX_ManNormal.dds")));	

	mesh_ptr->import_set_material(0, 0, mtl_ptr1);

	return mesh_ptr;
}

void basic_static_mesh_test::open(editor_camera_controller* cam)
{
	nrender_test::open(cam);
	add_point_light();

	//--
	nresource_static_mesh::ptr mesh_ptr = _load_man_mesh();
	box_sphere_bounds bounds = mesh_ptr->get_bounds();

	nactor::ptr actor_ptr = m_level_ptr->create_actor(_T("gow_man_actor"));
	shared_ptr<nstatic_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);	

	cam->set_look_at(bounds.origin);

	m_light_anim_radius = bounds.sphere_radius*1.5f;
}