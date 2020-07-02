#include "StdAfx.h"
#include "speed_tree_test.h"

nDEFINE_CLASS(speed_tree_test, nresource_test)

speed_tree_test::speed_tree_test(void)
{
}

speed_tree_test::~speed_tree_test(void)
{
}

nspeed_tree_component::ptr _load_spt(nlevel::ptr level_ptr, const vector3& pos, float scale)
{
	static int i = 0;
	wostringstream os;
	os << _T("speed_tree_actor")
		<< i;
	i++;
	
	nactor::ptr actor_ptr = level_ptr->create_actor(os.str());
	nspeed_tree_component::ptr spt_comp = actor_ptr->create_component_t<nspeed_tree_component>(_T("spt_comp"));

	int r = rand()%6;

	switch (4)
	{
	case 0:
		spt_comp->create(resource_location(_T("demo_data:spt/BambooBush_RT.spt")));
		break;
	case 1:
		spt_comp->create(resource_location(_T("demo_data:spt/Ficus_Cluster.spt")));
		break;
	case 2:
		spt_comp->create(resource_location(_T("demo_data:spt/Frailejon_Cluster.spt")));
		break;
	case 3:
		spt_comp->create(resource_location(_T("demo_data:spt/HydrillaCluster_RT.spt")));
		break;
	case 4:
		spt_comp->create(resource_location(_T(""/*"demo_data:spt/Kapok_RT.spt"*/)));
		break;
	case 5:
		spt_comp->create(resource_location(_T("demo_data:spt/MimosaTree_RT.spt")));
		break;	
	default:
		spt_comp->create(resource_location(_T("demo_data:spt/MonkeyPuzzle_RT.spt")));
		break;
	}
	

	actor_ptr->move(pos, vector3::zero, vector3(scale,scale,scale));	
	actor_ptr->set_depth_group(EDG_WorldGeometry);

	return spt_comp;
}

void speed_tree_test::open(perspective_camera_controller* cam)
{
	nresource_test::open(cam);
	
	int count = 1;

	for (int i = 0; i < count; i ++)
	{
		for (int j=0; j < 1; j ++)
		{
			vector3 pos((i - count/2)*100,0,(j - count/2)*100);
			m_spt_comp = _load_spt(m_level_ptr, pos, 1);
		}
	}

	m_light_anim_radius = 200;
	m_light_anim_radius = 200;

#ifdef ENABLE_LEVEL_SAVE_TEST	// Ë³±ã²âÊÔÒ»ÏÂlevel¶ÁÅÌ´æÅÌ
	resource_location lv_loc(_T("demo_data"), _T("level"));
	nengine::instance()->save_level(m_level_ptr->get_name(), lv_loc);
#endif

	cam->set_look_at(vector3::zero);
}

void speed_tree_test::debug_draw(nrender_primitive_draw_interface* PDI)
{
	if( m_spt_comp )
		m_spt_comp->draw_bounds(PDI, color4f(1,1,0,1));
}

void speed_tree_test::close()
{
	nresource_test::close();
	m_spt_comp.reset();
}