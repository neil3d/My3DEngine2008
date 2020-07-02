#include "StdAfx.h"
#include "d3d9_ui_canvas.h"
#include "d3d_device_manager.h"

namespace nexus
{
	#define  GUI_ALIGN_ELEMENTS_TO_PIXELS 1
	
	#if defined(GUI_ALIGN_ELEMENTS_TO_PIXELS)
	#	define PixelAligned(x)	( (float)(int)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f)) )
	#else
	#	define PixelAligned(x)	( x )
	#endif

	DWORD UI_SCREEN_VERTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2;

	DWORD UI_SCREEN_VERTEX_UNTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

	static const D3DMATRIX s_identityMatrix =
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	d3d9_ui_canvas::d3d9_ui_canvas(void) 
	{
	}

	d3d9_ui_canvas::~d3d9_ui_canvas(void)
	{
	}

	void d3d9_ui_canvas::begin(bool depth_enable)
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		d3d_device->SetFVF( UI_SCREEN_VERTEX::FVF );

		// no shaders initially
		d3d_device->SetVertexShader(0);
		d3d_device->SetPixelShader(0);

		if(depth_enable)
		{
			d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
		else
		{
			d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		d3d_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		d3d_device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0);


		ID3DXSprite* ui_sprite = d3d_device_manager::instance()->get_ui_sprite();
		ui_sprite->Begin( D3DXSPRITE_ALPHABLEND);
	}

	void d3d9_ui_canvas::end()
	{
		ID3DXSprite* ui_sprite = d3d_device_manager::instance()->get_ui_sprite();
		ui_sprite->End();
	}

