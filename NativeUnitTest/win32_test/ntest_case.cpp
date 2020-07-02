#include "StdAfx.h"
#include "ntest_case.h"

nDEFINE_VIRTUAL_CLASS(ntest_case, nobject)
nDEFINE_VIRTUAL_CLASS(nrender_test, ntest_case)
nDEFINE_VIRTUAL_CLASS(nresource_test, ntest_case)
nDEFINE_VIRTUAL_CLASS(nphys_test, ntest_case)

void ntest_case::add_point_light()
{
	wostringstream oss;
	oss << _T("light_actor_")
		<< ::rand();

	nactor::ptr new_actor = m_level_ptr->create_actor(oss.str());	
	new_actor->set_static(false);

	// 使用一个shape component显示light的位置
	shared_ptr<nshape_component> box(
		new_actor->create_component_t<nshape_component>(_T("box"))
		);	
	box->create_box(10,10,10);	
	box->set_accept_dynamic_light(false);

	// 创建light component
	shared_ptr<npoint_light_component> lgt(
		new_actor->create_component_t<npoint_light_component>(_T("point_light"))
		);	
	
	//lgt->m_color = color4f(1, 1, 1, 1);
	lgt->m_attenuation = vector3(m_light_anim_radius*2,2,3);
	m_light_array.push_back(new_actor);
}

void ntest_case::add_spot_light()
{
	wostringstream oss;
	oss << _T("light_actor_")
		<< ::rand();

	nactor::ptr new_actor = m_level_ptr->create_actor(oss.str());	
	new_actor->set_static(false);

	// 使用一个shape component显示light的位置
	shared_ptr<nshape_component> box(
		new_actor->create_component_t<nshape_component>(_T("spot_light"))
		);	
	box->create_box(10,10,10);	
	box->set_accept_dynamic_light(false);

	// 创建light component
	shared_ptr<nspot_light_component> lgt(
		new_actor->create_component_t<nspot_light_component>(_T("spot_light"))
		);	

	lgt->m_attenuation = vector3(m_light_anim_radius*2,0.5,1);
	m_light_array.push_back(new_actor);
}

//void ntest_case::anim_lights(float run_time)
//{
//	if( m_light_array.empty() )
//		return;
//
//	float step = 20;
//	for(size_t i=0; i<m_light_array.size(); i++)
//	{
//		nactor::ptr lgt_actor = m_light_array[i];
//
//		float t = run_time*0.6f+step*i;
//		float r = m_light_anim_radius;
//
//		vector3 pos;
//		switch(i%3)
//		{
//		case 0:
//			pos = vector3(sinf(t)*r, cosf(t)*r, sinf(t)*cosf(t)*r);
//			break;
//		case 1:
//			pos = vector3(sinf(t)*r, cosf(t)*r, 0);
//			break;
//		case 2:
//			pos = vector3(sinf(t)*r, 0, cosf(t)*r);
//			break;
//		}
//
//		lgt_actor->move(pos, vector3::zero, vector3::one);
//	}
//}

void ntest_case::anim_lights(float run_time)
{
	if( m_light_array.empty() )
		return;

	float step = 20;
	for(size_t i=0; i<m_light_array.size(); i++)
	{
		nactor::ptr lgt_actor = m_light_array[i];

		float t = run_time*0.6f+step*i;
		float r = m_light_anim_radius;
		vector3 pos;

		pos = vector3(sinf(t)*r, m_light_anim_radius, cosf(t)*r);
		//pos = vector3(0, m_light_anim_radius, 0);
		lgt_actor->move(pos, vector3::zero, vector3::one);

		lgt_actor->look_at(vector3::zero,vector3(1,0,0));
	}
}

void ntest_case::add_teapot(const nstring& actor_name,vector3 position,nmtl_base::ptr mtl_ptr)
{
	nactor::ptr actor_ptr = m_level_ptr->create_actor(actor_name);
	shared_ptr<nstatic_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));

	if (!m_teapot_ptr)
	{
		 m_teapot_ptr = nresource_manager::instance()->new_static_mesh(actor_name);
		 m_teapot_ptr->import_lod(-1, _T("../nexus_game/demo_data/unit_test/obj/teapot.obj"));
		 m_teapot_ptr->import_set_material(0,0,mtl_ptr);
		 m_teapot_ptr->post_edit_change(true);
	}
	
	comp_ptr->reset_resource(m_teapot_ptr);	
	comp_ptr->replace_material(0,0,mtl_ptr);
	actor_ptr->move(position, vector3::zero, vector3::one);
}

void ntest_case::add_sphere(const nstring& actor_name,vector3 position,nmtl_base::ptr mtl_ptr)
{
	//-- 创建一个简单的球体mesh Component
	nactor::ptr actor_ptr = m_level_ptr->create_actor(actor_name);
	actor_ptr->move(position, vector3::zero, vector3::one);
	shared_ptr<nshape_component> comp_ptr 
	= actor_ptr->create_component_t<nshape_component>(_T("shape_comp"));
	comp_ptr->create_sphere(50,16,16);
	comp_ptr->set_material(mtl_ptr);
	comp_ptr->set_enable_instance(false);
}

ndirectional_light_component* ntest_case::add_direct_light(const vector3& direction)
{
	wostringstream oss;
	oss << _T("light_actor_")
		<< ::rand();

	nactor::ptr new_actor = m_level_ptr->create_actor(oss.str());		
	new_actor->set_static(false);
	new_actor->move(-direction*m_light_anim_radius);
	new_actor->look_at(vector3(0,0,0));

	//-- 创建light component
	shared_ptr<ndirectional_light_component> lgt(
		new_actor->create_component_t<ndirectional_light_component>(_T("direct_light"))
		);	

	lgt->m_color = color4f(1.0f, 1.0f, 1.0f, 1);
	lgt->m_shadow_softness = 1;
	//lgt->project_shadow = false;

	//--
	m_light_array.push_back(new_actor);

	return lgt.get();
}

void ntest_case::remove_last_light()
{
	if( !m_light_array.empty() )
	{
		nactor::ptr rm_actor = m_light_array[m_light_array.size()-1];
		m_light_array.pop_back();
		m_level_ptr->destroy_actor(rm_actor->get_name());
	}
}

void ntest_case::open(perspective_camera_controller* cam)	
{
	//-- init level
	m_level_ptr = nengine::instance()->create_level(_T("test_lv"), _T("nlevel"));
#if 1
	m_level_ptr->init();
#else
	m_level_ptr->init();
#endif
}

nmtl_static::ptr nrender_test::create_test_material(const nstring& material_name)
{
	//-- 创建我们的材质
	nmtl_static::ptr mtl( new nmtl_static(material_name) );

	mtl->create_from_hlsl(material_name, resource_location(
		_T("engine_data"),
		nstring(_T("material/")) + material_name + nstring(_T(".hlsl")))
		);

	mtl->set_texture2d_parameter("MTL_DiffuseMap", resource_location(_T("demo_data:unit_test/detail_metal.dds")));
	//mtl->set_texture2d_parameter("MTL_NormalMap", resource_location(_T("demo_data:detail_metal_ddn.dds")));
	//mtl->set_texture2d_parameter("MTL_SpecularMap", resource_location(_T("demo_data:paint_whitemetal_df_.dds")));
	mtl->set_value_parameter("MTL_SpecularPower", vector4(64,0,0,0));

	return mtl;
}