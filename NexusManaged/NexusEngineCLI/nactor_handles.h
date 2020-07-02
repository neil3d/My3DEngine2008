#pragma once

namespace NexusEngine
{
	const int ACTOR_HANDLE_NUM = 1;

	class nactor_handles
	{
	public:
		nactor_handles(void);
		~nactor_handles(void);

		void init();
		void close()
		{
			m_handleMeshs.clear();
		}

		void draw_move(nrender_primitive_draw_interface* PDI, bool hit_proxy);		
		void draw_rotate(nrender_primitive_draw_interface* PDI, bool hit_proxy);		
		int find_handle(hit_id hid);
		void set_actor_space(const object_space& sp);
		void set_hot_handle(int i);

		//-- 自动计算坐标轴模型的缩放,使其在屏幕上画出的像素长度保持不变
		void fit_camera(const ncamera* cam);

	private:
		nstatic_mesh_component::ptr load_axis_mesh(const nstring& name, const vector4& color);
		nstatic_mesh_component::ptr load_rotate_mesh(const nstring& name, const vector4& color);

		void draw(size_t base_index, nrender_primitive_draw_interface* PDI, bool hit_proxy);		
	private:
		std::vector<nstatic_mesh_component::ptr>	m_handleMeshs;
	};
}//namespace NexusEngine