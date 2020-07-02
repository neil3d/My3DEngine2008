#ifndef _NEXUS_ENGINE_NAV_MAP_EDITOR_H_
#define _NEXUS_ENGINE_NAV_MAP_EDITOR_H_
#include "neditor_engine.h"
#include "../navigation_map/nav_map_actor.h"
#include "mouse_helper.h"
#include "nterrain_editor.h"

namespace nexus
{
	/**
	*	地形编辑器
	*	@remarks 提供编辑功能接口；提供方便编辑器调用的包装接口
	*/
	class nEDAPI nav_map_editor
	{
	public:
		nav_map_editor(void);
		~nav_map_editor(void);

		// 开始导航图编辑器模式
		void begin_navigate_map_editor(nlevel::ptr level);
		// 结束导航图编辑器模式
		void end_navigate_map_editor();

		// 显示或者隐藏导航图
		void set_nav_map_visible(bool value);
		bool get_nav_map_visible() const;

		// 重新生成导航图
		// max_height: 指定最大能达到的高度
		// step_height: 指定步阶
		// use_line_check: true使用射线检测确定导航图格子高度，false计算地形和地物的高度
		bool rebuild_navigation_map(float max_height, float step_height, bool use_line_check);

		// 设置导航图的大小和缩放
		void move_navigation_map(const vector3& location, const vector3& scale);
		// 重新调整导航图的大小，并且保持高度不变
		void resize_navigation_map(size_t width, size_t height);
		// 重新设置导航图的大小，并且将高度重置为0
		void reset_navigation_map(size_t width, size_t height);

		// 绑定nav_map_actor是否有效
		bool empty() const;

		void close()
		{
			m_nav_map_actor.reset();
		}

		// 导入高度图，会导致现有数据的丢失
		void import_heightmap(const nstring& img_file_name);

		void set_brush(const nterrain_brush& brush)		{	m_brush = brush; }
		void draw_widgets(nrender_primitive_draw_interface* PDI);

		void on_mouse_left_down(const npoint& pt);
		void on_mouse_left_up();
		void on_mouse_right_down();
		void on_mouse_right_up();
		void on_mouse_wheel(int delta);
		void on_mouse_move(const npoint& pt, bool ctrl_down, const ncamera* cam);

		// 设置裁剪距离
		float get_cull_dist() const;
		void set_cull_dist(float value);

		// 设置导航图顶点渲染颜色
		const color4f& get_height_nav_color() const;
		void set_height_nav_color(const color4f& value);

		const color4f& get_walkable_nav_color() const;
		void set_walkable_nav_color(const color4f& value);

		bool get_walkable_map_visibale() const;
		void set_walkable_map_visibale(bool value);
	protected:
		void draw_brush_cursor(nrender_primitive_draw_interface* PDI);
		void draw_terrain_circle(int centre_x, int centre_z, int radius, nrender_primitive_draw_interface* PDI);
		void draw_terrain_quad(int left,int right,int top,int bottom, nrender_primitive_draw_interface* PDI);

		void brush_paint(bool neg);

		void rebuild_nav_map_by_actor( navigation_map::ptr nav_map, nlevel* level );

		// 重新计算走行走区域
		// max_height: 指定最大能达到的高度
		// step_height: 指定步阶
		void rebuild_walkable_map( float &max_height, float &step_height, navigation_map::ptr nav_map );

	protected:
		nterrain_brush					m_brush;

		boost::weak_ptr<nav_map_actor>	m_nav_map_actor;
		boost::weak_ptr<navigation_map>	m_nav_map;

		//-- mouse input
		mouse_drag	m_left_drag;
		mouse_drag	m_right_drag;

		npoint	m_cursor_pos;
	};


} // end of namespace nexus

#endif // end of _NEXUS_ENGINE_NAV_MAP_EDITOR_H_