#pragma once
#include "ntest_case.h"
#include "../ncore/ncore.h"
#include "../ncore/ai/ngraph_navigation_map.h"
#include "../ncore/height_map/nbit_map.h"

class bitmap_pathfinder_test : public nrender_test
{
public:
	bitmap_pathfinder_test(void);
	virtual ~bitmap_pathfinder_test(void);

	virtual void open(perspective_camera_controller* cam);	
	virtual void close();

	virtual void debug_draw(nrender_primitive_draw_interface* PDI);
	virtual void debug_canvas(nui_canvas* canvas);

private:
	navigation_map::ptr				m_nav_map;
	ngraph_navigation_map*		m_graph;

	npoint m_start;
	npoint m_end;
	ngraph_navigation_map::path_result_type m_ret_paths;
	float m_take_times;

	nDECLARE_CLASS(bitmap_pathfinder_test)
};
