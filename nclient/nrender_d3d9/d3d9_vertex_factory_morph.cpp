#include "StdAfx.h"
#include "d3d9_vertex_factory_morph.h"
#include "d3d_exception.h"
#include "d3d_device_manager.h"
#include "dynamic_vb_pool.h"

namespace nexus
{
	d3d9_vertex_factory_morph::d3d9_vertex_factory_morph(void)
	{
		m_pos_stream_stride = sizeof(vector3);
	}

	d3d9_vertex_factory_morph::~d3d9_vertex_factory_morph(void)
	{
	}

	void d3d9_vertex_factory_morph::destroy()
	{
		d3d9_vertex_factory::destroy();

		m_vb_pos_a.reset();
		m_vb_pos_b.reset();
	}

	void d3d9_vertex_factory_morph::draw_set_pos_only()
	{
		if( !m_vb_pos_a
			|| !m_vb_pos_b)
			return;

		vertex_factory_type_lib::instance()->draw_set_vert_decl_morph_pos_only();
		
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		hr = d3d_device->SetStreamSource(0, m_vb_pos_a.get(), 0, m_pos_stream_stride);
		hr = d3d_device->SetStreamSource(1, m_vb_pos_b.get(), 0, m_pos_stream_stride);
	}

	void d3d9_vertex_factory_morph::create(const nmesh_vertex_data* mesh_vb)
	{
		destroy();

		nASSERT( mesh_vb != NULL);
		nASSERT( mesh_vb->get_num_stream() > 0);
		if( mesh_vb == NULL
			|| mesh_vb->get_num_stream()<=0)
			return;

		//-- vertex factory type
		vertex_factory_type_lib::instance()->conditional_create_for_morph(mesh_vb);
		vertex_factory_type* vf_type = vertex_factory_type_lib::instance()->get_type(mesh_vb->get_vertex_type_name());

		if( !vf_type )
			THROW_D3D_EXCEPTION(_T("vertex factory type not find."));

		//-- 这里不创建IDirect3DVertexBuffer9，而在每帧使用dynamic vb pool进行分配	

		//--
		m_num_vert = mesh_vb->get_num_vert();
		m_type = vf_type;
	}

	d3d_vb_ptr d3d9_vertex_factory_morph::create_vertex_for_frame(nmesh_vertex_data::ptr mesh_frame_vb)
	{
		d3d_vb_ptr pos_vb;

		for(size_t i=0; i<mesh_frame_vb->get_num_stream(); i++)
		{
			const vertex_stream* stream = mesh_frame_vb->get_stream(i);

			d3d_vb_ptr d3d_vb = dynamic_vb_pool::instance()->alloc_for_vertex_stream(stream, true);							
			if(i == 0)
				pos_vb = d3d_vb;

			m_vert_streams.push_back( d3d_vb );
			m_stream_stride.push_back( (UINT)stream->get_vertex_size() );
		}// end of for

		return pos_vb;
	}

	void d3d9_vertex_factory_morph::prepare_frame_data(const struct anim_mesh_morph& anim)
	{
		try
		{
			m_vb_pos_a = create_vertex_for_frame(anim.src_frame_vert);
			m_vb_pos_b = create_vertex_for_frame(anim.dst_frame_vert);
		}// end of try
		catch(nexception& e)
		{
			(void)e;
			m_vert_streams.clear();
			m_stream_stride.clear();
			throw;
		}
	}

	void d3d9_vertex_factory_morph::release_frame_data()
	{
		m_vb_pos_a.reset();
		m_vb_pos_b.reset();
		
		for(size_t i=0; i<m_vert_streams.size(); i++)
			dynamic_vb_pool::instance()->free( m_vert_streams[i] );

		m_vert_streams.clear();
		m_stream_stride.clear();
	}
}//namespace nexus