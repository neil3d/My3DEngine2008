#include "StdAfx.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"
#include "nrenderer_d3d9.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"dxguid.lib")
#ifdef _DEBUG
#	pragma comment(lib,"d3dx9d.lib")
#else
#	pragma comment(lib,"d3dx9.lib")
#endif

namespace nexus
{
	//-- 借助nexus core的反射机制,此dll作为plugin,无需产生任何dll export,
	//	但是为了方便其他vc project依赖于此project,所以产生一个无用的输出,以产生lib文件
	 __declspec(dllexport) int NEXUS_PLUGIN_TAG;

	 //--
	d3d_device_manager::d3d_device_manager(void)
	{
		m_device_handler_id = 0;
	}

	d3d_device_manager::~d3d_device_manager(void)
	{
	}

	void d3d_device_manager::create_device(const render_config& cfg)
	{
		m_cfg = cfg;

		HWND hRenderWnd = (HWND)cfg.window_handle;
		size_t width = cfg.width;
		size_t height = cfg.height;
		size_t color_bits = cfg.color_bits;
		bool bWindowed = cfg.bWindowed;

		//-- create d3d9 object
		d3d_ptr<IDirect3D9> d3d9_obj = Direct3DCreate9(D3D_SDK_VERSION);
		if( !d3d9_obj )
			THROW_D3D_EXCEPTION(_T("Direct3DCreate9() Failed."));

		//-- create d3d9 device
		UINT		adapter;
		D3DDEVTYPE	device_type;
		
		bool found = find_acceptable_adapter(d3d9_obj, adapter, device_type);

		if( !found )
			THROW_D3D_EXCEPTION(_T("D3D device create failed: can't find acceptable adapter."));

		
		DWORD		behavior_flags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
		
		D3DPRESENT_PARAMETERS present_param;
		memset(&present_param, 0, sizeof(D3DPRESENT_PARAMETERS));
		present_param.BackBufferWidth	= (UINT)width;
		present_param.BackBufferHeight	= (UINT)height;
		switch(color_bits)
		{
		case 16:
			present_param.BackBufferFormat = D3DFMT_R5G6B5;
			break;
		case 32:
			present_param.BackBufferFormat = D3DFMT_X8R8G8B8;
			break;
		default:
			THROW_D3D_EXCEPTION(_T("create device failed: color bits unsupport."));
			break;
		}
		present_param.BackBufferCount	= 1;
		present_param.MultiSampleType	= D3DMULTISAMPLE_NONE;
		present_param.MultiSampleQuality= 0;
		present_param.SwapEffect		= D3DSWAPEFFECT_COPY;//为了使用present的rect参数，必须使用copy
		present_param.hDeviceWindow		= hRenderWnd;
		present_param.Windowed			= bWindowed;
		present_param.EnableAutoDepthStencil = true;
		present_param.AutoDepthStencilFormat = D3DFMT_D24X8;
		present_param.Flags	= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		present_param.FullScreen_RefreshRateInHz = 0;
#if 0
		present_param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
#else
		present_param.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif 

		IDirect3DDevice9* d3d9_device = NULL;
		HRESULT hr = d3d9_obj->CreateDevice(adapter, device_type, hRenderWnd, behavior_flags,
			&present_param, &d3d9_device);
		if(FAILED(hr))
			THROW_D3D_HRESULT(hr, _T("CreateDevice Failed"));

		//--
		m_d3d9 = d3d9_obj;
		m_device.reset(d3d9_device);
		m_d3d_present_param = present_param;

		// 创建UI sprite 
		ID3DXSprite* new_sprite = NULL;
		hr = D3DXCreateSprite(d3d9_device,&new_sprite);
		if( FAILED(hr) )
		{
			THROW_D3D_HRESULT(hr, _T("create font sprite faild."));
		}
		m_ui_sprite.reset( new_sprite );
		new_sprite=0;
	}

	void d3d_device_manager::destroy_device()
	{
		m_ui_sprite.reset();
		int device_ref = m_device.reset();
		int d3d_ref = m_d3d9.reset();

		if(device_ref > 0)
		{
			nLog_Warning(_T("device still referenced when destroy_device() called\r\n"));
		}

		if(d3d_ref > 0)
		{
			nLog_Warning(_T("d3d object still referenced when destroy_device() called\r\n"));
		}
	}

	bool d3d_device_manager::find_acceptable_adapter(d3d_ptr<IDirect3D9> d3d9_obj, UINT& out_adapter, D3DDEVTYPE& out_type)
	{
		out_adapter = D3DADAPTER_DEFAULT;
		out_type = D3DDEVTYPE_HAL;
#if 1	// nvidia PerfHud
		for(UINT i=0; i<d3d9_obj->GetAdapterCount(); i++)
		{
			D3DADAPTER_IDENTIFIER9 adapter_id;
			
			HRESULT hr = d3d9_obj->GetAdapterIdentifier(i, 0, &adapter_id);

			if( SUCCEEDED(hr) )
			{
				if(strstr(adapter_id.Description, "PerfHUD") != 0)
				{
					out_adapter = i;
					out_type = D3DDEVTYPE_REF;
					return true;
				}
			}
		}
#else
#endif
		// todo : check device caps and formart

		return true;
	}

	HRESULT d3d_device_manager::set_view_render_target(UINT i, IDirect3DSurface9* prt)
	{
		//D3D9 Doc : Setting a new render target will cause the viewport to be set to the full size of the new render target.
		HRESULT hr;

		const D3DVIEWPORT9& vp =*(D3DVIEWPORT9*)&(nrenderer_d3d9::instance()->get_view_info()->m_view_port);
		hr = m_device->SetRenderTarget(i, prt);

		m_device->SetViewport(&vp);

		return hr;
	}

	int d3d_device_manager::register_device_handler(handler_device_lost hlost, handler_device_reset hreset)
	{
		int id = m_device_handler_id;
		m_device_handler_id++;
		m_on_device_lost.insert( make_pair(id, hlost) );
		m_on_device_reset.insert( make_pair(id, hreset) );

		return id;
	}

	void d3d_device_manager::unregister_device_handler(int id)
	{
		m_on_device_lost.erase(id);
		m_on_device_reset.erase(id);
	}

	void d3d_device_manager::on_device_lost(int param)
	{
		HRESULT hr;
		//-- 处理自己管理的资源
		if( m_ui_sprite )
			hr = m_ui_sprite->OnLostDevice();

		//-- 调用所有handler
		for (std::map<int, handler_device_lost>::iterator iter = m_on_device_lost.begin();
			iter != m_on_device_lost.end();
			++iter)
		{
			handler_device_lost handler = iter->second;
			handler(param);
		}			
	}

	bool d3d_device_manager::on_device_reset(int param)
	{
		//-- 处理自己管理的资源
		if( m_ui_sprite )
			m_ui_sprite->OnResetDevice();

		//-- 调用所有handler
		for (std::map<int, handler_device_reset>::iterator iter = m_on_device_reset.begin();
			iter != m_on_device_reset.end();
			++iter)
		{
			handler_device_reset handler = iter->second;
			if( !handler(param) )
				return false;
		}// end of for

		return true;
	}
}//namespace nexus