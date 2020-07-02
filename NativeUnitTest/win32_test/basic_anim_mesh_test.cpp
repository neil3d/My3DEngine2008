#include "StdAfx.h"
#include "basic_anim_mesh_test.h"

nDEFINE_CLASS(basic_anim_mesh_test, nrender_test)

basic_anim_mesh_test::basic_anim_mesh_test(void)
{
}

basic_anim_mesh_test::~basic_anim_mesh_test(void)
{
}

extern void _load_Arthas_mtl(std::vector<nmaterial::ptr>& ret);
nactor::ptr _load_anim_Arthas(nlevel::ptr level_ptr, box_sphere_bounds& bounds)
{
	nresource_anim_mesh::ptr mesh_ptr( 
		nresource_manager::instance()->new_anim_mesh(_T("anim_Arthas"))
		);

	mesh_ptr->import_lod(0, _T("../game_data/Arthas/Arthas.fak"));
	
	std::vector<nmaterial::ptr> mtl_array;
	_load_Arthas_mtl(mtl_array);

	mesh_ptr->import_set_material(0, 0, mtl_array[0]);
	mesh_ptr->import_set_material(0, 1, mtl_array[0]);
	mesh_ptr->import_set_material(0, 2, mtl_array[1]);
	mesh_ptr->import_set_material(0, 3, mtl_array[1]);
	
	//--
	nactor::ptr actor_ptr = level_ptr->create_actor(_T("anim_Arthas_actor"));
	shared_ptr<nanim_mesh_component> comp_ptr = actor_ptr->create_component_t<nanim_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);

	actor_ptr->move(vector3::zero, vector3::zero, vector3(100,100,100));

	bounds = comp_ptr->get_bounds();
	bounds.box_extent *= 100;
	bounds.sphere_radius *= 100;

	return actor_ptr;
}

void basic_anim_mesh_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	add_point_light();

	//--
	box_sphere_bounds bounds;	

	_load_anim_Arthas(m_level_ptr, bounds);

	cam->set_look_at(bounds.origin);
	m_light_anim_radius = bounds.sphere_radius*1.5f;
}