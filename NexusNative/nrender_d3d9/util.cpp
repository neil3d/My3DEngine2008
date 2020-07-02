#include "StdAfx.h"
#include "util.h"
#include "nrenderer_d3d9.h"
#include "d3d_exception.h"

namespace nexus
{
	void load_shder_source(const nstring& file_name, std::string& shader_source)
	{
		size_t file_size = g_file_sys->get_file_size(g_engine_package_name, file_name);
		if(file_size > 0
			&& file_size !=-1)
		{
			shader_source.resize(file_size, 0);
			nfile::ptr file_ptr = g_file_sys->open_file(g_engine_package_name, file_name, EFileRead);
			file_ptr->read_buffer((void*)shader_source.data(), file_size);
		}
	}

	void d3d_copy_vertex_buffer(d3d_vb_ptr dest_vb, const vertex_stream* src_stream)
	{
		nASSERT( dest_vb );
		nASSERT( src_stream != NULL);

		HRESULT hr;
		UINT vb_size = (UINT)src_stream->get_data_size();
		void* vb_data = NULL;
		hr = dest_vb->Lock(0, vb_size, &vb_data, 0 );
		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("Vertex Buffer Lock Failed."));

		memcpy(vb_data, src_stream->get_data(), vb_size);

		hr = dest_vb->Unlock();

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("Vertex Buffer Unlock Failed."));
	}
}//namespace nexus
