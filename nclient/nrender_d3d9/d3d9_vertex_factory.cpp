#include "StdAfx.h"
#include "d3d9_vertex_factory.h"
#include "d3d_exception.h"
#include "d3d_device_manager.h"
#include "util.h"


namespace nexus
{
	d3d9_vertex_factory::d3d9_vertex_factory(void)
	{
		m_type = NULL;
		m_num_vert = 0;
	}

	d3d9_vertex_factory::~d3d9_vertex_factory(void)
	{
	}

	void d3d9_vertex_factory::destroy()
	{
		m_vert_streams.clear();
		m_stream_stride.clear();
	}

	void d3d9_vertex_factory::draw_set_full()
	{
		if(m_vert_streams.size() <= 0
			|| m_type==NULL)
			return;

		m_type->draw_set_vert_decl();		

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		for(UINT i=0; i<m_vert_streams.size(); i++)
		{
			d3d_device->SetStreamSource(i, m_vert_streams[i].get(), 0, m_stream_stride[i]);
		}
	}

	d3d_vb_ptr d3d9_vertex_factory::create_d3d_vb_from_stream(const vertex_stream* stream, D3DPOOL pool, bool init_copy)
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		IDirect3DVertexBuffer9* vb_ptr = NULL;
		UINT vb_size = (UINT)stream->get_data_size();

		DWORD usage = 0;
		switch( pool )
		{
		case D3DPOOL_DEFAULT:
			usage |= D3DUSAGE_WRITEONLY;	// for static vb
			break;
		case D3DPOOL_MANAGED:
			break;
		default:
			nASSERT(0 && _T("unknown D3DPool"));
		}
			
		HRESULT hr = d3d_device->CreateVertexBuffer(vb_size,
			usage,	// Usage
			0,	// FVF
			pool,
			&vb_ptr, NULL);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("CreateVertexBuffer Failed."));

		// copy data
		d3d_vb_ptr smart_vb(vb_ptr);

		if( init_copy )
			d3d_copy_vertex_buffer(smart_vb, stream);		

		return smart_vb;
	}

	void d3d9_vertex_factory::draw_set_pos_only_static()
	{
		if(m_vert_streams.size() <= 0
			|| m_type==NULL)
			return;

		vertex_factory_type_lib::instance()->draw_set_vert_decl_static_pos_only();

		//-- 规定: postion stream必须为第0个stream
		d3d_vb_ptr pos_vb = m_vert_streams[0];

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		HRESULT hr = d3d_device->SetStreamSource(0, pos_vb.get(), 0, m_stream_stride[0]);
	}
}//namespace nexus