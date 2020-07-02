#include "StdAfx.h"
#include <sstream>
#include "vertex_factory_type.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"
#include "nrenderer_d3d9.h"
#include "shading_effect.h"
#include "util.h"

namespace nexus
{
	D3DDECLTYPE convert_vertex_element_type(enum EVertexElementType type)
	{
		D3DDECLTYPE ret=D3DDECLTYPE_FLOAT1;
		switch(type)
		{
		case EVET_FLOAT1: ret=D3DDECLTYPE_FLOAT1;
			break;
		case EVET_FLOAT2: ret=D3DDECLTYPE_FLOAT2;
			break;
		case EVET_FLOAT3: ret=D3DDECLTYPE_FLOAT3;
			break;
		case EVET_FLOAT4: ret=D3DDECLTYPE_FLOAT4;
			break;
		case EVET_BYTE4: ret=D3DDECLTYPE_UBYTE4;
			break;
		default:
			nASSERT(0);
		}
		return ret;
	}

	D3DDECLUSAGE convert_vertex_element_usage(enum EVertexElementUsage usage)
	{
		D3DDECLUSAGE ret=D3DDECLUSAGE_POSITION;
		switch(usage)
		{
		case EVEU_Position:			ret = D3DDECLUSAGE_POSITION;
			break;
		case EVEU_Normal:			ret = D3DDECLUSAGE_NORMAL;
			break;
		case EVEU_Tangent:			ret = D3DDECLUSAGE_TANGENT;
			break;
		case EVEU_TexCoord:			ret = D3DDECLUSAGE_TEXCOORD;
			break;
		case EVEU_BlendIndices:		ret = D3DDECLUSAGE_BLENDINDICES;
			break;
		case EVEU_BlendWeight:		ret = D3DDECLUSAGE_BLENDWEIGHT;
			break;
		case EVEU_Color:			ret = D3DDECLUSAGE_COLOR;
			break;
		default:
			nASSERT(0);
		}
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// class vertex_factory_type
	////////////////////////////////////////////////////////////////////////////////////////////////////
	vertex_factory_type::vertex_factory_type(const nstring& mesh_vertex_data_name)
		: m_mesh_vertex_data_name(mesh_vertex_data_name)
	{
	}

	vertex_factory_type::~vertex_factory_type(void)
	{
	}

	void vertex_factory_type::load_shader_file()
	{
		nASSERT( !m_vertex_factory_name.empty() );
		//-- load vertex factory shader code		
		wostringstream oss;
		oss <<_T("shader_d3d9/vertex_factory/")
			<< m_vertex_factory_name
			<< _T(".hlsl");
		load_shder_source(oss.str(), m_shader_code);
	}

	void vertex_factory_type::create_shared(const nmesh_vertex_data* vb, vector<D3DVERTEXELEMENT9>& d3d_element_array)
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		IDirect3DVertexDeclaration9 *d3d_decl = NULL;
		HRESULT hr = d3d_device->CreateVertexDeclaration(&d3d_element_array[0],&d3d_decl);
		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("CreateVertexDeclaration Failed"));

		m_vert_decl.reset(d3d_decl);

