#include "StdAfx.h"
#include <boost/bind.hpp>
#include "d3d9_PDI.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"
#include "util.h"
#include "d3d9_simple_mesh.h"
#include "nrenderer_d3d9.h"
#include "drawing_unlit.h"
#include "draw_hit_proxy.h"
#include "mesh_render.h"

namespace nexus
{
	d3d9_PDI::d3d9_PDI(void)
	{
		m_in_line_batch = false;
		m_view_info = NULL;

		d3d_device_manager::instance()->register_device_handler(
			boost::bind(&d3d9_PDI::on_device_lost, this, _1),
			boost::bind(&d3d9_PDI::on_device_reset, this, _1) );
	}

	d3d9_PDI::~d3d9_PDI(void)
	{
	}

	void d3d9_PDI::init(nview_info* view_ptr)
	{
		m_view_info = view_ptr;
		mat_set_identity(m_matrix);

		//-- create vertex decaration
		std::vector<D3DVERTEXELEMENT9>	color_vert_def;

		D3DVERTEXELEMENT9 d3d_def_pos	= {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0};
		D3DVERTEXELEMENT9 d3d_def_color = {0, sizeof(vector3), D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0};
		D3DVERTEXELEMENT9 end_def = D3DDECL_END();

		color_vert_def.push_back(d3d_def_pos);
		color_vert_def.push_back(d3d_def_color);
		color_vert_def.push_back(end_def);

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		IDirect3DVertexDeclaration9 *d3d_decl = NULL;
		HRESULT hr = d3d_device->CreateVertexDeclaration(&color_vert_def[0],&d3d_decl);
		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("CreateVertexDeclaration Failed"));

		m_color_vert_decl.reset(d3d_decl);

		//-- load effect
		std::string effect_code;
		load_shader_source(_T("shader_d3d9/misc/line_batch.fx"), effect_code);

		ID3DXEffect* d3d_effect = NULL;
		hr = D3DXCreateEffect(d3d_device,
			effect_code.c_str(),
			(UINT)effect_code.length(),
			NULL, NULL,
			0,
			NULL, &d3d_effect, NULL);

		if( SUCCEEDED(hr) )
			m_line_effect.reset(d3d_effect);

		//-- create d3d font
		D3DXFONT_DESC font;
		memset(&font,0,sizeof(font));
		font.Height = 12;
		font.Width = 0;
		font.Weight= 400;
		font.MipLevels = D3DX_DEFAULT;
		font.Italic = 0;
		font.CharSet = DEFAULT_CHARSET;
		font.OutputPrecision = OUT_DEFAULT_PRECIS;
		font.Quality = DEFAULT_QUALITY;
		font.PitchAndFamily = DEFAULT_PITCH;
		//strcpy( font.FaceName, "");

