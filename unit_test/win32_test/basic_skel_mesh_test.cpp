#include "StdAfx.h"
#include "basic_skel_mesh_test.h"

nDEFINE_CLASS(basic_skel_mesh_test, nrender_test)

basic_skel_mesh_test::basic_skel_mesh_test(void)
{
}

basic_skel_mesh_test::~basic_skel_mesh_test(void)
{
}

void _load_Arthas_mtl(std::vector<nmaterial::ptr>& ret)
{
	// make material	
	nmaterial_template::ptr mtl_temp = nresource_manager::instance()->load_material_template_script(
		resource_location(
		_T("engine_data"),
		_T("material/diffuse_basic.hlsl")));

	nmaterial::ptr mtl_ptr1( nNew nmaterial(_T("test_mtl")) );
	mtl_ptr1->create(mtl_temp);
	mtl_ptr1->set_texture_parameter(_T("MTL_DiffuseMap"), 
		resource_location(_T("game_data:Arthas/ARTHASLICHKING_01.tga"))
		);

	nmaterial::ptr mtl_ptr2( nNew nmaterial(_T("test_mtl")) );
	mtl_ptr2->create(mtl_temp);
	mtl_ptr2->set_texture_parameter(_T("MTL_DiffuseMap"), 
		resource_location(_T("game_data:Arthas/ARTHASLICHKING_02.tga"))
		);

	ret.push_back(mtl_ptr1);
	ret.push_back(mtl_ptr2);
}

static nskeletal_mesh_component::ptr _load_skel_Arthas(nlevel::ptr level_ptr, box_sphere_bounds& bounds)
{
	nresource_skeletal_mesh::ptr mesh_ptr = 
		nresource_manager::instance()->new_skeletal_mesh(_T("ActorX_Arthas"));
	mesh_ptr->import_lod(_T("../game_data/Arthas/Arthas.psk"));

	std::vector<nmaterial::ptr> mtl_array;
	_load_Arthas_mtl(mtl_array);
	mesh_ptr->import_set_material(0, 0, mtl_array[0]);
	mesh_ptr->import_set_material(0, 1, mtl_array[0]);
	mesh_ptr->import_set_material(0, 2, mtl_array[1]);
	mesh_ptr->import_set_material(0, 3, mtl_array[1]);

	//--
	nactor::ptr actor_ptr = level_ptr->create_actor(_T("skel_anim_Arthas"));
	shared_ptr<nskeletal_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nskeletal_mesh_component>(_T("mesh_comp"));

	matrix44 scale;
	mat_set_scaling(scale, 100, 100, 100);
	actor_ptr->move(scale);

	comp_ptr->reset_resource(mesh_ptr, true);

	//-- anim
	nresource_skeletal_anim_set::ptr anim_set = nresource_manager::instance()->new_skeletal_anim(_T("ActorX_Arthas_Anim"));
	anim_set->import_anim_sequence(_T("../game_data/Arthas/walk.psa"));

	shared_ptr<nskel_anim_play> anim_ctrl = dynamic_pointer_cast<nskel_anim_play>
		( comp_ptr->reset_anim_controller(_T("nskel_anim_play")) );
	anim_ctrl->init(anim_set, true);

	//--
	bounds.box_extent = vector3(200,200,200);
	bounds.origin = vector3(0,0,0);
	bounds.sphere_radius = 140;

	return comp_ptr;
}

void basic_skel_mesh_test::open(editor_camera_controller* cam)
{
	nrender_test::open(cam);
	add_point_light();

	//--
	box_sphere_bounds bounds;	

	_load_skel_Arthas(m_level_ptr, bounds);			

	cam->set_look_at(bounds.origin);
	m_light_anim_radius = bounds.sphere_radius*1.5f;
}