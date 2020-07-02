#include "StdAfx.h"
#include "neffect_vertex_data.h"

namespace nexus
{
	neffect_vertex_data::neffect_vertex_data(void)
	{
	}

	neffect_vertex_data::~neffect_vertex_data(void)
	{
	}

	nstring neffect_vertex_data::get_vertex_type_name() const	
	{	
		return _T("vertex_effect_PDT");
	}

	nstring neffect_vertex_data::get_vertex_factory_name() const	
	{	
		return _T("vfactory_sfx");
	}

	void neffect_vertex_data::serialize(narchive& ar)
	{
		nmesh_vertex_data::serialize(ar);
		nSERIALIZE(ar, m_diffuse_stream);
		nSERIALIZE(ar, m_uv_stream);
	}

	void neffect_vertex_data::init(size_t num_vert)
	{
		init_pos_stream(num_vert);

		//-- diffuse color stream		
		vertex_element_define diffuse_def = {1, 0, EVET_FLOAT4, EVEU_Color, 0};
		
		element_array diffuse_def_array;
		diffuse_def_array.push_back(diffuse_def);

		m_diffuse_stream.init(diffuse_def_array, num_vert);

		//-- uv		
		vertex_element_define uv_def = {2, 0, EVET_FLOAT2, EVEU_TexCoord, 0};

		element_array uv_def_array;
		uv_def_array.push_back(uv_def);

		m_uv_stream.init(uv_def_array, num_vert);
	}
}//namespace nexus