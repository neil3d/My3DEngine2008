#include "StdAfx.h"
#include <sstream>
#include "mtl_extend_tech_test.h"

nDEFINE_CLASS(mtl_extend_tech_test, nrender_test)

mtl_extend_tech_test::mtl_extend_tech_test(void)
{
}

mtl_extend_tech_test::~mtl_extend_tech_test(void)
{
}

extern nmtl_base::ptr _load_gow_man_mtl();
void mtl_extend_tech_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	add_point_light();

	//-- 创建mesh资源
	nresource_static_mesh::ptr mesh_ptr(
		nresource_manager::instance()->new_static_mesh(_T("gow_man_mesh"))
		);

	mesh_ptr->import_lod(-1, _T("../nexus_game/demo_data/unit_test/gow/man.obj"));

	nmtl_static::ptr mtl = dynamic_pointer_cast<nmtl_static>(_load_gow_man_mtl());
	
	//nmtl_tech::ptr zfail = mtl->create_from_hlsl( _T("ZFail"), resource_location(_T("engine_data:material/ENG_ZFail.hlsl")) );
	//zfail->set_render_state(ERS_ZTestEnable, true);
	//zfail->set_render_state(ERS_ZWriteEnable, false);
	//zfail->set_render_state(ERS_ZTestFunc, ECMP_NotEqual);	
	//mtl->bind_tech_usage(_T("ZFail"), EDP_Extend);

	//nmtl_tech::ptr slt = mtl->create_from_hlsl( _T("Silhouette"), resource_location(_T("engine_data:material/ENG_ConstColor.hlsl")) );
	//slt->set_render_state(ERS_ZTestEnable, true);
	//slt->set_render_state(ERS_ZWriteEnable, false);
	//slt->set_render_state(ERS_ZTestFunc, ECMP_LessEqual);	
	//slt->set_render_state(ERS_DepthBias, -0.0002f);
	//slt->set_render_state(ERS_SlopeScaleDepthBias, 1.9f);
	//slt->set_render_state(ERS_FillMode, EFM_Wireframe);
	//slt->set_render_state(ERS_CullMode, ECM_None);
	//mtl->bind_tech_usage(_T("Silhouette"), (enum EDrawingPolicy)(EDP_Extend+1) );

	

	mesh_ptr->import_set_material(0, 0, mtl);
	mesh_ptr->post_edit_change(true);

	//-- 向level中添加actor
	for(int y=0; y<3; y++)
	{
		for(int x=0; x<3; x++)
		{
			std::wostringstream ss;
			ss << _T("test_actor_") << x <<y;
			nactor::ptr actor_ptr = m_level_ptr->create_actor(ss.str());
			shared_ptr<nstatic_mesh_component> comp_ptr 
				= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
			comp_ptr->reset_resource(mesh_ptr);	

			float s = 300;
			vector3 r = vec3_rand_unit()*nTwoPI;
			actor_ptr->move(vector3(x*s,0,y*s), vector3(0, r.x, 0), vector3::one);
		}
	}	
}
