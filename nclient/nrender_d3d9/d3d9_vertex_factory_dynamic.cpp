#include "StdAfx.h"
#include "d3d9_vertex_factory_dynamic.h"
#include "d3d_exception.h"
#include "d3d_device_manager.h"
#include "dynamic_vb_pool.h"

namespace nexus
{
	d3d9_vertex_factory_dynamic::d3d9_vertex_factory_dynamic(void)
	{
	}

	d3d9_vertex_factory_dynamic::~d3d9_vertex_factory_dynamic(void)
	{
	}

	void d3d9_vertex_factory_dynamic::create(const nmesh_vertex_data* mesh_vb)
	{
		destroy();

		nASSERT( mesh_vb != NULL);
		nASSERT( mesh_vb->get_num_stream() > 0);
		if( mesh_vb == NULL
			|| mesh_vb->get_num_stream()<=0)
			return;

		vertex_factory_type_lib::instance()->conditional_create_for_static(mesh_vb);

		//-- vertex factory type
		vertex_factory_type_lib::instance()->conditional_create_for_static(mesh_vb);
		vertex_factory_type* vf_type = vertex_factory_type_lib::instance()->get_type(mesh_vb->get_vertex_type_name());

		if( !vf_type )
			THROW_D3D_EXCEPTION(_T("vertex factory type not find."));

		//--
		m_num_vert = mesh_vb->get_num_vert();
		m_type = vf_type;
	}

	void d3d9_vertex_factory_dynamic::draw_set_pos_only()
	{
		draw_set_pos_only_static();
	}

	void d3d9_vertex_factory_dynamic::prepare_frame_data(nmesh_vertex_data* mesh_vb)
	{
		for(size_t i=0; i<mesh_vb->get_num_stream(); i++)
		{
			const vertex_stream* stream = mesh_vb->get_stream(i);

			d3d_vb_ptr d3d_vb = dynamic_vb_pool::instance()->alloc_for_vertex_stream(stream, true);							
			
			m_vert_streams.push_back( d3d_vb );
			m_stream_stride.push_back( (UINT)stream->get_vertex_size() );
		}// end of for
	}

	void d3d9_vertex_factory_dynamic::release_frame_data()
	{
		for(size_t i=0; i<m_vert_streams.size(); i++)
			dynamic_vb_pool::instance()->free( m_vert_streams[i] );

		m_vert_streams.clear();
		m_stream_stride.clear();
	}
}//namespace nexus