#include "StdAfx.h"
#include "dynamic_vb_pool.h"
#include "util.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"

namespace nexus
{
	dynamic_vb_pool::dynamic_vb_pool(void)
	{
		m_max_item_size = 256*1024;
		m_pool_size = 0;
	}

	dynamic_vb_pool::~dynamic_vb_pool(void)
	{
	}

	dynamic_vb_pool* dynamic_vb_pool::instance()
	{
		static dynamic_vb_pool g_dvb_pool;
		return &g_dvb_pool;
	}

	d3d_vb_ptr dynamic_vb_pool::alloc_for_vertex_stream(const vertex_stream* stream, bool init_copy)
	{
		d3d_vb_ptr d3d_vb = pool_alloc(stream->get_data_size());

		if( init_copy )
			d3d_copy_vertex_buffer(d3d_vb, stream);

		return d3d_vb;
	}

	d3d_vb_ptr dynamic_vb_pool::pool_alloc(size_t s)
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		if(s > m_max_item_size)
		{
			nLog_Warning(_T("dynamic vb alloc beyond max item size"));
			
			IDirect3DVertexBuffer9* vb_ptr = NULL;

			hr = d3d_device->CreateVertexBuffer((UINT)s,
				D3DUSAGE_WRITEONLY,	// Usage
				0,	// FVF
				D3DPOOL_MANAGED,
				&vb_ptr, NULL);

			if( FAILED(hr) )
				THROW_D3D_HRESULT(hr, _T("CreateVertexBuffer Failed."));
			
			return d3d_vb_ptr(vb_ptr);
		}
		
		//-- find availabe item in pool
		// thanks the binary tree in std::map -- 因为我们使用item size作为key，所以我们会找到可用的最小的一个item
		for(item_map::iterator iter = m_item_map.begin();
			iter != m_item_map.end();
			++iter)			
		{
			struct pool_item& it = iter->second;
			if( !it.in_use
				&& it.size >= s)
			{
				it.in_use = true;
				it.use_count++;
				return it.vb;
			}
		}

		//-- add new item to pool
		size_t align = 32;
		size_t item_size = (s/align+1)*align;

		IDirect3DVertexBuffer9* vb_ptr = NULL;
		hr = d3d_device->CreateVertexBuffer((UINT)item_size,
			D3DUSAGE_WRITEONLY,	// Usage
			0,	// FVF
			D3DPOOL_MANAGED,
			&vb_ptr, NULL);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("CreateVertexBuffer Failed."));

		struct pool_item it;
		it.in_use = true;
		it.size = item_size;
		it.use_count = 1;
		it.vb.reset(vb_ptr);

		m_pool_size += item_size;
		m_item_map.insert(make_pair(item_size, it));
		
		return it.vb;
	}

	void dynamic_vb_pool::free(d3d_vb_ptr vb_ptr)
	{
		for(item_map::iterator iter = m_item_map.begin();
			iter != m_item_map.end();
			++iter)			
		{
			struct pool_item& it = iter->second;
			if( it.vb == vb_ptr)
			{
				it.in_use = false;
				return;				
			}
		}// end of for

		//-- must be alloc from device directly
	}
}//namespace nexus