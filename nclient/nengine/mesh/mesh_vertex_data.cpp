#include "StdAfx.h"
#include "mesh_vertex_data.h"

namespace nexus
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// class nmesh_vertex_data
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	nDEFINE_VIRTUAL_CLASS(nmesh_vertex_data, nobject)

	nmesh_vertex_data::nmesh_vertex_data(void)
	{
	}

	nmesh_vertex_data::~nmesh_vertex_data(void)
	{
	}

	void nmesh_vertex_data::init_pos_stream(size_t num_vert)
	{
		vertex_element_define pos_def = {0, 0, EVET_FLOAT3, EVEU_Position, 0};

		element_array pos_def_array;
		pos_def_array.push_back(pos_def);

		m_pos_stream.init(pos_def_array, num_vert);
	}

	void nmesh_vertex_data::serialize(narchive& ar)
	{
		nSERIALIZE(ar, m_pos_stream);
		nSERIALIZE(ar, m_macro_array);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// class mesh_vertex_data_pos_normal
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	nDEFINE_CLASS(nmesh_vertex_data_pos_normal, nmesh_vertex_data)
	nmesh_vertex_data_pos_normal::nmesh_vertex_data_pos_normal(void)
	{
	}

	nmesh_vertex_data_pos_normal::~nmesh_vertex_data_pos_normal(void)
	{
	}

	void nmesh_vertex_data_pos_normal::init(size_t num_vert)
	{
		init_pos_stream(num_vert);

		vertex_element_define norm_def = {1, 0, EVET_FLOAT3, EVEU_Normal, 0};

		element_array norm_def_array;
		norm_def_array.push_back(norm_def);

		m_normal_stream.init(norm_def_array, num_vert);
	}

	void nmesh_vertex_data_pos_normal::serialize(narchive& ar)
	{
		nmesh_vertex_data::serialize(ar);
		nSERIALIZE(ar, m_normal_stream);
	}

	nstring nmesh_vertex_data_pos_normal::get_vertex_type_name() const	
	{	
		return _T("vertex_P.N_static");
	}
	
	nstring nmesh_vertex_data_pos_normal::get_vertex_factory_name() const	
	{	
		return _T("vfactory_static_PN");
	}
}//namespace nexus