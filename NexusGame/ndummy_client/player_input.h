#pragma once

/**
 *	处理游戏中的键盘鼠标输入
*/
class player_input
{
public:
	player_input(void);
	~player_input(void);

	static player_input* instance();

	//!	处理鼠标左键按下事件
	void on_mouse_left_down(const npoint& pt);

	//!	处理鼠标左键抬起事件
	void on_mouse_left_up();

	//!	处理鼠标右键按下事件
	void on_mouse_right_down(const npoint& pt);

	//!	处理鼠标右键抬起事件
	void on_mouse_right_up();

	//!	处理鼠标滚轮操作
	void on_mouse_wheel(int delta);

	//!	处理鼠标移动操作
	void on_mouse_move(const npoint& pt, bool ctrl_down);

	//!	处理键按下操作
	void on_key_down(int key);
};
