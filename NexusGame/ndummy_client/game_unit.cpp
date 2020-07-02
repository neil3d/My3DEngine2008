#include "StdAfx.h"
#include "game_unit.h"
#include "game_map.h"
#include "game_canvas.h"

game_unit::game_unit(void)
{
	m_draw_color = RGB(98,98,98);
}

game_unit::~game_unit(void)
{
}

uint16 game_unit::move(const vector3& pos, const vector3& rotation, const vector3& acceleration, float time_stamp, uint8 flag)
{
	m_controller_ptr->set_position(pos);
		m_controller_ptr->set_rotation(rotation);
	m_controller_ptr->set_acceleration(acceleration);
	gameframework::nsavedmove::flags_to_controller(flag, m_controller_ptr.get());
	return 0;
}

void game_unit::update(float delta_time)
{
	gameframework::ngame_level* level_ptr = game_map::instance()->get_nav_level();

	if (level_ptr)
	{
		m_controller_ptr->move(game_map::instance()->get_nav_level(), delta_time);
	}
}

void game_unit::draw(game_canvas* cvs)
{
	const map_view& view = cvs->get_view();
	int unit_size = 8;

	const vector3& rotation = get_rotation();
	const vector3& pos = get_pos();

	float dis = 350.0f;


	vector3 pos2(pos.x - dis * sin(rotation.y), pos.y, pos.z - dis * cos(rotation.y)); 

	npoint pt = view.world2pixel(pos);
	npoint pt2 = view.world2pixel(pos2);

	// todo: 是否画一个锐角三角形，用来表示朝向？
	cvs->draw_rect(nrect(pt.x - unit_size / 2, pt.y - unit_size / 2, pt.x + unit_size / 2, pt.y + unit_size / 2), m_draw_color);
	cvs->draw_line(pt.x, pt.y, pt2.x, pt2.y, m_draw_color);
}

npoint game_unit::get_tile_pos() const
{
	navigation_map::ptr nav_map = game_map::instance()->get_nav_map();
	return nav_map->world2tile(get_pos().x, get_pos().z);
}