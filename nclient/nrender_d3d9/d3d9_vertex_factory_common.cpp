#include "StdAfx.h"
#include "d3d9_vertex_factory_common.h"
#include "d3d_exception.h"
#include "d3d_device_manager.h"
#include "util.h"

namespace nexus
{
	d3d9_vertex_factory_common::d3d9_vertex_factory_common(void)
	{
	}

	d3d9_vertex_factory_common::~d3d9_vertex_factory_common(void)
	{
	}

	void d3d9_vertex_factory_common::create(const nmesh_vertex_data* mesh_vb, D3DPOOL pool)
	{
		destroy();

		nASSERT( mesh_vb != NULL);
		nASSERT( mesh_vb->get_num_stream() > 0);
		if( mesh_vb == NULL
			|| mesh_vb->get_num_stream()<=0)
			return;


		//-- vertex factory type
		vertex_factory_type_lib::instance()->conditional_create_for_static(mesh_vb);
		vertex_factory_type* vf_type = vertex_factory_type_lib::instance()->get_type(mesh_vb->get_vertex_type_name());

		if( !vf_type )
			THROW_D3D_EXCEPTION(_T("vertex factory type not find."));

		//-- create IDirect3DVertexBuffer9
		try
		{
			for(size_t i=0; i<mesh_vb->get_num_stream(); i++)
			{
				const vertex_stream* stream = const_cast<nmesh_vertex_data*>(mesh_vb)->get_stream(i);
				m_vert_streams.push_back( create_d3d_vb_from_stream(stream, pool, true) );
				m_stream_stride.push_back( (UINT)stream->get_vertex_size() );
			}
		}
		catch(nexception& e)
		{
			(void)e;
			m_vert_streams.clear();
			m_stream_stride.clear();
			throw;
		}

		//--
		m_num_vert = mesh_vb->get_num_vert();
		m_type = vf_type;
	}

	void d3d9_vertex_factory_common::draw_set_pos_only()
	{
		draw_set_pos_only_static();
	}

	void d3d9_vertex_factory_common::update(const nmesh_vertex_data* mesh_vb)
	{
		nASSERT(mesh_vb);
		nASSERT(mesh_vb->get_num_stream() == m_vert_streams.size());

		for(size_t i=0; i<mesh_vb->get_num_stream(); i++)
		{
			d3d_copy_vertex_buffer(m_vert_streams[i],
				const_cast<nmesh_vertex_data*>(mesh_vb)->get_stream(i));
		}
	}
}//namespace nexus