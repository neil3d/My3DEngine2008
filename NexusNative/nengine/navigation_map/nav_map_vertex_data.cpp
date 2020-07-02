#include "StdAfx.h"
#include "nav_map_vertex_data.h"

namespace nexus
{
	nav_map_vertex_data::nav_map_vertex_data(void)
	{
	}

	nav_map_vertex_data::~nav_map_vertex_data(void)
	{
	}

	void nav_map_vertex_data::init( size_t num_vert )
	{
		init_pos_stream(num_vert);

		//-- diffuse color stream		
		vertex_element_define diffuse_def = {1, 0, EVET_FLOAT4, EVEU_TexCoord, 0};

		element_array diffuse_def_array;
		diffuse_def_array.push_back(diffuse_def);

		m_diffuse_stream.init(diffuse_def_array, num_vert);
	}

	nstring nav_map_vertex_data::get_vertex_type_name() const
	{
		return _T("vertex_nav_map");
	}

	nstring nav_map_vertex_data::get_vertex_factory_name() const
	{
		return _T("vfactory_nav_map");
	}

	vertex_stream* nav_map_vertex_data::get_stream( size_t stream_index )
	{
		if(stream_index == 0)
		{
			return &m_pos_stream;
		}
		else if(stream_index == 1)
		{
			return &m_diffuse_stream;
		}

		nASSERT(0 && _T("Invalid stream index!"));
		return NULL;
	}

	void nav_map_vertex_data::serialize( narchive& ar )
	{
		nmesh_vertex_data::serialize(ar);
		nSERIALIZE(ar, m_diffuse_stream);
	}

} // end of namespace nexus
