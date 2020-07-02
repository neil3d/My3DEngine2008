#include "StdAfx.h"
#include "d3d9_PDI.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"
#include "util.h"
#include "d3d9_simple_mesh.h"
#include "nrenderer_d3d9.h"
#include "drawing_unlit.h"
#include "draw_hit_proxy.h"

namespace nexus
{
	d3d9_PDI::d3d9_PDI(void)
	{
		m_in_line_batch = false;
		m_view_info = NULL;
	}

	d3d9_PDI::~d3d9_PDI(void)
	{
	}

	void d3d9_PDI::init(d3d_view_info* view_ptr)
	{
		m_view_info = view_ptr;

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
		load_shder_source(_T("shader_d3d9/misc/line_batch.fx"), effect_code);

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

		hr = m_line_effect->SetMatrix("g_matWorldViewPrj", (const D3DXMATRIX*)(&m_view_info->mat_view_project));

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

		x += m_view_info->d3d_view.X;
		y += m_view_info->d3d_view.Y;

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

		if( m_in_line_batch )
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

		if( m_in_line_batch )
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

	void d3d9_PDI::draw_primitive(const nrender_proxy* obj, bool hit_proxy, bool disable_ztest)
	{
		const d3d_view_info* v = nrenderer_d3d9::instance()->get_view_info();
		if( hit_proxy )
		{
			draw_hit_proxy dp;
			if(disable_ztest)
				dp.set_effect_tech("techDisableZ");
			dp.draw_mesh(v, obj);
		}
		else
		{
			drawing_unlit dp;
			if(disable_ztest)
				dp.set_effect_tech("techDisableZ");
			dp.draw_mesh(v, obj);
		}
	}
}//namespace nexus