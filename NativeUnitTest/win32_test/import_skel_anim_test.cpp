#include "StdAfx.h"
#include "import_skel_anim_test.h"

nDEFINE_CLASS(import_skel_anim_test, nresource_test)

import_skel_anim_test::import_skel_anim_test(void)
{
}

import_skel_anim_test::~import_skel_anim_test(void)
{
}

#if 1
	const TCHAR* PSK_FILE = _T("../nexus_game/demo_data/unit_test/Arthas/Arthas.psk");
	const TCHAR* PSA_FILE = _T("../nexus_game/demo_data/unit_test/Arthas/walk.psa");
#else
	const TCHAR* PSK_FILE = _T("../nexus_game/demo_data/unit_test/npc01/npc.psk");
	const TCHAR* PSA_FILE = _T("../nexus_game/demo_data/unit_test/npc01/npc_anim.psa");
#endif

extern void _load_Arthas_mtl(std::vector<nmtl_base::ptr>& ret);
void import_skel_anim_test::open(perspective_camera_controller* cam)
{
	nresource_test::open(cam);

	//-- 导入psk资源
	nresource_skeletal_mesh::ptr mesh_ptr = 
		nresource_manager::instance()->new_skeletal_mesh(_T("ActorX_Arthas"));
	mesh_ptr->import_lod(0, PSK_FILE);

	std::vector<nmtl_base::ptr> mtl_array;
	_load_Arthas_mtl(mtl_array);
	mesh_ptr->import_set_material(0, 0, mtl_array[0]);
	mesh_ptr->import_set_material(0, 1, mtl_array[0]);
	mesh_ptr->import_set_material(0, 2, mtl_array[1]);
	mesh_ptr->import_set_material(0, 3, mtl_array[1]);

	mesh_ptr->post_edit_change(true);

	//-- 创建actor
	m_skeletal_mesh_actor = m_level_ptr->create_actor(_T("skel_anim_Arthas"));
	m_skeletal_mesh_component = m_skeletal_mesh_actor->create_component_t<nskeletal_mesh_component>(_T("mesh_comp"));

	m_skeletal_mesh_actor->move(vector3::zero, vector3::zero, vector3(100,100,100));

	m_skeletal_mesh_component->reset_resource(mesh_ptr, true);

	//-- 导入psa资源
	nresource_skeletal_anim_set::ptr anim_set = nresource_manager::instance()->new_skeletal_anim(_T("ActorX_Arthas_Anim"));
	anim_set->import_anim_sequence(PSA_FILE);

	//-- 绑定动画控制器
	shared_ptr<nskel_anim_play> anim_ctrl = dynamic_pointer_cast<nskel_anim_play>
		( m_skeletal_mesh_component->reset_anim_controller(_T("nskel_anim_play")) );
	anim_ctrl->init(anim_set, true);

}

void import_skel_anim_test::debug_draw( nrender_primitive_draw_interface* PDI )
{
	if(m_skeletal_mesh_component)
	{
		m_skeletal_mesh_component->draw_skeleton(PDI);
	}
}