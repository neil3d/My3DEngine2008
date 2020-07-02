#include "StdAfx.h"
#include "basic_material_test.h"

nDEFINE_CLASS(basic_material_test, nrender_test)

basic_material_test::basic_material_test(void)
{
	
}

basic_material_test::~basic_material_test(void)
{
}

void basic_material_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	//add_point_light();

	test_material_static();
}

nmtl_static::ptr basic_material_test::create_material(const nstring& material_name)
{
	//-- 创建我们的材质
	nmtl_static::ptr mtl( new nmtl_static(material_name) );

	mtl->create_from_hlsl(material_name, resource_location(
		_T("engine_data"),
		nstring(_T("material/")) + material_name + nstring(_T(".hlsl")))
		);

	mtl->set_cube_map_parameter("MTL_EnvMap", resource_location(_T("demo_data:unit_test/sky_cube.dds")));
	mtl->set_texture2d_parameter("MTL_NormalMap", resource_location(_T("demo_data:unit_test/WaterNormal.jpg")));
//	mtl->set_value_parameter("MTL_SpecularPower", vector4(64,0,0,0));
	mtl->set_value_parameter("MTL_AlphaRef",vector4(0.7f,0,0,0));
	mtl->set_blend_type( ETranslucent );
	return mtl;
}

void basic_material_test::test_material_static()
{
	m_light_anim_radius = 100;

	//add_sphere(_T("sphere"),vector3(0,0,0),create_material(_T("standard")));

	//add_teapot(_T("teapot1"),vector3(-100,0,100),create_material(_T("specular_basic")));
	//add_teapot(_T("teapot2"),vector3(0,0,100),create_material(_T("CookTorrance")));
	/*add_teapot(_T("teapot3"),vector3(100,0,100),create_material(_T("schlick")));
	add_teapot(_T("teapot4"),vector3(-100,0,0),create_material(_T("OrenNayar")));
	add_teapot(_T("teapot5"),vector3(0,0,0),create_material(_T("BanksAnisotropic")));
	add_teapot(_T("teapot6"),vector3(100,0,0),create_material(_T("BlinnAnisotropic")));
	add_teapot(_T("teapot7"),vector3(0,0,-100),create_material(_T("KajiyaKayAnisotropic")));*/

	//-- 创建一个底板
	nactor::ptr box_ptr = m_level_ptr->create_actor(_T("Plane"));
	shared_ptr<nshape_component> comp_ptr = box_ptr->create_component_t<nshape_component>(_T("shape_comp"));
	comp_ptr->create_box(2000,5,2000);
	comp_ptr->set_material(create_material(_T("water")));
}