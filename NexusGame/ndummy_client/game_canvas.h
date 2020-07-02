#pragma once

#include "ncore.h"

/**
 *	地图视野--描述当前窗口显示地图的范围和一些参数
*/
struct map_view
{
	int		tile_size;	// 导航图一个格子的像素大小
	//int		left,		// 窗口左上角（0，0）坐标对应的地图格子坐标
	//		top;
	int		wnd_w,		// 窗口的大小，单位：像素
			wnd_h;

	vector3		loaction;

	COLORREF	clear_color;	// 填充色
	bool		show_grid;		// 是否显示地图格子线框
	COLORREF	grid_color;		// 格子线框的颜色
	
	map_view()
	{
		tile_size = 18;
		//left = top = 0;
		wnd_w = wnd_h = 100;

		loaction.x = 0;
		loaction.y = 0;
		loaction.z = 0;

		clear_color = RGB(230,230,230);
		show_grid = true;		
		grid_color = RGB(0, 28, 88);
	}


	npoint world2pixel(const vector3& pos) const
	{
		npoint pt;
		pt.x = int((pos.x - loaction.x) / tile_size + (wnd_w / 2));
		pt.y = int((loaction.z - pos.z) / tile_size + (wnd_h / 2));
		return pt;
	}

	vector3 pixel2world(npoint pt) const
	{
		vector3 pos;
		pos.x = (pt.x - (wnd_w / 2)) * tile_size + loaction.x;
		pos.z = ((wnd_h / 2) - pt.y) * tile_size + loaction.z;
		return pos;
	}
};

class game_unit;

/**
 *	游戏渲染用的画布
 *	@remakrs 封装面向应用层的GDI绘图接口；管理一个Double buffer
*/
class game_canvas
{
public:
	game_canvas(void);
	~game_canvas(void);

	//!	初始化，创建内部buffer对象
	void init(CWnd* wnd);

	//!	将内部buffer绘制到窗口中
	void flip(CWnd* wnd);

	//!	关闭，清楚内部对象
	void close();

	//!	根据本地玩家坐标更新视图坐标，使得摄像机始终锁定玩家
	void update_view(const vector3& pos);

	//!	擦除所有绘图
	void clear();

	//!	画线接口
	void draw_line(int start_x, int start_y, int end_x, int end_y, COLORREF clr);

	//!	绘制一个实心的矩形
	void draw_rect(const nrect& rc, COLORREF clr);

	const map_view& get_view() const	{	return m_view;}

	int get_scale() const { return m_view.tile_size; }
	void set_scale(int scale) { m_view.tile_size = scale; }

private:
	map_view	m_view;		//	视口描述对象

	CBitmap		m_mem_bmp;	//	双缓冲用的bitmap对象
	CBitmap*	m_old_bmp;	//	保留老的mem dc使用的bitmap对象，供恢复用
	CDC			m_mem_dc;	//	双缓冲使用的DC对象
};
