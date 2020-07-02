#pragma once

namespace nexus
{
	/*
	 *	主角操作控制:
	 *	.按WSAD键前后左右移动
	*/
	class player_ctrl : public nactor_controller
	{
	public:
		player_ctrl(void);
		~player_ctrl(void);

		virtual void _init(nactor* actor);

		void debug_draw(nrender_primitive_draw_interface* PDI);
		void process_input_message(const MSG& msg);
		virtual void update_actor(float delta_time);

	private:
		void on_key_down(unsigned int vk);
		void on_key_up(unsigned int vk);
	private:		
		vector3						m_vec[4];		// 前后左右,四个速度
		nanim_mesh_component::ptr	m_anim_comp;

		nDECLARE_CLASS(player_ctrl)
	};
}//nexus