		ID3DXFont* d3d_font = NULL;
		hr = D3DXCreateFontIndirect(d3d_device,
			&font,
			&d3d_font);
		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("D3DXCreateFontIndirect() Failed"));
		
		m_font.reset(d3d_font);

		//-----------------------------------------------------------------------------------
		//生成球体的顶点
		const int segment_count=36;
		float angle=nPI*2.0f/(segment_count-1.0f);
		vector3 v;
		for(int i=0;i<segment_count;++i)
		{
			float x=cosf(angle*i);
			float y=sinf(angle*i);
			float z=0;
			v.x=x;	v.y=z;	v.z=y;		m_sphere_vertex[0].push_back(v);

			v.x=x;	v.y=y;	v.z=z;		m_sphere_vertex[1].push_back(v);

			v.x=z;	v.y=y;	v.z=x;		m_sphere_vertex[2].push_back(v);			
		}
	}

	void d3d9_PDI::begin_line_batch()
	{
		m_line_vb.clear();
		m_in_line_batch = true;
	}

	void d3d9_PDI::draw_line(const vector3& start, const vector3& end, const color4f& color)
	{
		color_vertex vert1 = {start, color};
		color_vertex vert2 = {end, color};

		m_line_vb.push_back(vert1);
		m_line_vb.push_back(vert2);
	}

	void d3d9_PDI::end_line_batch()
	{
		m_in_line_batch = false;
		draw_line_shared(D3DPT_LINELIST);
	}

	void d3d9_PDI::draw_line_shared(D3DPRIMITIVETYPE prim_type)
	{
		if( m_line_vb.empty() )
			return;

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		hr = d3d_device->SetVertexDeclaration(m_color_vert_decl.get());

		matrix44 m;
		m=m_matrix*m_view_info->mat_view_project;
		hr = m_line_effect->SetMatrix("g_matWorldViewPrj", (const D3DXMATRIX*)(&m));

		UINT num_pass = 0;
		hr = m_line_effect->Begin(&num_pass, 0);

		if( FAILED(hr) )
			return;

		UINT prim_count = 0;
		switch(prim_type)
		{
		case D3DPT_LINELIST:
			prim_count = m_line_vb.size()/2;
			break;
		case D3DPT_LINESTRIP:
			prim_count = m_line_vb.size()-1;
			break;
		default:
			nASSERT(0 && "unknown line type");
		}

		for(UINT i=0; i<num_pass; i++)
		{
			m_line_effect->BeginPass(i);
			hr = d3d_device->DrawPrimitiveUP(prim_type, prim_count, &m_line_vb[0], sizeof(color_vertex));
			m_line_effect->EndPass();
		}
		m_line_effect->End();
	}

	void d3d9_PDI::draw_simple_mesh(const nrender_simple_mesh* mesh, const color4f& color)
	{
		const d3d9_simple_mesh* d3d_mesh = (const d3d9_simple_mesh*)mesh;

		d3d_mesh->draw(m_view_info, color );
	}

	void d3d9_PDI::draw_debug_string(size_t x, size_t y, const nstring& text, const color4ub& color)
	{
		size_t len = text.length();	

		x += m_view_info->m_view_port.X;
		y += m_view_info->m_view_port.Y;

		RECT rc;
		rc.left = x;
		rc.top = y;
		rc.right = x+len*12;
		rc.bottom = y+12+2;

		int offset = m_font->DrawText(NULL, text.c_str(), len, &rc, DT_TOP|DT_LEFT,
			D3DCOLOR_ARGB(color.A, color.R, color.G, color.B));
	}

	void d3d9_PDI::draw_line_strip(const std::vector<vector3>& pos_array, const color4f& color)
	{
		if( pos_array.empty() )
			return;

		bool line_batch=m_in_line_batch;
		if( line_batch )
		{
			end_line_batch();
		}

		m_line_vb.clear();

		color_vertex vert;
		vert.color = color;
		for(size_t i=0; i<pos_array.size(); i++)
		{
			vert.pos = pos_array[i];
			m_line_vb.push_back(vert);
		}

		draw_line_shared(D3DPT_LINESTRIP);

		if( line_batch )
		{
			begin_line_batch();
		}
	}

	void d3d9_PDI::draw_wired_box(const AABBox& box, const color4f& color)
	{
		vector3 pts[8];
		box.get_corners(pts);

		for (int i=0; i<4; i++)
		{
			draw_line(pts[i], pts[(i+1)%4], color);
			draw_line(pts[i+4], pts[(i+1)%4+4], color);
			draw_line(pts[i], pts[(i+4)%8], color);
		}
	}

	void d3d9_PDI::draw_mesh_component(const nmesh_component* comp, bool hit_proxy, bool disable_ztest)
	{
		const std::vector<mesh_element>& mesh_elements = (const_cast<nmesh_component*>(comp))->get_mesh_element(0);

		const nview_info* v = nrenderer_d3d9::instance()->get_view_info();

		draw_hit_proxy hit_dp;
		drawing_unlit unlit_dp;
		
		if(disable_ztest)
		{
			hit_dp.set_effect_tech("techDisableZ");
			unlit_dp.set_effect_tech("techDisableZ");
		}

		drawing_policy* dp = NULL;
		if( hit_proxy )
			dp = &hit_dp;
		else
			dp = &unlit_dp;

		for ( size_t i = 0; i < mesh_elements.size(); i ++)
		{
			dp->draw(v,mesh_elements[i]);
		}
	}

	void d3d9_PDI::on_device_lost(int param)
	{
		HRESULT hr;
		if( m_line_effect )
		{
			hr = m_line_effect->OnLostDevice();
			nASSERT( SUCCEEDED(hr) );
		}
		if( m_font )
		{
			hr = m_font->OnLostDevice();
			nASSERT( SUCCEEDED(hr) );
		}
	}

	bool d3d9_PDI::on_device_reset(int param)
	{
		HRESULT hr;
		if( m_line_effect )
		{
			hr = m_line_effect->OnResetDevice();
			if( FAILED(hr) )
				return false;
		}

		if( m_font )
		{
			hr = m_font->OnResetDevice();
			if( FAILED(hr) )
				return false;
		}

		return true;
	}

	void d3d9_PDI::draw_wired_sphere( const vector3& pos,float radius, const color4f& color )
	{
		mat_set_scaling(m_matrix,radius,radius,radius);
		mat_translate(m_matrix,pos);
		color4f colorX = color4f(1, 0, 0, 1);
		color4f colorY = color4f(0, 1, 0, 1);
		color4f colorZ = color4f(0, 0, 1, 1);
		draw_line_strip(m_sphere_vertex[0],colorY);
		draw_line_strip(m_sphere_vertex[1],colorZ);
		draw_line_strip(m_sphere_vertex[2],colorX);
		mat_set_identity(m_matrix);
	}

	void d3d9_PDI::draw_wired_cone( float bottom_radius, float length, const matrix44& transform, const color4f& color )
	{
		const int segment_count=36;
		float angle=nPI*2.0f/(segment_count-1.0f);
		vector3 v,p(0,0,0);
		std::vector<vector3>		m_cone_vertex;
		m_matrix=transform;
		for(int i=0;i<segment_count;++i)
		{
			v.x=bottom_radius*cosf(angle*i);
			v.y=bottom_radius*sinf(angle*i);
			v.z=length;
			m_cone_vertex.push_back(v);
			draw_line(v,p,color);
		}
		draw_line_strip(m_cone_vertex,color);
		mat_set_identity(m_matrix);
	}

	void d3d9_PDI::set_transform( const matrix44& matrix )
	{
		m_matrix=matrix;
	}
}//namespace nexus