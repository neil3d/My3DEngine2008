#include "StdAfx.h"
#include "shadow_test.h"

nDEFINE_NAMED_CLASS(TestActor, nactor)
nDEFINE_CLASS(shadow_test, nrender_test)

const float CHARA_HEIGHT=200;
const int	INSTANCE_COUNT=30;
const float MOVE_SPEED=100.0f;

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

	nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
	m_font=rres_mgr->alloc_render_font();
	m_font->create( _T("宋体"),0,24,800);

	m_test_icon=nresource_manager::instance()->load_texture_2d(resource_location(_T("game_data"),_T("test/test_icon.png")));
	m_icon_rect=frect(0.0f,0.0f,1.0f,1.0f);

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
	
	float h = 255;
	
	nresource_static_mesh::ptr mesh_ptr = _load_man_mesh();

	//-- 建立n个角色
	for(int i=0;i<INSTANCE_COUNT;++i)
	{
		std::wostringstream ss;
		ss 	<< "gow_man_actor id="<<i;
		nactor::ptr actor_ptr = m_level_ptr->create_actor(ss.str(),_T("TestActor"));
		shared_ptr<nstatic_mesh_component> comp_ptr 
				= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
		comp_ptr->reset_resource(mesh_ptr);	

		float s=0.7f+(rand()%6)/10.0f;
		float sh=0.7f+(rand()%6)/10.0f;
		int xoffset=i*(rand()%100-50);
		int yoffset=i*(rand()%100-50);
		actor_ptr->move(vector3(rand()%512-256+xoffset,h*sh,rand()%512-256+yoffset),
		vector3(0,rand()%360/360.0f*6.28f,0), vector3(s,sh,s));
		m_man.push_back(actor_ptr);
	}	
	
	cam->set_look_at( vector3(0, h, 0) );

	//-- 一个动画角色
	//box_sphere_bounds bounds;
	////nactor::ptr arthas = _load_anim_Arthas(m_level_ptr, bounds);
	//nactor::ptr arthas = _load_skel_Arthas(m_level_ptr, bounds);
	//
	//arthas->move(vector3(-400,0,0), vector3::zero, vector3(200,200,200));

	////--
	//nactor::ptr mask_test = _load_sun_char_head(m_level_ptr, bounds);
	//mask_test->move(vector3(400,60,0), vector3::zero, vector3(5,5,5));

}

void shadow_test::tick(float run_time)
{
	//-- 移动角色
	
}

void shadow_test::debug_canvas( nui_canvas* canvas )
{
	for(int i=0;i<INSTANCE_COUNT;++i)
	{
		// 计算角色的名字显示范围在当前摄像机参数下在屏幕上的投影位置
		shared_ptr<TestActor> ta=boost::shared_dynamic_cast<TestActor>(m_man[i]);
		if(ta)
		{
			const vector4& pt=ta->get_screen_position();
			frect rc;
			wstring name=ta->get_name().name_str.c_str();
			m_font->measure_string(name.c_str(),DT_CALCRECT,rc);
			frect rect(pt.x-rc.get_width()*0.5f,pt.y-rc.get_height()*0.5f,pt.x+rc.get_width()*0.5f,pt.y+rc.get_height()*0.5f);
			frect hp(rect.left+50.0f,rect.top-20.0f,rect.right-30.0f,rect.top-10.0f);
			frect icon(hp.left-hp.get_height()*2,hp.top-hp.get_height()*0.5f,hp.left,hp.bottom+hp.get_height()*0.5f);
			vector2 scale_center(rect.get_center().x,rect.get_center().y);
			canvas->draw_rect(hp,pt.z,(1.0f-pt.w)*(1.0f-pt.w),scale_center,0xaaff0000);
			canvas->draw_string(m_font,name.c_str(),rect,pt.z,(1.0f-pt.w)*(1.0f-pt.w),scale_center,0xFFFFFFFF,DT_CENTER|DT_VCENTER);
			canvas->draw_texture(m_test_icon->get_render_texture2d(),m_icon_rect,icon,pt.z,(1.0f-pt.w)*(1.0f-pt.w),scale_center,0xFFFFFFFF);
		}	
	}
	
}

void shadow_test::_on_device_lost( int param )
{
	m_test_icon->_on_device_lost(param);
}

bool shadow_test::_on_device_reset( int param )
{
	return m_test_icon->_on_device_reset(param);;
}
//===============================================================================

void TestActor::update( float delta_time, const nviewport& view )
{
	nactor::update(delta_time,view);
	m_screen_pos=view.camera.world2screen_with_depth(m_pos);
	float t=delta_time-m_start_time;

	if(m_time>2.0f)
	{
		vector3 v((rand()%100-50.0f)/50.0f,0,(rand()%100-50.0f)/50.0f);
		m_dir=vec_normalize(v);
		m_start_time=delta_time;
		m_time=0;
	}
	else
	{
		vector3 pos=m_space.location;
		pos=pos+m_dir*MOVE_SPEED*t;
		move(pos);
		m_time+=delta_time;
	}
}

const vector4& TestActor::get_screen_position()
{
	return m_screen_pos;
}

TestActor::TestActor(const nstring& name_str):nactor(name_str)
{
	m_time=0.0f;	m_start_time=0.0f;
}

TestActor::~TestActor()
{

}

void TestActor::move( const vector3& pos, const vector3& rot, const vector3& scale )
{
	nactor::move(pos,rot,scale);
	m_pos=pos;
	m_pos.y+=CHARA_HEIGHT;
}

void TestActor::move( const vector3& pos )
{
	nactor::move(pos);
	m_pos=pos;
	m_pos.y+=CHARA_HEIGHT;
}