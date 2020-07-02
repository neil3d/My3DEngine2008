#pragma once

#include "ngameframework.h"
#include "ncommon.h"
#include "nobject_fields.h"

class game_canvas;

using namespace gameframework;

/**
 *	游戏地图中可交互对象的基类
*/
class game_unit
{
public:
	typedef boost::shared_ptr<game_unit> ptr;

	game_unit(void);
	virtual ~game_unit(void);

	//!	取得世界坐标系坐标
	const vector3& get_pos() const { return m_controller_ptr->get_position(); }
	void set_pos(const vector3& pos) { m_controller_ptr->set_position(pos); }

	const vector3& get_rotation() const { return m_controller_ptr->get_rotation(); }
	void set_rotation(const vector3& rot) { m_controller_ptr->set_rotation(rot); }

	//!	取得格子坐标系坐标
	npoint get_tile_pos() const;

	uint16 move(const vector3& pos, const vector3& rotation, const vector3& acceleration, float time_stamp, uint8 flag);

	//!	更新自身状态
	virtual void update(float delta_time);

	//!	在画布上画出自己
	virtual void draw(game_canvas* cvs);

	uint64 get_id() const { return m_unit_id; }
	void set_id(uint64 id) { m_unit_id = id; }

	nobj_data& get_data() { return m_data; }

	bool has_flag(uint32 flag) const	{ return m_data.has_flag(EUF_UnitFlags, flag); }

protected:
	COLORREF	m_draw_color;	// 根据类型不同，可以设置不同的显示颜色值

	uint64						m_unit_id;
	ncharacter_controller::ptr	m_controller_ptr;

	nobj_data					m_data;
};
