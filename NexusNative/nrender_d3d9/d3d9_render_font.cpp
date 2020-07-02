#include "StdAfx.h"
#include "d3d9_render_font.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"

namespace nexus
{
	static const D3DXMATRIX s_font_identity_matrix
	(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	d3d9_render_font::d3d9_render_font(void)
	{
		
	}

	d3d9_render_font::~d3d9_render_font(void)
	{
	}

	void d3d9_render_font::create(const TCHAR* face_name, int width, int height, int weight/* = 0*/, bool italic/* = false*/)
	{
		D3DXFONT_DESC font;
		memset( &font, 0, sizeof(font));
		_tcscpy_s(font.FaceName, face_name); 
		font.Width = width;
		font.Height = height;
		font.Weight = weight;
		font.MipLevels = 1;
		font.Italic = italic;
		font.CharSet = DEFAULT_CHARSET;
		font.OutputPrecision = OUT_DEFAULT_PRECIS;
		font.Quality = italic ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY;
		font.PitchAndFamily = DEFAULT_PITCH;

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		ID3DXFont* new_font = NULL;
		HRESULT hr = D3DXCreateFontIndirect(d3d_device,&font,&new_font);
		if ( FAILED(hr) )
		{
			nLog_Error(_T("create font faild: name[%s] width[%d] height[%d] weight[%d] italic[%s]\r\n"),
				face_name,width,height,weight,(italic?_T("TRUE"):_T("FALSE")));
			THROW_D3D_HRESULT(hr, _T("create font faild."));
		}
		m_font.reset( new_font );
		m_hdc = new_font->GetDC();
		nASSERT(m_hdc!=NULL);
	
		//
		create_ascii_size_table();

		nLog_Info(_T("create font: name[%s] width[%d] height[%d] weight[%d] italic[%s]\r\n"),
			face_name,width,height,weight,(italic?_T("TRUE"):_T("FALSE")));
	}


	int d3d9_render_font::_draw_text( const TCHAR* text, const frect& target_area, const D3DXMATRIXA16& mat, unsigned int color, unsigned int shadow_color, unsigned int format )
	{
		// 完全透明不用画
		if(text == 0)
			return 0;

		bool no_text = (color & 0xFF000000) == 0;
		bool no_shadow = (shadow_color & 0xFF000000) == 0;
		if(no_text && no_shadow )
			return 0;

		int ret = 0;

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		ID3DXSprite* text_sprite = d3d_device_manager::instance()->get_ui_sprite();

		text_sprite->SetTransform( &mat );

		RECT rc;
		SetRect( &rc, 
			static_cast<long>(target_area.left),
			static_cast<long>(target_area.top), 
			static_cast<long>(target_area.right), 
			static_cast<long>(target_area.bottom) );

		// 绘制shade
		if(!no_shadow)
		{
			INT nIncX = 1, nIncY = 1;
			RECT	rcShade = rc;
			rcShade.left-=nIncX;
			rcShade.right-=nIncX;
			ret = m_font->DrawText(text_sprite,text,-1,&rcShade,format,shadow_color);
			rcShade = rc;
			rcShade.left+=nIncX;
			rcShade.right+=nIncX;
			ret = m_font->DrawText(text_sprite,text,-1,&rcShade,format,shadow_color);
			rcShade = rc;
			rcShade.top-=nIncY;
			rcShade.bottom-=nIncY;
			ret = m_font->DrawText(text_sprite,text,-1,&rcShade,format,shadow_color);
			rcShade = rc;
			rcShade.top+=nIncY;
			rcShade.bottom+=nIncY;
			ret = m_font->DrawText(text_sprite,text,-1,&rcShade,format,shadow_color);
		}
		if(!no_text)
		{
			ret = m_font->DrawText(text_sprite,text,-1,&rc,format,color);
		}
		return ret;
	}

	int d3d9_render_font::draw_text(const TCHAR* text, const frect& target_area, unsigned int color, unsigned int shadow_color, unsigned int format)
	{
		return _draw_text(text,target_area,s_font_identity_matrix,color,shadow_color,format);
	}

	int d3d9_render_font::draw_text(const TCHAR* text, long x, long y, unsigned int color, unsigned int shadow_color)
	{
		return _draw_text(text,frect(static_cast<float>(x), static_cast<float>(y), 0, 0),s_font_identity_matrix,color,shadow_color, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOCLIP);
	}

	int d3d9_render_font::draw_text(const TCHAR* text, const frect& target_area, float z ,float scale,unsigned int color, unsigned int shadow_color, unsigned int format) 
	{
		// 完全透明不用画
		if(text == 0)
			return 0;

		bool no_text = (color & 0xFF000000) == 0;
		bool no_shadow = (shadow_color & 0xFF000000) == 0;
		if(no_text && no_shadow )
			return 0;

		int ret = 0;

		D3DXMATRIXA16 m;
		D3DXVECTOR3 center(target_area.get_center().x,target_area.get_center().y,0.0f);
		D3DXVECTOR3 s(scale,scale,scale);
		D3DXVECTOR3 trans(0,0,z);
		D3DXMatrixTransformation(&m,&center,0,&s,0,0,&trans);

		return _draw_text(text,target_area, m, color, shadow_color, format);
	}

	int d3d9_render_font::draw_text(const TCHAR* text, const frect& target_area, float z ,float scale ,const vector2& scale_center,unsigned int color,unsigned int shadow_color, unsigned int format)
	{
		// 完全透明不用画
		if(text == 0)
			return 0;

		bool no_text = (color & 0xFF000000) == 0;
		bool no_shadow = (shadow_color & 0xFF000000) == 0;
		if(no_text && no_shadow )
			return 0;

		D3DXMATRIXA16 m;
		D3DXVECTOR3 center(scale_center.x,scale_center.y,0.0f);
		D3DXVECTOR3 s(scale,scale,scale);
		D3DXVECTOR3 trans(0,0,z);
		D3DXMatrixTransformation(&m,&center,0,&s,0,0,&trans);

		return _draw_text(text,target_area, m, color, shadow_color, format);
	}

	int d3d9_render_font::measure_string( const TCHAR* text, unsigned int format, frect& rectangle )
	{
		RECT rc;
		SetRect( &rc, static_cast<int>(rectangle.left), static_cast<int>(rectangle.top), static_cast<int>(rectangle.right), static_cast<int>(rectangle.bottom) );
		int result = m_font->DrawText( NULL, text, -1, &rc,(format | DT_CALCRECT), 0 );

		rectangle = frect( static_cast<float>(rc.left), 
			static_cast<float>(rc.top),	
			static_cast<float>(rc.right),
			static_cast<float>(rc.bottom) );
		return result;
	}

	void d3d9_render_font::get_text_size( const TCHAR* text, nsize& out_size )
	{
		if( text == 0 )
		{
			out_size.x=out_size.y=0;
			return;
		}
		nASSERT(m_hdc!=NULL);

		//--对获得单个ascii字符尺寸进行优化
		if( text[1]==_T('\0') && text[0]>=0 && text[0]<127 )
		{
			out_size=m_ascii_size[text[0]];
			return;
		}
		SIZE size;
		GetTextExtentPoint32(m_hdc,text,-1,&size);
		out_size.x = size.cx;
		out_size.y = size.cy;
	}

	void d3d9_render_font::create_ascii_size_table()
	{
		TCHAR szChar[2]={0};
		for(int i=0;i<127;i++)
		{
			szChar[0]=i;
			SIZE size;
			::GetTextExtentPoint32(m_hdc,szChar,1,&size);
			m_ascii_size[i].x = size.cx;
			m_ascii_size[i].y = size.cy;
		}
	}

	bool d3d9_render_font::measure_string_ex( const nstring& text,vector<int>& dx,vector<int>& caret_pos, frect& rectangle )
	{
		nASSERT(m_hdc!=NULL);
		static nchar s_buffer[max_measure_ex_length];
		static UINT s_order[max_measure_ex_length];
		static int	s_dx[max_measure_ex_length];
		static int s_caret[max_measure_ex_length];
		int len=text.length();
		if(len>max_measure_ex_length) len=max_measure_ex_length;
		GCP_RESULTS rst;
		ZeroMemory(&rst,sizeof(rst));
		rst.lStructSize=sizeof(GCP_RESULTS);
		rst.lpCaretPos=s_caret;
		rst.lpDx=s_dx;
		rst.lpOrder=s_order;
		DWORD size=GetCharacterPlacement(m_hdc,text.c_str(),len,0,&rst,GCP_JUSTIFY );
		if(size==0) return false;
		rectangle.left=0;	rectangle.top=0;
		rectangle.right=(float)(size&0xFFFF);	rectangle.bottom=(float)((size>>16)&0xFFFF);
		dx.clear();
		caret_pos.clear();
		for(int i=0;i<len;++i)
		{
			dx.push_back(s_dx[s_order[i]]);
			caret_pos.push_back(s_caret[i]);
		}
		return true;
	}
}
