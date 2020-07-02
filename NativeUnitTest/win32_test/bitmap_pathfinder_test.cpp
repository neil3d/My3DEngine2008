#include "StdAfx.h"
#include "bitmap_pathfinder_test.h"

nDEFINE_CLASS(bitmap_pathfinder_test, nrender_test)

bitmap_pathfinder_test::bitmap_pathfinder_test(void)
{
}

bitmap_pathfinder_test::~bitmap_pathfinder_test(void)
{
}

void bitmap_pathfinder_test::open( perspective_camera_controller* cam )
{
	nrender_test::open(cam);

	vector3 dir(50, 50,50);
	add_direct_light(vec_normalize(dir));

	m_nav_map.reset(new navigation_map());
	m_nav_map->serialize(nengine::instance()->get_file_sys(),_T("demo_data"), _T("/level/navigation_map_desc.xml"), EFileRead);

	nASSERT(m_nav_map);
	int w = m_nav_map->get_raw_width();
	int h = m_nav_map->get_raw_height();
	nASSERT(w>0 || h>0);

	m_graph = new ngraph_navigation_map(m_nav_map);

	m_start = npoint(rand()%w, rand()%h);
	m_end = npoint(rand()%w, rand()%h);
	clock_t clock_start = clock();
	m_graph->find_pather(m_start, m_end, 60.0f, m_ret_paths);
	clock_t clock_end = clock();

	m_take_times = float(clock_end - clock_start);
}

void bitmap_pathfinder_test::close()
{
	nrender_test::close();

	if(m_graph)
	{
		delete m_graph; m_graph = NULL;
	}
	m_nav_map.reset();
}

void bitmap_pathfinder_test::debug_canvas( nui_canvas* canvas )
{
	nrender_test::debug_canvas(canvas);
}

void bitmap_pathfinder_test::debug_draw( nrender_primitive_draw_interface* PDI )
{
	nrender_test::debug_draw(PDI);

	if(!m_nav_map)
		return;

	int w = m_nav_map->get_raw_width();
	int h = m_nav_map->get_raw_height();

	if( w<=0 || h<=0)
		return;

	// 显示debug信息
	std::wostringstream ss;
	ss << _T("start point: X=") << m_start.x << _T(", Y=") << m_start.y
		<< _T("  end point: X=") << m_end.x << _T(", Y=") << m_end.y
		<< _T("  paths size=") << m_ret_paths.size()
		<< _T("  take times=") << m_take_times << _T("ms");
	PDI->draw_debug_string(2, 24, ss.str(), color4ub(5,255,255,255));

	const vector3 nav_map_pos = m_nav_map->get_location();
	const vector3 tile_scale = m_nav_map->get_scale() * 0.5f;
	const color4f workable_color = color4f(1.0f,0.45f, 0.49f,1.0f); 
	const color4f tile_color = color4f(0.25f,0.65f, 0.49f,1.0f); 
	const color4f path_color = color4f(1.0f,0.f, 0.f,1.0f); 
	std::vector<fpoint> points;

	for (int i=0; i<w; ++i)
	{
		for (int j=0; j<w; ++j)
		{
			if(!m_nav_map->get_tile_walkable(i,j))
			{
				// 绘制起点和终点
				PDI->draw_wire_star(vector3(nav_map_pos.x + i * tile_scale.x, nav_map_pos.y, nav_map_pos.z + j * tile_scale.z),
					8, workable_color);
			}
		}
	}

	// 绘制格子
	for (int j=0; j<h; ++j)
	{
		PDI->draw_line(vector3(nav_map_pos.x, nav_map_pos.y, nav_map_pos.z + j * tile_scale.z), 
			vector3(nav_map_pos.x + w * tile_scale.x, nav_map_pos.y, nav_map_pos.z + j * tile_scale.z), 
			tile_color);
	}

	for (int i=0; i<w; ++i)
	{
		PDI->draw_line(vector3(nav_map_pos.x + i * tile_scale.x, nav_map_pos.y, nav_map_pos.z),
			vector3(nav_map_pos.x + i * tile_scale.x, nav_map_pos.y, nav_map_pos.z + h * tile_scale.z), tile_color);
	}

	// 绘制起点和终点
	PDI->draw_wire_star(vector3(nav_map_pos.x + m_start.x * tile_scale.x, nav_map_pos.y, nav_map_pos.z + m_start.y * tile_scale.z),
		 16, color4f(0.0f,1.0f,0.f,1.f));
	PDI->draw_wire_star(vector3(nav_map_pos.x + m_end.x * tile_scale.x, nav_map_pos.y, nav_map_pos.z + m_end.y * tile_scale.z),
		 16, color4f(1.0f,0.0f,0.f,1.f));

	// 绘制路径
	for( int index = 0; index<(int)m_ret_paths.size(); ++index)
	{
		const npoint& path = m_ret_paths[index];

		PDI->draw_wire_star(vector3(nav_map_pos.x + path.x * tile_scale.x, nav_map_pos.y, nav_map_pos.z + path.y * tile_scale.z),
			16, path_color);
	} 
}