		m_vertex_factory_name = vb->get_vertex_factory_name();
		//-- copy shader macro
		m_shader_macro_array = vb->get_macro_array();
	}

	void vertex_factory_type::create_for_morph(const nmesh_vertex_data* vb)
	{
		if( vb == NULL )
			return ;

		//-- create vertex declaration
		vector<D3DVERTEXELEMENT9>	d3d_element_array;
		for(size_t i=0; i<vb->get_num_stream(); i++)
		{
			const element_array& defines = vb->get_stream_elements_define(i);
			for(size_t j=0; j<defines.size(); j++)
			{
				const vertex_element_define& def = defines[j];

				D3DVERTEXELEMENT9 d3d_def;
				d3d_def.Stream	= (WORD)i;
				d3d_def.Offset	= def.offset;
				d3d_def.Type	= convert_vertex_element_type( def.type );
				d3d_def.Method	= D3DDECLMETHOD_DEFAULT;
				d3d_def.Usage	= convert_vertex_element_usage( def.usage );
				d3d_def.UsageIndex = def.usage_index;

				d3d_element_array.push_back(d3d_def);				
			}
		}		

		size_t num = d3d_element_array.size();
		for(size_t i=0; i<num; i++)
		{
			D3DVERTEXELEMENT9 d3d_def = d3d_element_array[i];

			// 拷贝一份：注意每种usage只有有一种，只支持一重uv
			d3d_def.Stream += (WORD)vb->get_num_stream();
			d3d_def.UsageIndex++;

			d3d_element_array.push_back(d3d_def);
		}

		D3DVERTEXELEMENT9 endElm = D3DDECL_END();
		d3d_element_array.push_back(endElm);

		create_shared(vb, d3d_element_array);
	}

	void vertex_factory_type::create_for_static(const nmesh_vertex_data* vb)
	{
		if( vb == NULL )
			return ;

		//-- create vertex declaration
		vector<D3DVERTEXELEMENT9>	d3d_element_array;
		for(size_t i=0; i<vb->get_num_stream(); i++)
		{
			const element_array& defines = vb->get_stream_elements_define(i);
			for(size_t j=0; j<defines.size(); j++)
			{
				const vertex_element_define& def = defines[j];
				
				D3DVERTEXELEMENT9 d3d_def;
				d3d_def.Stream	= (WORD)i;
				d3d_def.Offset	= def.offset;
				d3d_def.Type	= convert_vertex_element_type( def.type );
				d3d_def.Method	= D3DDECLMETHOD_DEFAULT;
				d3d_def.Usage	= convert_vertex_element_usage( def.usage );
				d3d_def.UsageIndex = def.usage_index;

				d3d_element_array.push_back(d3d_def);
			}
		}

		D3DVERTEXELEMENT9 endElm = D3DDECL_END();
		d3d_element_array.push_back(endElm);

		create_shared(vb, d3d_element_array);
	}

	void vertex_factory_type::destroy()
	{
		m_vert_decl.reset();
		m_shader_code.clear();
	}

	void vertex_factory_type::draw_set_vert_decl()
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		d3d_device->SetVertexDeclaration(m_vert_decl.get());
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// class vertex_factory_type_lib
	////////////////////////////////////////////////////////////////////////////////////////////////////
	vertex_factory_type_lib::vertex_factory_type_lib(void)
	{
	}

	vertex_factory_type_lib::~vertex_factory_type_lib(void)
	{
	}

	void vertex_factory_type_lib::draw_set_vert_decl_static_pos_only()
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		d3d_device->SetVertexDeclaration(m_vert_decl_static_pos_only.get());
	}

	void vertex_factory_type_lib::draw_set_vert_decl_morph_pos_only()
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		d3d_device->SetVertexDeclaration(m_vert_decl_morph_pos_only.get());
	}

	void vertex_factory_type_lib::conditional_create_for_morph(const nmesh_vertex_data* vb)
	{
		if( !m_vert_decl_morph_pos_only )
		{
			vector<D3DVERTEXELEMENT9>	d3d_element_array;

			D3DVERTEXELEMENT9 d3d_def1 = {0};
			d3d_def1.Type	= D3DDECLTYPE_FLOAT3 ;
			d3d_def1.Method	= D3DDECLMETHOD_DEFAULT;
			d3d_def1.Usage	= D3DDECLUSAGE_POSITION;
			d3d_element_array.push_back(d3d_def1);

			D3DVERTEXELEMENT9 d3d_def2 = {0};
			d3d_def2.Stream = 1;
			d3d_def2.Type	= D3DDECLTYPE_FLOAT3 ;
			d3d_def2.Method	= D3DDECLMETHOD_DEFAULT;
			d3d_def2.Usage	= D3DDECLUSAGE_POSITION;
			d3d_def2.UsageIndex = 1;
			d3d_element_array.push_back(d3d_def2);

			D3DVERTEXELEMENT9 endElm = D3DDECL_END();
			d3d_element_array.push_back(endElm);

			IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

			IDirect3DVertexDeclaration9 *d3d_decl = NULL;
			HRESULT hr = d3d_device->CreateVertexDeclaration(&d3d_element_array[0],&d3d_decl);
			if( FAILED(hr) )
				THROW_D3D_HRESULT(hr, _T("CreateVertexDeclaration Failed"));

			m_vert_decl_morph_pos_only.reset(d3d_decl);
		}

		//--
		conditional_create(vb, ECreate_Morph);
	}

	void vertex_factory_type_lib::conditional_create_for_static(const nmesh_vertex_data* vb)
	{
		if( !m_vert_decl_static_pos_only )
		{
			vector<D3DVERTEXELEMENT9>	d3d_element_array;

			D3DVERTEXELEMENT9 d3d_def = {0};
			d3d_def.Type	= D3DDECLTYPE_FLOAT3 ;
			d3d_def.Method	= D3DDECLMETHOD_DEFAULT;
			d3d_def.Usage	= D3DDECLUSAGE_POSITION;
			d3d_element_array.push_back(d3d_def);
			
			D3DVERTEXELEMENT9 endElm = D3DDECL_END();
			d3d_element_array.push_back(endElm);

			IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

			IDirect3DVertexDeclaration9 *d3d_decl = NULL;
			HRESULT hr = d3d_device->CreateVertexDeclaration(&d3d_element_array[0],&d3d_decl);
			if( FAILED(hr) )
				THROW_D3D_HRESULT(hr, _T("CreateVertexDeclaration Failed"));

			m_vert_decl_static_pos_only.reset(d3d_decl);

		}
		
		//--
		conditional_create(vb, ECreate_Static);
	}

	void vertex_factory_type_lib::conditional_create(const nmesh_vertex_data* vb, enum ECreateType ct)
	{
		vertex_factory_type::ptr vf_type;

		nstring type_name = vb->get_vertex_type_name();

		vf_type_map::iterator iter = m_type_map.find(type_name);
		if( iter != m_type_map.end() )
		{
			vf_type = iter->second;
		}
		else
		{
			vf_type.reset(nNew vertex_factory_type(vb->get_vertex_type_name()));
			m_type_map.insert(make_pair(vf_type->get_name(), vf_type));
		}


		if( !vf_type->is_created() )
		{
			switch(ct)
			{
			case ECreate_Static:
				vf_type->create_for_static(vb);		
				break;
			case ECreate_Morph:
				vf_type->create_for_morph(vb);
				break;
			default:
				nASSERT(0 && "bad create type");
				break;
			}
		}
	}

	vertex_factory_type* vertex_factory_type_lib::get_type(nstring type_name)
	{
		vf_type_map::iterator iter = m_type_map.find(type_name);
		if( iter != m_type_map.end() )
			return iter->second.get();

		return NULL;
	}
}//namespace nexus