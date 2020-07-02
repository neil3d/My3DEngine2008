#include "StdAfx.h"
#include "game_map.h"
#include "game_canvas.h"
#include "ConsoleDlg.h"
#include "config.h"
#include "local_player.h"
#include "client_session.h"
#include "file/nstd_file_system.h"
#include "object/narchive_xml.h"
#include "object/serialize.h"
#include "game_map/navigation_map.h"

const float AOI_SIZE = 3500.0f;

game_map::game_map(void)
{
}

game_map::~game_map(void)
{
}

void game_map::draw(game_canvas* cvs)
{
	////-- 画出导航图
	int map_w = int(m_nav_map_ptr->get_raw_width() * m_nav_map_ptr->get_scale().x);
	int map_h = int(m_nav_map_ptr->get_raw_height() * m_nav_map_ptr->get_scale().z);

	vector3 pos = local_player::instance()->get_pos();
	const map_view& view = cvs->get_view();

	//int ts = view.tile_size;
	//int view_x = view.left;
	//int view_y = view.top;
	//int view_w = view.wnd_w/view.tile_size+1;
	//int view_h = view.wnd_h/view.tile_size+1;

	//for (int y=view_y; y<view_y+view_h && y<map_h; y++)
	//{
	//	int draw_y = y*ts;
	//	for (int x=view_x; x<view_x+view_w && x<map_w; x++)
	//	{
	//		bool walk_able = m_nav_map_ptr->get_tile_walkable(x, y);			
	//		int draw_x = x*ts;
	//		if(walk_able)
	//		{
	//			// 可行走的格子根据高度计算颜色
	//			int tile_h = m_nav_map_ptr->get_raw_height(x, y);
	//			cvs->draw_rect( nrect(draw_x, draw_y, draw_x+ts, draw_y+ts), RGB(200,200,200) );
	//		}
	//		else
	//		{
	//			// 不可行走的格子
	//			cvs->draw_rect( nrect(draw_x, draw_y, draw_x+ts, draw_y+ts), RGB(0,0,0) );
	//		}
	//	}
	//}

	////-- 画出地图格子线
	//if (view.show_grid)
	//{
	//	//-- 画出横线
	//	int num_line = view.wnd_h/ts+1;
	//	for(int i=0; i<num_line; i++)
	//	{
	//		cvs->draw_line(0, i*ts, view.wnd_w, i*ts, view.grid_color);
	//	}

	//	//-- 画出竖线
	//	num_line = view.wnd_w/ts+1;
	//	for(int i=0; i<num_line; i++)
	//	{
	//		cvs->draw_line(i*ts, 0, i*ts, view.wnd_h, view.grid_color);
	//	}
	//}

	if (ECSS_Gaming != client_session::instance()->get_status())
		{
		return;
		}

	const vector3& location = local_player::instance()->get_pos();
	// 画出aoi
	// 1--2
	// 4--3
	npoint pt1, pt3;
	vector3 pos1, pos3;
	pos1.x = location.x - AOI_SIZE;
	pos1.z = location.z - AOI_SIZE;

	pos3.x = location.x + AOI_SIZE;
	pos3.z = location.z + AOI_SIZE;

	pt1 = view.world2pixel(pos1);
	pt3 = view.world2pixel(pos3);

	cvs->draw_line(pt1.x, pt1.y, pt3.x, pt1.y, view.grid_color);
	cvs->draw_line(pt3.x, pt1.y, pt3.x, pt3.y, view.grid_color);
	cvs->draw_line(pt1.x, pt3.y, pt3.x, pt3.y, view.grid_color);
	cvs->draw_line(pt1.x, pt1.y, pt1.x, pt3.y, view.grid_color);

	local_player::instance()->draw( cvs );
	//-- 画出游戏对象
	for(UNIT_MAP::const_iterator iter = m_aoi_units.begin(); iter != m_aoi_units.end(); ++iter)
	{
		iter->second->draw( cvs );
	}
}

