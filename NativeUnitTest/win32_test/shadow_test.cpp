#include "StdAfx.h"
#include "shadow_test.h"

nDEFINE_CLASS(shadow_test, nrender_test)

shadow_test::shadow_test(void)
{
}

shadow_test::~shadow_test(void)
{
}

extern nresource_static_mesh::ptr _load_man_mesh();
extern nactor::ptr _load_anim_Arthas(nlevel::ptr level_ptr, box_sphere_bounds& bounds);
extern nactor::ptr _load_skel_Arthas(nlevel::ptr level_ptr, box_sphere_bounds& bounds);
extern nactor::ptr _load_sun_char_head(nlevel::ptr level_ptr, box_sphere_bounds& bounds);

void shadow_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	vector3 lgt(0.76,-0.53,-0.37);
	add_direct_light( vec_normalize(lgt) );

	matrix44 mat;
	//-- box
	nactor::ptr box_actor = m_level_ptr->create_actor(_T("box_actor"));
	shared_ptr<nshape_component> box_comp_ptr 
		= box_actor->create_component_t<nshape_component>(_T("box_comp"));
	box_comp_ptr->create_box(2000, 20, 2000);

	nstring pkg_name(_T("game_data:"));
	nmaterial_template::ptr mtl_temp = nresource_manager::instance()->load_material_template_script(
		resource_location(
		_T("engine_data"),
		_T("material/diffuse_basic.hlsl")));

	nmaterial::ptr mtl_ptr1( nNew nmaterial(_T("box_mtl")) );
	mtl_ptr1->create(mtl_temp);
	mtl_ptr1->set_texture_parameter(_T("MTL_DiffuseMap"), resource_location(pkg_name+_T("terrain/cell.jpg")) );

	box_comp_ptr->set_material( mtl_ptr1 );
	
#if 0
	//-- sphere
	nactor::ptr sphere_actor = m_level_ptr->create_actor(_T("sphere_actor"));
	shared_ptr<nshape_component> sp_comp_ptr 
		= sphere_actor->create_component_t<nshape_component>(_T("sphere_comp"));

	sp_comp_ptr->create_sphere(120, 30, 20);
	sp_comp_ptr->set_material( mtl_ptr1 );

	mat_set_translation(mat, 200, 100, 200);
	sphere_actor->move(mat);
#endif
	
	
	//-- 建立一个角色
	nresource_static_mesh::ptr mesh_ptr = _load_man_mesh();
	
	nactor::ptr actor_ptr = m_level_ptr->create_actor(_T("gow_man_actor"));
	shared_ptr<nstatic_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);	

	float h = 255;
	actor_ptr->move(vector3(0,h,0),
		vector3::zero, vector3::one);

	m_man = actor_ptr;

	cam->set_look_at( vector3(0, h, 0) );

	//-- 一个动画角色
	box_sphere_bounds bounds;
	//nactor::ptr arthas = _load_anim_Arthas(m_level_ptr, bounds);
	nactor::ptr arthas = _load_skel_Arthas(m_level_ptr, bounds);
	
	arthas->move(vector3(-400,0,0), vector3::zero, vector3(200,200,200));

	//--
	nactor::ptr mask_test = _load_sun_char_head(m_level_ptr, bounds);
	mask_test->move(vector3(400,60,0), vector3::zero, vector3(5,5,5));
}

void shadow_test::tick(float run_time)
{
	//-- 移动角色
	float r = 120;
	float t = run_time*0.6f;

	vector3 pos(sinf(t)*r, 255, cosf(t)*r);
	m_man->move(pos, vector3::zero, vector3::one);
}