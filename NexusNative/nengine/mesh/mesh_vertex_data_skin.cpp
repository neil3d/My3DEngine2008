#include "StdAfx.h"
#include "mesh_vertex_data_skin.h"

namespace nexus
{
	nDEFINE_CLASS(nmesh_vertex_data_skin, nmesh_vertex_data)

	nmesh_vertex_data_skin::nmesh_vertex_data_skin(void)
	{
		m_tangent = false;
	}

	nmesh_vertex_data_skin::~nmesh_vertex_data_skin(void)
	{
	}

	nstring nmesh_vertex_data_skin::get_vertex_type_name() const
	{
		std::wostringstream ss;
		ss << _T("vertex_skin_T")
			<< m_tangent;
		return ss.str();			
	}

	nstring nmesh_vertex_data_skin::get_vertex_factory_name() const
	{
		return _T("vfactory_skin");
	}

	void nmesh_vertex_data_skin::init(size_t num_vert, bool have_tangent)
	{
		m_tangent = have_tangent;
		init_pos_stream(num_vert);

		//-- 初始化second stream
		unsigned short offset = 0;
		element_array elem_array;

		vertex_element_define norm_def = {1, offset, EVET_FLOAT3, EVEU_Normal, 0};
		offset += sizeof(float)*3;
		elem_array.push_back(norm_def);

		if( m_tangent )
		{
			vertex_element_define tangent_def = {1, offset, EVET_FLOAT3, EVEU_Tangent, 0};
			offset += sizeof(float)*3;
			elem_array.push_back(tangent_def);			
		}		

		vertex_element_define uv_def = {1, offset, EVET_FLOAT2, EVEU_TexCoord, 0};
		offset += sizeof(float)*2;
		elem_array.push_back(uv_def);

		// bone index
		vertex_element_define idx_def = {1, offset, EVET_BYTE4, EVEU_BlendIndices, 0};
		offset += 4;
		elem_array.push_back(idx_def);

		// bone weight
		vertex_element_define wgt_def = {1, offset, EVET_FLOAT4, EVEU_BlendWeight, 0};
		offset += sizeof(float)*4;
		elem_array.push_back(wgt_def);

		// init stream
		m_second_stream.init(elem_array, num_vert);

		//-- 创建shader define
		shader_define tangent_def("SKIN_TANGENT",boost::lexical_cast<string>(m_tangent));
		m_macro_array.push_back(tangent_def);
	}
}//namespace nexus