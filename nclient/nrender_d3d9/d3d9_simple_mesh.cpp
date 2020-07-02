#include "StdAfx.h"
#include "d3d9_simple_mesh.h"
#include "d3d_device_manager.h"
#include "mesh_func.h"
#include "global_shader_lib.h"

namespace nexus
{
	d3d9_simple_mesh::d3d9_simple_mesh(void)
		: m_num_tri(0), m_num_vert(0)
	{
	}

	d3d9_simple_mesh::~d3d9_simple_mesh(void)
	{
	}

	void d3d9_simple_mesh::create(const nsimple_mesh* mesh)
	{
		nASSERT( mesh != NULL );

		UINT vb_size = mesh->vert_buffer.size()*sizeof(vector3);
		UINT ib_size = mesh->index_buffer.size()*sizeof(unsigned short);

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		//-- create vertex buffer
		IDirect3DVertexBuffer9* vb_ptr = NULL;
		HRESULT hr = d3d_device->CreateVertexBuffer(vb_size,
			D3DUSAGE_WRITEONLY,	// Usage
			0,	// FVF
			D3DPOOL_DEFAULT,
			&vb_ptr, NULL);

		void* vb_data = NULL;
		hr = vb_ptr->Lock(0, vb_size, &vb_data, 0 );		
		memcpy(vb_data, &(mesh->vert_buffer[0]), vb_size);
		hr = vb_ptr->Unlock();

		//-- create index buffer
		IDirect3DIndexBuffer9* ib_ptr = NULL;
		hr = d3d_device->CreateIndexBuffer(ib_size,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&ib_ptr,
			NULL);

		void* ib_data = NULL;
		hr = ib_ptr->Lock(0, 0, &ib_data, 0);
		memcpy(ib_data, &(mesh->index_buffer[0]), ib_size);
		hr = ib_ptr->Unlock();

		//--
		m_num_tri = get_primitive_count_by_vert(EDraw_TriangleList, mesh->index_buffer.size());
		m_num_vert = mesh->vert_buffer.size();
		m_vert_buffer.reset(vb_ptr);
		m_index_buffer.reset(ib_ptr);
	}

	void d3d9_simple_mesh::draw(const d3d_view_info* view, const color4f& color) const
	{
		d3d_effect_ptr effect = global_shader_lib::instance()->find_shader(
			_T("draw_simple_mesh.fx"));

		if( !effect )
			return;

		HRESULT hr;
		hr = effect->SetMatrix("g_matWorldViewPrj", (const D3DXMATRIX*)(&view->mat_view_project));

		UINT num_pass = 0;
		hr = effect->Begin(&num_pass, 0);

		if( FAILED(hr) )
			return;

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		hr = d3d_device->SetStreamSource(0, m_vert_buffer.get(), 0, sizeof(vector3));
		hr = d3d_device->SetIndices(m_index_buffer.get());		

		for(UINT i=0; i<num_pass; i++)
		{
			hr = effect->BeginPass(i);
			hr = d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_num_vert, 0, m_num_tri);
			hr = effect->EndPass();
		}

		hr = effect->End();
	}
}//namespace nexus