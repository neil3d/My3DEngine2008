#include "StdAfx.h"
#include "d3d9_mesh_section.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"
#include "mesh_func.h"

namespace nexus
{
	d3d9_mesh_section::d3d9_mesh_section(void)
		: m_valid_index_count(0)
	{
		m_base_vertex_index = 0;
	}

	d3d9_mesh_section::~d3d9_mesh_section(void)
	{		
	}

	void d3d9_mesh_section::create_none_index(int mtl_id, enum EPrimitiveType primitive_type, size_t primitive_count)
	{
		m_material_id = mtl_id;
		m_draw_type = primitive_type;
		m_primitive_count = primitive_count;
		m_d3d_pm_type = conv_primitive_type(m_draw_type);
		m_base_vertex_index = 0;
	}

	void d3d9_mesh_section::create(const index_buffer16* index_data, int mtl_id, enum EPrimitiveType primitive_type, D3DPOOL pool)
	{
		//-- create d3d9 index buffer
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();


		IDirect3DIndexBuffer9* new_ib = NULL;
		HRESULT hr = d3d_device->CreateIndexBuffer((UINT)index_data->get_data_size(),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			pool,
			&new_ib,
			NULL);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("create index buffer failed."));

		d3d_index_ptr index_buffer(new_ib);

		void* lock_data = NULL;
		hr = index_buffer->Lock(0, 0, &lock_data, 0);
		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("index buffer lock failed."));

		memcpy(lock_data, index_data->get_data_ptr(), index_data->get_data_size());
		hr = index_buffer->Unlock();

		//-- exception safe
		m_material_id = mtl_id;
		m_index_buffer = index_buffer;
		m_valid_index_count = index_data->get_index_count();
		m_primitive_count = get_primitive_count_by_vert(primitive_type, m_valid_index_count);
		m_draw_type = primitive_type;
		m_d3d_pm_type = conv_primitive_type(m_draw_type);		
	}

	void d3d9_mesh_section::create(const nmesh_section* sec_ptr, D3DPOOL pool)
	{
		create(&sec_ptr->m_index_buffer,
			sec_ptr->m_material_id,
			sec_ptr->m_primitive_type,
			pool);
		m_base_vertex_index = sec_ptr->m_base_vertex_index;
	}

	void d3d9_mesh_section::draw_set_index_buffer()
	{
		if(m_index_buffer)
		{
			IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
			HRESULT hr = d3d_device->SetIndices(m_index_buffer.get());			
		}
	}

	void d3d9_mesh_section::draw_call(size_t num_vert)
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		if(m_index_buffer)
		{
			hr = d3d_device->DrawIndexedPrimitive(m_d3d_pm_type, 0, 0, (UINT)num_vert, 0, (UINT)m_primitive_count);
		}
		else
		{
			hr = d3d_device->DrawPrimitive(m_d3d_pm_type, 0, (UINT)m_primitive_count);
		}
	}

	void d3d9_mesh_section::update_index(const index_buffer16* index_data, size_t valid_count)
	{
		HRESULT hr;

		void* lock_data = NULL;
		hr = m_index_buffer->Lock(0, 0, &lock_data, 0);
		if( SUCCEEDED(hr) )
		{
			memcpy(lock_data, index_data->get_data_ptr(), index_data->get_data_size());
			hr = m_index_buffer->Unlock();
		}
		else
		{
			nLog_Error(_T("d3d9_mesh_section lock index buffer failed."));
		}

		m_valid_index_count = valid_count;
		m_primitive_count = get_primitive_count_by_vert(m_draw_type, m_valid_index_count);
	}

}//namespace nexus