game_map* game_map::instance()
{
	static game_map s_inst;
	return &s_inst;
}

void game_map::init()
{
	load_map(1);
	//-- 注册控制台命令
	CConsoleDlg::instance()->register_cmd( _T("mapinfo"), _T("显示地图基本信息"), 
		boost::bind(&game_map::cmd_map_info, this, _1) );
}

void game_map::load_map(uint32 map_id)
{
	using namespace gameframework;

	navigation_map::ptr nav_ptr(new navigation_map);

	if (!nav_ptr)
	{
		nLog_Info(_T("alloc navigation_map error!!!"));
		return;
}


	m_nav_map_ptr.swap(nav_ptr);

	if (m_game_level_ptr)
	{
		delete m_game_level_ptr;
		m_game_level_ptr = NULL;
	}

	m_game_level_ptr = new ngame_level;

	if (!m_game_level_ptr)
	{
		nLog_Info(_T("alloc game level error!!!"));
		return;
	}

	nlevel_info_resource::ptr level_res_ptr(new nlevel_info_resource);

	if (!level_res_ptr)
	{
		nLog_Info(_T("alloc nlevel_info_resource error!!!"));
		return;
	}

	m_level_res_ptr.swap(level_res_ptr);

	tstring path = config::instance()->get_tstring(ECI_ResPath);

	nstd_file_system fs;
	fs.init(path);

	nchar name[MAX_PATH];
	_sntprintf_s(name, MAX_PATH, MAX_PATH, _T("%d\\navigation_map_desc.xml"), map_id);

	const nstring file_name(name); 
	m_nav_map_ptr->serialize(&fs, _T("map"), file_name, EFileRead);

	m_game_level_ptr->attach_nav_map(m_nav_map_ptr);

	//
	//_sntprintf_s(name, MAX_PATH, MAX_PATH, _T("%d\\navigation_map_desc.xml"), 2);

	//const nstring file_name2(name); 
	//m_nav_map_ptr->create(1024, 1024, vector3(-65536.0f, 0.0f, -65536.0f), vector3(128.0f, 2.0f, 128.0f));
	//m_nav_map_ptr->serialize(&fs, _T("map"), file_name, EFileWrite);

	//_sntprintf_s(name, MAX_PATH, MAX_PATH, _T("%d\\level_res.xml"), map_id);

	//const nstring file_name2(name); 
	//level_res_ptr->serialize(&fs, _T("map"), file_name2, EFileRead, ELIRT_ALL);
}

void game_map::update(float delta_time)
{
	for(UNIT_MAP::const_iterator iter = m_aoi_units.begin(); iter != m_aoi_units.end(); ++iter)
	{
		iter->second->update(delta_time);
	}
}

void game_map::cmd_map_info(const console_cmd& cmd)
{
	std::wostringstream ss;
	ss << _T("Map Width = ") << m_nav_map_ptr->get_raw_width()
		<< _T(", Map Height = ") << m_nav_map_ptr->get_raw_height()
		<< _T(", Num Unit = ") << m_aoi_units.size();
	CConsoleDlg::instance()->print_line(ss.str().c_str());
}

bool game_map::add_remote_unit(game_unit::ptr remote_unit_ptr)
{
	if (!m_aoi_units.insert(std::make_pair(remote_unit_ptr->get_id(), remote_unit_ptr)).second)
	{
		return false;
	}

	return true;
}

void game_map::remove_remote_unit(uint64 unit_id)
{
	m_aoi_units.erase(unit_id);
}

uint32 game_map::get_remote_unit_num()
{
	return (uint32)m_aoi_units.size();
}

game_unit::ptr game_map::get_game_unit(uint64 id)
{
	UNIT_MAP::const_iterator iter = m_aoi_units.find(id);

	if (iter == m_aoi_units.end())
	{
		game_unit::ptr null_ptr;
		return null_ptr;
	}

	return iter->second;
}
