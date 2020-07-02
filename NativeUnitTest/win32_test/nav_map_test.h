#pragma once
#include "ntest_case.h"
#include "../ncore/ai/ngraph_navigation_map.h"

#include "ngameframework.h"

using namespace gameframework;

class debug_draw_imp : public ndebug_draw_interface
{
public:
	void pre_draw(nrender_primitive_draw_interface* PDI)
	{
		m_PDI = PDI;
	}

	virtual void draw_line(const vector3& start, const vector3& end, const color4f& color)
	{
		m_PDI->draw_line(start, end, color);
	}

	virtual void draw_line_strip(const std::vector<vector3>& pos_array, const color4f& color)
	{
		m_PDI->draw_line_strip(pos_array, color);
	}

	virtual void draw_wired_box(const AABBox& box, const color4f& color) 
	{
		m_PDI->draw_wired_box(box, color);
	}

	virtual void draw_debug_string(size_t x, size_t y, const nstring& text, const color4ub& color)
	{
		m_PDI->draw_debug_string(x, y, text, color);
	}

private:
	nrender_primitive_draw_interface* m_PDI;
};

class nav_map_test : public nrender_test
{
public:
	nav_map_test(void);
	virtual ~nav_map_test(void);

	virtual void open(perspective_camera_controller* cam);	

	void re_find_pather();
	virtual void close();

	virtual void tick(float run_time,float delta_time);

	virtual void debug_draw(nrender_primitive_draw_interface* PDI);

	virtual void on_key_down(unsigned int key);

private:
	perspective_camera_controller* m_camera_ctrl;

	navigation_map::ptr			m_nav_map;
	ngraph_navigation_map*		m_graph;

	npoint m_start;
	npoint m_end;
	ngraph_navigation_map::path_result_type m_ret_paths;
	float m_take_times;

	// ²âÊÔ½ÇÉ«¿ØÖÆ
	debug_draw_imp*				m_debug_draw;
	ngame_level::ptr			m_game_level;
	ncharacter_controller::ptr	m_character;

	nDECLARE_CLASS(nav_map_test)
};
