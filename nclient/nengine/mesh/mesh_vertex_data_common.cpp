#include "StdAfx.h"
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "mesh_vertex_data_common.h"

namespace nexus
{
	nDEFINE_CLASS(nmesh_vertex_data_common, nmesh_vertex_data)
	nDEFINE_CLASS(nmesh_vertex_data_common_morph, nmesh_vertex_data_common)

	nstring nmesh_vertex_data_common::get_vertex_factory_name() const
	{
		return _T("vfactory_static_common");
	}

	nstring nmesh_vertex_data_common::get_vertex_type_name() const
	{
		std::wostringstream ss;
		ss << _T("vertex_common_P.N")
			<< m_num_normal
			<< _T("T")
			<< m_num_uv
			<< _T("_static");

		return ss.str();
	}

	void nmesh_vertex_data_common::init(size_t num_vert, int num_normal, int num_uv)
	{
		m_num_normal = num_normal;
		m_num_uv = num_uv;
		init_pos_stream(num_vert);

		element_array elem_array;

		// normal and tangent
		nASSERT(num_normal <= 2);
		if(num_normal >= 1)
		{
			vertex_element_define norm_def = {1, 0, EVET_FLOAT3, EVEU_Normal, 0};
			elem_array.push_back(norm_def);
		}
		if(num_normal >= 2)
		{
			vertex_element_define tangent_def = {1, 0, EVET_FLOAT3, EVEU_Tangent, 0};
			elem_array.push_back(tangent_def);
		}

		// UVs
		for(int i=0; i<num_uv; i++)
		{
			vertex_element_define uv_def = {1, 0, EVET_FLOAT2, EVEU_TexCoord, (unsigned char)i};
			elem_array.push_back(uv_def);
		}

		// init stream
		m_second_stream.init(elem_array, num_vert);

		// make macro define
		shader_define normal_def("VERT_NUM_NORMAL",boost::lexical_cast<string>(num_normal));
		shader_define uv_def("VERT_NUM_TEXCOORD",boost::lexical_cast<string>(num_uv));

		m_macro_array.push_back(normal_def);
		m_macro_array.push_back(uv_def);
	}

	void nmesh_vertex_data_common::serialize(narchive& ar)
	{
		nmesh_vertex_data::serialize(ar);
		nSERIALIZE(ar, m_num_normal);
		nSERIALIZE(ar, m_num_uv);
		nSERIALIZE(ar, m_second_stream);
	}

	nstring nmesh_vertex_data_common_morph::get_vertex_factory_name() const
	{
		return _T("vfactory_morph_common");
	}

	nstring nmesh_vertex_data_common_morph::get_vertex_type_name() const
	{
		std::wostringstream ss;
		ss << _T("vertex_common_P.N")
			<< m_num_normal
			<< _T("T")
			<< m_num_uv
			<< _T("_morph");

		return ss.str();
	}
}//namespace nexus