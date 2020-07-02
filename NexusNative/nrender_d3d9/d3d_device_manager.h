/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _D3D_DEVICE_MANAGER_H_
#define _D3D_DEVICE_MANAGER_H_
#include "ncore.h"
#include "d3d_ptr.h"

namespace nexus
{
	/**
	 *	管理device对象，以及创建过程
	*/
	class d3d_device_manager 
		: public singleton<d3d_device_manager>
	{
	public:
		d3d_device_manager(void);
		~d3d_device_manager(void);

		void create_device(const render_config& cfg);
		void destroy_device();

		IDirect3DDevice9* get_device()	{	return m_device.get(); }
		ID3DXSprite* get_ui_sprite() { return m_ui_sprite.get(); }

		HRESULT set_view_render_target(UINT i, IDirect3DSurface9* prt);
		const D3DPRESENT_PARAMETERS& get_present_param() const	{	return m_d3d_present_param;}

		int register_device_handler(handler_device_lost hlost, handler_device_reset hreset);
		void unregister_device_handler(int id);

		void on_device_lost(int param);
		bool on_device_reset(int param);

		const render_config& get_render_cfg() const	{	return m_cfg; }

	private:
		bool find_acceptable_adapter(d3d_ptr<IDirect3D9> d3d9_obj, UINT& out_adapter, D3DDEVTYPE& out_type);
	private:
		d3d_ptr<IDirect3D9>		m_d3d9;
		d3d_device_ptr			m_device;
		D3DPRESENT_PARAMETERS	m_d3d_present_param;

		// device lost 相关处理
		int		m_device_handler_id;
		std::map<int, handler_device_lost>	m_on_device_lost;
		std::map<int, handler_device_reset>	m_on_device_reset;

		render_config	m_cfg;

		// 用于绘制2D渲染的sprite
		d3d_sprite_ptr			m_ui_sprite;
	};
}//namespace nexus
#endif //_D3D_DEVICE_MANAGER_H_