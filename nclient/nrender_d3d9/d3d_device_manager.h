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

		void create_device(HWND hRenderWnd, size_t width, size_t height, size_t color_bits, bool bWindowed);
		void destroy_device();

		IDirect3DDevice9* get_device()	{	return m_device.get(); }

	private:
		bool find_acceptable_adapter(d3d_ptr<IDirect3D9> d3d9_obj, UINT& out_adapter, D3DDEVTYPE& out_type);
	private:
		d3d_ptr<IDirect3D9>		m_d3d9;
		d3d_device_ptr			m_device;
	};
}//namespace nexus
#endif //_D3D_DEVICE_MANAGER_H_