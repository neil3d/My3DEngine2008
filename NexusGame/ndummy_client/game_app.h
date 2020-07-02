#pragma once
#include "game_canvas.h"

/**
 *	游戏应用类--管理update、绘图等
*/
class game_app
{
public:
	game_app(void);
	~game_app(void);

	static game_app* instance();

	//!	初始化自己及其他游戏组件
	void init();

	//!	指定绘图窗口
	void bind_render_wnd(CWnd* wnd);

	//!	更新所有游戏逻辑
	void update(float delta_time);

	//!	使用游戏逻辑的当前状态进行渲染
	void render();

	//!	关闭，清除资源
	void close();

	//!	缩放地图视口的显示比例
	void scale_view(int delta);

	const game_canvas& get_game_canvas() const { return m_canvas; }
 
	float get_game_time() const { return m_game_time; }
	void set_game_time(float game_time) { m_game_time = game_time; }
 
private:
	bool _load_att_data();

private:
	CWnd*		m_render_wnd;	// 绑定的渲染窗口
	game_canvas	m_canvas;		// 渲染用的画布对象
	float		m_game_time;
};
