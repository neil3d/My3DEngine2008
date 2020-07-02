#include "StdAfx.h"
#include "base_shadow_test.h"
#include <boost/lexical_cast.hpp>
#include "../../NexusNative/nengine/actor/nwater_component.h"

nDEFINE_CLASS(base_shadow_test, nrender_test)

base_shadow_test::base_shadow_test(void)
{
}

base_shadow_test::~base_shadow_test(void)
{
}


void base_shadow_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	m_light_anim_radius = 500;
//	add_spot_light();
	//add_direct_light(vector3(-1,-1,-1));

	nmtl_static::ptr mtl= create_test_material(_T("diffuse_basic"));
	mtl->set_two_side(true);

	int index = 0;
	int count = 2;
	float dist = 512.0f;
	float offset = (count - 1)*dist/2; 
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j ++)
		{
			for (int k = 0; k < count; k++)
			{
				add_sphere(nstring(_T("Sphere"))+boost::lexical_cast<std::wstring>(index++),vector3(i*dist - offset,k*dist,j*dist - offset),mtl);
			}			
		}
	}
	
	//add_teapot(_T("teapot"),vector3(0,30,0),mtl);


	////-- 创建一个底板
	//nactor::ptr box_ptr = m_level_ptr->create_actor(_T("Plane"));
	//shared_ptr<nshape_component> comp_ptr = box_ptr->create_component_t<nshape_component>(_T("shape_comp"));
	//comp_ptr->create_box(2000,5,2000);
	//comp_ptr->set_material(mtl);
	//box_ptr->move(vector3(0,-400,0));

	////雾
	//nactor::ptr fog_ptr = m_level_ptr->create_actor(_T("Fog"));
	//shared_ptr<nfog_volume_component> fog_comp_ptr 
	//	= fog_ptr->create_component_t<nfog_volume_component>(_T("fog_comp"));
	//
	//fog_comp_ptr->shape_type = nfog_volume_component::e_sphere;
	//fog_ptr->move(vector3(500,0,-300),vector3::zero,vector3(3,3,3));
	

	//水面
	//nactor::ptr water_ptr = m_level_ptr->create_actor(_T("water"));
	//shared_ptr<nwater_component> water_comp_ptr 
	//	= water_ptr->create_component_t<nwater_component>(_T("water_comp"));
	//
	//water_comp_ptr->move(vector3(100,0,300),vector3::zero,vector3(1,1,1));
}