	vector2 d3d9_ui_canvas::get_viewport_size()
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		nASSERT( d3d_device != 0 );
		D3DVIEWPORT9 viewport;
		d3d_device->GetViewport(&viewport);
		return vector2(static_cast<float>(viewport.Width),static_cast<float>(viewport.Height));
	}


	void d3d9_ui_canvas::set_scissor_rect( const RECT* rect )
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		if(rect == NULL)
		{
			d3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		}
		else
		{
			// setup clip region
			d3d_device->SetScissorRect(rect);
			d3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
		}
	}

	// 设置裁减区域
	void d3d9_ui_canvas::push_scissor_rect(const frect* rect)
	{
		// setup clip region
		RECT clip;
		SetRect(&clip,static_cast<LONG>(rect->left),
			static_cast<LONG>(rect->top),
			static_cast<LONG>(rect->right),
			static_cast<LONG>(rect->bottom) );
		m_scissor_rects.push_back(clip);
		
		set_scissor_rect(&clip);
	}

	void d3d9_ui_canvas::pop_scissor_rect()
	{
		if(!m_scissor_rects.empty())
			m_scissor_rects.pop_back();
		if(m_scissor_rects.empty())
		{
			set_scissor_rect(NULL);
		}
		else
		{
			set_scissor_rect( &m_scissor_rects.back());
		}
	}

	void d3d9_ui_canvas::get_scissor_rect(frect& rect)
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		RECT result;
		d3d_device->GetScissorRect( &result );

		rect = frect( static_cast<float>(result.left),
			static_cast<float>(result.top), 
			static_cast<float>(result.right), 
			static_cast<float>(result.bottom) );
	}

	// 绘制文本
	void d3d9_ui_canvas::draw_string(nrender_font* font,  const TCHAR* text, const frect& target_area, unsigned int color, unsigned int shadow_color, unsigned int format)
	{
		font->draw_text(text,target_area,color,shadow_color,format);
	}

	void d3d9_ui_canvas::draw_string( nrender_font* font,const TCHAR* text, long x, long y, unsigned int color, unsigned int shadow_color)
	{
		font->draw_text(text,x,y,color,shadow_color);
	}

	void d3d9_ui_canvas::draw_string( nrender_font* font, const TCHAR* text, const frect& target_area, float z ,float scale ,unsigned int color, unsigned int shadow_color, unsigned int format)
	{
		font->draw_text(text,target_area,z,scale,color,shadow_color,format);
	}

	void d3d9_ui_canvas::draw_string( nrender_font* font, const TCHAR* text,const frect& target_area, float z ,float scale ,const vector2& scale_center,unsigned int color,unsigned int shadow_color, unsigned int format)
	{
		font->draw_text(text,target_area,z,scale,scale_center,color,shadow_color,format);
	}
	// 绘制图像
	void d3d9_ui_canvas::draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,const color_rect& color )
	{
		if( texture == NULL )return;

		bool is_visible = ((color.left_top&0xFF000000) |(color.right_top&0xFF000000)
			| (color.right_top&0xFF000000) | (color.right_bottom&0xFF000000)) != 0;

		if( !is_visible )return;

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		if( d3d_device == NULL )return;
		IDirect3DTexture9* d3d_tex = static_cast<IDirect3DTexture9*>(texture->get_handle());
		if( d3d_tex==NULL )return;

		ID3DXSprite* ui_sprite = d3d_device_manager::instance()->get_ui_sprite();
		ui_sprite->Flush();
		IDirect3DVertexDeclaration9* pDecl = NULL;
		d3d_device->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
		d3d_device->SetFVF( UI_SCREEN_VERTEX::FVF );

		UI_SCREEN_VERTEX vertices[4] =
		{
			target_area.left,  PixelAligned(target_area.top),    0.0f, 1.0f, color.left_top,	source_area.left,	source_area.top,0,0,	
			target_area.right, PixelAligned(target_area.top),    0.0f, 1.0f, color.right_top,	source_area.right,	source_area.top,1.0f,0,	
			target_area.right, PixelAligned(target_area.bottom), 0.0f, 1.0f, color.right_bottom,source_area.right,	source_area.bottom,1.0f,1.0f,
			target_area.left,  PixelAligned(target_area.bottom), 0.0f, 1.0f, color.left_bottom,	source_area.left,	source_area.bottom, 0.0f,1.0f,
		};

		d3d_device->SetTexture(0, d3d_tex);
		d3d_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof( UI_SCREEN_VERTEX ) );

		// Restore the vertex decl
		d3d_device->SetVertexDeclaration( pDecl );
		pDecl->Release();
	}

	void d3d9_ui_canvas::draw_texture( nrender_texture2D* texture, const frect& source_area, const frect& target_area,float z,float s,argb_t color /*= 0xFFFFFFFF*/ )
	{
		if( texture == NULL )return;

		if( (color&0xFF000000) == 0 ) return;

		ID3DXSprite* sprite = d3d_device_manager::instance()->get_ui_sprite();
		if( sprite==NULL )return;
		IDirect3DTexture9* d3d_tex=static_cast<IDirect3DTexture9*>(texture->get_handle());
		if( d3d_tex==NULL || sprite==NULL )return;

		//因为输入参数是[0,1]之间的纹理坐标，sprite接受的是[0,width][0,height]之间的坐标，所以需要进行一次转换
		//其实这次转换很多余，既损失性能又可能引入误差
		D3DSURFACE_DESC desc;
		d3d_tex->GetLevelDesc(0,&desc);

		float scale_x = target_area.get_width() / (source_area.get_width()*desc.Width);
		float scale_y = target_area.get_height() / (source_area.get_height()*desc.Height);

		D3DXMATRIXA16 m,m2;
		D3DXVECTOR3 center(target_area.get_center().x,target_area.get_center().y,0.0f);
		D3DXVECTOR3 scale(s,s,s);
		D3DXVECTOR3 trans(0,0,z);
		D3DXMatrixTransformation(&m,&center,0,&scale,0,0,&trans);
		D3DXMatrixScaling(&m2,scale_x,scale_y,1.0f);
		m2*=m;
		sprite->SetTransform(&m2);

		D3DXVECTOR3 pos(target_area.left,target_area.top, 0 );
		pos.x /= scale_x;	pos.y /= scale_y;

		RECT rc; 
		rc.left=static_cast<LONG>(source_area.left*desc.Width);		rc.top=static_cast<LONG>(source_area.top*desc.Height);
		rc.bottom=static_cast<LONG>(source_area.bottom*desc.Height);	rc.right=static_cast<LONG>(source_area.right*desc.Width);
		sprite->Draw(d3d_tex,&rc,0,&pos,color);
	}

	void d3d9_ui_canvas::draw_texture( nrender_texture2D* texture, const frect& source_area, const frect& target_area,argb_t color /*= 0xFFFFFFFF */ )
	{
		if( texture == NULL )return;

		if( (color&0xFF000000) == 0 ) return;

		ID3DXSprite* sprite = d3d_device_manager::instance()->get_ui_sprite();
		if( sprite==NULL )return;
		IDirect3DTexture9* d3d_tex=static_cast<IDirect3DTexture9*>(texture->get_handle());
		if( d3d_tex==NULL || sprite==NULL )return;

		//因为输入参数是[0,1]之间的纹理坐标，sprite接受的是[0,width][0,height]之间的坐标，所以需要进行一次转换
		D3DSURFACE_DESC desc;
		d3d_tex->GetLevelDesc(0,&desc);

		float scale_x = target_area.get_width() / (source_area.get_width()*desc.Width);
		float scale_y = target_area.get_height() / (source_area.get_height()*desc.Height);

		D3DXMATRIXA16 matrix;
		D3DXMatrixScaling(&matrix,scale_x,scale_y,1.0f);
		sprite->SetTransform(&matrix);

		D3DXVECTOR3 pos(target_area.left,target_area.top, 0 );
		pos.x /= scale_x;	pos.y /= scale_y;

		RECT rc; 
		rc.left=static_cast<LONG>(source_area.left*desc.Width);		rc.top=static_cast<LONG>(source_area.top*desc.Height);
		rc.bottom=static_cast<LONG>(source_area.bottom*desc.Height);	rc.right=static_cast<LONG>(source_area.right*desc.Width);
		sprite->Draw(d3d_tex,&rc,0,&pos,color);
	}

	void d3d9_ui_canvas::draw_texture( nrender_texture2D* texture, const frect& source_area, const frect& target_area,float z,float s,const vector2& scale_center,argb_t color /*= 0xFFFFFFFF */ )
	{
		if( texture == NULL )return;

		if( (color&0xFF000000) == 0 ) return;

		ID3DXSprite* sprite = d3d_device_manager::instance()->get_ui_sprite();
		if( sprite==NULL )return;
		IDirect3DTexture9* d3d_tex=static_cast<IDirect3DTexture9*>(texture->get_handle());
		if( d3d_tex==NULL || sprite==NULL )return;

		//因为输入参数是[0,1]之间的纹理坐标，sprite接受的是[0,width][0,height]之间的坐标，所以需要进行一次转换
		D3DSURFACE_DESC desc;
		d3d_tex->GetLevelDesc(0,&desc);

		float scale_x = target_area.get_width() / (source_area.get_width()*desc.Width);
		float scale_y = target_area.get_height() / (source_area.get_height()*desc.Height);

		D3DXMATRIXA16 m,m2;
		D3DXVECTOR3 center(scale_center.x,scale_center.y,0);
		D3DXVECTOR3 scale(s,s,s);
		D3DXVECTOR3 trans(0,0,z);
		D3DXMatrixTransformation(&m,&center,0,&scale,0,0,&trans);
		D3DXMatrixScaling(&m2,scale_x,scale_y,1.0f);
		m2*=m;
		sprite->SetTransform(&m2);

		D3DXVECTOR3 pos(target_area.left,target_area.top, 0 );
		pos.x /= scale_x;	pos.y /= scale_y;

		RECT rc; 
		rc.left=static_cast<LONG>(source_area.left*desc.Width);		rc.top=static_cast<LONG>(source_area.top*desc.Height);
		rc.bottom=static_cast<LONG>(source_area.bottom*desc.Height);	rc.right=static_cast<LONG>(source_area.right*desc.Width);
		sprite->Draw(d3d_tex,&rc,0,&pos,color);
	}

	void d3d9_ui_canvas::draw_texture( nrender_texture2D* texture, const frect& source_area, const frect& target_area,float rotation,const vector2& rot_center,argb_t color /*= 0xFFFFFFFF */ )
	{
		if( texture == NULL )return;

		if( (color&0xFF000000) == 0 ) return;

		ID3DXSprite* sprite = d3d_device_manager::instance()->get_ui_sprite();
		if( sprite==NULL )return;
		IDirect3DTexture9* d3d_tex=static_cast<IDirect3DTexture9*>(texture->get_handle());
		if( d3d_tex==NULL || sprite==NULL )return;

		//因为输入参数是[0,1]之间的纹理坐标，sprite接受的是[0,width][0,height]之间的坐标，所以需要进行一次转换
		D3DSURFACE_DESC desc;
		d3d_tex->GetLevelDesc(0,&desc);

		float scale_x = target_area.get_width() / (source_area.get_width()*desc.Width);
		float scale_y = target_area.get_height() / (source_area.get_height()*desc.Height);

		D3DXMATRIXA16 m,m2;
		D3DXVECTOR3 center(rot_center.x,rot_center.y,0);
		D3DXQUATERNION  rot;
		D3DXVECTOR3 axis(0,0,1);
		D3DXQuaternionRotationAxis(&rot,&axis,rotation);
		D3DXMatrixTransformation(&m,NULL,NULL,NULL,&center,&rot,NULL);
		D3DXMatrixScaling(&m2,scale_x,scale_y,1.0f);
		m2*=m;
		sprite->SetTransform(&m2);

		D3DXVECTOR3 pos(target_area.left,target_area.top, 0 );
		pos.x /= scale_x;	pos.y /= scale_y;

		RECT rc; 
		rc.left=static_cast<LONG>(source_area.left*desc.Width);		rc.top=static_cast<LONG>(source_area.top*desc.Height);
		rc.bottom=static_cast<LONG>(source_area.bottom*desc.Height);	rc.right=static_cast<LONG>(source_area.right*desc.Width);
		sprite->Draw(d3d_tex,&rc,0,&pos,color);
	}

	void d3d9_ui_canvas::draw_texture( nrender_texture2D* texture, const frect& source_area, nrender_texture2D* alpha_texture,const frect& alpha_source_area,const frect& target_area,float rotation,argb_t color /*= 0xFFFFFFFF */ )
	{
		if( texture == NULL || alpha_texture==NULL )return;

		bool is_visible = (color&0xFF000000) != 0;

		if( !is_visible )return;

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		if( d3d_device == NULL )return;
		IDirect3DTexture9* d3d_tex = static_cast<IDirect3DTexture9*>(texture->get_handle());
		if( d3d_tex==NULL )return;

		IDirect3DTexture9* alpha_tex = static_cast<IDirect3DTexture9*>(alpha_texture->get_handle());
		if( alpha_tex==NULL )return;

		ID3DXSprite* ui_sprite = d3d_device_manager::instance()->get_ui_sprite();
		ui_sprite->Flush();
		IDirect3DVertexDeclaration9* pDecl = NULL;
		d3d_device->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
		d3d_device->SetFVF( UI_SCREEN_VERTEX::FVF );

		D3DXMATRIXA16 m,m2;
		D3DXVECTOR3 center(target_area.left+target_area.get_width()*0.5f,target_area.top+target_area.get_height()*0.5f,0);
		D3DXQUATERNION  rot;
		D3DXVECTOR3 axis(0,0,1);
		D3DXQuaternionRotationAxis(&rot,&axis,rotation);
		D3DXMatrixTransformation(&m,NULL,NULL,NULL,&center,&rot,NULL);

		UI_SCREEN_VERTEX vertices[4] =
		{
			target_area.left,  PixelAligned(target_area.top),    0.0f, 1.0f, color,	source_area.left,	source_area.top,alpha_source_area.left,alpha_source_area.top,
			target_area.right, PixelAligned(target_area.top),    0.0f, 1.0f, color,	source_area.right,	source_area.top,alpha_source_area.right,alpha_source_area.top,	
			target_area.right, PixelAligned(target_area.bottom), 0.0f, 1.0f, color,source_area.right,	source_area.bottom, alpha_source_area.right,alpha_source_area.bottom,
			target_area.left,  PixelAligned(target_area.bottom), 0.0f, 1.0f, color,	source_area.left,	source_area.bottom, alpha_source_area.left,alpha_source_area.bottom,
		};

		for(int i=0;i<4;++i)
		{
			vertices[i].pos=D3DXVECTOR4((FLOAT*)&mat_transform((vector4&)vertices[i].pos,(matrix44&)m));
		}

		d3d_device->SetTexture(0, d3d_tex);		
		d3d_device->SetTexture(1, alpha_tex);

		d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
		d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

		d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
		d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
		d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

		d3d_device->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
		d3d_device->SetTextureStageState(1,D3DTSS_COLORARG1,D3DTA_CURRENT);
		d3d_device->SetTextureStageState(1,D3DTSS_COLORARG2,D3DTA_TEXTURE);

		d3d_device->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
		d3d_device->SetTextureStageState(1,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
		d3d_device->SetTextureStageState(1,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
		
		d3d_device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
		d3d_device->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
		d3d_device->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
		
		d3d_device->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
		d3d_device->SetSamplerState(1,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
		d3d_device->SetSamplerState(1,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);

		d3d_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof( UI_SCREEN_VERTEX ) );

		// Restore the vertex decl
		d3d_device->SetVertexDeclaration( pDecl );
		pDecl->Release();

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

		d3d_device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
		d3d_device->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
	}

	// 绘制一个矩形块
	void d3d9_ui_canvas::draw_rect(const frect& target_area,const color_rect& color)
	{
		bool is_visible = ((color.left_top&0xFF000000) |(color.right_top&0xFF000000)
					| (color.right_top&0xFF000000) | (color.right_bottom&0xFF000000)) != 0;
		
		if( !is_visible )return;

		UI_SCREEN_VERTEX_UNTEX vertices[4] =
		{
			target_area.left - 0.5f,  target_area.top - 0.5f,    0.0f, 1.0f, color.left_top,
			target_area.right - 0.5f, target_area.top - 0.5f,    0.0f, 1.0f, color.right_top,
			target_area.right - 0.5f, target_area.bottom - 0.5f, 0.0f, 1.0f, color.right_bottom,
			target_area.left - 0.5f,  target_area.bottom - 0.5f, 0.0f, 1.0f, color.left_bottom,
		};

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		IDirect3DVertexDeclaration9* pDecl = NULL;
		d3d_device->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
		d3d_device->SetFVF( UI_SCREEN_VERTEX_UNTEX::FVF );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

		d3d_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof( UI_SCREEN_VERTEX_UNTEX ) );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

		// Restore the vertex decl
		d3d_device->SetVertexDeclaration( pDecl );
		pDecl->Release();
	}

	void d3d9_ui_canvas::draw_rect( const frect& target_area,float z,float s,const color_rect& color )
	{
		bool is_visible = ((color.left_top&0xFF000000) |(color.right_top&0xFF000000)
			| (color.right_top&0xFF000000) | (color.right_bottom&0xFF000000)) != 0;

		if( !is_visible )return;

		UI_SCREEN_VERTEX_UNTEX vertices[4] =
		{
			target_area.left - 0.5f,  target_area.top - 0.5f,    0, 1.0f, color.left_top,
			target_area.right - 0.5f, target_area.top - 0.5f,    0, 1.0f, color.right_top,
			target_area.right - 0.5f, target_area.bottom - 0.5f, 0, 1.0f, color.right_bottom,
			target_area.left - 0.5f,  target_area.bottom - 0.5f, 0, 1.0f, color.left_bottom,
		};

		D3DXMATRIXA16 m;
		D3DXVECTOR3 center(target_area.get_center().x,target_area.get_center().y,0.0f);
		D3DXVECTOR3 scale(s,s,s);
		D3DXVECTOR3 trans(0,0,z);
		D3DXMatrixTransformation(&m,&center,0,&scale,0,0,&trans);
		D3DXVec4TransformArray((D3DXVECTOR4*)vertices,sizeof(UI_SCREEN_VERTEX_UNTEX),(D3DXVECTOR4*)vertices,sizeof(UI_SCREEN_VERTEX_UNTEX),&m,4);

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		ID3DXSprite* ui_sprite = d3d_device_manager::instance()->get_ui_sprite();

		IDirect3DVertexDeclaration9* pDecl = NULL;
		d3d_device->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
		d3d_device->SetFVF( UI_SCREEN_VERTEX_UNTEX::FVF );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

		d3d_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof( UI_SCREEN_VERTEX_UNTEX ) );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

		// Restore the vertex decl
		d3d_device->SetVertexDeclaration( pDecl );
		pDecl->Release();
	}

	void d3d9_ui_canvas::draw_rect( const frect& target_area,float z,float s,const vector2& scale_center,const color_rect& color )
	{
		bool is_visible = ((color.left_top&0xFF000000) |(color.right_top&0xFF000000)
			| (color.right_top&0xFF000000) | (color.right_bottom&0xFF000000)) != 0;

		if( !is_visible )return;

		UI_SCREEN_VERTEX_UNTEX vertices[4] =
		{
			target_area.left - 0.5f,  target_area.top - 0.5f,    0, 1.0f, color.left_top,
			target_area.right - 0.5f, target_area.top - 0.5f,    0, 1.0f, color.right_top,
			target_area.right - 0.5f, target_area.bottom - 0.5f, 0, 1.0f, color.right_bottom,
			target_area.left - 0.5f,  target_area.bottom - 0.5f, 0, 1.0f, color.left_bottom,
		};

		D3DXMATRIXA16 m;
		D3DXVECTOR3 center(scale_center.x,scale_center.y,0);
		D3DXVECTOR3 scale(s,s,s);
		D3DXVECTOR3 trans(0,0,z);
		D3DXMatrixTransformation(&m,&center,0,&scale,0,0,&trans);
		D3DXVec4TransformArray((D3DXVECTOR4*)vertices,sizeof(UI_SCREEN_VERTEX_UNTEX),(D3DXVECTOR4*)vertices,sizeof(UI_SCREEN_VERTEX_UNTEX),&m,4);

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		ID3DXSprite* ui_sprite = d3d_device_manager::instance()->get_ui_sprite();

		// Since we're doing our own drawing here we need to flush the sprites
		ui_sprite->Flush();
		IDirect3DVertexDeclaration9* pDecl = NULL;
		d3d_device->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
		d3d_device->SetFVF( UI_SCREEN_VERTEX_UNTEX::FVF );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

		d3d_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof( UI_SCREEN_VERTEX_UNTEX ) );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

		// Restore the vertex decl
		d3d_device->SetVertexDeclaration( pDecl );
		pDecl->Release();
	}

	// 画线
	void d3d9_ui_canvas::draw_line( fpoint* points, unsigned int num_points, unsigned int color )
	{
		if( (color&0xFF000000) == 0 )return;

		UI_SCREEN_VERTEX_UNTEX* vertices = new UI_SCREEN_VERTEX_UNTEX[ num_points ];
		// E_OUTOFMEMORY
		if( vertices == NULL )
			return;

		UI_SCREEN_VERTEX_UNTEX* pVertex = vertices;
		fpoint* pt = points;
		for( UINT i = 0; i < num_points; i++ )
		{
			pVertex->x = pt->x;
			pVertex->y = pt->y;
			pVertex->z = 0.0f;
			pVertex->w=1.0f;
			pVertex->color = color;
			pVertex++;
			pt++;
		}

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		IDirect3DVertexDeclaration9* pDecl = NULL;
		d3d_device->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
		d3d_device->SetFVF( UI_SCREEN_VERTEX_UNTEX::FVF );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

		d3d_device->DrawPrimitiveUP( D3DPT_LINESTRIP, num_points - 1, vertices, sizeof( UI_SCREEN_VERTEX_UNTEX ) );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

		// Restore the vertex decl
		d3d_device->SetVertexDeclaration( pDecl );
		pDecl->Release();

		delete[] vertices;
		vertices = NULL;
	}

	void d3d9_ui_canvas::draw_line( fpoint* points, unsigned int num_points, float z,float s,const vector2& scale_center,unsigned int color )
	{
		if( (color&0xFF000000) == 0 )return;

		UI_SCREEN_VERTEX_UNTEX* vertices = new UI_SCREEN_VERTEX_UNTEX[ num_points ];
		// E_OUTOFMEMORY
		if( vertices == NULL )
			return;

		UI_SCREEN_VERTEX_UNTEX* pVertex = vertices;
		fpoint* pt = points;
		for( UINT i = 0; i < num_points; i++ )
		{
			pVertex->x = pt->x;
			pVertex->y = pt->y;
			pVertex->z = 0;
			pVertex->w = 1.0f;
			pVertex->color = color;
			pVertex++;
			pt++;
		}

		D3DXMATRIXA16 m;
		D3DXVECTOR3 center(scale_center.x,scale_center.y,0.0f);
		D3DXVECTOR3 scale(s,s,s);
		D3DXVECTOR3 trans(0,0,z);
		D3DXMatrixTransformation(&m,&center,0,&scale,0,0,&trans);
		D3DXVec4TransformArray((D3DXVECTOR4*)vertices,sizeof(UI_SCREEN_VERTEX_UNTEX),(D3DXVECTOR4*)vertices,sizeof(UI_SCREEN_VERTEX_UNTEX),&m,num_points);

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		IDirect3DVertexDeclaration9* pDecl = NULL;
		d3d_device->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
		d3d_device->SetFVF( UI_SCREEN_VERTEX_UNTEX::FVF );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

		d3d_device->DrawPrimitiveUP( D3DPT_LINESTRIP, num_points - 1, vertices, sizeof( UI_SCREEN_VERTEX_UNTEX ) );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

		// Restore the vertex decl
		d3d_device->SetVertexDeclaration( pDecl );
		pDecl->Release();

		delete[] vertices;
		vertices = NULL;
	}

	void d3d9_ui_canvas::draw_polygon(fpoint* points,unsigned int num_points, unsigned int color /* = 0xFFFFFFFF */ )
	{
		if( (color&0xFF000000) == 0 )return;

		UI_SCREEN_VERTEX_UNTEX* vertices = new UI_SCREEN_VERTEX_UNTEX[ num_points ];
		// E_OUTOFMEMORY
		if( vertices == NULL )
			return;

		UI_SCREEN_VERTEX_UNTEX* pVertex = vertices;
		fpoint* pt = points;
		for( UINT i = 0; i < num_points; i++ )
		{
			pVertex->x = pt->x;
			pVertex->y = pt->y;
			pVertex->z = 0.0f;
			pVertex->w = 1.0f;
			pVertex->color = color;

			pVertex++;
			pt++;
		}

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		IDirect3DVertexDeclaration9* pDecl = NULL;
		d3d_device->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
		d3d_device->SetFVF( UI_SCREEN_VERTEX_UNTEX::FVF );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

		d3d_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, num_points - 1, vertices, sizeof( UI_SCREEN_VERTEX_UNTEX ) );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

		// Restore the vertex decl
		d3d_device->SetVertexDeclaration( pDecl );
		pDecl->Release();

		delete[] vertices;
		vertices = NULL;
	}

	void d3d9_ui_canvas::flush()
	{
		ID3DXSprite* ui_sprite = d3d_device_manager::instance()->get_ui_sprite();

		// Since we're doing our own drawing here we need to flush the sprites
		ui_sprite->Flush();
	}
	void d3d9_ui_canvas::draw_polygon( fpoint* points,unsigned int num_points, float z,float s,const vector2& scale_center,unsigned int color /*= 0xFFFFFFFF */ )
	{
		if( (color&0xFF000000) == 0 )return;

		UI_SCREEN_VERTEX_UNTEX* vertices = new UI_SCREEN_VERTEX_UNTEX[ num_points ];
		// E_OUTOFMEMORY
		if( vertices == NULL )
			return;

		UI_SCREEN_VERTEX_UNTEX* pVertex = vertices;
		fpoint* pt = points;
		for( UINT i = 0; i < num_points; i++ )
		{
			pVertex->x = pt->x;
			pVertex->y = pt->y;
			pVertex->z = 0;
			pVertex->w = 1.0f;
			pVertex->color = color;

			pVertex++;
			pt++;
		}

		D3DXMATRIXA16 m;
		D3DXVECTOR3 center(scale_center.x,scale_center.y,0.0f);
		D3DXVECTOR3 scale(s,s,s);
		D3DXVECTOR3 trans(0,0,z);
		D3DXMatrixTransformation(&m,&center,0,&scale,0,0,&trans);
		D3DXVec4TransformArray((D3DXVECTOR4*)vertices,sizeof(UI_SCREEN_VERTEX_UNTEX),(D3DXVECTOR4*)vertices,sizeof(UI_SCREEN_VERTEX_UNTEX),&m,num_points);

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		ID3DXSprite* ui_sprite = d3d_device_manager::instance()->get_ui_sprite();

		// Since we're doing our own drawing here we need to flush the sprites
		ui_sprite->Flush();
		IDirect3DVertexDeclaration9* pDecl = NULL;
		d3d_device->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
		d3d_device->SetFVF( UI_SCREEN_VERTEX_UNTEX::FVF );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

		d3d_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, num_points - 1, vertices, sizeof( UI_SCREEN_VERTEX_UNTEX ) );

		d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

		// Restore the vertex decl
		d3d_device->SetVertexDeclaration( pDecl );
		pDecl->Release();

		delete[] vertices;
		vertices = NULL;
	}

} // end of namespace nexus

