#pragma once
using namespace nexus;

/*
	镜头控制
	.镜头一直锁定主角;
	.鼠标右键拖动, 旋转镜头, 并使得主角朝向于镜头朝向一致;
	.鼠标左键拖动, 旋转镜头, 不影响角色;
*/
class player_camera_ctrl : public camera_controller
{
public:
	player_camera_ctrl(void);
	~player_camera_ctrl(void);

	virtual void update_camera(ncamera* cam)
	{
		lock_player_pos();
		m_compute.update_camera_view(cam);
	}

	void process_input_message(const MSG& msg);

	void bind_player(nactor::ptr player);
	void close()
	{
		m_player.reset();
	}

protected:
	virtual void on_mouse_left_down(const npoint& pt);
	virtual void on_mouse_left_up();
	virtual void on_mouse_right_down(const npoint& pt);
	virtual void on_mouse_right_up();
	virtual void on_mouse_wheel(int delta);
	virtual void on_mouse_move(const npoint& pt, bool ctrl_down);

	void mouse_left_drag(const npoint& pt, bool ctrl_down);
	void mouse_right_drag(const npoint& pt, bool ctrl_down);

	void lock_player_pos();

private:
	camera_YPR		m_compute;
	nactor::ptr		m_player;
};
